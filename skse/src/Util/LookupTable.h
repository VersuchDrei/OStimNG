#pragma once

namespace Util {
    class LookupTable {
    public:
        inline static RE::BGSKeyword* ActorTypeNPC;

        inline static RE::TESQuest* OSexIntegrationMainQuest;
        inline static RE::TESObjectSTAT* OStimVehicle;
        inline static RE::TESPackage* OStimScenePackage;
        inline static RE::TESFaction* OStimEmptyFaction;
        inline static RE::TESImageSpaceModifier* OStimNutEffect;

        static void setupForms();
    };
}