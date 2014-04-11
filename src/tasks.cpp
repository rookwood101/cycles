#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <cmath>
#include <utility>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/assign.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/lexical_cast.hpp>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "interface.h"
#include "tasks.h"
#include "utility.h"
#include "database_types.h"


using namespace std;
using namespace boost;
using namespace soci;


int initialise(ProgramSettings& settings, string calendar_database_location) {
	settings.calendars.load(calendar_database_location);
	return 0;
}


int rebuildCache(ProgramSettings& settings) {
	vector< map<string, string> > current_calendar_tasks = settings.calendars.getTable(settings.calendars.getCurrentCalendar()).getRows();
	Table current_calendar_cache = settings.calendars.getTable(settings.calendars.getCurrentCalendar() + "_c");
	current_calendar_cache.truncate();

	for(vector< map<string, string> >::iterator it = current_calendar_tasks.begin(); it != current_calendar_tasks.end(); ++it) {
		gregorian::date task_start_date = gregorian::from_string((*it)["start_date"]);
		gregorian::days task_regularity( lexical_cast<int>((*it)["regularity"]) );
		map<string, string> sql_row = assign::map_list_of("task_id", (*it)["id"].c_str());
		gregorian::date task_occurrence_date = task_start_date;
		int occurrences_in_cache = 0;
		while((task_occurrence_date <= gregorian::day_clock::local_day() + gregorian::days(settings.cache_distance_days)) && occurrences_in_cache < 2) {
			
			if(task_occurrence_date >= gregorian::day_clock::local_day()) {
				sql_row["date"] = gregorian::to_iso_extended_string(task_occurrence_date);
				++occurrences_in_cache;
				current_calendar_cache.insertRow(sql_row);
			}
			task_occurrence_date += task_regularity;
		}
	}

	return 0;
}


string selectCalendar(ProgramSettings& settings) {
	vector<string> calendar_options;

	calendar_options = settings.calendars.getTables();

	for(vector<string>::iterator it = calendar_options.begin(); it != calendar_options.end(); ++it) {
		if (*((*it).end()-2) == '_')
			it = calendar_options.erase(it) - 1;
	}

	calendar_options.push_back("Create new");
	unsigned int selected_calendar_option = askList("Select a Calendar or Create a new Calendar:", calendar_options);
	if (selected_calendar_option == calendar_options.size() -1) {
		settings.calendars.createCalendar(askOpen("Name new Calendar:"));
		return selectCalendar(settings);
	}
	return calendar_options[selected_calendar_option];
}


int askRegularity() {
	vector<string> regularity_options = assign::list_of("Daily")("Weekly")("Fortnightly")("Monthly")("Six-monthly")("Yearly")("Custom");
	vector<int> regularity_option_values = assign::list_of(1)(7)(14)(30)(182)(365)(-1);
	unsigned int regularity_option = askList("Regularity of Task (How often it must be done):", regularity_options);

	if (regularity_option == regularity_options.size() -1) {
		//custom regularity
		regularity_option_values[regularity_option] = askOpenInt("Custom Task Regularity in Days:");
	}
	
	return regularity_option_values[regularity_option];
}

gregorian::date calculateStartDate(ProgramSettings& settings, gregorian::days regularity) {
	gregorian::days days_to_look_ahead( round(((log(regularity.days()) / log(1.215)) - 3)) );
	gregorian::date tomorrow = gregorian::day_clock::local_day() + gregorian::days(1);
	gregorian::date latest_start_date = gregorian::day_clock::local_day() + days_to_look_ahead;


	int tasks_on_day_before_date = settings.calendars.getTable(settings.calendars.getCurrentCalendar() + "_c").getCountBy("date", gregorian::to_iso_extended_string(tomorrow));
	gregorian::date start_date = tomorrow;

	for(gregorian::date date = tomorrow + gregorian::days(1); date <= latest_start_date; date += gregorian::days(1)) {
		int tasks_on_date = settings.calendars.getTable(settings.calendars.getCurrentCalendar() + "_c").getCountBy("date", gregorian::to_iso_extended_string(date));

		if(tasks_on_date <= tasks_on_day_before_date) {
			tasks_on_day_before_date = tasks_on_date;
			start_date = date;
		}

	}
	return start_date;
}

int createNewTask(ProgramSettings& settings) {
	string task_name = askOpen("Name of Task:");
	string task_description = askOpen("Description of Task (optional):");
	gregorian::days task_regularity(askRegularity());
	string task_regularity_sql = integerToString(task_regularity.days());

	gregorian::date task_start_date;
	if (askYesNo("Do you want to specify a start date? (Otherwise, the program will calculate one for you)")) {
		int day = askOpenInt("Day (Numerical):");
		int month = askOpenInt("Month (Numerical):");
		int year = askOpenInt("Year:");

		task_start_date = gregorian::date(year, month, day);
	}
	else {
		task_start_date = calculateStartDate(settings, task_regularity);
	}
	map<string, string> task_record = assign::map_list_of("name", task_name)("description", task_description)("regularity", task_regularity_sql)("start_date", gregorian::to_iso_extended_string(task_start_date));
	Table current_calendar = settings.calendars.getTable(settings.calendars.getCurrentCalendar());
	current_calendar.insertRow(task_record);
	
	rebuildCache(settings);
	return 0;
}


double calculateTaskImportance(gregorian::date occurrence_date, gregorian::days regularity) {
	gregorian::days days_away = occurrence_date - gregorian::day_clock::local_day();
	return ( static_cast<double>(regularity.days()) / (static_cast<double>(days_away.days()) * sqrt(regularity.days())) );
}


double calculateTaskImportance(ProgramSettings& settings, int task_id, gregorian::date occurrence_date) {
	vector< map<string, string> > fields_to_values = settings.calendars.getTable(settings.calendars.getCurrentCalendar()).getRowsBy("id", integerToString(task_id));
	gregorian::days task_regularity( lexical_cast<int>(fields_to_values[0]["regularity"]) );
	return calculateTaskImportance(occurrence_date, task_regularity);
}

string prettifyDate(gregorian::date date) {
	int offset = (date - gregorian::day_clock::local_day()).days();
	static vector<tuple<int, string, int> > positive_offset_name_divisor = assign::list_of(tuple<int, string, int>(0, "today", 0))(tuple<int, string, int>(1, "tomorrow", 0))(tuple<int, string, int>(2, " days", 1))(tuple<int, string, int>(7, " weeks", 7))(tuple<int, string, int>(30, " months", 30));
	string output = "";

	tuple<int, string, int> *prettify_rule = &*(positive_offset_name_divisor.begin());
	for(vector< tuple<int, string, int> >::iterator it = positive_offset_name_divisor.begin() + 1; it != positive_offset_name_divisor.end(); ++it) {
		if(offset >= it->get<0>())
			prettify_rule = &*it;
	}

	if (prettify_rule->get<2>() != 0) {
		output += integerToString(offset);
	}
	output += prettify_rule->get<1>();

	return output;
}

vector<string> createOrderedHumanReadableTaskEntries(ProgramSettings& settings, multimap<double, pair<int, gregorian::date> > importance_to_id_date, bool date_included) {
	vector<string> output;

	for(multimap<double, pair<int, gregorian::date> >::iterator it = importance_to_id_date.begin(); it != importance_to_id_date.end(); ++it) {
		int task_id = it->second.first;

		vector< map<string, string> > fields_to_values = settings.calendars.getTable(settings.calendars.getCurrentCalendar()).getRowsBy("id", integerToString(task_id));

		string task_name = fields_to_values[0]["name"];
		string task_description = fields_to_values[0]["description"];
		string task_date = prettifyDate(it->second.second);
		string task_entry = task_name;
		if (task_description != "")
			task_entry += ": " + task_description;
		if (date_included)
			task_entry += " " + task_date;
		output.push_back(task_entry);
	}

	return output;
}

int viewUpcomingTasks(ProgramSettings& settings) {
	rebuildCache(settings);
	multimap<double, pair<int, gregorian::date> > todays_tasks_importance_to_id, other_task_importance_to_id;
	vector< map<string, string> > task_occurrences = settings.calendars.getTable(settings.calendars.getCurrentCalendar() + "_c").getRows();
	for(vector< map<string, string> >::iterator it = task_occurrences.begin(); it != task_occurrences.end(); ++it) {
		gregorian::date task_occurrence_date( gregorian::from_string((*it)["date"]) );
		int task_id = lexical_cast<int>((*it)["task_id"]);

		if(task_occurrence_date == gregorian::day_clock::local_day()) {
			todays_tasks_importance_to_id.insert(pair<double, pair<int, gregorian::date> >(calculateTaskImportance(settings, task_id, task_occurrence_date), pair<int, gregorian::date>(task_id, task_occurrence_date)));
		}
		else {
			other_task_importance_to_id.insert(pair<double, pair<int, gregorian::date> >(calculateTaskImportance(settings, task_id, task_occurrence_date), pair<int, gregorian::date>(task_id, task_occurrence_date)));
		}
	}

	vector<string> todays_ordered_task_entries = createOrderedHumanReadableTaskEntries(settings, todays_tasks_importance_to_id, false);
	vector<string> other_ordered_task_entries = createOrderedHumanReadableTaskEntries(settings, other_task_importance_to_id, true);
	
	tellList("[Today]", todays_ordered_task_entries);
	tellList("[Upcoming]", other_ordered_task_entries);
	
	return 0;
}


int quit(ProgramSettings& settings) {
	if (askYesNo("Are you sure you want to quit?") == 1)
		settings.exit = true;
	return 0;
}


int dashboard(ProgramSettings& settings) {
	vector<string> dashboard_options = assign::list_of("View Upcoming Tasks")("Create New Task")("Switch Calendar")("Quit");
	while (!settings.exit) {
		int dashboard_selection = askList("Dashboard", dashboard_options);
		switch (dashboard_selection) {
		case 0:
			viewUpcomingTasks(settings);
			break;
		case 1:
			createNewTask(settings);
			break;
		case 2:
			selectCalendar(settings);
			break;
		case 3:
			quit(settings);
			break;
		}
	}

	return 0;
}

