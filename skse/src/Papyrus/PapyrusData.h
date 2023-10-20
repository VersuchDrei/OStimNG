#pragma once

#include "Sound/SoundTable.h"
#include "Trait/TraitTable.h"
#include "MCM/MCMTable.h"
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


	std::vector<std::string> GetEquipObjectPairs(RE::StaticFunctionTag*, int formID, std::string type) {
        StringUtil::toLower(&type);
        return Trait::TraitTable::getEquipObjectPairs(formID, type);
	}

	std::string GetEquipObjectName(RE::StaticFunctionTag*, int formID, std::string type) {
        StringUtil::toLower(&type);
        return Trait::TraitTable::getEquipObjectName(formID, type);
	}

	void SetEquipObjectID(RE::StaticFunctionTag*, int formID, std::string type, std::string id) {
        StringUtil::toLower(&type);
        StringUtil::toLower(&id);
        Trait::TraitTable::setEquipObjectID(formID, type, id);
	}


	std::vector<std::string> GetVoiceSetPairs(RE::StaticFunctionTag*) {
		return Sound::SoundTable::getVoiceSetPairs();
	}

	std::string GetVoiceSetName(RE::StaticFunctionTag*, int formID) {
        return Sound::SoundTable::getVoiceSetName(formID);
	}

	void SetVoiceSet(RE::StaticFunctionTag*, int formID, std::string voice) {
        Sound::SoundTable::setVoiceSet(formID, voice);
	}


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

		BIND(GetEquipObjectPairs);
        BIND(GetEquipObjectName);
        BIND(SetEquipObjectID);

		BIND(GetVoiceSetPairs);
        BIND(GetVoiceSetName);
        BIND(SetVoiceSet);

		BIND(ResetSettings);
        BIND(ExportSettings);
        BIND(ImportSettings);

        return true;
	}
}