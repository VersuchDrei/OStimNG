# pragma once

#include "GameActor.h"
#include "GameAPIUtil.h"
#include "GameRecord.h"

namespace GameAPI {
    struct GameSound : GameRecord<RE::BGSSoundDescriptorForm> {
    public:
        void play(GameActor actor, float volume);
    };
}