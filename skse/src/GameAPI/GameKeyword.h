#pragma once

namespace GameAPI {
    struct GameKeyword {
        inline static GameKeyword fromFile(std::string mod, uint32_t formID) {
            return RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSKeyword>(formID, mod);
        }

        RE::BGSKeyword* keyword;

        inline GameKeyword(RE::BGSKeyword* keyword) : keyword{keyword} {}

        inline operator bool() const { return keyword; }
        inline bool operator==(const GameKeyword other) { return keyword == other.keyword; }
        inline bool operator!=(const GameKeyword other) { return keyword != other.keyword; }
    };
}