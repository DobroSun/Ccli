#include <iostream>
#include <string>
#include <csignal>
#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <memory>
#include <cstring>
#include <readline/readline.h>
#include <readline/history.h>

#include "ccli/IManager.hpp"
#include "ccli/CompileExec.hpp"
#include "ccli/Loader.hpp"

#define TMP_FILE "../tmp/tmp.cpp"
#define TMP_DIR "../tmp/"


std::string welcome(State state) {
    std::string welcome("Hello world");
    if (state == Closed) {
        welcome += "> ";
    } else {
        welcome += ". ";
    }
    return welcome;
}

void close_all(std::ofstream *tmp_file) {
    tmp_file->close();
}

int main(int argc, char* argv[]) {
    Loader loader;
    if (argc > 1) {
        std::cout << "Doing something with " << argv[1] << std::endl;
        // the thing is file is deleting on every iteration
        // it's crazy to rewrite full file on each command.
        loader.load_file(argv[1]);
    }

    State state = Closed;
    IManager i_manager(&state);
    Compiler compiler;
    Executor executor;
    
    std::ofstream tmp_file(TMP_FILE);
    if (!tmp_file) {
        std::cout << "No tmp_file!\n";
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
    rl_bind_key ('\t', rl_insert);
    
	while (1) {
        const char *line = readline(welcome(state).c_str());

        if (line == NULL) {std::cout << "\n"; close_all(&tmp_file); exit(0);}
        if (!std::strcmp(line, "")) continue;


        // Shit
		std::remove(TMP_FILE);
		std::ofstream tmp_file(TMP_FILE);

        add_history(line);


        i_manager.analise_input(line);
        i_manager.make_file(tmp_file);


        std::string res_comp = "";
        std::string res_exe = "";

        if (state == Closed) {
            res_comp = compiler.compile();

            if (compiler.is_compiled) {
                res_exe = executor.execute();
                i_manager.remove_command(line);

            } else {
                i_manager.remove_error(line);
            }

        }

        std::string result = res_comp + res_exe;
        rl_replace_line(result.c_str(), 0);
        std::cout << rl_line_buffer;
    }
    return 0;
}
