#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <boost/utility.hpp>
#include <boost/assign.hpp>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "database_types.h"

using namespace std;
using namespace soci;
using namespace boost;


int Table::load(string location, string table_name) {
	database_location = location;
	name = table_name;
	return 0;
}

bool Table::exists() {
	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON";

	int exists;

	string sql_query = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='" + name + "'";

	sql_session << sql_query, into(exists);

	return exists;
}

rowset<row> Table::getRowsBy(string search_field, string search_value) {
	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON";

	rowset<row> output = (sql_session.prepare << "SELECT * FROM " << name << " WHERE " << search_field << "='" << search_value << "'");

	
	return output;
}

int Table::insertRow(map<string, string> field_to_value) {
	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON";

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

	int primary_key;
	sql_session << "SELECT last_insert_rowid()", into(primary_key);
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
	sql_session << "PRAGMA foreign_keys = ON";

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
	if(!isLoaded())
		throw runtime_error("Database Unintialised");

	session sql_session(sqlite3, database_location);
	sql_session << "PRAGMA foreign_keys = ON";

	string sql_query = "CREATE TABLE if not exists " + table_name + " (" + table_fields[0];
	for(vector<string>::const_iterator it = table_fields.begin() + 1; it != table_fields.end(); ++it) {
		sql_query += ", " + *it;
	}
	sql_query += ")";

	sql_session << sql_query;
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

	vector<string> table_fields = assign::list_of("id INTEGER PRIMARY KEY")("name TEXT")("description TEXT")("regularity INTEGER")("start_date DATE");
	return createTable(calendar_name, table_fields);
}



Table CalendarDatabase::createCache(string calendar_name) {
	if(!isLoaded())
		throw runtime_error("Database Unintialised");
	if(!getTable(calendar_name).exists())
		throw runtime_error("Calendar does not exist");
	if(getTable(calendar_name + "_c").exists())
		throw runtime_error("Cache for this calendar already created");

	string foreign_key_string = "FOREIGN KEY(task_id) REFERENCES " + calendar_name + "(id)";
	vector<string> table_fields = assign::list_of("id INTEGER PRIMARY KEY")("date DATE")("task_id INTEGER")(foreign_key_string.c_str());
	return createTable(calendar_name + "_c", table_fields);
}
