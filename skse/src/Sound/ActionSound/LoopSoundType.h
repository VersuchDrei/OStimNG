#pragma once

#include "SoundType.h"

namespace Sound {
    class LoopSoundType : public SoundType{
    public:
        inline LoopSoundType(GameAPI::GameSound sound, bool muteWithActor, bool muteWithTarget, int delay)
            : SoundType(sound, muteWithActor, muteWithTarget), delay{delay} {};

        virtual SoundPlayer* create(OStim::ThreadActor* actor, OStim::ThreadActor* target);

    private:
        int delay;
    };
}