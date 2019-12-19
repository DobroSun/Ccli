#include <stdexcept>
#include <memory>
#include <array>
#include <iostream>
#include <regex>
#include <cstring>

#include "llvm/Support/raw_ostream.h"

#include "ccli/Utility.hpp"

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
std::string join(std::vector<std::string> &&context, char ch) {
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
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
		llvm::errs() << "Processing popen failed!\n";
		return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
std::string exec(std::string &&cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
		llvm::errs() << "Processing popen failed!\n";
		return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
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
std::vector<std::string> split(std::string &&str, char ch) {
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
std::vector<std::string> get_splitted_exec(std::string &&cmd, char ch) {
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


std::string add_I_option(const std::string &dest) {
    std::string tmp = "-I";
    tmp += dest;
    return tmp;
}
std::string add_I_option(std::string &&dest) {
    std::string tmp = "-I";
    tmp += dest;
    return tmp;
}
