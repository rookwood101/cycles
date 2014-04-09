#include "utility.h"
#include "tasks.h"

int main()
{
	ProgramSettings program_settings;
	initialise(program_settings, "calendars.sql");

	program_settings.calendars.setCurrentCalendar(selectCalendar(program_settings));
	dashboard(program_settings);
	return 0;
}
