#include "ccli/NormOpWriter.hpp"
#include "ccli/helpers.hpp"

NormOpWriter::NormOpWriter() {
}

void NormOpWriter::put_command(std::string cmd) {
    commands.push_back(cmd);
}

void NormOpWriter::remove_command(std::string cmd) {

}

void NormOpWriter::remove_error(std::string cmd) {

}

std::string NormOpWriter::get_commands() {
    std::string res = "";
    if (opened_pars == 0) {
        for (auto cmd: commands) {
            res += cmd;
        }
    }
    return res;
}

void NormOpWriter::clear() {
    commands.clear();
    opened_pars = 0;
}

void NormOpWriter::increase_pars() {
    opened_pars += 1;
}

void NormOpWriter::decrease_pars() {
    opened_pars -= 1;
}
