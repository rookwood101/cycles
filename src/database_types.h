#ifndef GUARD_database_type_h
#define GUARD_database_type_h

#include <string>
#include <vector>
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
	bool is_empty;
	soci::session sql_session;
public:
	Database(): is_empty(true){}
	Database(std::string location): is_empty(true) {load(location);}

	int load(std::string location);

	std::vector<std::string> getTables();
	Table getTable(std::string table_name);
};


class CalendarDatabase : public Database {
private:
	int current_calendar;
public:
	CalendarDatabase(): current_calendar(-1), Database() {}
};


#endif
