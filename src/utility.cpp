#include <string>
#include <boost/lexical_cast.hpp>


using namespace std;

struct ProgramSettings;

bool stringIsInteger(std::string input) {
	bool is_integer = false;
	try {
		boost::lexical_cast<int>(input);
		is_integer = true;
	}
	catch (boost::bad_lexical_cast &){}
	return is_integer;
}

string integerToString(int input) {
	return boost::lexical_cast<string>(input);
}
