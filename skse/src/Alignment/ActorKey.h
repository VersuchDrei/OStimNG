#pragma once

#include "GameAPI/GameSex.h"

namespace Alignment {
    struct ActorKey {
        GameAPI::GameSex sex = GameAPI::GameSex::NONE;
        int height = 100;
        int heels = 0;

        ActorKey(bool isFemale, float scale, float heels);

        inline std::string toString() {
            return GameAPI::GameSexAPI::toIcon(sex) + std::to_string(height) + "x" + std::to_string(heels);
        }

        inline bool operator==(const ActorKey other) {
            return sex == other.sex && height == other.height && heels == other.heels;
        }

        inline bool operator!=(const ActorKey other) {
            return sex != other.sex || height != other.height || heels != other.heels;
        }
    };
}