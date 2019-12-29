#ifndef STATEMANAGER_CPP
#define STATEMANAGER_CPP

#include <memory>

#include "ccli/GlobalContext.hpp"

namespace ccli {

struct StateScope {
    bool is_decl = false;
    bool is_main = true;
};

class StateManager {
    GlobalContext GC;
    
    std::unique_ptr<ContextState> MainState;
    std::unique_ptr<ContextState> DeclState;
public:
    StateManager(GlobalContext &GC_);
    void change_state(StateScope &scope);
};
} // namespace
#endif
