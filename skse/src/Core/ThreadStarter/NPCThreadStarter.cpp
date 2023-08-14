#include "NPCThreadStarter.h"

#include "Core/ThreadManager.h"

#include "Furniture/FurnitureTable.h"
#include "Graph/GraphTable.h"
#include "MCM/MCMTable.h"
#include "Util/ActorUtil.h"

namespace OStim {
    int startNPCThread(ThreadStartParams& params) {
        if (!params.noFurniture && !params.furniture && MCM::MCMTable::useFurniture()) {
            if (params.startingNode) {
                params.furniture = Furniture::findBed(params.actors[0].form, MCM::MCMTable::furnitureSearchDistance(), 96.0f);
            } else {
                std::vector<std::pair<Furniture::FurnitureType*, GameAPI::GameObject>> furniture = Furniture::findFurniture(params.actors.size(), params.actors[0].form, MCM::MCMTable::furnitureSearchDistance(), 96.0f);
                if (!furniture.empty()) {
                    params.furniture = furniture.front().second;
                }
            }
        }

        if (!params.startingNode && !params.startingSequence) {
            ActorUtil::sort(params.actors, params.dominantActors, -1);
        }

        if (!params.startingNode && !params.startingSequence) {
            Furniture::FurnitureType* furnitureType = Furniture::FurnitureTable::getFurnitureType(params.furniture, false);
            std::string furnitureTypeID = furnitureType->getListType()->id;

            if (furnitureTypeID == "none") {
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorCondition::create(params.actors), [&](Graph::Node* node) { return node->hasActorTagOnAny("standing"); });
            } else if (furnitureTypeID == "bed") {
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorCondition::create(params.actors), [&](Graph::Node* node) { return !node->hasActorTagOnAny("standing"); });
            } else {
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorCondition::create(params.actors), [&](Graph::Node*) { return true; });
            }
        }

        if (!params.startingNode && !params.startingSequence) {
            return -1;
        }

        return ThreadManager::GetSingleton()->startThread(params);

        return -1;
    }
}