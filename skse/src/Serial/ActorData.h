#pragma once

#include "Util/SerializationUtil.h"

namespace Serialization {
    const uint32_t ACTOR_DATA_VERSION = 2;

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
            if (version >= 2) {
                data.actionActorStimulations = SerializationUtil::readFloatMap(serial);
                data.actionActorMaxStimulations = SerializationUtil::readFloatMap(serial);
                data.actionTargetStimulations = SerializationUtil::readFloatMap(serial);
                data.actionTargetMaxStimulations = SerializationUtil::readFloatMap(serial);
                data.actionPerformerStimulations = SerializationUtil::readFloatMap(serial);
                data.actionPerformerMaxStimulations = SerializationUtil::readFloatMap(serial);

                data.eventActorStimulations = SerializationUtil::readFloatMap(serial);
                data.eventActorMaxStimulations = SerializationUtil::readFloatMap(serial);
                data.eventTargetStimulations = SerializationUtil::readFloatMap(serial);
                data.eventTargetMaxStimulations = SerializationUtil::readFloatMap(serial);
                data.eventPerformerStimulations = SerializationUtil::readFloatMap(serial);
                data.eventPerformerMaxStimulations = SerializationUtil::readFloatMap(serial);
            }

            return data;
        }

        std::unordered_map<std::string, std::string> equipObjects;
        RE::FormID voiceSet = 0;
        std::unordered_map<std::string, float> actionActorStimulations;
        std::unordered_map<std::string, float> actionActorMaxStimulations;
        std::unordered_map<std::string, float> actionTargetStimulations;
        std::unordered_map<std::string, float> actionTargetMaxStimulations;
        std::unordered_map<std::string, float> actionPerformerStimulations;
        std::unordered_map<std::string, float> actionPerformerMaxStimulations;

        std::unordered_map<std::string, float> eventActorStimulations;
        std::unordered_map<std::string, float> eventActorMaxStimulations;
        std::unordered_map<std::string, float> eventTargetStimulations;
        std::unordered_map<std::string, float> eventTargetMaxStimulations;
        std::unordered_map<std::string, float> eventPerformerStimulations;
        std::unordered_map<std::string, float> eventPerformerMaxStimulations;

        inline void serialize(SKSE::SerializationInterface* serial) {
            SerializationUtil::writeStringMap(serial, equipObjects);

            serial->WriteRecordData(&voiceSet, sizeof(voiceSet));

            SerializationUtil::writeFloatMap(serial, actionActorStimulations);
            SerializationUtil::writeFloatMap(serial, actionActorMaxStimulations);
            SerializationUtil::writeFloatMap(serial, actionTargetStimulations);
            SerializationUtil::writeFloatMap(serial, actionTargetMaxStimulations);
            SerializationUtil::writeFloatMap(serial, actionPerformerStimulations);
            SerializationUtil::writeFloatMap(serial, actionPerformerMaxStimulations);

            SerializationUtil::writeFloatMap(serial, eventActorStimulations);
            SerializationUtil::writeFloatMap(serial, eventActorMaxStimulations);
            SerializationUtil::writeFloatMap(serial, eventTargetStimulations);
            SerializationUtil::writeFloatMap(serial, eventTargetMaxStimulations);
            SerializationUtil::writeFloatMap(serial, eventPerformerStimulations);
            SerializationUtil::writeFloatMap(serial, eventPerformerMaxStimulations);
        }
    };
}