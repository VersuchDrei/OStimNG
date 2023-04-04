#pragma once

#include "Util/SerializationUtil.h"

namespace Serialization {
    struct ActorData {
        inline static ActorData deserialize(SKSE::SerializationInterface* serial) {
            ActorData data;
            data.equipObjects = SerializationUtil::readStringMap(serial);
            return data;
        }

        std::unordered_map<std::string, std::string> equipObjects;

        inline void serialize(SKSE::SerializationInterface* serial) {
            SerializationUtil::writeStringMap(serial, equipObjects);
        }
    };
}