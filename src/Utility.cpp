#include <string>

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
