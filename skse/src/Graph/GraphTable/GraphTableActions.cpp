#include "Graph/GraphTable.h"

#include "Util/JsonFileLoader.h"
#include "Util/JsonUtil.h"
#include "Util/MapUtil.h"
#include "Util/StringUtil.h"

namespace Graph {
    const char* ACTION_FILE_PATH{"Data/SKSE/Plugins/OStim/actions"};

    ActionActor parseActionActor(std::string path, std::string filename, json& json) {
        ActionActor actor;

        JsonUtil::loadFloat(json, actor.stimulation, "stimulation", filename, "action", false);
        JsonUtil::loadFloat(json, actor.maxStimulation, "maxStimulation", filename, "action", false);
        JsonUtil::loadBool(json, actor.fullStrip, "fullStrip", filename, "action", false);
        JsonUtil::loadBool(json, actor.moan, "moan", filename, "action", false);
        JsonUtil::loadBool(json, actor.talk, "talk", filename, "action", false);
        JsonUtil::loadBool(json, actor.muffled, "muffled", filename, "action", false);
        JsonUtil::loadLowerString(json, actor.expressionOverride, "expressionOverride", filename, "action", false);

        JsonUtil::consumeLowerStringList(json, [&actor](std::string requirement) { actor.requirements.insert(requirement); }, "requirements", filename, "action", false);

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
                attr.type = filename;
                StringUtil::toLower(&attr.type);

                attr.roles.forEach([&path, &filename, &json](Role role, ActionActor& actor) {
                    std::string key = *RoleMapAPI::KEYS.get(role);
                    if (json.contains(key)) {
                        actor = parseActionActor(path, filename, json[key]);
                    }
                });

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

                actions[attr.type] = attr;
            });
    }

    std::vector<std::string> GraphTable::getActions() {
        return MapUtil::keyList(actions);
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