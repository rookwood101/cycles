#ifndef GUARD_tasks_h
#define GUARD_tasks_h

#include "utility.h"
#include <string>

int initialise(ProgramSettings& settings, std::string calendar_database_location);

std::string selectCalendar(ProgramSettings& settings);

int askRegularity();
int createNewTask(ProgramSettings& settings);

int viewUpcomingTasks(ProgramSettings& settings);

int dashboard(ProgramSettings& settings);

#endif
