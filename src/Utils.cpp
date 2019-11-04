#include <string>
#include <iostream>
#include <list>

#include "ccli/Utils.hpp"

History::History() {
    std::list<std::string > history;
    std::string *last_used;
}

void History::update_history(std::string cmd) {
	//this->last_used = &cmd;
    this->history.push_back(cmd);
}

void History::show_history() {
	for (auto line: this->history) {
		std::cout << line << "\n";
	}	
}
