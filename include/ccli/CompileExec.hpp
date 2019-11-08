#ifndef COMPILEEXEC_CPP
#define COMPILEEXEC_CPP

class Compiler {
public:
    bool is_compiled;

    Compiler();
    std::string compile();
};

class Executor {
public:
    Executor();
    std::string execute();
};
#endif
