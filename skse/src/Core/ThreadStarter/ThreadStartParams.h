#pragma once

#include "GameAPI/GameActor.h"
#include "GameAPI/GameObject.h"
#include "Graph/Node.h"
#include "Graph/Sequence.h"

namespace OStim {
    struct ThreadStartParams {
        std::vector<GameAPI::GameActor> actors;
        std::vector<GameAPI::GameActor> dominantActors;
        GameAPI::GameObject furniture;
        Graph::Node* startingNode = nullptr;
        Graph::Sequence* startingSequence = nullptr;
        bool endAfterSequence = false;
        bool stripActors = false;
        bool noAutoMode = false;
        bool noFurniture = false;
        std::vector<std::string> metadata;
    };
}