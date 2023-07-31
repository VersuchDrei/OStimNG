#include "NPCThreadStarter.h"

#include "Core/ThreadManager.h"

#include "Furniture/Furniture.h"
#include "Graph/GraphTable.h"
#include "MCM/MCMTable.h"
#include "Util/ActorUtil.h"

namespace OStim {
    int startNPCThread(ThreadStartParams& params) {
        if (!params.noFurniture && !params.furniture && MCM::MCMTable::useFurniture()) {
            if (params.startingNode) {
                params.furniture = Furniture::findBed(params.actors[0].form, MCM::MCMTable::furnitureSearchDistance(), 96.0f);
            } else {
                std::vector<RE::TESObjectREFR*> furniture = Furniture::findFurniture(params.actors.size(), params.actors[0].form, MCM::MCMTable::furnitureSearchDistance(), 96.0f);
                for (RE::TESObjectREFR* object : furniture) {
                    if (object) {
                        params.furniture = object;
                        break;
                    }
                }
            }
        }

        if (!params.startingNode && !params.startingSequence) {
            ActorUtil::sort(params.actors, params.dominantActors, -1);
        }

        if (!params.startingNode && !params.startingSequence) {
            Furniture::FurnitureType furnitureType = Furniture::getFurnitureType(params.furniture, false);
            switch (furnitureType) {
            case Furniture::FurnitureType::NONE:
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorCondition::create(params.actors), [&](Graph::Node* node) { return node->hasActorTagOnAny("standing"); });
                break;
            case Furniture::FurnitureType::BED:
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorCondition::create(params.actors), [&](Graph::Node* node) { return !node->hasActorTagOnAny("standing"); });
                break;
            default:
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorCondition::create(params.actors), [&](Graph::Node*) { return true; });
                break;
            }
        }

        if (!params.startingNode && !params.startingSequence) {
            return -1;
        }

        return ThreadManager::GetSingleton()->startThread(params);

        return -1;
    }
}