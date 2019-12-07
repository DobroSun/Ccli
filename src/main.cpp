#include "ccli/runToolOnCode.hpp"
#include "ccli/GlobalContext.hpp"
#include "ccli/exec_expr.hpp"
#include "ccli/Utility.hpp"

//#include "clang/Frontend/FrontedActions.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <string>
#include <csignal>
#include <fstream>
#include <cstdio>
#include <cstring>


std::string welcome() {
    std::string welcome = "Hello world> ";
    return welcome;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::cout << "Loading: " << argv[1] << std::endl;
    }

    ccli::ClangTool Tool;
    ccli::GlobalContext global_context;
    
    // Handles Ctrl-C interruption
    struct sigaction act;
    act.sa_handler = [](int sig){
                        std::cout << "\n";
                        rl_on_new_line();
                        rl_replace_line("", 0);
                        rl_redisplay();
                     };
    sigaction(SIGINT, &act, NULL);

    rl_bind_key('\t', rl_insert);
    while (1) {
        const char *cmd = readline(welcome().c_str());

        // Handles C-d and whitespace
        if (cmd == NULL) {std::cout << "\n"; exit(0);}
        if (!std::strcmp(cmd, "")) continue;

        add_history(cmd);
        global_context.add_command(cmd);

        Tool.run(global_context.get_context());
        std::string result = exec_expr(cmd);

        std::cout << result;
    }
    return 0;
}
