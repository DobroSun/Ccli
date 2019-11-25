#include <iostream>
#include <string>
#include <csignal>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <readline/readline.h>
#include <readline/history.h>

#include "ccli/exec_expr.hpp"


std::string welcome() {
    std::string welcome("Hello world> ");
    return welcome;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::cout << "Doing something with " << argv[1] << std::endl;
    }
    
	// handle Ctrl-C interruption
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
        const char *line = readline(welcome().c_str());

        if (line == NULL) {std::cout << "\n"; exit(0);}
        if (!std::strcmp(line, "")) continue;

        std::string result = exec_expr(line);

        std::cout << result << "\n";
    }
    return 0;
}
