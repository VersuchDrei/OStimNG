#pragma once

#include "SKEE.h"

namespace Util {
    class LookupTable {
    public:
        inline static RE::TESQuest* OSexIntegrationMainQuest;
        inline static RE::TESObjectSTAT* OStimVehicle;
        inline static RE::TESPackage* OStimScenePackage;
        inline static RE::TESFaction* OStimEmptyFaction;

        inline static RE::BGSSoundDescriptorForm* OStimMoanFemaleSD;
        inline static RE::BGSSoundDescriptorForm* OStimMoanMaleSD;

        inline static SKEE::INiTransformInterface* niTransformInterface = nullptr;

        static void setupForms();
    };
}