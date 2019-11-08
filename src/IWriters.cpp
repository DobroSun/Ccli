#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <iostream>

#include "ccli/IWriters.hpp"


bool endswith(std::string cmd, std::string ch) {
    return (cmd.size() > ch.size() &&  cmd.compare(cmd.size() - ch.size(), ch.size(), ch) == 0);
}

bool startswith(std::string cmd, std::string ch) {
    return cmd.rfind(ch) == 0;
}

bool find(std::string cmd, std::string ch) {
    int a = cmd.find(ch);
    return a != -1;
}

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


Pre_Writer::Pre_Writer() {
    opened_parentheses = 0;
    //std::cout << opened_parentheses << std::endl;;
    state = None;
}

int Pre_Writer::get_pars() {
    return opened_parentheses;
}

Norm_Writer::Norm_Writer() {
    opened_parentheses = 0;
    state = None;
}

int Norm_Writer::get_pars() { 
    return opened_parentheses;
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
    if (!find(cmd, "=") && commands.size() > 0) {
        commands.pop_back();
    }
}

IManager::IManager() {
    state = None;
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

bool IManager::is_opened_pars() {
    if (norm_writer.get_pars() < 0 || pre_writer.get_pars() < 0) {
        std::cout << "bracket is closed before opening\n";
        exit(0);
    }
    return (norm_writer.get_pars() > 0)? norm_writer.get_pars(): pre_writer.get_pars();
}

void IManager::analise_input(std::string cmd) {
    // { functions, classes, enums, ...
    if (startswith(cmd, "|") && endswith(cmd, "{")) {
        std::cout << "PRE {" << std::endl;

    // } classes, structs, enums.
    } else if (startswith(cmd, "|") && endswith(cmd, "};")) {
        std::cout << "PRE };" << std::endl;

    // } functions.
    } else if (startswith(cmd, "|") && endswith(cmd, "}")) {
        std::cout << "PRE }" << std::endl;

    // constants, includes, defines.
    } else if (startswith(cmd, "|")) {
        std::cout << "PRE " << std::endl;

    // { loops, while, if stms.
    } else if (endswith(cmd, "{")) {
        std::cout << "NORM {" << std::endl;

    // } loops, while, if stms.
    } else if (endswith(cmd, "}")) {
        std::cout << "NORM }" << std::endl;

    // normal commands (int a = 2; std::cout << "Yea";)
    } else if (endswith(cmd, ";")) {
        norm_writer.put_command(cmd);

    // executable commands (2 + 3, a, sizeof(a))
    } else {
        exe_writer.put_command(cmd);
    }
}

void IManager::remove_error(std::string cmd) {
    if (endswith(cmd, ";")) {
        norm_writer.delete_command();
    } else {
        // pre_command
    }
}

void IManager::remove_command(std::string cmd) {
    if (endswith(cmd, ";")) {
        norm_writer.delete_if_not_definition(cmd);
    }
}
