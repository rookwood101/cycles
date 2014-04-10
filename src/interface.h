#ifndef GUARD_interface_h
#define GUARD_interface_h

#include <string>
#include <vector>

int askYesNo(std::string question);
int askList(std::string question, std::vector<std::string> possible_answers);
std::string askOpen(std::string question);
int askOpenInt(std::string question);
int tellList(std::string title, std::vector<std::string> list);

#endif
