#ifndef COMPILEEXECCPP
#define COMPILEEXECCPP

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
