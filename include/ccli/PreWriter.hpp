#ifndef PREWRITER_CPP
#define PREWRITER_CPP

#include "ccli/IWriter.hpp"

class PreWriter: public Writer {
    std::vector<std::string> commands;
public:
    void put_command(std::string cmd);
    void remove_command(std::string cmd);
    void remove_error(std::string cmd);
    std::string get_commands();
};
#endif
