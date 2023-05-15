#pragma once

namespace GameAPI {
    struct GameRace {
    public:
        inline static GameRace fromFile(std::string mod, uint32_t formID) {
            return RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESRace>(formID, mod);
        }

        RE::TESRace* race;
        
        inline GameRace(RE::TESRace* race) : race{race} {}

        inline operator bool() const { return race; }
        inline bool operator==(const GameRace other) { return race == other.race; }
        inline bool operator!=(const GameRace other) { return race != other.race; }
    };
}