#pragma once

#include "SoundPlayer.h"

namespace Sound {
    class LoopSoundPlayer : public SoundPlayer {
    public:
        inline LoopSoundPlayer(GameAPI::GameSound sound, bool muteWithActor, bool muteWithTarget, OStim::ThreadActor* actor, OStim::ThreadActor* target, int delay)
            : SoundPlayer(sound, muteWithActor, muteWithTarget, actor, target), delay{delay} {
            currentDelay = delay;
            play();
        }

        inline ~LoopSoundPlayer() {
            if (sound.isPlaying()) {
                sound.stop();
            }
        }

        virtual void loop();

    private:
        int delay;

        int currentDelay = 0;
    };
}