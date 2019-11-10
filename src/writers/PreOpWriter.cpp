#include "ccli/PreOpWriter.hpp"

void PreOpWriter::put_command(std::string cmd) {
    if (opened_pars == 0) {
        lines_to_clear = 0;
    }
    lines_to_clear += 1;

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

void PreOpWriter::remove_command(std::string cmd) {
    // None
}

void PreOpWriter::remove_error(std::string cmd) {
    for (int i = 0; i < lines_to_clear; i++) {
        commands.pop_back();
    }
}

std::string PreOpWriter::get_commands() {
    std::string res = "";
    if (opened_pars == 0) {
        for(auto cmd: commands) {
            res += cmd;
            res += "\n";
        }
    }
    return res;
}

void PreOpWriter::clear() {
    remove_error("");
    //commands.clear();
    opened_pars = 0;
}

void PreOpWriter::increase_pars() {
    opened_pars += 1;
}

void PreOpWriter:: decrease_pars() {
    if (opened_pars > 0) {
        opened_pars -= 1;
    }
}
