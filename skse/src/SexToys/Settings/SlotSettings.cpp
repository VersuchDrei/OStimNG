#include "SlotSettings.h"

#include "Util/JsonUtil.h"

namespace Toys {
    namespace Settings {
        void SlotSettings::serialize(GameAPI::GameSerializationInterface serial) {
            serial.write<SynchronizationType>(synchronizationType);
            serial.write<ScalingType>(scalingType);
            serial.write<float>(minScale);
            serial.write<float>(maxScale);
            serial.write<float>(minMagnitude);
            serial.write<float>(maxMagnitude);
            serial.write<bool>(doPeaks);
            serial.write<float>(minPeak);
            serial.write<float>(maxPeak);
        }

        void SlotSettings::deserialize(GameAPI::GameSerializationInterface serial) {
            synchronizationType = serial.read<SynchronizationType>();
            scalingType = serial.read<ScalingType>();
            minScale = serial.read<float>();
            maxScale = serial.read<float>();
            minMagnitude = serial.read<float>();
            maxMagnitude = serial.read<float>();
            doPeaks = serial.read<bool>();
            minPeak = serial.read<float>();
            maxPeak = serial.read<float>();
        }

        json SlotSettings::toJson() {
            json json = json::object();

            json["synchronizationType"] = static_cast<int>(synchronizationType);
            json["scalingType"] = static_cast<int>(scalingType);
            json["minScale"] = minScale;
            json["maxScale"] = maxScale;
            json["minMagnitude"] = minMagnitude;
            json["maxMagnitude"] = maxMagnitude;
            json["doPeaks"] = doPeaks;
            json["minPeak"] = minPeak;
            json["maxPeak"] = maxPeak;

            return json;
        }

        void SlotSettings::loadJson(json& json) {
            if (!json.is_object()) {
                return;
            }

            std::string objectName = "export";

            JsonUtil::consumeInt(json, [this](int value) { synchronizationType = static_cast<SynchronizationType>(value); }, "synchronizationType", objectName, "setting", false);
            JsonUtil::consumeInt(json, [this](int value) { scalingType = static_cast<ScalingType>(value); }, "scalingType", objectName, "setting", false);
            JsonUtil::loadFloat(json, minScale, "minScale", objectName, "setting", false);
            JsonUtil::loadFloat(json, maxScale, "maxScale", objectName, "setting", false);
            JsonUtil::loadFloat(json, minMagnitude, "minMagnitude", objectName, "setting", false);
            JsonUtil::loadFloat(json, maxMagnitude, "maxMagnitude", objectName, "setting", false);
            JsonUtil::loadBool(json, doPeaks, "doPeaks", objectName, "setting", false);
            JsonUtil::loadFloat(json, minPeak, "minPeak", objectName, "setting", false);
            JsonUtil::loadFloat(json, maxPeak, "maxPeak", objectName, "setting", false);
        }
    }
}