#include "ccli/GlobalContext.hpp"
#include "ccli/Utility.hpp"

namespace ccli {

MainCmd::MainCmd() {
}
void MainCmd::add_command(GlobalContext *cnt, std::string cmd) {
    debug() << " -> Pushing to mains" << std::endl;
    cnt->get_mainc().push_back(cmd);
}


DeclCmd::DeclCmd() {
}
void DeclCmd::add_command(GlobalContext *cnt, std::string cmd) {
    debug() << " -> Pushing to decls" << std::endl;
    cnt->get_declc().push_back(cmd);
}



GlobalContext::GlobalContext() {
    agent = new DeclCmd();


    main_commands = {""};
    decl_commands = {""};

    _main = {
            "int main() {",
            };
     _return_main = {
            "return 0;",
            "}",
            };
    _include = {
            "#include <iostream>",
            "#include <string>",
            };
    filename = "ccli.cpp";
}

GlobalContext::~GlobalContext() {
    delete agent;
}


std::vector<std::string> &GlobalContext::get_mainc() {
    return main_commands;
}
std::vector<std::string> &GlobalContext::get_declc() {
    return decl_commands;
}


void GlobalContext::add_command(std::string cmd) {
    agent->add_command(this, cmd);
}

void GlobalContext::setMainState() {
    delete agent;
    debug() << "Changing ContextState" << std::endl;
    agent = new MainCmd();
}

void GlobalContext::setDeclState() {
    delete agent;
    debug() << "Changing ContextState" << std::endl;
    agent = new DeclCmd();
}

// Pushes all commands to string,
// Representing file content.
// Than returns it.
std::string GlobalContext::get_context() {
    std::string file_buffer;
    
    file_buffer += join(_include);
    file_buffer += join(decl_commands);
    file_buffer += join(_main);
    file_buffer += join(main_commands);
    file_buffer += join(_return_main);

    return file_buffer;
}
} // namespace
