#include "GameTable.h"

namespace GameAPI {
    void GameTable::setup() {
        RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();

        Timescale = handler->LookupForm<RE::TESGlobal>(0x00003A, "Skyrim.esm");

        ActorTypeNPC = handler->LookupForm<RE::BGSKeyword>(0x013794, "Skyrim.esm");
        ManakinRace = handler->LookupForm<RE::TESRace>(0x10760A, "Skyrim.esm");

        FadeToBlackImod = handler->LookupForm<RE::TESImageSpaceModifier>(0x0F756D, "Skyrim.esm");
        FadeToBlackHoldImod = handler->LookupForm<RE::TESImageSpaceModifier>(0x0F756E, "Skyrim.esm");
        FadeToBlackBackImod = handler->LookupForm<RE::TESImageSpaceModifier>(0x0F756F, "Skyrim.esm");

        OStimImprovedCamSupport = handler->LookupForm<RE::TESGlobal>(0xDE6, "OStim.esp");
    }
}