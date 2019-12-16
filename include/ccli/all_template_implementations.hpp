#ifndef ALL_TEMPLATE_IMPLEMENTATIONS
#define ALL_TEMPLATE_IMPLEMENTATIONS

#include <vector>
#include <iostream>
#include <algorithm>

// Prints all items from given vector.
template<typename T>
void print(std::vector<T> &vec) {
    std::cout << "[";
    for(unsigned int i = 0; i < vec.size(); i++) {
        if((vec.size() - i) > 1)
            std::cout << vec[i] << ", ";
        else
            std::cout << vec[i];
    }
    std::cout << "]\n";
}


template<typename T, typename A>
A map(T func, A &vec) {
    std::transform(vec.begin(), vec.end(), vec.begin(), func);
    return vec;
}
#endif
