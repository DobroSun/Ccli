#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "ccli/IWriters.hpp"


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

Exe_Writer::Exe_Writer() {
    std::array<std::string, 1> commands;
}

std::string Exe_Writer::get_commands() {
    if (commands[0].empty()) return "";
    std::string cmd;
    cmd += "std::cout << (";
    cmd += commands[0];
    cmd += ") << std::endl;";

    commands[0] = "";
    return cmd;
}

void Exe_Writer::put_command(std::string cmd) {
    commands[0] = cmd;
}


Pre_Writer::Pre_Writer() {
    opened_parentheses = 0;
    state = All_closed;
}

int Pre_Writer::get_pars() {
    return opened_parentheses;
}

Norm_Writer::Norm_Writer() {
    opened_parentheses = 0;
    state = All_closed;
}

int Norm_Writer::get_pars() { 
    return opened_parentheses;
}

void Norm_Writer::increase_pars() {
    opened_parentheses += 1;
}

void Norm_Writer::decrease_pars() {
    opened_parentheses -= 1;
}

std::string Norm_Writer::get_commands() {
    std::string res;
    for (std::string str: commands) {
        res += str;
    }
    return res;
}

void Norm_Writer::put_command(std::string cmd) {
    if (state == Norm_opened) {
        expressions.push_back(cmd);
    } else {
        commands.push_back(cmd);
    }
}

void Norm_Writer::put_expression(std::string cmd) {
    increase_pars();
    state = Norm_opened;

    expressions.push_back(cmd);
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

void Norm_Writer::apply_to_commands(std::string cmd) {
    decrease_pars();
    expressions.push_back(cmd);

    for (auto str: expressions) {
        commands.push_back(str);
    }
}

void Norm_Writer::clear_expressions() {
    for (unsigned int i = 0; i < expressions.size(); i++) {
        commands.pop_back();
    }
    expressions.clear();
}

IManager::IManager() {
    state = All_closed;
}

void IManager::change_state(std::string cmd) {
    if (endswith(cmd, "}")) {
        state = All_closed;
    }
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
    return norm_writer.get_pars() || pre_writer.get_pars();
}

void IManager::analise_input(std::string cmd) {
    // { functions, classes, enums, ...
    if (startswith(cmd, "|") && endswith(cmd, "{")) {
        std::cout << "PRE {" << std::endl;

    // } functions, classes, structs, enums, ...
    } else if (startswith(cmd, "|") && (endswith(cmd, "};") || endswith(cmd, "}"))) {
        std::cout << "PRE }; or }" << std::endl;

    // constants, includes, defines.
    } else if (startswith(cmd, "|")) {
        std::cout << "PRE " << std::endl;

    // { loops, while, if stms.
    } else if (endswith(cmd, "{")) {
        norm_writer.put_expression(cmd);

    // } loops, while, if stms.
    } else if (endswith(cmd, "}")) {
        norm_writer.apply_to_commands(cmd);

    // normal commands (int a = 2; std::cout << "Yea";)
    } else if (endswith(cmd, ";")) {
        norm_writer.put_command(cmd);

    // executable commands (2 + 3, a, sizeof(a))
    } else {
        exe_writer.put_command(cmd);
    }
}

void IManager::remove_error(std::string cmd) {
    if (state == All_closed) {
        if (endswith(cmd, ";")) {
            norm_writer.delete_command();
        } else {
            // pre_command
        }
    } else if (state == Norm_opened) {
        norm_writer.clear_expressions();
    } else if (state == Pre_opened) {

    }

}

void IManager::remove_command(std::string cmd) {
    if (state == All_closed) {
        if (endswith(cmd, ";")) {
            norm_writer.delete_if_not_definition(cmd);
        }
    } else if (state == Norm_opened) {
        norm_writer.clear_expressions();
    } else if (state == Pre_opened) {
    
    }
}
