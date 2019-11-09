#ifndef IWRITER_CPP
#define IWRITER_CPP
#include <vector>
#include <string>

class Writer {
    std::vector<std::string> commands;
public:
    virtual ~Writer() {}
    virtual void put_command(std::string cmd) = 0;
    virtual void remove_command(std::string cmd) = 0;
    virtual void remove_error(std::string cmd) = 0;
    virtual std::string get_commands() = 0;
};
#endif
