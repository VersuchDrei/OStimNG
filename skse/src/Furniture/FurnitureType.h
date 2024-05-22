#pragma once

#include "GameAPI/GameVariable.h"
#include "GameAPI/GameObject.h"

namespace Furniture {
    struct FurnitureType {
    public:
        std::string id;
        std::string name = "";
        std::function<bool(GameAPI::GameObject)> condition = [](GameAPI::GameObject object) { return false; };
        int priority = 0;

        bool ignoreMarkerOffsetX = false;
        bool ignoreMarkerOffsetY = false;
        bool ignoreMarkerOffsetZ = false;
        float offsetX = 0.0f;
        float offsetY = 0.0f;
        float offsetZ = 0.0f;
        float rotation = 0.0f;
        float multiplyScale = 1.0f;
        GameAPI::GameVariable offsetXGlobal;
        GameAPI::GameVariable offsetYGlobal;
        GameAPI::GameVariable offsetZGlobal;

        FurnitureType* supertype = nullptr;
        bool listIndividually = false;

        FurnitureType* getMasterType();
        FurnitureType* getListType();

        bool isChildOf(FurnitureType* other);
    };
}