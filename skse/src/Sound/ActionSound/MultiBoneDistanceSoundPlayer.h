#pragma once

#include "BoneDistanceSoundPlayer.h"

namespace Sound {
    class MultiBoneDistanceSoundPlayer : public BoneDistanceSoundPlayer {
    public:
        inline MultiBoneDistanceSoundPlayer(GameAPI::GameSound sound, bool muteWithActor, bool muteWithTarget, OStim::ThreadActor* actor, OStim::ThreadActor* target, bool inverse, int minInterval, int maxInterval, std::vector<GameAPI::GameActorBone> actorBones, std::vector<GameAPI::GameActorBone> targetBones)
            : BoneDistanceSoundPlayer(sound, muteWithActor, muteWithTarget, actor, target, inverse, minInterval, maxInterval), actorBones{actorBones}, targetBones{targetBones} {}

    protected:
        virtual float calculateDistance();

    private:
        std::vector<GameAPI::GameActorBone> actorBones;
        std::vector<GameAPI::GameActorBone> targetBones;
    };
}