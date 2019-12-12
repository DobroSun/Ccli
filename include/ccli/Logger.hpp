#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <ostream>

class Logger {
public:
    template<typename T>
    inline Logger& operator<<(T str) {
    #ifdef DEBUG
        std::cout << str;
    #endif
        return *this;
    }
    inline Logger& operator<<(std::ostream& (*fun)(std::ostream&)) {
    #ifdef DEBUG
        std::cout << std::endl;
    #endif
        return *this;
    }
} debug;
#endif
