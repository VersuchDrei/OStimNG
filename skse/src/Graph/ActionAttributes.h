#pragma once

#include "ActionActor.h"
#include "RoleMap.h"

#include "Sound/ActionSound/SoundType.h"

namespace Graph {
    struct ActionAttributes {
    public:
        std::string type;
        RoleMap<ActionActor> roles;
        std::vector<Sound::SoundType*> sounds;
        std::vector<std::string> tags;

        bool hasTag(std::string tag);

        float getStimulation(Role role, GameAPI::GameActor actor);
        float getMaxStimulation(Role role, GameAPI::GameActor actor);
    };
}