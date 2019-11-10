#ifndef IOPWRITER_CPP
#define IOPWRITER_CPP
#include <vector>
#include <string>

#include "ccli/helpers.hpp"

class OpWriter {
    std::vector<std::string> commands;
    int opened_pars;
public:
    virtual ~OpWriter() {}
    virtual void put_command(std::string cmd) = 0;
    virtual void remove_command(std::string cmd) = 0;
    virtual void remove_error(std::string cmd) = 0;
    virtual std::string get_commands() = 0;
    void clear();
};
#endif
