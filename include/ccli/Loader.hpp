#ifndef LOADER_CPP
#define LOADER_CPP
#include <string>
#include <fstream>

class Loader {
public:
    Loader();
    void load_file(std::string filename);
};
#endif
