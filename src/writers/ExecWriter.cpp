#include "ccli/ExecWriter.hpp"


void ExecWriter::put_command(std::string cmd) {
    commands.push_back(cmd);
}

void ExecWriter::remove_command(std::string cmd) {
    if (commands.size() > 0) {
        commands.pop_back();
    }
}

void ExecWriter::remove_error(std::string cmd) {
    if (commands.size() > 0) {
        commands.pop_back();
    }
}

std::string ExecWriter::get_commands() {
    std::string res;
    if (commands.size() > 0) {
        res = "std::cout << (";
        res += commands.back();
        res += ") << std::endl;";
    } else {
        res = "";
    }
    return res;
}
