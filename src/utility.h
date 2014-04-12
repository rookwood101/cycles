#ifndef GUARD_utility_h
#define GUARD_utility_h

#include <string>
#include <vector>
#include "database_types.h"

struct ProgramSettings {
	bool exit;
	int cache_distance_days;
	int max_tasks_other;
	CalendarDatabase calendars;

	ProgramSettings(): exit(false), cache_distance_days(30), max_tasks_other(10), {}
};

bool stringIsInteger(std::string input);

std::string integerToString(int input);

#endif
