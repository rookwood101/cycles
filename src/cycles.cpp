#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

#include "calendar.h"
#include "interface.h"
#include "utility.h"
#include "database.h"
#include "tasks.h"


using namespace std;
using namespace soci;

int main()
{
	ProgramSettings program_settings;
	initialise(program_settings, "");

	program_settings.database.current_calendar = selectCalendar(program_settings);
	dashboard(program_settings);
	return 0;
}
