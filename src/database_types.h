#ifndef GUARD_database_type_h
#define GUARD_database_type_h

#include <string>
#include <vector>
#include <map>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

class Table {
private:
	std::string name;
	soci::session* sql_session;
public:
	Table(soci::session database_sql_session, std::string table_name) {load(database_sql_session, table_name)}

	int load(soci::session database_sql_session, std::string table_name);

	soci::rowset<soci::row> getRowsBy(std::string search_field, std::string search value);
};


class Database {
private:
	std::string location_on_disk;
	bool is_loaded;
	soci::session sql_session;
public:
	Database(): is_loaded(true){}
	Database(std::string location): is_loaded(true) {load(location);}

	bool isLoaded(){return is_loaded;}

	int load(std::string location);

	std::vector<std::string> getTables();
	Table getTable(std::string table_name);

	Table createTable(std::string table_name, std::vector<std::string> table_fields);
};


class CalendarDatabase : public Database {
private:
	std::string current_calendar;
public:
	CalendarDatabase(): current_calendar(""), Database() {}
	int setCurrentCalendar(std::string new_current_calendar);
	Table createCalendar(std::string calendar_name);
};

class CacheDatabase : public Database {

};


#endif
