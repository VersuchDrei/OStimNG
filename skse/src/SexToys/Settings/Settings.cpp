#include "Settings.h"

#include "Util/JsonUtil.h"
#include "Util/SerializationUtil.h"

namespace Toys {
    namespace Settings {
        ToySettings* Settings::getToySettings(std::string id) {
            return &toySettings.insert({id, {}}).first->second;
        }


        void Settings::serialize(GameAPI::GameSerializationInterface serial) {
            serial.write<SynchronizationType>(synchronizationType);
            
            serial.write<size_t>(toySettings.size());
            for (auto& [toy, setting] : toySettings) {
                SerializationUtil::writeString(serial, toy);
                setting.serialize(serial);
            }
        }

        void Settings::deserialize(GameAPI::GameSerializationInterface serial) {
            synchronizationType = serial.read<SynchronizationType>();

            toySettings.clear();
            for (size_t size = serial.read<size_t>(); size > 0; size--) {
                std::string toy = SerializationUtil::readString(serial);
                ToySettings settings;
                settings.deserialize(serial);
                toySettings.emplace(toy, settings);
            }
        }

        void Settings::reset() {
            synchronizationType = SynchronizationType::PLAYER;
            toySettings.clear();
        }


        void Settings::writeJson(json& json) {
            json["toys"] = json::object();
            json["toys"]["synchronizationType"] = static_cast<int>(synchronizationType);

            json["toys"]["toySettings"] = json::object();
            for (auto& [toy, setting] : toySettings) {
                json["toys"]["toySettings"][toy] = setting.toJson();
            }
        }

        void Settings::loadJson(json& json) {
            if (!json.contains("toys")) {
                return;
            }

            std::string objectName = "export";

            nlohmann::json& toys = json["toys"];

            JsonUtil::consumeInt(toys, [this](int value) { synchronizationType = static_cast<SynchronizationType>(value); }, "synchronizationType", objectName, "setting", false);
            toySettings.clear();
            if (toys.contains("toySettings") && toys["toySettings"].is_object()) {
                for (auto& [toy, value] : toys["toySettings"].items()) {
                    ToySettings settings;
                    settings.loadJson(value);
                    toySettings.emplace(toy, settings);
                }
            }
        }
    }
}