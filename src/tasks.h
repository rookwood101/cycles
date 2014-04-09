#ifndef GUARD_tasks_h
#define GUARD_tasks_h

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "utility.h"


int initialise(ProgramSettings& settings, std::string calendar_database_location);

int rebuildCache(ProgramSettings& settings);

double calculateTaskImportance(boost::gregorian::date occurrence_date, boost::gregorian::days regularity);

std::string selectCalendar(ProgramSettings& settings);

int askRegularity();
boost::gregorian::date calculateStartDate(ProgramSettings& settings, boost::gregorian::days regularity);
int createNewTask(ProgramSettings& settings);

int viewUpcomingTasks(ProgramSettings& settings);

int dashboard(ProgramSettings& settings);

#endif
