#pragma once

#include "Graph/GraphTable.h"
#include "MCM/MCMTable.h"
#include "Serial/Manager.h"
#include "Sound/SoundTable.h"
#include "Trait/TraitTable.h"
#include "Util/StringUtil.h"

namespace PapyrusData {
	using VM = RE::BSScript::IVirtualMachine;

	uint32_t GetUndressingSlotMask(RE::StaticFunctionTag*) {
		return MCM::MCMTable::getUndressingMask();
	}

	void SetUndressingSlotMask(RE::StaticFunctionTag*, uint32_t mask) {
		MCM::MCMTable::setUndressingMask(mask); }


    std::vector<std::string> PairsToNames(RE::StaticFunctionTag*, std::vector<std::string> pairs) {
        std::vector<std::string> names;
        for (int i = 1; i < pairs.size(); i += 2) {
            names.push_back(pairs[i]);
        }
        return names;
    }


#pragma region equip_objects
	std::vector<std::string> GetEquipObjectTypes(RE::StaticFunctionTag*) {
        return Trait::TraitTable::getEquipObjectTypes();
	}

	std::vector<std::string> GetEquipObjectPairs(RE::StaticFunctionTag*, int formID, std::string type) {
        StringUtil::toLower(&type);
        return Trait::TraitTable::getEquipObjectPairs(formID, type);
	}

	std::string GetEquipObjectName(RE::StaticFunctionTag*, int formID, std::string type) {
        StringUtil::toLower(&type);
        return Trait::TraitTable::getEquipObjectName(formID, type);
	}

	void SetEquipObjectID(RE::StaticFunctionTag*, int formID, std::string type, std::string id) {
        Trait::TraitTable::setEquipObjectID(formID, type, id);
	}
#pragma endregion

#pragma region voice
	std::vector<std::string> GetVoiceSetPairs(RE::StaticFunctionTag*) {
		return Sound::SoundTable::getVoiceSetPairs();
	}

	std::string GetVoiceSetName(RE::StaticFunctionTag*, int formID) {
        return Sound::SoundTable::getVoiceSetName(formID);
	}

	void SetVoiceSet(RE::StaticFunctionTag*, int formID, std::string voice) {
        Sound::SoundTable::setVoiceSet(formID, voice);
	}
#pragma endregion

#pragma region preferences
#pragma region actions
	float GetActionActorStimulation(RE::StaticFunctionTag*, int formID, std::string action) {
        float stimulation = Serialization::getActionActorStimulation(formID, action);
        if (!std::isnan(stimulation)) {
            return stimulation;
		}

        Graph::ActionAttributes* attributes = Graph::GraphTable::GetActionAttributesByType(action);
        if (!attributes) {
            return 0.0f;
		}

		return attributes->actor.stimulation;
    }

    void SetActionActorStimulation(RE::StaticFunctionTag*, int formID, std::string action, float stimulation) {
        Serialization::setActionActorStimulation(formID, action, stimulation);
    }

    float GetActionActorMaxStimulation(RE::StaticFunctionTag*, int formID, std::string action) {
        float stimulation = Serialization::getActionActorMaxStimulation(formID, action);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::ActionAttributes* attributes = Graph::GraphTable::GetActionAttributesByType(action);
        if (!attributes) {
            return 0.0f;
        }

        return attributes->actor.maxStimulation;
    }

    void SetActionActorMaxStimulation(RE::StaticFunctionTag*, int formID, std::string action, float stimulation) {
        Serialization::setActionActorMaxStimulation(formID, action, stimulation);
    }

    float GetActionTargetStimulation(RE::StaticFunctionTag*, int formID, std::string action) {
        float stimulation = Serialization::getActionTargetStimulation(formID, action);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::ActionAttributes* attributes = Graph::GraphTable::GetActionAttributesByType(action);
        if (!attributes) {
            return 0.0f;
        }

        return attributes->target.stimulation;
    }

    void SetActionTargetStimulation(RE::StaticFunctionTag*, int formID, std::string action, float stimulation) {
        Serialization::setActionTargetStimulation(formID, action, stimulation);
    }

    float GetActionTargetMaxStimulation(RE::StaticFunctionTag*, int formID, std::string action) {
        float stimulation = Serialization::getActionTargetMaxStimulation(formID, action);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::ActionAttributes* attributes = Graph::GraphTable::GetActionAttributesByType(action);
        if (!attributes) {
            return 0.0f;
        }

        return attributes->target.maxStimulation;
    }

    void SetActionTargetMaxStimulation(RE::StaticFunctionTag*, int formID, std::string action, float stimulation) {
        Serialization::setActionTargetMaxStimulation(formID, action, stimulation);
    }

    float GetActionPerformerStimulation(RE::StaticFunctionTag*, int formID, std::string action) {
        float stimulation = Serialization::getActionPerformerStimulation(formID, action);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::ActionAttributes* attributes = Graph::GraphTable::GetActionAttributesByType(action);
        if (!attributes) {
            return 0.0f;
        }

        return attributes->performer.stimulation;
    }

    void SetActionPerformerStimulation(RE::StaticFunctionTag*, int formID, std::string action, float stimulation) {
        Serialization::setActionPerformerStimulation(formID, action, stimulation);
    }

    float GetActionPerformerMaxStimulation(RE::StaticFunctionTag*, int formID, std::string action) {
        float stimulation = Serialization::getActionPerformerMaxStimulation(formID, action);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::ActionAttributes* attributes = Graph::GraphTable::GetActionAttributesByType(action);
        if (!attributes) {
            return 0.0f;
        }

        return attributes->performer.maxStimulation;
    }

    void SetActionPerformerMaxStimulation(RE::StaticFunctionTag*, int formID, std::string action, float stimulation) {
        Serialization::setActionPerformerMaxStimulation(formID, action, stimulation);
    }
#pragma endregion

#pragma region events
    float GetEventActorStimulation(RE::StaticFunctionTag*, int formID, std::string evt) {
        float stimulation = Serialization::getEventActorStimulation(formID, evt);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::Event* graphEvent = Graph::GraphTable::getEvent(evt);
        if (!graphEvent) {
            return 0.0f;
        }

        return graphEvent->actor.stimulation;
    }

    void SetEventActorStimulation(RE::StaticFunctionTag*, int formID, std::string evt, float stimulation) {
        Serialization::setEventActorStimulation(formID, evt, stimulation);
    }

    float GetEventActorMaxStimulation(RE::StaticFunctionTag*, int formID, std::string evt) {
        float stimulation = Serialization::getEventActorMaxStimulation(formID, evt);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::Event* graphEvent = Graph::GraphTable::getEvent(evt);
        if (!graphEvent) {
            return 0.0f;
        }

        return graphEvent->actor.maxStimulation;
    }

    void SetEventActorMaxStimulation(RE::StaticFunctionTag*, int formID, std::string evt, float stimulation) {
        Serialization::setEventActorMaxStimulation(formID, evt, stimulation);
    }

    float GetEventTargetStimulation(RE::StaticFunctionTag*, int formID, std::string evt) {
        float stimulation = Serialization::getEventTargetStimulation(formID, evt);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::Event* graphEvent = Graph::GraphTable::getEvent(evt);
        if (!graphEvent) {
            return 0.0f;
        }

        return graphEvent->target.stimulation;
    }

    void SetEventTargetStimulation(RE::StaticFunctionTag*, int formID, std::string evt, float stimulation) {
        Serialization::setEventTargetStimulation(formID, evt, stimulation);
    }

    float GetEventTargetMaxStimulation(RE::StaticFunctionTag*, int formID, std::string evt) {
        float stimulation = Serialization::getEventTargetMaxStimulation(formID, evt);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::Event* graphEvent = Graph::GraphTable::getEvent(evt);
        if (!graphEvent) {
            return 0.0f;
        }

        return graphEvent->target.maxStimulation;
    }

    void SetEventTargetMaxStimulation(RE::StaticFunctionTag*, int formID, std::string evt, float stimulation) {
        Serialization::setEventTargetMaxStimulation(formID, evt, stimulation);
    }

    float GetEventPerformerStimulation(RE::StaticFunctionTag*, int formID, std::string evt) {
        float stimulation = Serialization::getEventPerformerStimulation(formID, evt);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::Event* graphEvent = Graph::GraphTable::getEvent(evt);
        if (!graphEvent) {
            return 0.0f;
        }

        return graphEvent->performer.stimulation;
    }

    void SetEventPerformerStimulation(RE::StaticFunctionTag*, int formID, std::string evt, float stimulation) {
        Serialization::setEventPerformerStimulation(formID, evt, stimulation);
    }

    float GetEventPerformerMaxStimulation(RE::StaticFunctionTag*, int formID, std::string evt) {
        float stimulation = Serialization::getEventPerformerMaxStimulation(formID, evt);
        if (!std::isnan(stimulation)) {
            return stimulation;
        }

        Graph::Event* graphEvent = Graph::GraphTable::getEvent(evt);
        if (!graphEvent) {
            return 0.0f;
        }

        return graphEvent->performer.maxStimulation;
    }

    void SetEventPerformerMaxStimulation(RE::StaticFunctionTag*, int formID, std::string evt, float stimulation) {
        Serialization::setEventPerformerMaxStimulation(formID, evt, stimulation);
    }
#pragma endregion
#pragma endregion

	void ResetSettings(RE::StaticFunctionTag*) {
		MCM::MCMTable::restoreDefaults();
	}

	void ExportSettings(RE::StaticFunctionTag*) {
		MCM::MCMTable::exportSettings();
	}

	void ImportSettings(RE::StaticFunctionTag*) {
		MCM::MCMTable::importSettings();
	}


	bool Bind(VM* a_vm) {
        const auto obj = "OData"sv;

		BIND(GetUndressingSlotMask);
        BIND(SetUndressingSlotMask);

        BIND(PairsToNames);

		BIND(GetEquipObjectTypes);
		BIND(GetEquipObjectPairs);
        BIND(GetEquipObjectName);
        BIND(SetEquipObjectID);

		BIND(GetVoiceSetPairs);
        BIND(GetVoiceSetName);
        BIND(SetVoiceSet);

        BIND(GetActionActorStimulation);
        BIND(SetActionActorStimulation);
        BIND(GetActionActorMaxStimulation);
        BIND(SetActionActorMaxStimulation);
        BIND(GetActionTargetStimulation);
        BIND(SetActionTargetStimulation);
        BIND(GetActionTargetMaxStimulation);
        BIND(SetActionTargetMaxStimulation);
        BIND(GetActionPerformerStimulation);
        BIND(SetActionPerformerStimulation);
        BIND(GetActionPerformerMaxStimulation);
        BIND(SetActionPerformerMaxStimulation);

        BIND(GetEventActorStimulation);
        BIND(SetEventActorStimulation);
        BIND(GetEventActorMaxStimulation);
        BIND(SetEventActorMaxStimulation);
        BIND(GetEventTargetStimulation);
        BIND(SetEventTargetStimulation);
        BIND(GetEventTargetMaxStimulation);
        BIND(SetEventTargetMaxStimulation);
        BIND(GetEventPerformerStimulation);
        BIND(SetEventPerformerStimulation);
        BIND(GetEventPerformerMaxStimulation);
        BIND(SetEventPerformerMaxStimulation);

		BIND(ResetSettings);
        BIND(ExportSettings);
        BIND(ImportSettings);

        return true;
	}
}