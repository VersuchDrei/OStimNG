#pragma once

#include "VoiceSet.h"

#include "GameAPI/GameActor.h"

namespace Sound {
    class SoundTable {
    public:
        static void setup();

        static VoiceSet getVoiceSet(GameAPI::GameActor actor);

    private:
        inline static std::unordered_map<uint32_t, VoiceSet> voiceSets;

        static void loadSoundSets(json& json, std::vector<SoundSet>& soundSets, std::string propertyName, std::string defaultExpression, std::string expressionPropertyName, std::string& fileName, std::string& filePath);
    };
}