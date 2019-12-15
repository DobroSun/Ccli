#include <vector>
#include <iostream>

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
