#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <boost/utility.hpp>
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "database_types.h"
#include "interface.h"

using namespace std;
using namespace soci;
using namespace boost;


int Table::load(string location, string table_name) {
	database_location = location;
	algorithm::to_lower(table_name);
	name = table_name;
	return 0;
}

bool Table::exists() {
	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON;";

	int exists;
	sql_session << "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=':name';", use(name), into(exists);

	return exists;
}
vector< map<string, string> > Table::getRows() {
	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON;";

	rowset<row> rs = (sql_session.prepare << "SELECT * FROM :name", use(name));

	vector< map<string, string> > output_field_to_value;

	for(rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
		map<string, string> field_to_value;	
		row const& r = *it;

		for(map<string, string>::size_type i = 0; i < r.size(); ++i) {
			const column_properties & props = r.get_properties(i);
			switch(props.get_data_type()) {
				case dt_string:
					field_to_value[props.get_name()] = r.get<std::string>(i);
					break;
				case dt_double:
					field_to_value[props.get_name()] = boost::lexical_cast<string>(r.get<double>(i));
					break;
				case dt_integer:
					field_to_value[props.get_name()] = boost::lexical_cast<string>(r.get<int>(i));
					break;
				case dt_long_long:
				case dt_unsigned_long_long:
				case dt_date:
					break;
			}
		}

		output_field_to_value.push_back(field_to_value);
	}
	sql_session.close();
	return output_field_to_value;
}

vector< map<string, string> > Table::getRowsBy(string search_field, string search_value) {
	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON;";

	rowset<row> rs = (sql_session.prepare << "SELECT * FROM :name WHERE :searchfield=':searchvalue'", use(name), use(search_field), use(search_value));

	vector< map<string, string> > output_field_to_value;

	for(rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
		map<string, string> field_to_value;	
		row const& r = *it;

		for(map<string, string>::size_type i = 0; i < r.size(); ++i) {
			const column_properties & props = r.get_properties(i);
			switch(props.get_data_type()) {
				case dt_string:
					field_to_value[props.get_name()] = r.get<std::string>(i);
					break;
				case dt_double:
					field_to_value[props.get_name()] = boost::lexical_cast<string>(r.get<double>(i));
					break;
				case dt_integer:
					field_to_value[props.get_name()] = boost::lexical_cast<string>(r.get<int>(i));
					break;
				case dt_long_long:
				case dt_unsigned_long_long:
				case dt_date:
					break;
			}
		}

		output_field_to_value.push_back(field_to_value);
	}
	sql_session.close();
	return output_field_to_value;
}

int Table::getCountBy(string search_field, string search_value) {
	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON;";

	int output;

	sql_session << "SELECT COUNT(*) FROM :name WHERE :searchfield=':searchvalue'", use(name), use(searchfield), use(searchvalue), into(output);

	return output;
}

int Table::truncate() {
	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON;";

	sql_session << "DELETE FROM :name;", use(name);
	sql_session << "VACUUM";

	sql_session.close();
	return 0;
}

int Table::insertRow(map<string, string> field_to_value) {
	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON;";

	string sql_query = "INSERT INTO " + name + " (" + field_to_value.begin()->first;
	for(map<string, string>::const_iterator it = next(field_to_value.begin()); it != field_to_value.end(); ++it) {
		sql_query += ", " + it->first;
	}
	sql_query += ") VALUES ('" + field_to_value.begin()->second + "'";
	for(map<string, string>::const_iterator it = next(field_to_value.begin()); it != field_to_value.end(); ++it) {
		sql_query += ", '" + it->second + "'";
	}
	sql_query += ");";
	sql_session << sql_query;

	int primary_key = 0;

	sql_session << "SELECT last_insert_rowid();", into(primary_key);

	sql_session.close();
	return primary_key;
}



int Database::load(string location) {
	database_location = location;
	is_loaded = true;
	return 0;
}

vector<string> Database::getTables() {
	if(!isLoaded())
		throw runtime_error("Database Unintialised");

	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON;";

	rowset<string> rs = (sql_session.prepare << "SELECT name FROM sqlite_master WHERE type='table'");
	vector<string> output;
	copy(rs.begin(), rs.end(), back_inserter(output));

	return output;
}

Table Database::getTable(string table_name) {
	if(!isLoaded())
		throw runtime_error("Database Unintialised");

	return Table(database_location, table_name);
}

Table Database::createTable(string table_name, vector<string> table_fields) {
	algorithm::to_lower(table_name);

	if(!isLoaded())
		throw runtime_error("Database Unintialised");

	if(getTable(table_name).exists())
		throw runtime_error("Calendar already exists");

	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON;";

	string sql_query = "CREATE TABLE if not exists :tablename (" + table_fields[0];
	for(vector<string>::const_iterator it = table_fields.begin() + 1; it != table_fields.end(); ++it) {
		sql_query += ", " + *it;
	}
	sql_query += ")";

	sql_session << sql_query, use(table_name);
	return Table(database_location, table_name);
}



int CalendarDatabase::setCurrentCalendar(std::string new_current_calendar) {
	current_calendar = new_current_calendar;
	return 0;
}

string CalendarDatabase::getCurrentCalendar() {
	return current_calendar;
}

Table CalendarDatabase::createCalendar(string calendar_name) {
	if(!isLoaded())
		throw runtime_error("Database Unintialised");
	if(getTable(calendar_name).exists())
		throw runtime_error("Calendar already exists");

	if(calendar_name[calendar_name.size() - 2] == '_')
		throw runtime_error("Calendar name reserved");

	vector<string> table_fields = assign::list_of("id INTEGER PRIMARY KEY")("name TEXT")("description TEXT")("regularity INTEGER")("start_date CHAR(10)");
	Table output = createTable(calendar_name, table_fields);
	createCache(calendar_name);
	return output;
}



Table CalendarDatabase::createCache(string calendar_name) {
	if(!isLoaded())
		throw runtime_error("Database Unintialised");
	if(!getTable(calendar_name).exists())
		throw runtime_error("Calendar does not exist");
	if(getTable(calendar_name + "_c").exists())
		throw runtime_error("Cache for this calendar already created");

	string foreign_key_string = "FOREIGN KEY(task_id) REFERENCES " + calendar_name + "(id)";
	vector<string> table_fields = assign::list_of("id INTEGER PRIMARY KEY")("date CHAR(10)")("task_id INTEGER")(foreign_key_string.c_str());
	return createTable(calendar_name + "_c", table_fields);
}
