#pragma once

#include "GameAPI/GameActor.h"
#include "GameAPI/GameObject.h"

namespace ScriptAPI {
    namespace Thread {
        std::vector<int> getAllThreadIDs();


        void changeFurniture(int threadID, GameAPI::GameObject furniture, std::string sceneID);

        bool swapActors(int threadID, int positionA, int positionB);
        std::vector<int> getSwapPartners(int threadID, GameAPI::GameActor actor);
        void swapActorsWithUI(int threadID);

        bool addActorToThread(int threadID, GameAPI::GameActor actor);
        void addActorWithUI(int threadID);

        bool removeActorFromThread(int threadID, int position);
        void removeActorWithUI(int threadID);

        bool hasMetadata(int threadID, std::string metadata);
        void addMetadata(int threadID, std::string metadata);
        std::vector<std::string> getMetadata(int threadID);

        bool hasMetaFloat(int threadID, std::string key);
        float getMetaFloat(int threadID, std::string key);
        void setMetaFloat(int threadID, std::string key, float value);

        bool hasMetaString(int threadID, std::string key);
        std::string getMetaString(int threadID, std::string key);
        void setMetaString(int threadID, std::string key, std::string value);
    }
}