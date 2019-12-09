#ifndef DECLMATHCER_CPP
#define DECLMATHCER_CPP
#include <string>


namespace ccli {

class DeclMatcher {
    
public:
    DeclMatcher();
    bool findDecl(std::string cmd);
    getAST(std::string cmd);
};
}
#endif
