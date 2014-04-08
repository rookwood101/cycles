#ifndef GUARD_utility_h
#define GUARD_utility_h

#include <string>
#include <vector>
#include "database_types.h"

struct ProgramSettings {
	bool exit;
	CalendarDatabase calendars;
	CacheDatabase cache;

	ProgramSettings(): exit(false) {}
};

bool string_is_integer(std::string input);

#endif
