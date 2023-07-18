#include "Graph/GraphTable.h"

#include "Util/JsonFileLoader.h"
#include "Util/StringUtil.h"

namespace Graph {
    const char* EVENT_FILE_PATH{"Data/SKSE/Plugins/OStim/events"};

    EventActor parseEventActor(json& json) {
        EventActor actor;
        if (json.contains("stimulation")) {
            actor.stimulation = json["stimulation"];
        }

        if (json.contains("maxStimulation")) {
            actor.maxStimulation = json["maxStimulation"];
        }

        if (json.contains("ints")) {
            for (auto& [key, val] : json["ints"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                actor.ints.insert(std::make_pair(mutableKey, val.get<int>()));
            }
        }

        if (json.contains("intLists")) {
            for (auto& [key, val] : json["intLists"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                std::vector<int> ints;
                for (auto& entry : val) {
                    ints.push_back(entry.get<int>());
                }
                actor.intLists.insert(std::make_pair(mutableKey, ints));
            }
        }

        if (json.contains("floats")) {
            for (auto& [key, val] : json["floats"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                actor.floats.insert(std::make_pair(mutableKey, val.get<float>()));
            }
        }

        if (json.contains("floatLists")) {
            for (auto& [key, val] : json["floatLists"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                std::vector<float> floats;
                for (auto& entry : val) {
                    floats.push_back(entry.get<float>());
                }
                actor.floatLists.insert(std::make_pair(mutableKey, floats));
            }
        }

        if (json.contains("strings")) {
            auto& strings = json["strings"];
            for (auto& [key, val] : json["strings"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                std::string value = val.get<std::string>();
                StringUtil::toLower(&value);
                actor.strings.insert(std::make_pair(mutableKey, value));
            }
        }

        if (json.contains("stringLists")) {
            for (auto& [key, val] : json["stringLists"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                std::vector<std::string> strings;
                for (auto& entry : val) {
                    std::string value = entry.get<std::string>();
                    StringUtil::toLower(&value);
                    strings.push_back(value);
                }
                actor.stringLists.insert(std::make_pair(mutableKey, strings));
            }
        }

        return actor;
    };

    void GraphTable::setupEvents() {
        Util::JsonFileLoader::LoadFilesInFolder(
            EVENT_FILE_PATH, [&](std::string path, std::string filename, json json) {
                Graph::Event graphEvent;
                if (json.contains("actor")) {
                    graphEvent.actor = parseEventActor(json["actor"]);
                }
                if (json.contains("target")) {
                    graphEvent.target = parseEventActor(json["target"]);
                }
                if (json.contains("performer")) {
                    graphEvent.performer = parseEventActor(json["performer"]);
                }

                if (json.contains("sound")) {
                    graphEvent.sound.loadJson(path, json["sound"]);
                }

                if (json.contains("cameraShakeStrength")) {
                    graphEvent.cameraShakeStrength = json["cameraShakeStrength"];
                }
                if (json.contains("cameraShakeDuration")) {
                    graphEvent.cameraShakeDuration = json["cameraShakeDuration"];
                }
                if (json.contains("controllerRumbleStrength")) {
                    graphEvent.controllerRumbleStrength = json["controllerRumbleStrength"];
                }
                if (json.contains("controllerRumbleDuration")) {
                    graphEvent.controllerRumbleDuration = json["controllerRumbleDuration"];
                }

                if (json.contains("tags")) {
                    for (auto& tag : json["tags"]) {
                        std::string tagStr = tag.get<std::string>();
                        StringUtil::toLower(&tagStr);
                        graphEvent.tags.push_back(tagStr);
                    }
                }

                StringUtil::toLower(&filename);
                events[filename] = graphEvent;
            });
    }

    Event* GraphTable::getEvent(std::string eventName) {
        auto iter = events.find(eventName);
        if (iter != events.end()) {
            return &iter->second;
        }
        return nullptr;
    }
}