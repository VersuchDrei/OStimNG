#pragma once

#include "Core/ThreadStarter/ThreadBuilder.h"
#include "Core/ThreadStarter/ThreadStarter.h"
#include "Core/Core.h"
#include "GameAPI/GameActor.h"
#include "Graph/GraphTable.h"
#include "Util/StringUtil.h"

namespace PapyrusThreadBuilder {
    using VM = RE::BSScript::IVirtualMachine;

    int Create(RE::StaticFunctionTag*, std::vector<RE::Actor*> actors) {
        std::vector<GameAPI::GameActor> gameActors;
        for (RE::Actor* actor : actors) {
            if (!Threading::isEligible(actor)) {
                return -1;
            }
            gameActors.push_back(actor);
        }

        Threading::ThreadStartParams params;
        params.actors = gameActors;

        return Threading::ThreadBuilder::add(params);
    }

    void SetDominantActors(RE::StaticFunctionTag*, int builderID, std::vector<RE::Actor*> actors) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->dominantActors = GameAPI::GameActor::convertVector(actors);
    }

    void SetFurniture(RE::StaticFunctionTag*, int builderID, RE::TESObjectREFR* furniture) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->furniture = furniture;
    }

    void SetDuration(RE::StaticFunctionTag*, int builderID, float duration) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->duration = duration * 1000.0f;
    }

    void SetStartingAnimation(RE::StaticFunctionTag*, int builderID, std::string animation) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        Graph::Node* node = Graph::GraphTable::getNodeById(animation);
        if (!node) {
            logger::warn("animation {} could not be found", animation);
            return;
        }

        params->startingNodes = {{node->animationLengthMs, node}};
    }

    void AddStartingAnimation(RE::StaticFunctionTag*, int builderID, std::string animation, float duration, bool navigateTo) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        Graph::Node* node = Graph::GraphTable::getNodeById(animation);
        if (!node) {
            logger::warn("animation {} could not be found", animation);
            return;
        }

        if (navigateTo && !params->startingNodes.empty() && params->startingNodes.back().node != node) {
            std::vector<Graph::SequenceEntry> path = params->startingNodes.back().node->getRoute(MCM::MCMTable::navigationDistanceMax(), Trait::ActorCondition::create(params->actors), node);
            if (path.empty()) {
                params->startingNodes.push_back({duration > 0.0f ? static_cast<int>(duration * 1000) : node->animationLengthMs, node});
            } else {
                for (Graph::SequenceEntry& entry : path) {
                    params->startingNodes.push_back(entry);
                }

                if (duration > 0.0f) {
                    params->startingNodes.back().duration = duration;
                }
            }
            
        } else {
            params->startingNodes.push_back({duration > 0.0f ? static_cast<int>(duration * 1000) : node->animationLengthMs, node});
        }
    }

    void SetStartingSequence(RE::StaticFunctionTag*, int builderID, std::string sequence) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        Graph::Sequence* sequencePtr = Graph::GraphTable::getSequenceByID(sequence);
        if (!sequencePtr) {
            logger::warn("sequence {} could not be found", sequence);
            return;
        }

        params->startingNodes.clear();
        for (Graph::SequenceEntry& entry : sequencePtr->nodes) {
            params->startingNodes.push_back(entry);
        }
    }

    void ConcatStartingSequence(RE::StaticFunctionTag*, int builderID, std::string sequence, bool navigateTo) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        Graph::Sequence* sequencePtr = Graph::GraphTable::getSequenceByID(sequence);
        if (!sequencePtr) {
            logger::warn("sequence {} could not be found", sequence);
            return;
        }

        if (navigateTo && !params->startingNodes.empty() && params->startingNodes.back().node != sequencePtr->nodes.front().node) {
            std::vector<Graph::SequenceEntry> path = params->startingNodes.back().node->getRoute( MCM::MCMTable::navigationDistanceMax(), Trait::ActorCondition::create(params->actors), sequencePtr->nodes.front().node);
            if (!path.empty()) {
                for (Graph::SequenceEntry& entry : path) {
                    params->startingNodes.push_back(entry);
                }
                params->startingNodes.pop_back();
            }
        }

        for (Graph::SequenceEntry& entry : sequencePtr->nodes) {
            params->startingNodes.push_back(entry);
        }
    }

    void EndAfterSequence(RE::StaticFunctionTag*, int builderID) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->endAfterSequence = true;
    }

    void UndressActors(RE::StaticFunctionTag*, int builderID) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->threadFlags |= Threading::ThreadFlag::UNDRESS;
    }

    void NoAutoMode(RE::StaticFunctionTag*, int builderID) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->threadFlags |= Threading::ThreadFlag::NO_AUTO_MODE;
    }

    void NoPlayerControl(RE::StaticFunctionTag*, int builderID) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->threadFlags |= Threading::ThreadFlag::NO_PLAYER_CONTROL;
    }

    void NoUndressing(RE::StaticFunctionTag*, int builderID) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->threadFlags |= Threading::ThreadFlag::NO_UNDRESSING;
    }

    void NoFurniture(RE::StaticFunctionTag*, int builderID) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->noFurniture = true;
    }

    void SetMetadata(RE::StaticFunctionTag*, int builderID, std::vector<std::string> metadata) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        StringUtil::toLower(&metadata);
        params->metadata = metadata;
    }

    void SetMetadataCSV(RE::StaticFunctionTag*, int builderID, std::string metadata) {
        Threading::ThreadStartParams* params = Threading::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        StringUtil::toLower(&metadata);
        std::vector<std::string> metadataVector = StringUtil::toTagVector(metadata);
        params->metadata = metadataVector;
    }


    int Start(RE::StaticFunctionTag*, int builderID) {
        Threading::ThreadStartParams* paramsPtr = Threading::ThreadBuilder::get(builderID);
        if (!paramsPtr) {
            return -1;
        }

        Threading::ThreadStartParams params = *paramsPtr;
        Threading::ThreadBuilder::remove(builderID);
        return Threading::startThread(params);
    }

    void Cancel(RE::StaticFunctionTag*, int builderID) {
        Threading::ThreadBuilder::remove(builderID);
    }


    bool Bind(VM* a_vm) {
        const auto obj = "OThreadBuilder"sv;

        BIND(Create);
        BIND(SetDominantActors);
        BIND(SetFurniture);
        BIND(SetDuration);
        BIND(SetStartingAnimation);
        BIND(AddStartingAnimation);
        BIND(SetStartingSequence);
        BIND(ConcatStartingSequence);
        BIND(EndAfterSequence);
        BIND(UndressActors);
        BIND(NoAutoMode);
        BIND(NoPlayerControl);
        BIND(NoUndressing);
        BIND(NoFurniture);
        BIND(SetMetadata);
        BIND(SetMetadataCSV);

        BIND(Start);
        BIND(Cancel);

        return true;
    }
}