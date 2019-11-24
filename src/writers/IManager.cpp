#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

#include "ccli/IManager.hpp"
#include "ccli/helpers.hpp"

IManager::IManager(State *_state) {
    state = _state;
}

void IManager::change_state(State _state) {
    *state = _state;
}

void IManager::clear_opened() {
    norm_op_writer.clear();
    pre_op_writer.clear();
    change_state(Closed);
}

void IManager::make_file(std::ofstream &tmp_file) {
    tmp_file << "#include <iostream>\n";
    tmp_file << pre_writer.get_commands();

    tmp_file << "\n";
    tmp_file << pre_op_writer.get_commands();

    // call pre_commands
    tmp_file << "int main() {";


    // call normal_commands
    tmp_file << norm_writer.get_commands();


    // executable_commands
    tmp_file << exec_writer.get_commands();


    tmp_file << norm_op_writer.get_commands();


    tmp_file << ";";
    tmp_file << "return 0;";
    tmp_file << "}";
    tmp_file.flush();
}

void IManager::analise_input(std::string cmd) {
    // { functions, classes, enums, ...
    if (startswith(cmd, "|") && endswith(cmd, "{")) {
        pre_op_writer.put_command(cmd);
        change_state(Opened_Pre);
        pre_op_writer.increase_pars();

    // } functions, classes, structs, enums, ...
    } else if (startswith(cmd, "|") && (endswith(cmd, "};") || endswith(cmd, "}"))) {
        pre_op_writer.put_command(cmd);
        change_state(Closed);
        pre_op_writer.decrease_pars();

    // constants, includes, defines.
    } else if (startswith(cmd, "|")) {
        if (*state == Closed) {
            pre_writer.put_command(cmd);
        } else if (*state == Opened_Pre) {
            pre_op_writer.put_command(cmd);
        } else {
            std::cout << "Not possible |" << std::endl;
            clear_opened();
        }

    // { loops, while, if stms.
    } else if (endswith(cmd, "{")) {
        if (find_place(cmd, "}") < find_place(cmd, "{")) norm_op_writer.decrease_pars();
        norm_op_writer.put_command(cmd);
        change_state(Opened_Norm);
        norm_op_writer.increase_pars();

    // } loops, while, if stms.
    } else if (endswith(cmd, "}")) {
        norm_op_writer.put_command(cmd);
        change_state(Closed);
        norm_op_writer.decrease_pars();

    // normal commands (int a = 2; std::cout << "Yea";)
    } else if (endswith(cmd, ";")) {
        if (*state == Closed) {
            norm_writer.put_command(cmd);
        } else if (*state == Opened_Norm) {
            norm_op_writer.put_command(cmd);
        } else {
            std::cout << "Not possible ;" << std::endl;
            clear_opened();
        }

    // additional commands
    } else if (startswith(cmd, ":")) {

    // executable commands (2 + 3, a, sizeof(a))
    } else {
        if (*state == Closed) {
            exec_writer.put_command(cmd);
        } else {
            std::cout << "Not possible" << std::endl;
            clear_opened();
        }
    }
}

void IManager::remove_command(std::string cmd) {
    // { functions, classes, enums, ...
    if (startswith(cmd, "|") && endswith(cmd, "{")) {
        pre_op_writer.remove_command(cmd);

    // } functions, classes, structs, enums, ...
    } else if (startswith(cmd, "|") && (endswith(cmd, "};") || endswith(cmd, "}"))) {
        pre_op_writer.remove_command(cmd);

    // constants, includes, defines.
    } else if (startswith(cmd, "|")) {
        pre_writer.remove_command(cmd);

    // { loops, while, if stms.
    } else if (endswith(cmd, "{")) {
        norm_op_writer.remove_command(cmd);

    // } loops, while, if stms.
    } else if (endswith(cmd, "}")) {
        norm_op_writer.remove_command(cmd);

    // normal commands (int a = 2; std::cout << "Yea";)
    } else if (endswith(cmd, ";")) {
        norm_writer.remove_command(cmd);
    // additional commands
    } else if (startswith(cmd, ":")) {

    // executable commands (2 + 3, a, sizeof(a))
    } else {
        exec_writer.remove_command(cmd);
    }
}

void IManager::remove_error(std::string cmd) {
    // { functions, classes, enums, ...
    if (startswith(cmd, "|") && endswith(cmd, "{")) {
        pre_op_writer.remove_error(cmd);

    // } functions, classes, structs, enums, ...
    } else if (startswith(cmd, "|") && (endswith(cmd, "};") || endswith(cmd, "}"))) {
        pre_op_writer.remove_error(cmd);

    // constants, includes, defines.
    } else if (startswith(cmd, "|")) {
        pre_writer.remove_error(cmd);

    // { loops, while, if stms.
    } else if (endswith(cmd, "{")) {
        norm_op_writer.remove_error(cmd);

    // } loops, while, if stms.
    } else if (endswith(cmd, "}")) {
        norm_op_writer.remove_error(cmd);

    // normal commands (int a = 2; std::cout << "Yea";)
    } else if (endswith(cmd, ";")) {
        norm_writer.remove_error(cmd);

    // additional commands
    } else if (startswith(cmd, ":")) {

    // executable commands (2 + 3, a, sizeof(a))
    } else {
        exec_writer.remove_error(cmd);
    }
}
