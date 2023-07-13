#pragma once

#include "GameAPI/GameActor.h"
#include "Graph/Node.h"

namespace OStim {
    struct ThreadStartParams {
        std::vector<GameAPI::GameActor> actors;
        std::vector<GameAPI::GameActor> dominantActors;
        Graph::Node* startingNode = nullptr;
        RE::TESObjectREFR* furniture = nullptr;
        bool stripActors = false;
        bool noAutoMode = false;
        bool noFurniture = false;
        std::vector<std::string> metadata;

        // legacy support
        int threadID = -1;
    };
}