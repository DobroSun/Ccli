#include <string>
#include <algorithm>

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
