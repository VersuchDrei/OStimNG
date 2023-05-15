#pragma once

#include "VoiceSet.h"

namespace Sound {
    class SoundTable {
    public:
        static void setup();

        static VoiceSet* getVoiceSet(RE::Actor* actor);

    private:
        inline static std::unordered_map<uint32_t, VoiceSet> voiceSets;
    };
}