#include <stdexcept>
#include <memory>
#include <array>
#include <iostream>
#include <regex>
#include <cstring>

#include "ccli/utility.hpp"

namespace ccli {
namespace utility {

// Joins all strings in vector.
// Returns resulted string.
// Separator is ch, by deafault '\n'.
std::string join(const std::vector<std::string> &context, char ch) {
	std::string res;
	for(auto i: context) {
			res += i;
			res += ch;
	}
	return res;
}


// Executes specified bash command
// In terminal and gives output in string.
std::string exec(const std::string &cmd) {
  std::string execute = cmd + " 2>&1";
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(execute.c_str(), "r"), pclose);
	if(!pipe) {
    std::cerr << "Processing popen failed!\n";
    return "";
	}
	while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
	}
	return result;
}


// Splits given string with ch.
// Returns vector of strings.
std::vector<std::string> split(const std::string &str, char ch) {
	std::vector<std::string> res;
	std::string accumulate; 
	for(auto s_ch: str) {
		if(s_ch == ch) {
						res.push_back(std::move(accumulate));
						continue;
				}
				accumulate += s_ch;
	}
	if(!accumulate.empty())
			res.push_back(accumulate);
	return res;
}


// Executes bash command.
// Returns splitted output.
std::vector<std::string> get_splitted_exec(const std::string &cmd, char ch) {
	return split(exec(cmd), ch);
}


std::string ltrim(std::string &str) {
	return std::regex_replace(str, std::regex("^ +"), "");
}
std::string rtrim(std::string &str) {
	return std::regex_replace(str, std::regex(" +$"), "");
}

std::string trim(std::string &str) {
	return std::regex_replace(str, std::regex("^ +| +$|( ) +"), "$1");
}


bool equals(const std::string &s1, const std::string &s2) {
  return !s1.compare(s2);
}
bool ends_with(std::string &cmd, const std::string &ch) {
	cmd = rtrim(cmd);
	return (cmd.size() >= ch.size() &&  cmd.compare(cmd.size() - ch.size(), ch.size(), ch) == 0);
}
bool starts_with(std::string &cmd, const std::string &ch) {
	cmd = ltrim(cmd);
	return cmd.rfind(ch) == 0;
}
bool find(std::string &cmd, const std::string &ch) {
	int a = cmd.find(ch);
	return a != -1;
}
int find_place(std::string &cmd, const std::string &ch) {
		int a = cmd.find(ch);
		if (!find(cmd, ch)) a = cmd.size() + 1;
		return a;
}

}; // namespace utility
}; // namespace ccli

