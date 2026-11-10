#pragma once

#include "GameAPI/GameActor.h"

namespace Frameworks {
    namespace PPA {
        enum PPAAction {
            ANUS,
            HAND_L,
            HAND_R,
            HANDS,
            MOUTH,
            NONE,
            UNDEFINED,
            VAGINA,
        };

        namespace PPAActionAPI {
            PPAAction fromString(std::string action);
        }

        void setUp();
    }
}