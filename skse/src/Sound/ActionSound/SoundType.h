#pragma once

#include "SoundPlayer.h"

#include "GameAPI/GameSound.h"

namespace Sound {
    class SoundType {
    public:
        static SoundType* fromJson(std::string path, json json);

        inline SoundType(GameAPI::GameSound sound, bool muteWithActor, bool muteWithTarget)
            : sound{sound}, muteWithActor{muteWithActor}, muteWithTarget{muteWithTarget} {}

        virtual SoundPlayer* create(OStim::ThreadActor* actor, OStim::ThreadActor* target) = 0;

    protected:
        GameAPI::GameSound sound;
        bool muteWithActor;
        bool muteWithTarget;
    };
}