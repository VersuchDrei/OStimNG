#pragma once

#include "SoundType.h"

namespace Sound {
    class NodeDistanceSoundType : public SoundType {
    public:
        inline NodeDistanceSoundType(GameAPI::GameSound sound, std::vector<std::string> actorBones, std::vector<std::string> targetBones)
            : SoundType(sound), actorBones{actorBones}, targetBones{targetBones} {}

        SoundPlayer* create(OStim::ThreadActor* actor, OStim::ThreadActor* target);

    private:
        std::vector<std::string> actorBones;
        std::vector<std::string> targetBones;
    };
}