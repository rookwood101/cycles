#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/assign.hpp>
#include "interface.h"
#include "tasks.h"
#include "utility.h"
#include "database_types.h"


using namespace std;
using namespace boost;


int initialise(ProgramSettings& settings, string calendar_database_location, string cache_database_location) {
	settings.calendars.load(calendar_database_location);
	settings.cache.load(cache_database_location);
	return 0;
}


string selectCalendar(ProgramSettings& settings) {
	vector<string> calendar_options;

	if(!settings.calendars.isLoaded()) {
		throw runtime_error("Database Unintialised");
	}

	calendar_options = settings.calendars.getTables();
	calendar_options.push_back("Create new");

	int selected_calendar_option = askList("Select a Calendar or Create a new Calendar:", calendar_options);

	if (selected_calendar_option == calendar_options.size() -1) {
		settings.calendars.createCalendar(askOpen("Name new Calendar:"));
		return selectCalendar(settings);
	}

	return calendar_options[selected_calendar_option];
}


int askRegularity() {
	vector<string> regularity_options = assign::list_of("Daily")("Weekly")("Fortnightly")("Monthly")("Six-monthly")("Yearly")("Custom");
	vector<int> regularity_option_values = assign::list_of(1)(7)(14)(30)(182)(365)(-1);
	int regularity_option = askList("Regularity of Task (How often it must be done):", regularity_options);

	if (regularity_option == regularity_options.size() -1) {
		//custom regularity
		regularity_option_values[regularity_option] = askOpenInt("Custom Task Regularity in Days:");
	}
	
	return regularity_option_values[regularity_option];
}

int createNewTask(ProgramSettings& settings) {
	string task_name = askOpen("Name of Task:");
	string task_description = askOpen("Description of Task (optional):");
	gregorian::days regularity(askRegularity());

	gregorian::date start_date;
	if (askYesNo("Do you want to specify a start date? (Otherwise, the program will calculate one for you)")) {
		int day = askOpenInt("Day (Numerical):");
		int month = askOpenInt("Month (Numerical):");
		int year = askOpenInt("Year:");

		start_date = gregorian::date(year, month, day);
	}
	else {
		
	}

	return 0;
}


int viewUpcomingTasks(ProgramSettings& settings) {
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

