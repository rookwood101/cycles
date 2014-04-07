#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

using namespace std;
using namespace soci;

int Database::load(string location) {
	location_on_disk = location;
	sql_session = session(sqlite3, location_on_disk);
}

vector<string> Database::getTables() {
	rowset<string> rs = (sql_session.prepare << "SELECT name FROM sqlite_master WHERE type='table'");
	vector<string> output;
	copy(rs.begin(), rs.end(), output.begin());
	return output;
}
