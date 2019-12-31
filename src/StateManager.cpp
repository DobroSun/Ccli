
#include "ccli/StateManager.hpp"
#include "ccli/Logger.hpp"

#include <utility>
#include <cassert>

namespace ccli {

void StateScope::change_to_main() {
    is_decl = false;
    is_main = true;
}
void StateScope::change_to_decl() {
    is_decl = true;
    is_main = false;
}


StateManager::StateManager(GlobalContext &GC_) {
    MainState = std::make_unique<MainCmd>();
    DeclState = std::make_unique<DeclCmd>();

    GC = GC_;
}

void StateManager::change_state(StateScope &scope) {
    if(scope.is_decl) {
        assert(!scope.is_main && "Is_decl -> requires is_main=0.");

        debug() << " -> DeclState" << std::endl;
        GC.setDeclState();
    } else if(scope.is_main) {
        assert(!scope.is_decl && "Is_main -> requires is_decl=0.");

        debug() << " -> MainState" << std::endl;
        GC.setMainState();
    }
}
} // namespace
