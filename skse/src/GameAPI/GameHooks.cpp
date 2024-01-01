#include "GameHooks.h"

namespace GameAPI {
    void installHooks() {
        IsThirdPerson::Install();
        GetHeading::Install();
    }

    void installHooksLate() {
        PackageStart::Install();
    }
}