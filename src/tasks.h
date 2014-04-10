#ifndef GUARD_tasks_h
#define GUARD_tasks_h

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "utility.h"


int initialise(ProgramSettings& settings, std::string calendar_database_location);

int rebuildCache(ProgramSettings& settings);

std::string selectCalendar(ProgramSettings& settings);

int askRegularity();
boost::gregorian::date calculateStartDate(ProgramSettings& settings, boost::gregorian::days regularity);
int createNewTask(ProgramSettings& settings);

double calculateTaskImportance(boost::gregorian::date occurrence_date, boost::gregorian::days regularity);
double calculateTaskImportance(ProgramSettings& settings, int task_id, boost::gregorian::date occurrence_date);
std::string prettifyDate(boost::gregorian::date date);
std::vector<std::string> createOrderedHumanReadableTaskEntries(ProgramSettings& settings, std::multimap<double, std::pair<int, boost::gregorian::date> > importance_to_id_date, bool date_included);
int viewUpcomingTasks(ProgramSettings& settings);

int dashboard(ProgramSettings& settings);

#endif
