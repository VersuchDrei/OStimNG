#pragma once

#include "Trait/Condition.h"
#include "Trait/FacialExpression.h"

namespace Graph {
    struct GraphActor {
    public:
        uint32_t requirements;
        int sosBend = 0;
        float scale = 1.0;
        float scaleHeight = 120.748;
        bool feetOnGround = false;
        int expressionAction = -1;
        std::string expressionOverride = "";
        bool moan = false;
        bool talk = false;
        bool muffled = false;
        std::unordered_map<int, Trait::FaceModifier> eyeballModifierOverride;
        std::vector<std::string> tags;
        Trait::ActorConditions conditions;
        std::unordered_map<std::string, std::string> autotransitions;
    };
}