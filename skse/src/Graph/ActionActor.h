#pragma once

#include "GameAPI/GameFaction.h"

namespace Graph {
    struct ActionActor {
    public:
        float stimulation = 0.0;
        float maxStimulation = 100.0;
        bool fullStrip = false;
        bool moan = false;
        bool talk = false;
        bool muffled = false;
        std::string expressionOverride = "";
        std::set<std::string> requirements;
        uint32_t strippingMask = 0;
        std::vector<GameAPI::GameFaction> factions;
        std::unordered_map<std::string, int> ints;
        std::unordered_map<std::string, std::vector<int>> intLists;
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, std::vector<float>> floatLists;
        std::unordered_map<std::string, std::string> strings;
        std::unordered_map<std::string, std::vector<std::string>> stringLists;
    };
}