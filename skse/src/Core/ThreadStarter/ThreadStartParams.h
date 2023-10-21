#pragma once

#include "Core/ThreadFlag.h"

#include "GameAPI/GameActor.h"
#include "GameAPI/GameObject.h"
#include "Graph/Node.h"
#include "Graph/Sequence.h"

namespace OStim {
    struct ThreadStartParams {
        std::vector<GameAPI::GameActor> actors;
        std::vector<GameAPI::GameActor> dominantActors;
        GameAPI::GameObject furniture;
        int duration = 0;
        Graph::Node* startingNode = nullptr;
        Graph::Sequence* startingSequence = nullptr;
        bool endAfterSequence = false;
        bool stripActors = false;
        bool noFurniture = false;
        ThreadFlags threadFlags = 0;
        std::vector<std::string> metadata;
    };
}