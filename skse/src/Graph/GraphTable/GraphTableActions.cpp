#include "Graph/GraphTable.h"

#include "Util/JsonFileLoader.h"
#include "Util/StringUtil.h"

namespace Graph {
    const char* ACTION_FILE_PATH{"Data/SKSE/Plugins/OStim/actions"};

    ActionActor parseActionActor(std::string path, json& json) {
        ActionActor actor;
        if (json.contains("stimulation")) {
            actor.stimulation = json["stimulation"];
        }

        if (json.contains("maxStimulation")) {
            actor.maxStimulation = json["maxStimulation"];
        }

        if (json.contains("fullStrip")) {
            actor.fullStrip = json["fullStrip"];
        }

        if (json.contains("moan")) {
            actor.moan = json["moan"];
        }

        if (json.contains("talk")) {
            actor.talk = json["talk"];
        }

        if (json.contains("muffled")) {
            actor.muffled = json["muffled"];
        }

        if (json.contains("expressionOverride")) {
            actor.expressionOverride = json["expressionOverride"];
            StringUtil::toLower(&actor.expressionOverride);
        }

        if (json.contains("requirements")) {
            for (auto& req : json["requirements"]) {
                actor.requirements |= GraphTable::getRequirement(req);
            }
        }

        if (json.contains("strippingSlots")) {
            for (auto& slot : json["strippingSlots"]) {
                actor.strippingMask |= 1 << (slot.get<int>() - 30);
            }
        }

        if (json.contains("faction")) {
            if (json["faction"].is_array()) {
                for (auto& jsonFaction : json["faction"]) {
                    GameAPI::GameFaction faction;
                    faction.loadJson(path, jsonFaction);
                    if (faction) {
                        actor.factions.push_back(faction);
                    }
                }
            } else {
                GameAPI::GameFaction faction;
                faction.loadJson(path, json["faction"]);
                if (faction) {
                    actor.factions.push_back(faction);
                }
            }
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

    void GraphTable::SetupActions() {
        Util::JsonFileLoader::LoadFilesInFolder(
            ACTION_FILE_PATH, [&](std::string path, std::string filename, json json) {
                if (json.contains("aliases")) {
                    if (json["aliases"].is_array()) {
                        int index = 0;
                        for (auto& alias : json["aliases"]) {
                            if (alias.is_string()) {
                                actionAliases[alias] = filename;
                            } else {
                                logger::warn("alias {} of action '{}' is not a string", index, filename);
                            }
                            index++;
                        }
                    } else {
                        logger::warn("property 'aliases' of action '{}' is not a list", filename);
                    }
                }

                Graph::ActionAttributes attr;
                if (json.contains("actor")) {
                    attr.actor = parseActionActor(path, json["actor"]);
                }
                if (json.contains("target")) {
                    attr.target = parseActionActor(path, json["target"]);
                }
                if (json.contains("performer")) {
                    attr.performer = parseActionActor(path, json["performer"]);
                }

                if (json.contains("sounds")) {
                    for (auto& sound : json["sounds"]) {
                        Sound::SoundType* type = Sound::SoundType::fromJson(path, sound);
                        if (type) {
                            attr.sounds.push_back(type);
                        }
                    }
                }

                if (json.contains("tags")) {
                    for (auto& tag : json["tags"]) {
                        std::string tagStr = tag.get<std::string>();
                        StringUtil::toLower(&tagStr);
                        attr.tags.push_back(tagStr);
                    }
                }

                StringUtil::toLower(&filename);
                actions[filename] = attr;
            });
    }

    std::string GraphTable::getActionAlias(std::string type) {
        if (auto it = actionAliases.find(type); it != actionAliases.end()) {
            return it->second;
        }

        return type;
    }

    ActionAttributes* GraphTable::GetActionAttributesByType(std::string type) {
        if (auto it = actions.find(type); it != actions.end()) {
            return &it->second;
        } else {
            logger::warn("No action found for {} using default", type);
            return &actions.at("default");
        }
    }
}