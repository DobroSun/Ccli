#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <iostream>

#include "ccli/IWriters.hpp"

Exe_Writer::Exe_Writer() {
    std::array<std::string, 1> commands;
}

std::string Exe_Writer::get_commands() {
    if (commands[0].empty()) return "";
    std::string cmd;
    cmd += "std::cout << ";
    cmd += commands[0];
    cmd += " << std::endl;";

    commands[0] = "";
    return cmd;
}

void Exe_Writer::put_command(std::string cmd) {
    commands[0] = cmd;
}


Norm_Writer::Norm_Writer() {
    std::vector<std::string> commands;
}

std::string Norm_Writer::get_commands() {
    std::string res;
    for (std::string str: commands) {
        res += str;
    }
    return res;
}

void Norm_Writer::put_command(std::string cmd) {
    commands.push_back(cmd);
}

void Norm_Writer::delete_command() {
    if (commands.size() > 0) {
        commands.pop_back();
    }
}

void Norm_Writer::delete_if_not_definition(std::string cmd) {
    int found = cmd.find("=");
    if (found == -1 && commands.size() > 0) {
        commands.pop_back();
    }
}

IManager::IManager() {
    Norm_Writer norm_writer;
    Exe_Writer exe_writer;
}

void IManager::make_file(std::ofstream &tmp_file) {
	tmp_file << "#include <iostream>\n";
    
    // call pre_commands
    tmp_file << "int main() {";

    // call normal_commands
    tmp_file << norm_writer.get_commands();


    // executable_commands
    tmp_file << exe_writer.get_commands();

    tmp_file << ";";
    tmp_file << "return 0;";
    tmp_file << "}";
	tmp_file.flush();
}

void IManager::analise_input(std::string cmd) {
    
    if (cmd.compare(cmd.size() - 1, 1, ";") == 0) {
        norm_writer.put_command(cmd);
    } else {
        exe_writer.put_command(cmd);
    }
}

void IManager::remove_error(std::string cmd) {
    if (cmd.compare(cmd.size() - 1, 1, ";") == 0) {
        norm_writer.delete_command();
    } else {
        // pre_command
    }
}

void IManager::remove_command(std::string cmd) {
    if (cmd.compare(cmd.size() - 1, 1, ";") == 0) {
        norm_writer.delete_if_not_definition(cmd);
    }
}
