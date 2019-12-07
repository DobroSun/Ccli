#ifndef GLOBALCONTEXTCPP
#define GLOBALCONTEXTCPP
#include <string>
#include <vector>


namespace ccli {

class GlobalContext {
    std::string filename;
    
    std::vector<std::string> main_commands;
    std::vector<std::string> decl_commands;

    std::vector<std::string> _main;
    std::vector<std::string> _return_main;
    std::vector<std::string> _include;
public:
    GlobalContext();
    void add_command(std::string cmd);
    std::string get_context();
};
}
#endif
