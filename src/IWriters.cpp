#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "ccli/IWriters.hpp"

Exe_Writer::Exe_Writer() {
}

std::string Exe_Writer::get_commands() {
    std::string cmd;
    if (commands.empty()) {
        return "";
    }
    cmd += "std::cout << ";
    cmd += commands.back();
    cmd += " << std::endl;";

    commands.pop_back();
    return cmd;
}

void Exe_Writer::put_command(std::string cmd) {
    commands.push_back(cmd);
}


Norm_Writer::Norm_Writer() {
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
