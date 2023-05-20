#pragma once

#include "GameAPI/GameSound.h"
#include "MCM/MCMTable.h"

// this is required because C++ can't do circular references in header files
namespace OStim {
    class ThreadActor;
}

namespace Sound {
    class SoundPlayer {
    public:
        inline SoundPlayer(GameAPI::GameSound sound, bool muteWithActor, bool muteWithTarget, OStim::ThreadActor* actor, OStim::ThreadActor* target)
            : sound{sound}, muteWithActor{muteWithActor}, muteWithTarget{muteWithTarget}, actor{actor}, target{target} {};

        virtual void loop() = 0;

    protected:
        GameAPI::GameSound sound;
        bool muteWithActor;
        bool muteWithTarget;
        OStim::ThreadActor* actor;
        OStim::ThreadActor* target;

        void play();
    };
}