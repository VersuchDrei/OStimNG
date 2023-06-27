#pragma once

#include "GameAPI/GameActor.h"

namespace Compatibility {
    class CompatibilityTable {
    public:
        static void setupForms();

        static bool sosInstalled();
        static bool hasSchlong(GameAPI::GameActor actor);
    private:
        static void tryAddNoSchlongFaction(std::string modName, RE::FormID formID);

        inline static GameAPI::GameFaction SOS_SchlongifiedFaction;
        inline static std::vector<GameAPI::GameFaction> noSchlongFactions;
    };
}