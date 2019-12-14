#include <memory>
#include <array>
#include <stdexcept>

#include "llvm/Support/raw_ostream.h"

#include "ccli/Utility.hpp"

// Joins all strings in vector.
// Returns resulted string.
// Separator is ch, by deafault '\n'.
std::string join(std::vector<std::string> &context, char ch) {
    std::string res;
    for(auto i: context) {
        res += i;
        res += ch;
    }
    return res;
}

// Executes specified bash command
// In terminal and gives output in string.
std::string exec(std::string cmd) {
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


std::vector<std::string> split(std::string str, char ch) {
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

