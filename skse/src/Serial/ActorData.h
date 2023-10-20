#pragma once

#include "Util/SerializationUtil.h"

namespace Serialization {
    const uint32_t ACTOR_DATA_VERSION = 1;

    struct ActorData {
        inline static ActorData deserialize(SKSE::SerializationInterface* serial, uint32_t version) {
            ActorData data;
            data.equipObjects = SerializationUtil::readStringMap(serial);

            if (version >= 1) {
                RE::FormID oldID;
                serial->ReadRecordData(&oldID, sizeof(oldID));
                RE::FormID newID;
                bool valid = serial->ResolveFormID(oldID, newID);

                if (valid) {
                    data.voiceSet = newID;
                } else {
                    logger::warn("cannot resolve voice set id {:x}, missing mod?", oldID);
                }
            }

            return data;
        }

        std::unordered_map<std::string, std::string> equipObjects;
        RE::FormID voiceSet = 0;

        inline void serialize(SKSE::SerializationInterface* serial) {
            SerializationUtil::writeStringMap(serial, equipObjects);
            serial->WriteRecordData(&voiceSet, sizeof(voiceSet));
        }
    };
}