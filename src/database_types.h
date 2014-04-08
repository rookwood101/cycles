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
	std::string database_location;
public:
	Table(std::string location, std::string table_name){load(location, table_name);}

	int load(std::string location, std::string table_name);

	soci::rowset<soci::row> getRowsBy(std::string search_field, std::string search_value);
	int insertRow(std::map<std::string, std::string> field_to_value);
};


class Database {
private:
	std::string database_location;
	bool is_loaded;
public:
	Database(): database_location(""), is_loaded(false) {}
	Database(std::string location): database_location(""), is_loaded(false) {load(location);}

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
	CalendarDatabase(): Database(), current_calendar("") {}
	int setCurrentCalendar(std::string new_current_calendar);
	std::string getCurrentCalendar();
	Table createCalendar(std::string calendar_name);
};

class CacheDatabase : public Database {
private:

public:
	CacheDatabase(): Database() {}
};


#endif
