#pragma once

namespace GameAPI {
    class GameTable {
    public:
        static void setup();

        inline static void setCurrentCrosshairRef(RE::TESObjectREFR* ref) { currentCrosshairRef = ref; }
        inline static RE::TESObjectREFR* getCurrentCrosshairRef() { return currentCrosshairRef; }

        inline static RE::TESGlobal* getTimescale() { return Timescale; }

        inline static RE::BGSKeyword* getNPCKeyword() { return ActorTypeNPC; }
        inline static RE::BGSKeyword* getVampireKeyword() { return Vampire; }
        inline static RE::TESRace* getManakinRace() { return ManakinRace; }

        inline static RE::TESImageSpaceModifier* getFadeToBlackHoldImod() { return FadeToBlackHoldImod; }
        inline static RE::TESImageSpaceModifier* getFadeToBlackBackImod() { return FadeToBlackBackImod; }

        inline static bool improvedCamSupport() { return OStimImprovedCamSupport->value != 0; }

        inline static RE::TESFaction* getOStimEmptyFaction() { return OStimEmptyFaction; }

    private:
        inline static RE::TESObjectREFR* currentCrosshairRef = nullptr;

        inline static RE::TESGlobal* Timescale = nullptr;

        inline static RE::BGSKeyword* ActorTypeNPC = nullptr;
        inline static RE::BGSKeyword* Vampire = nullptr;
        inline static RE::TESRace* ManakinRace = nullptr;

        inline static RE::TESImageSpaceModifier* FadeToBlackImod = nullptr;
        inline static RE::TESImageSpaceModifier* FadeToBlackHoldImod = nullptr;
        inline static RE::TESImageSpaceModifier* FadeToBlackBackImod = nullptr;

        inline static RE::TESGlobal* OStimImprovedCamSupport = nullptr;

        inline static RE::TESFaction* OStimEmptyFaction = nullptr;
    };
}