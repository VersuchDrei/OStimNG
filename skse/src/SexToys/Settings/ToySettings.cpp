#include "ToySettings.h"

#include "Util/JsonUtil.h"
#include "Util/SerializationUtil.h"

namespace Toys {
    namespace Settings {
        SlotSettings* ToySettings::getSlotSettings(std::string slot) {
            return &slotSettings.insert({slot, {}}).first->second;
        }

        SlotSettings* ToySettings::getSlotSettingsUnsafe(std::string slot) {
            auto iter = slotSettings.find(slot);
            return iter == slotSettings.end() ? nullptr : &iter->second;
        }


        void ToySettings::serialize(GameAPI::GameSerializationInterface serial) {
            serial.write<bool>(enabled);
            serial.write<SynchronizationType>(synchronizationType);
            serial.write<bool>(climax);
            serial.write<float>(climaxMagnitude);

            serial.write<size_t>(slotSettings.size());
            for (auto& [slot, settings] : slotSettings) {
                SerializationUtil::writeString(serial, slot);
                settings.serialize(serial);
            }
        }

        void ToySettings::deserialize(GameAPI::GameSerializationInterface serial) {
            enabled = serial.read<bool>();
            synchronizationType = serial.read<SynchronizationType>();
            climax = serial.read<bool>();
            climaxMagnitude = serial.read<float>();

            slotSettings.clear();
            for (size_t size = serial.read<size_t>(); size > 0; size--) {
                std::string slot = SerializationUtil::readString(serial);
                SlotSettings settings;
                settings.deserialize(serial);
                slotSettings.emplace(slot, settings);
            }
        }

        json ToySettings::toJson() {
            json json = json::object();

            json["enabled"] = enabled;
            json["synchronizationType"] = static_cast<int>(synchronizationType);
            json["climax"] = climax;
            json["climaxMagnitude"] = climaxMagnitude;

            json["slotSettings"] = json::object();
            for (auto& [slot, setting] : slotSettings) {
                json["slotSettings"][slot] = setting.toJson();
            }

            return json;
        }

        void ToySettings::loadJson(json& json) {
            if (!json.is_object()) {
                return;
            }

            std::string objectName = "export";

            JsonUtil::loadBool(json, enabled, "enabled", objectName, "setting", false);
            JsonUtil::consumeInt(json, [this](int value) { synchronizationType = static_cast<SynchronizationType>(value); }, "synchronizationType", objectName, "setting", false);
            JsonUtil::loadBool(json, climax, "climax", objectName, "setting", false);
            JsonUtil::loadFloat(json, climaxMagnitude, "climaxMagnitude", objectName, "setting", false);

            slotSettings.clear();
            if (json.contains("slotSettings") && json["slotSettings"].is_object()) {
                for (auto& [slot, value] : json["slotSettings"].items()) {
                    SlotSettings settings;
                    settings.loadJson(value);
                    slotSettings.emplace(slot, settings);
                }
            }
        }
    }
}