#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <ostream>

class Logger {
public:
    Logger& operator<<(const char* str) {
    #ifdef DEBUG
        if(DEBUG) std::cout << str;
    #endif
        return *this;
    }
    Logger& operator<<(std::ostream& (*fun)(std::ostream&)) {
    #ifdef DEBUG
        std::cout << std::endl;
    #endif
        return *this;
    }
} debug;
#endif
