#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <boost/assign.hpp>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

using namespace std;
using namespace soci;
using namespace boost;

int Table::load(session& database_sql_session, string table_name) {
	sql_session = &database_sql_session;
	name = table_name;
}

rowset<row> Table::getRowsBy(string search_field, string search_value) {
	rowset<row> output = ((*sql_session).prepare << "SELECT * FROM " << name << " WHERE " << search_field << "='" << search_value << "'");
	return output;
}




int Database::load(string location) {
	location_on_disk = location;
	sql_session = session(sqlite3, location_on_disk);
	is_loaded = false;
	return 0;
}

vector<string> Database::getTables() {
	if(isLoaded())
		throw runtime_error("Database Unintialised");

	rowset<string> rs = (sql_session.prepare << "SELECT name FROM sqlite_master WHERE type='table'");
	vector<string> output;
	copy(rs.begin(), rs.end(), output.begin());
	return output;
}

Table Database::getTable(string table_name) {
	if(isLoaded())
		throw runtime_error("Database Unintialised");

	return Table(sql_session, table_name);
}

Table Database::createTable(string table_name, vector<string> table_fields) {
	if(isLoaded())
		throw runtime_error("Database Unintialised");

	string sql_query = "CREATE TABLE if not exists " + table_name + " (";
	sql_query << table_fields[0];
	for(vector<string>::const_iterator it = table_fields.begin() + 1; it != table_fields.end(); ++it) {
		sql_query << ", " << *it;
	}
	sql_query << ")";

	sql_session << sql_query;
	return Table(sql_session, table_name);
}



int setCurrentCalendar(std::string new_current_calendar) {
	current_calendar = new_current_calendar;
}

Table CalendarDatabase::createCalendar(string calendar_name) {
	vector<string> table_fields = assign::list_of("id INTEGER PRIMARY KEY")("name TEXT")("description TEXT")("regularity INTEGER")("start_date DATE");
	return createTable(calendar_name, table_fields);
}
