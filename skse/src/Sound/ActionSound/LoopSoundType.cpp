#include "LoopSoundType.h"

#include "LoopSoundPlayer.h"

#include "Core/ThreadActor.h"

namespace Sound {
    SoundPlayer* LoopSoundType::create(OStim::ThreadActor* actor, OStim::ThreadActor* target) {
        return new LoopSoundPlayer(sound, muteWithActor, muteWithTarget, actor, target, delay);
    }
}