#ifndef CCLI_RUNTOOLONCODE
#define CCLI_RUNTOOLONCODE
#include <string>
#include <vector>
#include <memory>
#include <iostream>


namespace ccli {

class ClangTool {
public:
    ClangTool();
    bool run(std::string code);
};
}
#endif
