#pragma once

namespace GameAPI {
    class GameTable {
    public:
        static void setup();

        inline static void setCurrentCrosshairRef(RE::TESObjectREFR* ref) { currentCrosshairRef = ref; }
        inline static RE::TESObjectREFR* getCurrentCrosshairRef() { return currentCrosshairRef; }

        inline static RE::TESGlobal* getTimescale() { return Timescale; }

    private:
        inline static RE::TESObjectREFR* currentCrosshairRef = nullptr;
        inline static RE::TESGlobal* Timescale = nullptr;
    };
}