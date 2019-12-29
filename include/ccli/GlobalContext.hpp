#ifndef GLOBALCONTEXT_CPP
#define GLOBALCONTEXT_CPP
#include <string>
#include <vector>

#include "ccli/Logger.hpp"


namespace ccli {
class GlobalContext;


class ContextState {
public:
    virtual void add_command(GlobalContext *cnt, std::string cmd) = 0; //{
        //debug() << " -> Pushing by default" << std::endl;
    //}
};

class MainCmd: public ContextState {
public:
    ~MainCmd() {}
    void add_command(GlobalContext *cnt, std::string cmd) override;
};

class DeclCmd: public ContextState {
public:
    ~DeclCmd() {}
    void add_command(GlobalContext *cnt, std::string cmd) override;
};



class GlobalContext {
    ContextState *agent;
    std::string filename;
    
    std::vector<std::string> main_commands;
    std::vector<std::string> decl_commands;

    std::vector<std::string> _main;
    std::vector<std::string> _return_main;
    std::vector<std::string> _include;
public:
    GlobalContext();
    std::vector<std::string> &get_mainc();
    std::vector<std::string> &get_declc();

    void setState(ContextState *state);
    void add_command(std::string cmd);
    std::string get_context();
};
}
#endif
