#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

using namespace std;
using namespace soci;

int Table::load(session& database_sql_session, string table_name) {
	sql_session = &database_sql_session;
	name = table_name;
}

rowset<row> Table::getRowsBy(string search_field, string search_value) {
	rowset<row> output = ((*sql_session).prepare << "SELECT * FROM " << name << " WHERE " << search_field << "='" << search_value << "'");
	return output;
}

int Database::load(string location) {
	location_on_disk = location;
	sql_session = session(sqlite3, location_on_disk);
	is_empty = false;
	return 0;
}

vector<string> Database::getTables() {
	rowset<string> rs = (sql_session.prepare << "SELECT name FROM sqlite_master WHERE type='table'");
	vector<string> output;
	copy(rs.begin(), rs.end(), output.begin());
	return output;
}

Table Database::getTable(string table_name) {
	return Table(sql_session, table_name);
}
