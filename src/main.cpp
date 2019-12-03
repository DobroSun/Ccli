#include <iostream>
#include <string>
#include <csignal>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <readline/readline.h>
#include <readline/history.h>

#include "ccli/exec_expr.hpp"
#include "ccli/runToolOnCode.hpp"


std::string welcome() {
    std::string welcome("Hello world> ");
    return welcome;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::cout << "Doing something with " << argv[1] << std::endl;
    }

    ccli::ClangTool Tool;
    
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
        add_history(cmd);

        // Handles C-d and namespace
        if (cmd == NULL) {std::cout << "\n"; exit(0);}
        if (!std::strcmp(cmd, "")) continue;

        Tool.run(cmd);
        std::string result = exec_expr(cmd);

        std::cout << result;
    }
    return 0;
}
