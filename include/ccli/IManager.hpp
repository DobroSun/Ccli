#ifndef IMANAGER_CPP
#define IMANAGER_CPP

#include "ccli/ExecWriter.hpp"
#include "ccli/NormWriter.hpp"
#include "ccli/PreWriter.hpp"

#include "ccli/NormOpWriter.hpp"
#include "ccli/PreOpWriter.hpp"

#include "ccli/helpers.hpp"

class IManager {
    NormOpWriter norm_op_writer;
    PreOpWriter pre_op_writer;

    PreWriter pre_writer;
    ExecWriter exec_writer;
    NormWriter norm_writer;

    State *state;
public:
    IManager(State *_state);
    void analise_input(std::string cmd);
    void remove_command(std::string cmd);
    void remove_error(std::string cmd);

    void change_state(State _state);
    void make_file(std::ofstream& tmp_file);
    void clear_opened();
};
#endif
