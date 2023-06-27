#pragma once

#include "SoundType.h"

namespace Sound {
    class BoneDistanceSoundType : public SoundType {
    public:
        inline BoneDistanceSoundType(SoundTypeParams params, bool inverse, int minInterval, int maxInterval, std::vector<std::string> actorBones, std::vector<std::string> targetBones)
            : SoundType(params), inverse{inverse}, minInterval{minInterval}, maxInterval{maxInterval}, actorBones{actorBones}, targetBones{targetBones} {}

        virtual SoundPlayer* create(OStim::ThreadActor* actor, OStim::ThreadActor* target);

    private:
        bool inverse;
        int minInterval;
        int maxInterval;
        std::vector<std::string> actorBones;
        std::vector<std::string> targetBones;
    };
}