#include "LoopSoundType.h"

#include "LoopSoundPlayer.h"

#include "Core/ThreadActor.h"

namespace Sound {
    SoundPlayer* LoopSoundType::create(int actionIndex, OStim::ThreadActor* actor, OStim::ThreadActor* target) {
        return new LoopSoundPlayer({actionIndex, sound, muteWithActor, muteWithTarget, actor, target}, delay);
    }
}