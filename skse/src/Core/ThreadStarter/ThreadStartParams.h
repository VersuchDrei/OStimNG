#pragma once

#include "GameAPI/GameActor.h"
#include "Graph/Node.h"

namespace OStim {
    struct ThreadStartParams {
        std::vector<GameAPI::GameActor> actors;
        Graph::Node* startingNode;
        RE::TESObjectREFR* furniture;
        std::vector<std::string> metadata;
        std::vector<GameAPI::GameActor> dominantActors;
    };
}