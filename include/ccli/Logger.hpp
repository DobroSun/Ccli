#ifndef LOGGER_CPP
#define LOGGER_CPP

#include <ostream>

// Works so shitty.
// Doesn't handle numbers, ... .
// All except std::string and chars*.
// T&& and std::forwawd<T> doesn't help.
// TODO: Fix debug. It works correct only in main.cpp.
class debug {
public:
    template<class T>
    inline debug& operator<<(T str) {
    #ifdef DEBUG
        std::cout << str;
    #endif
        return *this;
    }
    inline debug& operator<<(std::ostream& (*fun)(std::ostream&)) {
    #ifdef DEBUG
        std::cout << std::endl;
    #endif
        return *this;
    }
};
#endif
