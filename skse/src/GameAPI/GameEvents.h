#pragma once

#include "GameActor.h"

#include "Graph/RoleMap.h"

namespace Threading {
    class Thread;
}

namespace GameAPI {
    namespace GameEvents {
        void sendStartEvent(int threadID);
        void sendSceneChangedEvent(int threadID, std::string sceneID);
        void sendSpeedChangedEvent(int threadID, std::string sceneID, int speed);
        void sendEndEvent(int threadID, Threading::Thread* thread, std::vector<GameActor> actors);

        void sendOrgasmEvent(int threadID, std::string sceneID, int index, GameAPI::GameActor actor);
        void sendOStimEvent(int threadID, std::string type, Graph::RoleMap<GameActor> actors);
    }
}