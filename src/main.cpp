#include "ccli/utility.hpp"
#include "ccli/context.hpp"

#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <string>
#include <csignal>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <exception>
#include <regex>
#include <fstream>


using ccli::push_to;
using namespace ccli::utility;


std::string welcome(ccli::context &cc) {
  auto includes = cc.get_includes();
  if(includes.empty()) {
    return "iostream> ";
  }

  std::string last_include = includes[includes.size()-1];

  std::regex re("#include? ?[ <\"](.*)?[>\"]");
  std::string result;
  std::smatch match;
  try {
    if(std::regex_search(last_include, match, re) && match.size() > 1) {
      result = match.str(1);
    } else {
      result = "iostream";
    }
  } catch(std::regex_error& e) {
    std::cout << e.what() << std::endl;
  }
  return result + "> ";
}

bool compile() {
  return exec("g++ tmp.cpp").empty();
}

std::string execute() {
  return exec("./a.out");
}


int main(int argc, const char **argv) {
  ccli::context cc;

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

  auto state = push_to::mains;
  bool one_command = false;
  while(1) {
    const char *cmd = readline(welcome(cc).c_str());

    // Handles C-d and whitespace
    if(cmd == nullptr) {std::cout << "\n"; exit(0);}
    if(!std::strcmp(cmd, "")) continue;
    add_history(cmd);


    std::ofstream file("tmp.cpp");
    if(!file) throw std::runtime_error("Cannot create file to process");

    std::string view = cmd;
    bool can_compile = false;
    if(starts_with(view, ":")) {
      // additional commands.
      if(starts_with(view, ":run") || starts_with(view, ":r")) {
        can_compile = true;

      } else if(starts_with(view, ":decl") || starts_with(view, ":d")) {
        state = push_to::decls;

      } else if(starts_with(view, ":enddecl") || starts_with(view, ":ed")) {
        state = push_to::mains;

      } else if(starts_with(view, ":include") || starts_with(view, ":in")) {
        state = push_to::includes;

      } else if(starts_with(view, ":endinclude") || starts_with(view, ":ein")) {
        state = push_to::mains;

      } else if(starts_with(view, ":only") || starts_with(view, ":on")) {
        state = push_to::decls;
        one_command = true;

      } else if(starts_with(view, ":ls")) {
        std::cout << cc.get_content();
      
      } else if(starts_with(view, ":undo")) {
        

      } else if(starts_with(view, ":clearmains") || starts_with(view, ":cm")) {
        cc.clear_mains();

      } else if(starts_with(view, ":cleardecls") || starts_with(view, "cd")) {
        cc.clear_decls();

      } else if(starts_with(view, ":clearincludes") || starts_with(view, "ci")) {
        cc.clear_includes();

      } else if(starts_with(view, ":clear") || starts_with(view, ":c")) {
        cc.clear_mains();
        cc.clear_decls();
        cc.clear_includes();

      }
    } else {
      // adding command to main/decl/includes.
      if(state == push_to::mains) {
        cc.add_main_cmd(view);
      } else if(state == push_to::decls) {
        cc.add_decl_cmd(view);
      } else if(state == push_to::includes) {
        cc.add_include_cmd(view);
      } else {
        throw std::runtime_error("Cannot decide where to put given command");
      }

      if(one_command) {
        state = push_to::mains;
        one_command = false;
      }
    }

    std::string content = cc.get_content();

    file << content;
    file.flush();

    if(can_compile) {
      std::string res = "";
      res = compile();

      if(res.empty()) {
      } else {
        res = execute();
      }
      std::cout << res;
    }
    file.close();

    // ----------------------------------------------------
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
    //  elif(:decl):
    //    (context pushes to main by default)
    //    set !command to push everything to decl until :enddecl.


    // after analyzing push to main/decl in context.



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
    // true -> exec_command: runs execution.
    // false -> err_command: set content to previous state.
    //  -> returns std::string result.



    // ----------------------------------------------------
    //  TEXT:
    // ./ccli
    // Have to delete commands that use side effects(IO,...).
    // So:
    // Check if current command is
    // DeclVal or Stmt(ForStmt, IfStmt...)
    // or whether it changes state of some variable in scope.
    //  <- via Clang.
    // then: have no need to delete it, just leaving.




    //std::string result = "";
    // std::string result = execute(ss);
    //std::cout << "lol" << std::endl;
  }
  return 0;
}
