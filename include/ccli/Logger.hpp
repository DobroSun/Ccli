#ifndef LOGGER_CPP
#define LOGGER_CPP

#include <ostream>
#include <iostream>

class debug: public std::ostream, std::streambuf {
public:
    debug(): std::ostream(this) {}
    int overflow(int c) {
        #ifdef DEBUG
        foo(c);
        #endif
        return 0;
    }
    void foo(char c) {
        std::cout.put(c);
    }
};
#endif
