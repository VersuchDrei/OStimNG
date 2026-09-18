#include "GameHooks.h"

namespace GameLogic {
    void installHooks() {
        SKSE::AllocTrampoline(64);
        IsThirdPerson::Install();
        GetHeading::Install();
    }

    void installHooksPostPost() {
        PackageStart::Install();
    }
}