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
            if (!OStim::isEligible(actor)) {
                return -1;
            }
            gameActors.push_back(actor);
        }

        OStim::ThreadStartParams params;
        params.actors = gameActors;

        return OStim::ThreadBuilder::add(params);
    }

    void SetDominantActors(RE::StaticFunctionTag*, int builderID, std::vector<RE::Actor*> actors) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->dominantActors = GameAPI::GameActor::convertVector(actors);
    }

    void SetFurniture(RE::StaticFunctionTag*, int builderID, RE::TESObjectREFR* furniture) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->furniture = furniture;
    }

    void SetStartingAnimation(RE::StaticFunctionTag*, int builderID, std::string animation) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        Graph::Node* node = Graph::GraphTable::getNodeById(animation);
        if (!node) {
            return;
        }

        params->startingNode = node;
    }

    void SetStartingSequence(RE::StaticFunctionTag*, int builderID, std::string sequence) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        Graph::Sequence* sequencePtr = Graph::GraphTable::getSequenceByID(sequence);
        if (!sequencePtr) {
            return;
        }

        params->startingSequence = sequencePtr;
    }

    void EndAfterSequence(RE::StaticFunctionTag*, int builderID) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->endAfterSequence = true;
    }

    void UndressActors(RE::StaticFunctionTag*, int builderID) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->stripActors = true;
    }

    void NoAutoMode(RE::StaticFunctionTag*, int builderID) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->noAutoMode = true;
    }

    void NoFurniture(RE::StaticFunctionTag*, int builderID) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        params->noFurniture = true;
    }

    void SetMetadata(RE::StaticFunctionTag*, int builderID, std::vector<std::string> metadata) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        StringUtil::toLower(&metadata);
        params->metadata = metadata;
    }

    void SetMetadataCSV(RE::StaticFunctionTag*, int builderID, std::string metadata) {
        OStim::ThreadStartParams* params = OStim::ThreadBuilder::get(builderID);
        if (!params) {
            return;
        }

        StringUtil::toLower(&metadata);
        std::vector<std::string> metadataVector = StringUtil::toTagVector(metadata);
        params->metadata = metadataVector;
    }


    int Start(RE::StaticFunctionTag*, int builderID) {
        OStim::ThreadStartParams* paramsPtr = OStim::ThreadBuilder::get(builderID);
        if (!paramsPtr) {
            return -1;
        }

        OStim::ThreadStartParams params = *paramsPtr;
        OStim::ThreadBuilder::remove(builderID);
        return OStim::startThread(params);
    }

    void Cancel(RE::StaticFunctionTag*, int builderID) {
        OStim::ThreadBuilder::remove(builderID);
    }


    bool Bind(VM* a_vm) {
        const auto obj = "OThreadBuilder"sv;

        BIND(Create);
        BIND(SetDominantActors);
        BIND(SetFurniture);
        BIND(SetStartingAnimation);
        BIND(SetStartingSequence);
        BIND(EndAfterSequence);
        BIND(UndressActors);
        BIND(NoAutoMode);
        BIND(SetMetadata);
        BIND(SetMetadataCSV);

        BIND(Start);
        BIND(Cancel);

        return true;
    }
}