#ifndef GUARD_database_type_h
#define GUARD_database_type_h

#include <string>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>


class Database {
private:
	int current_calendar;
	std::string calendar_database_location;
	std::string cache_database_location;
	bool is_empty;
public:
	Database(): is_empty(true), current_calendar(-1) {}
	int load(std::string calendar_location, std::string cache_location);
};

int Database::load(std::string calendar_location, std::string cache_location) {
	calendar_database_location = calendar_location;
	cache_database_location = cache_location;
}


#endif
