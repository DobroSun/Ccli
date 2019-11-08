#include <string>
#include <iostream>
#include <array>
#include <memory>

#include "ccli/CompileExec.hpp"

#define TMP_FILE "../tmp/tmp.cpp"
#define TMP_DIR "../tmp/"

Compiler::Compiler() {
    is_compiled = false;
}

std::string Compiler::compile() {
    std::string cmd("cd ");
    cmd += TMP_DIR; cmd += ";g++ "; cmd += TMP_FILE; cmd += " 2>&1";
    std::array<char, 128> buffer;
    std::string result = "";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Compilation popen error");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != NULL) {
        result += buffer.data();
    }

    if (result.empty()) {
        is_compiled = true;
    } else { 
        is_compiled = false;
    }
    return result;
}

Executor::Executor() {
}

std::string Executor::execute() {
    std::string cmd("cd ");
    cmd += TMP_DIR; cmd += "; ./a.out";

    std::array<char, 128> buffer;
    std::string result = "";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Execution popen error");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != NULL) {
        result += buffer.data();
    }
    //std::cout << "Executing..." << std::endl;
    return result;
}
