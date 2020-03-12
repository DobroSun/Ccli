#include "ccli/utility.hpp"

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


int main(int argc, const char **argv) {

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
  while(1) {
    const char *cmd = readline(welcome().c_str());

    // Handles C-d and whitespace
    if(cmd == nullptr) {std::cout << "\n"; exit(0);}
    if(!std::strcmp(cmd, "")) continue;
    add_history(cmd);


    // have to decide whether cmd is
    // an additional command (:run, :show)
    // or just text (int a = 1; std::cout << ...).

    // so need a class that will take
    // cmd string and analyze it.

    // if additional command
    // then: 
    //  if(:run):
    //    set !command! to run compilation.
    //  elif(:show):
    //    set !command! to show smth.
    // else:
    //   check current working command.
    //   set !command! corresponding state_command.
    //   /* have to put it to main or to decls? */

    // context returns 1 string containing
    // all content.
    
    // !command! calls needed action.
    // ex: * Do nothing.
    //     * Compile.
    //     * Add string to content.
    //     * Clear content.

    //     * Put space?  /*if(!std::strcmp(cmd, "")) continue;*/
    //     * Exit?  /*if(cmd == nullptr) {std::cout << "\n"; exit(0);}*/
    // if !command! fails?


    // OK.
    // run_command: runs compilation -> get result ?(true | false)?.
    // sets !new_command! whether execute or handle error.

    // exec_command: runs execution -> returns result.
    // err_command: set content to previous state.

    //std::string result = "";
    // std::string result = execute(ss);
    //std::cout << "lol" << std::endl;
  }
  return 0;
}
