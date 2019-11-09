#include "ccli/NormWriter.hpp"
#include "ccli/helpers.hpp"


void NormWriter::put_command(std::string cmd) {
    commands.push_back(cmd);
}

void NormWriter::remove_command(std::string cmd) {
    if (!find(cmd, "=") && commands.size() > 0) {
        commands.pop_back();
    }
}

void NormWriter::remove_error(std::string cmd) {
    if (commands.size() > 0) {
        commands.pop_back();
    }
}

std::string NormWriter::get_commands() {
    std::string res = "";
    if (commands.size() > 0) {
        for (auto cmd: commands) {
            res += cmd;
        }
    }
    res += ";";
    return res;
}
