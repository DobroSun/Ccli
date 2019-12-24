#ifndef ALL_TEMPLATE_IMPLEMENTATIONS
#define ALL_TEMPLATE_IMPLEMENTATIONS

#include <vector>
#include <iostream>
#include <algorithm>

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
#endif
