#pragma once

#include "GameAPI/GameCondition.h"

namespace Trait {
    struct ActorOverride {
    public:
        GameAPI::GameCondition condition;
        float skeletonHeight = 0;
        std::string expression = "";
        bool mute = false;
        bool muffle = false;
        uint32_t meetRequirements = 0;
        uint32_t unmeetRequirements = 0;
    };
}