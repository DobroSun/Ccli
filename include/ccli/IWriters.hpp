#ifndef IWRITERS_CPP
#define IWRITERS_CPP

//class Pre_Writer;

class Exe_Writer { 
    std::array<std::string, 1> commands;
public:
    Exe_Writer();
    std::string get_commands();
    void put_command(std::string cmd);
};


class Norm_Writer {
    std::vector<std::string> commands;
public:
    Norm_Writer();
    std::string get_commands();
    void put_command(std::string cmd);
    void delete_command();
    void delete_if_not_definition(std::string cmd);
};


class IManager {
    //Pre_Writer pre_writer;
    Exe_Writer exe_writer;
    Norm_Writer norm_writer;
public:
    IManager();
    void make_file(std::ofstream &tmp_file);
    void analise_input(std::string cmd);
    void remove_error(std::string cmd);
    void remove_command(std::string cmd);
};
#endif
