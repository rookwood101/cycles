#ifndef GUARD_utility_h
#define GUARD_utility_h

#include <string>
#include <vector>
#include "database_types.h"

struct ProgramSettings {
	bool exit;
	int cache_distance_days;
<<<<<<< HEAD
	vector<string>::size_type max_tasks_other;
	CalendarDatabase calendars;

	ProgramSettings(): exit(false), cache_distance_days(30), max_tasks_other(10) {}
=======
	int max_tasks_other;
	CalendarDatabase calendars;

	ProgramSettings(): exit(false), cache_distance_days(30), max_tasks_other(10), {}
};
>>>>>>> 49ac88bbd2a5f88b4a181641c490369a1bc13cee

bool stringIsInteger(std::string input);

std::string integerToString(int input);

#endif
