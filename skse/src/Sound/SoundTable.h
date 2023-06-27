#pragma once

#include "VoiceSet.h"

#include "GameAPI/GameActor.h"

namespace Sound {
    class SoundTable {
    public:
        static void setup();

        static VoiceSet* getVoiceSet(GameAPI::GameActor actor);

    private:
        inline static std::unordered_map<uint32_t, VoiceSet> voiceSets;
    };
}