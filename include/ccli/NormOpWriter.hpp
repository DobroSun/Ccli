#ifndef NORMOPWRITER_CPP
#define NORMOPWRITER_CPP

#include "ccli/IOpWriter.hpp"

class NormOpWriter: public OpWriter {
    std::vector<std::string> commands;
    int opened_pars = 0;
public:
    NormOpWriter();
    void put_command(std::string cmd);
    void remove_command(std::string cmd);
    void remove_error(std::string cmd);
    std::string get_commands();

    void clear();
    void increase_pars();
    void decrease_pars();
};
#endif
