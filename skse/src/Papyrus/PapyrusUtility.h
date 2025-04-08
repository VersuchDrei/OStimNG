#pragma once

namespace PapyrusUtility {
	using VM = RE::BSScript::IVirtualMachine;

	std::vector<RE::TESQuest*> GetQuestsWithGlobal(RE::StaticFunctionTag*, RE::TESGlobal* tag) {
        std::vector<RE::TESQuest*> quests;

		for (RE::TESQuest* quest : RE::TESDataHandler::GetSingleton()->GetFormArray<RE::TESQuest>()) {
            if (std::find(quest->textGlobals->begin(), quest->textGlobals->end(), tag) != quest->textGlobals->end()) {
                quests.push_back(quest);
			}
		}

		return quests;
	}

	bool Bind(VM* a_vm) {
		const auto obj = "OUtility"sv;

		BIND(GetQuestsWithGlobal);

		return true;
	}
}