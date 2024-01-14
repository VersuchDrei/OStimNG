#pragma once

#include "ActionAttributes.h"

#include "GameAPI/GameFaction.h"

namespace Graph {
    struct Action {
    public:
        std::string type = "";
        ActionAttributes* attributes = nullptr;
        RoleMap<int> roles = {-1, -1, -1};
        bool muted = false;

        bool doFullStrip(int position);
        uint32_t getStrippingMask(int position);
        bool isType(std::string type);
        bool isType(std::vector<std::string> types);
    };
}