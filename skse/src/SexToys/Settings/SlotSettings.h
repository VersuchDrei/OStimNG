#pragma once

#include "ScalingType.h"
#include "SynchronizationType.h"

#include "GameAPI/GameSerializationInterface.h"

namespace Toys {
    namespace Settings {
        struct SlotSettings {
        public:
            bool enabled = true;
            SynchronizationType synchronizationType = SynchronizationType::PLAYER;
            ScalingType scalingType = ScalingType::STIMULATION_VALUE;
            float minScale = 0.0f;
            float maxScale = 100.0f;
            float minMagnitude = 0.1f;
            float maxMagnitude = 0.5f;
            bool doPeaks = true;
            float minPeak = 0.6f;
            float maxPeak = 1.0f;

            void serialize(GameAPI::GameSerializationInterface serial);
            void deserialize(GameAPI::GameSerializationInterface serial);

            json toJson();
            void loadJson(json& json);
        };
    }
}