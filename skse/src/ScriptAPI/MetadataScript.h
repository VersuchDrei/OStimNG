#pragma once

#include "GameAPI/GameActor.h"

namespace ScriptAPI {
    namespace Metadata {
        std::vector<std::string> getNodesInRange(std::string id, std::vector<GameAPI::GameActor> actors, int distance);
        std::vector<std::string> nodesToNames(std::vector<std::string> ids);
    }
}