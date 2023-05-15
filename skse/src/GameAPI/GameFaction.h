#pragma once

namespace GameAPI {
    struct GameFaction {
    public:
        inline static GameFaction fromFile(std::string mod, uint32_t formID) {
            return RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESFaction>(formID, mod);
        }

        RE::TESFaction* faction;

        inline GameFaction(RE::TESFaction* faction) : faction{faction} {};

        inline operator bool() const { return faction; }
        inline bool operator==(const GameFaction other) { return faction == other.faction; }
        inline bool operator!=(const GameFaction other) { return faction != other.faction; }
    };
}