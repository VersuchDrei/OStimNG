#pragma once

#include "BoneDistanceSoundPlayer.h"

#include "GameAPI/GameActorBone.h"

namespace Sound {
    class SingleBoneDistanceSoundPlayer : public BoneDistanceSoundPlayer {
    public:
        inline SingleBoneDistanceSoundPlayer(GameAPI::GameSound sound, bool muteWithActor, bool muteWithTarget, OStim::ThreadActor* actor, OStim::ThreadActor* target, bool inverse, int minInterval, int maxInterval, GameAPI::GameActorBone actorBone, GameAPI::GameActorBone targetBone)
            : BoneDistanceSoundPlayer(sound, muteWithActor, muteWithTarget, actor, target, inverse, minInterval, maxInterval), actorBone{actorBone}, targetBone{targetBone} {}

    protected:
        virtual float calculateDistance();

    private:
        GameAPI::GameActorBone actorBone;
        GameAPI::GameActorBone targetBone;
    };
}