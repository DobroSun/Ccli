#ifndef IWRITERS_CPP
#define IWRITERS_CPP

bool endswith(std::string cmd, std::string ch);
bool startswith(std::string cmd, std::string ch);
bool find(std::string cmd, std::string ch);

enum State {
    All_closed,
    Norm_opened,
    Pre_opened
};

class Exe_Writer { 
    std::array<std::string, 1> commands;
public:
    Exe_Writer();
    std::string get_commands();
    void put_command(std::string cmd);
};


class Pre_Writer {
    std::vector<std::string> commands;
    int opened_parentheses;
    State state;
public:
    Pre_Writer();
    int get_pars();
};


class Norm_Writer {
    std::vector<std::string> commands;
    std::vector<std::string> expressions;

    int opened_parentheses;
    State state;
public:
    Norm_Writer();
    std::string get_commands();
    void put_command(std::string cmd);
    void put_expression(std::string cmd);
    void delete_command();
    void delete_if_not_definition(std::string cmd);
    void apply_to_commands(std::string cmd);
    void clear_expressions();

    int get_pars();
    void increase_pars();
    void decrease_pars();
};

class IManager {
    Pre_Writer pre_writer;
    Exe_Writer exe_writer;
    Norm_Writer norm_writer;
    State state;
public:
    IManager();
    void make_file(std::ofstream &tmp_file);
    void analise_input(std::string cmd);
    void remove_error(std::string cmd);
    void remove_command(std::string cmd);
    void change_state(std::string cmd);

    bool is_opened_pars();
};
#endif
