#ifndef ALL_TEMPLATE_IMPLEMENTATIONS
#define ALL_TEMPLATE_IMPLEMENTATIONS

#include <vector>
#include <iostream>
#include <algorithm>
#include <type_traits>

#include <unistd.h>


#include "ccli/Logger.hpp"


// Prints all items from given collection.
template<typename T>
void print(const std::vector<T> &vec) {
    debug() << "[";
    for(unsigned int i = 0; i < vec.size(); i++) {
        if((vec.size() - i) > 1)
            debug() << vec[i] << ", ";
        else
            debug() << vec[i];
    }
    debug() << "]\n";
}
template<typename T>
void print(std::vector<T> &&vec) {
    debug() << "[";
    for(unsigned int i = 0; i < vec.size(); i++) {
        if((vec.size() - i) > 1)
            debug() << vec[i] << ", ";
        else
            debug() << vec[i];
    }
    debug() << "]\n";
}

// Runs given function over a container.
template<typename T, typename A>
A map(T func, const A &vec) {
    std::transform(vec.begin(), vec.end(), vec.begin(), func);
    return vec;
}
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
        int saved_stdout;

        saved_stdout = dup(STDOUT_FILENO);
        if(pipe(out_pipe) != 0) {
            std::cout << "Cannot open pipe in decorator!" << std::endl;
            exit(1);
        }

        dup2(out_pipe[1], STDOUT_FILENO);
        auto res = func(args...);

        close(out_pipe[1]);
        dup2(saved_stdout, STDOUT_FILENO);
        return res;
    };
    return r_func;
}


// Executes given function,
// Returns all output send to stdout
// While processing function.
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
