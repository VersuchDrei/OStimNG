#pragma once

#include "GameAPI/GameSex.h"
#include "Trait/Condition.h"
#include "Trait/FacialExpression.h"

namespace Graph {
    struct GraphActor {
    public:
        int sosBend = 0;
        float scale = 1.0;
        float scaleHeight = 120.748;
        bool feetOnGround = false;
        int expressionAction = -1;
        int animationIndex = -1;
        std::string expressionOverride = "";
        bool moan = false;
        bool talk = false;
        bool muffled = false;
        std::unordered_map<int, Trait::FaceModifier> eyeballModifierOverride;
        std::vector<std::string> tags;
        Trait::ActorCondition condition;
        std::unordered_map<std::string, std::string> autoTransitions;
    };
}