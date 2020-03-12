#ifndef ALL_TEMPLATE_IMPLEMENTATIONS
#define ALL_TEMPLATE_IMPLEMENTATIONS
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <array>
#include <vector>

#include <unistd.h>


#include "ccli/logger.hpp"


// Prints all items from given collection.
template<typename T>
void print(const std::vector<T> &vec) {
    debug() << "[";
    for(unsigned int i = 0; i < vec.size(); i++) {
        bool pr = false;
        bool cm = false;
        if constexpr (std::is_same<T, std::string>::value ||
                      std::is_same<T, const char*>::value) {
            debug() << "\"";
            pr = true;
        }
        if((vec.size() - i) > 1) {
            cm = true;
        }
        debug() << vec[i];
        if(pr) debug() << "\"";
        if(cm) debug() << ", ";
    }
    debug() << "]\n";
}


// Runs given function over a container.
template<typename T, typename A>
A map(T func, A &&vec) {
    std::transform(vec.begin(), vec.end(), vec.begin(), func);
    return vec;
}


// Executes given function silently.

// FIXME:
// Cannot handle function with default arguments.
// Requires complete argument list.
template<typename T>
auto silent(T func) {
    auto r_func = [=](auto ...args) {
        int out_pipe[2];
        int saved_stdout, saved_stderr;

        saved_stdout = dup(STDOUT_FILENO);
        saved_stderr = dup(STDERR_FILENO);
        if(pipe(out_pipe) != 0) {
            std::cout << "Cannot open pipe in decorator!" << std::endl;
            exit(1);
        }

        dup2(out_pipe[1], STDOUT_FILENO);
        dup2(out_pipe[1], STDERR_FILENO);
        auto res = func(args...);

        close(out_pipe[1]);
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stderr, STDERR_FILENO);
        return res;
    };
    return r_func;
}

template<typename T>
auto no_errors(T func) {
    auto r_func = [=](auto ...args) {
        int out_pipe[2];
        int saved_stderr;

        saved_stderr = dup(STDERR_FILENO);
        if(pipe(out_pipe) != 0) {
            std::cout << "Cannot open pipe in decorator!" << std::endl;
            exit(1);
        }

        dup2(out_pipe[1], STDERR_FILENO);
        auto res = func(args...);

        close(out_pipe[1]);
        dup2(saved_stderr, STDERR_FILENO);
        return res;
    };
    return r_func;
}

// Executes given function,
// Returns all output send to stdout
// While processing function.

// Does not work as expected.
// Returns needed string with ovehead.
template<typename T>
auto get_output(T func) {
    auto r_func = [=](auto... args) {
        std::array<char, 128> buffer;
        int out_pipe[2];
        int saved_stdout;

        saved_stdout = dup(STDOUT_FILENO);
        if(pipe(out_pipe) != 0) {
            std::cout << "Cannot open pipe in decorator!" << std::endl;
            exit(1);
        }

        dup2(out_pipe[1], STDOUT_FILENO);
        func(args...);

        fflush(stdout);
        close(out_pipe[1]);
        dup2(saved_stdout, STDOUT_FILENO);

        std::string output;
        while(read(out_pipe[0], buffer.data(), buffer.size()) != 0) {
            output += buffer.data();
        }
        return output;
    };
    return r_func;
}
#endif
