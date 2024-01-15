#include "Graph/GraphTable.h"

#include "Furniture/FurnitureTable.h"
#include "Game/LocaleManager.h"
#include "Util/JsonFileLoader.h"
#include "Util/JsonUtil.h"
#include "Util/StringUtil.h"
#include "Util/VectorUtil.h"

namespace Graph {
    const char* NODE_FILE_PATH{"Data/SKSE/Plugins/OStim/scenes"};

    void GraphTable::setupNodes() {
        std::vector<Graph::RawNavigation> navigations;

        Util::JsonFileLoader::LoadFilesInSubfolders(NODE_FILE_PATH, [&navigations](std::string path, std::string filename, json json) {
            Node* node = new Node();
            node->lowercase_id = node->scene_id = filename;
            StringUtil::toLower(&node->lowercase_id);
            
            if (json.contains("name")) {
                if (json["name"].is_string()) {
                    node->lowercase_name = node->scene_name = LocaleManager::GetSingleton()->GetLocalization(static_cast<std::string>(json["name"]));
                    StringUtil::toLower(&node->lowercase_name);
                } else {
                    logger::warn("name property of scene {} isn't a string", node->scene_id);
                }
            } else {
                node->lowercase_name = node->scene_name = LocaleManager::GetSingleton()->GetLocalization("$" + node->scene_id);
            }

            if (json.contains("modpack")) {
                if (json["modpack"].is_string()) {
                    node->modpack = json["modpack"];
                } else {
                    logger::warn("modpack property of scene {} isn't a string", node->scene_id);
                }
            }

            if (json.contains("length")) {
                if (json["length"].is_number()) {
                    node->animationLengthMs = static_cast<int>(static_cast<float>(json["length"]) * 1000.0f);
                } else {
                    logger::warn("length property of scene {} isn't a number", node->scene_id);
                }
            } else {
                logger::warn("scene {} doesn't have a length property defined", node->scene_id);
            }

            if (json.contains("destination")) {
                if (json["destination"].is_string()) {
                    node->isTransition = true;
                    navigations.push_back({.origin = node->scene_id, .destination = json["destination"]});
                    if (json.contains("origin")) {
                        if (json["origin"].is_string()) {
                            RawNavigation navigation = {.origin = json["origin"], .destination = node->scene_id};

                            if (json.contains("priority")) {
                                if (json["priority"].is_number_integer()) {
                                    navigation.priority = json["priority"];    
                                } else {
                                    logger::warn("priority property of scene {} isn't an integer", node->scene_id);
                                }
                            }

                            if (json.contains("description")) {
                                if (json["description"].is_string()) {
                                    navigation.description = json["description"];
                                } else {
                                    logger::warn("description property of scene {} isn't a string", node->scene_id);
                                }
                            }

                            if (json.contains("icon")) {
                                if (json["icon"].is_string()) {
                                    navigation.icon = json["icon"];
                                } else {
                                    logger::warn("icon property of scene {} isn't a string", node->scene_id);
                                }
                            }

                            if (json.contains("border")) {
                                if (json["border"].is_string()) {
                                    navigation.border = json["border"];
                                } else {
                                    logger::warn("border property of scene {} isn't a string", node->scene_id);
                                }
                            }

                            if (json.contains("noWarnings")) {
                                if (json["noWarnings"].is_boolean()) {
                                    navigation.noWarnings = json["noWarnings"];
                                } else {
                                    logger::warn("noWarnings property of scene {} isn't a string", node->scene_id);
                                }
                            }

                            navigations.push_back(navigation);
                        } else {
                            logger::warn("origin property of scene {} isn't a string", node->scene_id);
                        }
                    }
                } else {
                    logger::warn("destination property of scene {} isn't a string", node->scene_id);
                }
            } else {
                if (json.contains("navigations")) {
                    if (json["navigations"].is_array()) {
                        int index = 0;
                        for (auto& jsonNavigation : json["navigations"]) {
                            if (jsonNavigation.is_object()) {
                                RawNavigation navigation = {.origin = node->scene_id, .destination = node->scene_id};

                                if (jsonNavigation.contains("destination")) {
                                    if (jsonNavigation["destination"].is_string()) {
                                        navigation.destination = jsonNavigation["destination"];
                                    } else {
                                        logger::warn("destination property of navigation {} of scene {} isn't a string", index, node->scene_id);
                                    }
                                }

                                if (jsonNavigation.contains("origin")) {
                                    if (jsonNavigation["origin"].is_string()) {
                                        navigation.origin = jsonNavigation["origin"];
                                    } else {
                                        logger::warn("origin property of navigation {} of scene {} isn't a string", index, node->scene_id);
                                    }
                                }

                                if (jsonNavigation.contains("priority")) {
                                    if (jsonNavigation["priority"].is_number_integer()) {
                                        navigation.priority = jsonNavigation["priority"];
                                    } else {
                                        logger::warn("priority property of navigation {} of scene {} isn't an integer", index, node->scene_id);
                                    }
                                }

                                if (jsonNavigation.contains("description")) {
                                    if (jsonNavigation["description"].is_string()) {
                                        navigation.description = LocaleManager::GetSingleton()->GetLocalization(static_cast<std::string>(jsonNavigation["description"]));
                                    } else {
                                        logger::warn("description property of navigation {} of scene {} isn't a string", index, node->scene_id);
                                    }
                                }

                                if (jsonNavigation.contains("icon")) {
                                    if (jsonNavigation["icon"].is_string()) {
                                        navigation.icon = jsonNavigation["icon"];
                                    } else {
                                        logger::warn("icon property of navigation {} of scene {} isn't a string", index, node->scene_id);
                                    }
                                }

                                if (jsonNavigation.contains("border")) {
                                    if (jsonNavigation["border"].is_string()) {
                                        navigation.border = jsonNavigation["border"];
                                    } else {
                                        logger::warn("border property of navigation {} of scene {} isn't a string", index, node->scene_id);
                                    }
                                }

                                if (jsonNavigation.contains("noWarnings")) {
                                    if (jsonNavigation["noWarnings"].is_boolean()) {
                                        navigation.noWarnings = jsonNavigation["noWarnings"];
                                    } else {
                                        logger::warn("noWarnings property of navigation {} of scene {} isn't a boolean", index, node->scene_id);
                                    }
                                }

                                if (navigation.origin == navigation.destination) {
                                    logger::warn("navigation {} of scene {} has neither an origin nor a destination property defined", index, node->scene_id);
                                } else {
                                    navigations.push_back(navigation);
                                }
                            } else {
                                logger::warn("navigation {} of scene {} is malformed", index, node->scene_id);
                            }
                            index++;
                        }
                    } else {
                        logger::warn("navigations property of scene {} isn't a list", node->scene_id);
                    }
                }
            }

            if (json.contains("speeds")) {
                if (json["speeds"].is_array()) {
                    int index = 0;
                    for (auto& jsonSpeed : json["speeds"]) {
                        if (jsonSpeed.is_object()) {
                            Speed speed;
                            if (jsonSpeed.contains("animation")) {
                                if (jsonSpeed["animation"].is_string()) {
                                    speed.animation = jsonSpeed["animation"];
                                } else {
                                    logger::warn("animation property of speed {} of scene {} isn't a string", index, node->scene_id);
                                }
                            } else {
                                logger::warn("speed {} of scene {} doesn't have an animation property", index, node->scene_id);
                            }

                            if (jsonSpeed.contains("playbackSpeed")) {
                                if (jsonSpeed["playbackSpeed"].is_number()) {
                                    speed.playbackSpeed = jsonSpeed["playbackSpeed"];
                                } else {
                                    logger::warn("playbackSpeed property of speed {} of scene {} isn't a number", index, node->scene_id);
                                }
                            }

                            if (jsonSpeed.contains("displaySpeed")) {
                                if (jsonSpeed["displaySpeed"].is_number()) {
                                    speed.displaySpeed = jsonSpeed["displaySpeed"];
                                } else {
                                    logger::warn("displaySpeed property of speed {} of scene {} isn't a number", index, node->scene_id);
                                }
                            }
                            node->speeds.push_back(speed);
                        } else {
                            logger::warn("speed {} of scene {} is malformed", index, node->scene_id);
                        }
                        index++;
                    }
                } else {
                    logger::warn("speeds property of scene {} isn't a list", node->scene_id);
                }
            } else {
                logger::warn("scene {} doesn't have a speeds property defined", node->scene_id);
            }

            if (json.contains("defaultSpeed")) {
                if (json["defaultSpeed"].is_number_integer()) {
                    node->defaultSpeed = json["defaultSpeed"];
                } else {
                    logger::warn("defaultSpeed property of scene {} isn't an integer", node->scene_id);
                }
                
            }

            if (json.contains("noRandomSelection")) {
                if (json["noRandomSelection"].is_boolean()) {
                    node->noRandomSelection = json["noRandomSelection"];
                } else {
                    logger::warn("noRandomSelection property of scene {} isn't a boolean", node->scene_id);
                }
            }

            if (json.contains("furniture")) {
                if (json["furniture"].is_string()) {
                    node->furnitureType = Furniture::FurnitureTable::getFurnitureType(json["furniture"]);
                    if (node->furnitureType->id == "none") {
                        logger::warn("furniture type of scene {} doesn't exist", node->scene_id);
                    }
                } else {
                    node->furnitureType = Furniture::FurnitureTable::getFurnitureType("none");
                    logger::warn("furniture property of scene {} isn't a string", node->scene_id);
                }
            } else {
                node->furnitureType = Furniture::FurnitureTable::getFurnitureType("none");
            }

            if (json.contains("offset")) {
                node->offset.loadJson(path, json["offset"]);
            }
            JsonUtil::loadBool(json, node->scaleOffsetWithFurniture, "scaleOffsetWithFurniture", node->scene_id, "scene", false);

            if (json.contains("tags")) {
                if (json["tags"].is_array()) {
                    int index = 0;
                    for (auto& jsonTag : json["tags"]) {
                        if (jsonTag.is_string()) {
                            std::string tag = jsonTag;
                            StringUtil::toLower(&tag);
                            node->tags.push_back(tag);
                        } else {
                            logger::warn("tag {} of scene {} isn't a string", index, node->scene_id);
                        }
                        index++;
                    }
                } else {
                    logger::warn("tags property of scene {} isn't a list", node->scene_id);
                }
            }

            if (json.contains("autoTransitions")) {
                if (json["autoTransitions"].is_object()) {
                    for (auto& [id, transition] : json["autoTransitions"].items()) {
                        if (transition.is_string()) {
                            node->autoTransitions[id] = transition;
                        } else {
                            logger::warn("autoTransition {} of scene {} isn't a string", id, node->scene_id);
                        }
                    }
                } else {
                    logger::warn("autoTransitions property of scene {} is malformed", node->scene_id);
                }
            }

            if (json.contains("actors")) {
                if (json["actors"].is_array()) {
                    int index = 0;
                    for (auto& jsonActor : json["actors"]) {
                        Graph::GraphActor actor;

                        if (jsonActor.is_object()) {
                            std::string objectType = "actor " + std::to_string(index) + " of scene";

                            if (jsonActor.contains("type")) {
                                if (jsonActor["type"].is_string()) {
                                    actor.condition.type = jsonActor["type"];
                                } else {
                                    logger::warn("type property of actor {} of scene {} isn't a string", index, node->scene_id);
                                }
                            }

                            if (jsonActor.contains("intendedSex")) {
                                if (jsonActor["intendedSex"].is_string()) {
                                    actor.condition.sex = GameAPI::GameSexAPI::fromString(jsonActor["intendedSex"]);
                                } else {
                                    logger::warn("itendedSex property of actor {} of scene {} isn't a string", index, node->scene_id);
                                }
                            }

                            JsonUtil::loadInt(jsonActor, actor.sosBend, "sosBend", node->scene_id, objectType, false);
                            JsonUtil::loadInt(jsonActor, actor.sosBend, "tngBend", node->scene_id, objectType, false);

                            if (jsonActor.contains("scale")) {
                                if (jsonActor["scale"].is_number()) {
                                    actor.scale = jsonActor["scale"];
                                } else {
                                    logger::warn("scale property of actor {} of scene {} isn't a number", index, node->scene_id);
                                }
                            }

                            JsonUtil::loadFloat(jsonActor, actor.scaleHeight, "scaleHeight", node->scene_id, objectType, false);
                            JsonUtil::loadString(jsonActor, actor.underlyingExpression, "underlyingExpression", node->scene_id, objectType, false);
                            JsonUtil::loadString(jsonActor, actor.expressionOverride, "expressionOverride", node->scene_id, objectType, false);
                            JsonUtil::loadInt(jsonActor, actor.expressionAction, "expressionAction", node->scene_id, objectType, false);

                            if (jsonActor.contains("animationIndex")) {
                                if (jsonActor["animationIndex"].is_number_integer()) {
                                    actor.animationIndex = jsonActor["animationIndex"];
                                } else {
                                    actor.animationIndex = index;
                                    logger::warn("animationIndex property of actor {} of scene {} isn't an integer", index, node->scene_id);
                                }
                            } else {
                                actor.animationIndex = index;
                            }
                            
                            JsonUtil::loadBool(jsonActor, actor.noStrip, "noStrip", node->scene_id, objectType, false);

                            // TODO: this is too skyrim specific
                            if (jsonActor.contains("lookUp")) {
                                if (jsonActor["lookUp"].is_number_integer()) {
                                    int value = jsonActor["lookUp"];
                                    int type = 11;
                                    if (value < 0) {
                                        value *= -1;
                                        type = 8;
                                    }
                                    actor.eyeballModifierOverride[type] = {.type = type, .baseValue = static_cast<float>(value)};
                                } else {
                                    logger::warn("lookUp property of actor {} of scene {} isn't an integer", index, node->scene_id);
                                }
                            } else if (jsonActor.contains("lookDown")) {
                                if (jsonActor["lookDown"].is_number_integer()) {
                                    int value = jsonActor["lookDown"];
                                    int type = 8;
                                    if (value < 0) {
                                        value *= -1;
                                        type = 11;
                                    }
                                    actor.eyeballModifierOverride[type] = {.type = type, .baseValue = static_cast<float>(value)};
                                } else {
                                    logger::warn("lookDown property of actor {} of scene {} isn't an integer", index, node->scene_id);
                                }
                            }

                            if (jsonActor.contains("lookLeft")) {
                                if (jsonActor["lookLeft"].is_number_integer()) {
                                    int value = jsonActor["lookLeft"];
                                    int type = 9;
                                    if (value < 0) {
                                        value *= -1;
                                        type = 10;
                                    }
                                    actor.eyeballModifierOverride[type] = {.type = type, .baseValue = static_cast<float>(value)};
                                } else {
                                    logger::warn("lookLeft property of actor {} of scene {} isn't an integer", index, node->scene_id);
                                }
                            } else if (jsonActor.contains("lookRight")) {
                                if (jsonActor["lookRight"].is_number_integer()) {
                                    int value = jsonActor["lookRight"];
                                    int type = 10;
                                    if (value < 0) {
                                        value *= -1;
                                        type = 9;
                                    }
                                    actor.eyeballModifierOverride[type] = {.type = type, .baseValue = static_cast<float>(value)};
                                } else {
                                    logger::warn("lookRight property of actor {} of scene {} isn't an integer", index, node->scene_id);
                                }
                            }

                            if (jsonActor.contains("offset")) {
                                actor.offset.loadJson(path, jsonActor["offset"]);
                            }

                            if (jsonActor.contains("tags")) {
                                if (jsonActor["tags"].is_array()) {
                                    int index2 = 0;
                                    for (auto& jsonTag : jsonActor["tags"]) {
                                        if (jsonTag.is_string()) {
                                            std::string tag = jsonTag;
                                            StringUtil::toLower(&tag);
                                            actor.tags.push_back(tag);
                                        } else {
                                            logger::warn("tag {} of actor {} of scene {} isn't a string", index2, index, node->scene_id);
                                        }
                                        index2++;
                                    }
                                } else {
                                    logger::warn("tags property of actor {} of scene {} isn't a list", index, node->scene_id);
                                }
                            }

                            if (jsonActor.contains("feetOnGround")) {
                                if (jsonActor["feetOnGround"].is_boolean()) {
                                    actor.feetOnGround = jsonActor["feetOnGround"];
                                } else {
                                    logger::warn("feetOnGround property of actor {} of scene {} isn't a boolean", index, node->scene_id);
                                }
                            } else {
                                actor.feetOnGround = VectorUtil::containsAny(actor.tags, {"standing", "squatting"});
                            }

                            if (jsonActor.contains("autoTransitions")) {
                                if (jsonActor["autoTransitions"].is_object()) {
                                    for (auto& [id, transition] : jsonActor["autoTransitions"].items()) {
                                        if (transition.is_string()) {
                                            actor.autoTransitions[id] = transition;
                                        } else {
                                            logger::warn("autoTransition {} of actor {} of scene {} isn't a string", index, id, node->scene_id);
                                        }
                                    }
                                } else {
                                    logger::warn("autoTransitions property of actor {} of scene {} is malformed", index, node->scene_id);
                                }
                            }
                        } else {
                            logger::warn("actor {} of scene {} is malformed", index, node->scene_id);
                        }
                        node->actors.push_back(actor);
                        index++;
                    }
                } else {
                    logger::warn("actors property of scene {} isn't a list", node->scene_id);
                }
            } else {
                logger::warn("scene {} doesn't have an actors property defined", node->scene_id);
            }

            if (json.contains("actions")) {
                if (json["actions"].is_array()) {
                    int index = 0;
                    for (auto& jsonAction : json["actions"]) {
                        Graph::Action action;
                        if (jsonAction.is_object()) {
                            if (jsonAction.contains("type")) {
                                if (jsonAction["type"].is_string()) {
                                    action.type = getActionAlias(jsonAction["type"]);
                                    StringUtil::toLower(&action.type);
                                } else {
                                    logger::warn("property type of action {} of scene {} isn't a string", index, node->scene_id);
                                }
                            } else {
                                logger::warn("action {} of scene {} doesn't have a type property", index, node->scene_id);
                            }

                            RoleMapAPI::KEYS.forEach([&jsonAction, &action, index, &node](Role role, std::string key) {
                                if (jsonAction.contains(key)) {
                                    if (jsonAction[key].is_number_integer()) {
                                        *action.roles.get(role) = jsonAction[key];
                                    } else {
                                        logger::warn("property {} of action {} of scene {} isn't an integer", key, index, node->scene_id);
                                    }
                                } else {
                                    if (role == Role::ACTOR) {
                                        logger::warn("action {} of scene {} doesn't have an actor property", index, node->scene_id);
                                    } else {
                                        *action.roles.get(role) = *action.roles.get(Role::ACTOR);
                                    }
                                }
                            });

                            if (jsonAction.contains("muted")) {
                                if (jsonAction["muted"].is_boolean()) {
                                    action.muted = jsonAction["muted"];
                                } else {
                                    logger::warn("property muted of action {} of scene {} isn't a boolean", index, node->scene_id);
                                }
                            }
                        } else {
                            logger::warn("action {} of scene {} is malformed", index, node->scene_id);
                        }
                        action.attributes = GetActionAttributesByType(action.type);
                        node->actions.push_back(action);
                        index++;
                    }
                } else {
                    logger::warn("actions property of scene {} isn't a list", node->scene_id);
                }
            }

            if (node->isTransition) {
                node->tryAddTag("transition");
            }

            if (node->actors.size() >= 2) {
                bool gay = true;
                bool lesbian = true;

                for (GraphActor actor : node->actors) {
                    if (actor.condition.sex != GameAPI::GameSex::MALE) {
                        gay = false;
                    }
                    if (actor.condition.sex != GameAPI::GameSex::FEMALE) {
                        lesbian = false;
                    }

                    if (!gay && !lesbian) {
                        break;
                    }
                }

                if (gay) {
                    node->tryAddTag("gay");
                } else if (lesbian) {
                    node->tryAddTag("lesbian");
                }
            }

            node->mergeNodeIntoActors();

            addNode(node);
        });

        std::stable_sort(navigations.begin(), navigations.end(), [&](RawNavigation navigationA, RawNavigation navigationB){
            return navigationA.priority < navigationB.priority;
        });

        GraphTable::addNavigations(navigations);
    }
}