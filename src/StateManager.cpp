
#include "ccli/StateManager.hpp"
#include "ccli/Logger.hpp"

#include <utility>

namespace ccli {


StateManager::StateManager(GlobalContext &GC_) {
    MainState = std::make_unique<MainCmd>();
    DeclState = std::make_unique<DeclCmd>();

    GC = GC_;
}

void StateManager::change_state(StateScope &scope) {
    if(scope.is_decl) {
        debug() << " -> Changing state to push to decls" << std::endl;
        GC.setState(DeclState.get());
    } else {
        debug() << " -> Changing state to push to mains" << std::endl;
        GC.setState(MainState.get());
    }
}
} // namespace
