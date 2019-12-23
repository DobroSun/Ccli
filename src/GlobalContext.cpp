#include "ccli/GlobalContext.hpp"
#include "ccli/Utility.hpp"


namespace ccli {

GlobalContext::GlobalContext() {
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
            "#include <vector>",
            };
    filename = "ccli.cpp";
}

void GlobalContext::add_command(std::string cmd) {
    // Has to be in 'State' pattern.
    main_commands.push_back(cmd);
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
