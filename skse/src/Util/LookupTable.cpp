#include "LookupTable.h"

namespace Util {
    void LookupTable::setupForms() {
        auto handler = RE::TESDataHandler::GetSingleton();
        OSexIntegrationMainQuest = handler->LookupForm<RE::TESQuest>(0x801, "OStim.esp");
        OStimVehicle = handler->LookupForm<RE::TESObjectSTAT>(0xDD6, "OStim.esp");
        OStimScenePackage = handler->LookupForm<RE::TESPackage>(0xDE1, "OStim.esp");
        OStimEmptyFaction = handler->LookupForm<RE::TESFaction>(0xDED, "OStim.esp");

        OStimMoanFemaleSD = handler->LookupForm<RE::BGSSoundDescriptorForm>(0xE08, "OStim.esp");
        OStimMoanMaleSD = handler->LookupForm<RE::BGSSoundDescriptorForm>(0xE09, "OStim.esp");
    }
}