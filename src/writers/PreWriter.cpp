#include "ccli/PreWriter.hpp"

void PreWriter::put_command(std::string cmd) {
    if (cmd[0] != '|') {
        for (auto it = cmd.begin(); it < cmd.end(); it++) {
            if (cmd[0] == ' ' || cmd[0] == '\t') {
                cmd = cmd.substr(1);
            } else {
                break;
            }
        }
    }
    cmd = cmd.substr(1);
    commands.push_back(cmd);
}

void PreWriter::remove_command(std::string cmd) {
    // None
}

void PreWriter::remove_error(std::string cmd) {
    if (commands.size() > 0) {
        commands.pop_back();
    }
}

std::string PreWriter::get_commands() {
    std::string res = "";
    if (commands.size() > 0) {
        for (auto cmd: commands) {
            res += cmd;
            res += "\n";
        }
    }
    return res;
}
