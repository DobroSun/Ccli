#ifndef UTILS_CPP
#define UTILS_CPP

class History {
    std::string last_used;
    std::list<std::string > history;
public:
    History();
    void update_history(std::string cmd);
    void show_history();
};
#endif
