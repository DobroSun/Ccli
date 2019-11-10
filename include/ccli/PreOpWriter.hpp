#ifndef PREOPWRITER_CPP
#define PREOPWRITER_CPP

#include "ccli/IOpWriter.hpp"

class PreOpWriter: public OpWriter {
    std::vector<std::string> commands;
    int opened_pars = 0;
    int lines_to_clear = 0;
public:
    void put_command(std::string cmd);
    void remove_command(std::string cmd);
    void remove_error(std::string cmd);
    std::string get_commands();

    void clear();
    void increase_pars();
    void decrease_pars();
};
#endif
