#pragma once

#include "GameAPI/GameSound.h"
#include "MCM/MCMTable.h"

namespace Sound {
    class SoundPlayer {
    public:
        inline SoundPlayer(GameAPI::GameSound sound, GameAPI::GameActor actor, GameAPI::GameActor target) : sound{sound}, actor{actor}, target{target} {};

        virtual void loop() = 0;

        inline void play() {
            sound.play(actor, MCM::MCMTable::getSoundVolume());
        }

    private:
        GameAPI::GameSound sound;

    protected:
        GameAPI::GameActor actor;
        GameAPI::GameActor target;
    };
}