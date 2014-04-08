#include <iostream>
#include <string>
#include <vector>
#include "utility.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

bool askYesNo(string question) {
	string answer;
	do {
		cout << question << " <y/n>" << endl;
		getline(cin, answer);
		trim(answer);
	} while (answer != "y" && answer != "n");

	if (answer == "y") return 1;
	else return 0;
}

int askList(string question, vector<string> possible_answers) {
	string answer;
	int answer_int;
	do {
		answer_int = -1;
		if (possible_answers.size() > 1)
			cout << question << " <0-" << possible_answers.size() - 1 << ">" << endl;
		else if (possible_answers.size() == 1) {
			cout << question << " <0>" << endl;
		}
		for (vector<string>::size_type i = 0; i < possible_answers.size(); i++) {
			cout << "[" << i << "] " << possible_answers[i] << endl;
		}
		cout << endl;
		getline(cin, answer);
		trim(answer);
		if (stringIsInteger(answer))
			answer_int = lexical_cast<int>(answer);
	} while ( !( answer_int >= 0 && answer_int < static_cast<int>(possible_answers.size()) ) );

	return answer_int;
}

string askOpen(string question) {
	cout << question << " ";

	string answer;
	getline(cin, answer);
	trim(answer);

	cout << endl << endl;

	return answer;
}

int askOpenInt(string question) {
	string answer;
	while(!stringIsInteger(answer = askOpen(question))) { }
	return lexical_cast<int>(answer);
}
