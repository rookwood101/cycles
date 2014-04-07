#include <string>
#include <boost/lexical_cast.hpp>


using namespace std;

struct ProgramSettings;

bool string_is_integer(std::string input) {
	bool is_integer = false;
	try {
		boost::lexical_cast<int>(input);
		is_integer = true;
	}
	catch (boost::bad_lexical_cast &){}
	return is_integer;
}
