#ifndef GUARD_tasks_h
#define GUARD_tasks_h

#include "utility.h"
#include <string>

int initialise(ProgramSettings& settings, std::string calendar_database_location, std::string cache_database_location);

int askRegularity();
int createNewTask(ProgramSettings& settings);

int viewUpcomingTasks(ProgramSettings& settings);

int selectCalendar(ProgramSettings& settings);

int dashboard(ProgramSettings& settings);

#endif
