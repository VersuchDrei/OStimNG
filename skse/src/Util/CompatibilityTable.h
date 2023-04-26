#pragma once

namespace Compatibility {
    class CompatibilityTable {
    public:
        static void setupForms();

        static bool sosInstalled();
        static bool hasSchlong(RE::Actor* actor);
    private:
        static void tryAddNoSchlongFaction(std::string modName, RE::FormID formID);

        inline static RE::TESFaction* SOS_SchlongifiedFaction = nullptr;
        inline static std::vector<RE::TESFaction*> noSchlongFactions;
    };
}