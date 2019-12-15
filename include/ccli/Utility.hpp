#ifndef UTILITY_CPP
#define UTILITY_CPP
#include <vector>
#include <string>

std::string join(std::vector<std::string> &context, char ch='\n');
std::string exec(std::string cmd);
std::vector<std::string> split(std::string str, char ch='\n');
#endif
