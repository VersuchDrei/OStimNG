#include "GameTable.h"

namespace GameAPI {
    void GameTable::setup() {
        RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();
        Timescale = handler->LookupForm<RE::TESGlobal>(0x00003A, "Skyrim.esm");
        OStimImprovedCamSupport = handler->LookupForm<RE::TESGlobal>(0xDE6, "OStim.esp");
    }
}