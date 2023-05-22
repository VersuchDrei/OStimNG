#pragma once

namespace GameAPI {
    class GameTable {
    public:
        static void setup();

        inline static RE::TESGlobal* getTimescale() { return Timescale; }

    private:
        inline static RE::TESGlobal* Timescale = nullptr;
    };
}