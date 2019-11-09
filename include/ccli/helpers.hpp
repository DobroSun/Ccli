#ifndef HELPERS
#define HELPERS

enum State {
    Closed,
    Opened_Norm,
    Opened_Pre
};

bool endswith(std::string cmd, std::string ch);
bool startswith(std::string cmd, std::string ch);
bool find(std::string cmd, std::string ch);
int find_place(std::string cmd, std::string ch);
#endif
