#include <string>
#include <algorithm>
#include <stdexcept>

bool endswith(std::string cmd, std::string ch) {
    cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end());
    return (cmd.size() >= ch.size() &&  cmd.compare(cmd.size() - ch.size(), ch.size(), ch) == 0);
}
bool startswith(std::string cmd, std::string ch) {
    cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end());
    return cmd.rfind(ch) == 0;
}
bool find(std::string cmd, std::string ch) {
    int a = cmd.find(ch);
    return a != -1;
}

int find_place(std::string cmd, std::string ch) {
    int a = cmd.find(ch);
    if (!find(cmd, ch)) a = cmd.size() + 1;
    return a;
}
