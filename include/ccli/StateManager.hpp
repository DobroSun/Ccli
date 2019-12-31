#ifndef STATEMANAGER_CPP
#define STATEMANAGER_CPP

#include <memory>

#include "ccli/GlobalContext.hpp"

namespace ccli {

struct StateScope {
    bool is_decl = true;
    bool is_main = false;

    void change_to_main();
    void change_to_decl();
};

class StateManager {
    std::unique_ptr<ContextState> MainState;
    std::unique_ptr<ContextState> DeclState;

    GlobalContext GC;
public:
    StateManager(GlobalContext &GC_);
    void change_state(StateScope &scope);
};
} // namespace
#endif
