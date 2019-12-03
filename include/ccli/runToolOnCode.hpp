#ifndef CCLI_RUNTOOLONCODE
#define CCLI_RUNTOOLONCODE
#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace ccli {

class ClangTool {
    std::string file_buffer;
    std::string file_name;

    std::vector<std::pair<std::string, std::string>> VirtualFiles;
public:
    ClangTool();
    bool run(std::string cmd);
    void push_to_vf(std::string cmd);
    
};
}
#endif
