#pragma once

#include "Furniture/FurnitureTable.h"
#include "Graph/LookupTable.h"
#include "Util/StringUtil.h"
#include "Util/VectorUtil.h"

namespace PapyrusDatabase {
    using VM = RE::BSScript::IVirtualMachine;

    std::set<std::string> aggressive_class{"Ro", "HhPJ", "HhBj", "HhPo", "SJ"};
    std::set<std::string> aggressive_mod{"BG", "BB"};

    json BuildJson(pugi::xml_document& xml_doc, const fs::path& mod_path, const fs::path& cls_path) {
        auto node = new Graph::Node();
        auto j_obj = json::object();

        std::vector<std::string> split_id;

        bool isOldFormat = true;

        if (auto scene = xml_doc.child("scene")) {
            if (auto id = scene.attribute("id")) {
                auto id_val = id.value();
                j_obj["sceneid"] = id_val;
                node->scene_id = id_val;
                node->lowercase_id = id_val;
                StringUtil::toLower(&node->lowercase_id);
                split_id = stl::string_split(id_val, '|');
            }

            int actorCount = 1;
            if (auto actors = scene.attribute("actors")) {
                actorCount = actors.as_int();
            }
            j_obj["NumActors"] = actorCount;

            if (auto info = scene.child("info")) {
                if (auto name = info.attribute("name")) {
                    j_obj["name"] = name.value();
                    node->scene_name = name.value();
                }
            }
                

            j_obj["OAanimids"] = json::array();

            int is_transitory = 0;
            int is_hub = 1;

            if (auto anim = scene.child("anim"))
                if (auto transitory = anim.attribute("t")) {
                    std::string trans_str{transitory.value()};
                    if (trans_str == "T"s) {
                        is_transitory = 1;
                        is_hub = 0;
                    }
                }

            j_obj["istransitory"] = is_transitory;
            node->isTransition = is_transitory;

            auto speed = scene.child("speed");
            if (speed && !is_transitory) {
                if (auto max = speed.attribute("max")) is_hub = 0;

                if (!is_hub) {
                    if (auto actor = speed.attribute("a")) j_obj["mainActor"] = actor.as_int();

                    j_obj["hasIdleSpeed"] = 0;
                    node->hasIdleSpeed = false;

                    for (auto& sp : speed.children("sp")) {
                        if (auto mtx = sp.attribute("mtx")) {
                            std::string mtx_str{mtx.value()};
                            if (mtx_str == "^idle"s) {
                                j_obj["hasIdleSpeed"] = 1;
                                node->hasIdleSpeed = true;
                                node->defaultSpeed = 1;
                            }
                        }

                        if (auto anim = sp.child("anim")) {
                            if (auto id = anim.attribute("id")) {
                                j_obj["OAanimids"].push_back(id.value());
                                if (auto playbackSpeed = anim.attribute("playbackSpeed")) {
                                    node->speeds.push_back({.animation = id.value(), .playbackSpeed = playbackSpeed.as_float()});
                                } else {
                                    node->speeds.push_back({.animation = id.value()});
                                }
                            }
                        }
                            
                    }
                }
            }

            j_obj["ishub"] = is_hub;
            node->isHub = is_hub;

            if (j_obj["OAanimids"].empty()) {
                auto anim_children = scene.children("anim");
                auto anim_count = std::distance(anim_children.begin(), anim_children.end());
                bool anim_first = true;

                for (auto& anim : anim_children) {
                    if (anim_first && (anim_count > 1)) {
                        anim_first = false;
                        continue;
                    }

                    if (auto id = anim.attribute("id")) {
                        j_obj["OAanimids"].push_back(id.value());
                        node->speeds.push_back({.animation = id.value()});
                    }
                }
            }

            // what the hell?
            if (auto animations = scene.child("animation")) {
                auto role_children = animations.children("animrole");
                auto role_count = std::distance(role_children.begin(), role_children.end());
                bool role_first = true;

                for (auto& role : role_children) {
                    if (role_first && (role_count > 1)) {
                        role_first = false;
                        continue;
                    }

                    if (auto anim = role.child("animplan").child("anim"))
                        if (auto id = anim.attribute("id")) {
                            j_obj["OAanimids"].push_back(id.value());
                            node->speeds.push_back({.animation = id.value()});
                        }
                }
            }

            if (auto metadata = scene.child("metadata")) {
                if (auto tags = metadata.attribute("tags")) {
                    for (std::string tag : StringUtil::toTagVector(tags.as_string())) {
                        node->tags.push_back(tag);
                    }
                }

                if (auto noRandomSelection = metadata.attribute("noRandomSelection")) {
                    node->noRandomSelection = noRandomSelection.as_bool();
                }

                if (auto furnitureType = metadata.attribute("furniture")) {
                    node->furnitureType = Furniture::FurnitureTable::getFurnitureType(furnitureType.as_string());
                }

                isOldFormat = false;
            }

            for (int i = 0; i < actorCount; i++) {
                node->actors.push_back(new Graph::Actor());
            }
            node->actors[0]->scale = 1.03;

            if (auto actors = scene.child("actors")) {
                for (auto& actor : actors.children("actor")) {

                    if (auto position = actor.attribute("position")) {
                        int pos = position.as_int();
                        if (pos >= 0 && pos < actorCount) {

                            if (auto penisAngle = actor.attribute("penisAngle")) {
                                node->actors[pos]->penisAngle = penisAngle.as_int();
                            }

                            if (auto scale = actor.attribute("scale")) {
                                node->actors[pos]->scale = scale.as_float();
                            }

                            if (auto scaleHeight = actor.attribute("scaleHeight")) {
                                node->actors[pos]->scaleHeight = scaleHeight.as_float();
                            }

                            if (auto expressionAction = actor.attribute("expressionAction")) {
                                node->actors[pos]->expressionAction = expressionAction.as_int();
                            }

                            if (auto lookUp = actor.attribute("lookUp")) {
                                float value = lookUp.as_float();
                                int type = 11;
                                if (value < 0) {
                                    value *= -1;
                                    type = 8;
                                }
                                node->actors[pos]->eyeballModifierOverride.insert({type, {.type = type, .baseValue = value}});
                            } else if (auto lookDown = actor.attribute("lookDown")) {
                                float value = lookUp.as_float();
                                int type = 8;
                                if (value < 0) {
                                    value *= -1;
                                    type = 11;
                                }
                                node->actors[pos]->eyeballModifierOverride.insert({type, {.type = type, .baseValue = value}});
                            }

                            if (auto lookLeft = actor.attribute("lookLeft")) {
                                float value = lookLeft.as_float();
                                int type = 9;
                                if (value < 0) {
                                    value *= -1;
                                    type = 10;
                                }
                                node->actors[pos]->eyeballModifierOverride.insert({type, {.type = type, .baseValue = value}});
                            } else if (auto lookRight = actor.attribute("lookRight")) {
                                float value = lookRight.as_float();
                                int type = 10;
                                if (value < 0) {
                                    value *= -1;
                                    type = 9;
                                }
                                node->actors[pos]->eyeballModifierOverride.insert({type, {.type = type, .baseValue = value}});
                            }

                            if (auto tags = actor.attribute("tags")) {
                                for (std::string tag : StringUtil::toTagVector(tags.as_string())) {
                                    node->actors[pos]->tags.push_back(tag);
                                }
                            }

                            if (auto feetOnGround = actor.attribute("feetOnGround")) {
                                node->actors[pos]->feetOnGround = feetOnGround.as_bool();
                            } else {
                                node->actors[pos]->feetOnGround = VectorUtil::containsAny(node->actors[pos]->tags, {"standing", "squatting"});
                            }

                            for (auto& autotransition : actor.children("autotransition")) {
                                auto type = autotransition.attribute("type");
                                auto destination = autotransition.attribute("destination");
                                if (!type || !destination) {
                                    continue;
                                }
                                node->actors[pos]->autotransitions.insert({type.as_string(), destination.as_string()});
                            }
                        }
                    }
                }

                isOldFormat = false;
            }

            if (auto actions = scene.child("actions")) {
                for (auto& action : actions.children("action")) {
                    auto type = action.attribute("type");
                    auto actor = action.attribute("actor");
                    if (!type || !actor) {
                        continue;
                    }

                    auto actionObj = new Graph::Action();

                    std::string typeStr = type.as_string();
                    StringUtil::toLower(&typeStr);
                    actionObj->type = typeStr;
                    actionObj->actor = actor.as_int();

                    if (auto target = action.attribute("target")) {
                        actionObj->target = target.as_int();
                    } else {
                        actionObj->target = actor.as_int();
                    }

                    if (auto performer = action.attribute("performer")) {
                        actionObj->performer = performer.as_int();
                    } else {
                        actionObj->performer = actor.as_int();
                    }
                    actionObj->attributes = Graph::LookupTable::GetActionAttributesByType(actionObj->type);
                    node->actions.push_back(actionObj);
                }

                node->mergeActionRequirementsIntoActors();

                isOldFormat = false;
            }
        }

        auto anim_class = cls_path.filename().string();
        auto source_mod = mod_path.filename().string();

        // ugh...
        if (anim_class == "PO") anim_class = "Po";

        int is_aggressive = 0;
        if (aggressive_class.find(anim_class) != aggressive_class.end())
            is_aggressive = 1;
        else if (aggressive_mod.find(source_mod) != aggressive_mod.end())
            is_aggressive = 1;
        else if (anim_class.find("Ag") != std::string::npos)
            is_aggressive = 1;

        j_obj["aggressive"] = is_aggressive;
        node->isAggresive = is_aggressive;
        j_obj["sourcemodule"] = source_mod;
        node->sourceModule = source_mod;
        j_obj["animclass"] = anim_class;
        node->animClass = anim_class;
        j_obj["positiondata"] = split_id[1];

        // auto generate data for legacy packs
        if (isOldFormat) {
            Graph::Action* actionObj = nullptr;
            Graph::Action* actionObj2 = nullptr;
            if (anim_class == "Sx") {
                actionObj = new Graph::Action();
                actionObj->type = "vaginalsex";
                actionObj->actor = 0;
                actionObj->target = 1;
            } else if (anim_class == "An") {
                actionObj = new Graph::Action();
                actionObj->type = "analsex";
                actionObj->actor = 0;
                actionObj->target = 1;
            } else if (anim_class == "BJ" || anim_class == "HhBJ" || anim_class == "PJ" || anim_class == "HhPJ") {
                actionObj = new Graph::Action();
                actionObj->type = "blowjob";
                actionObj->actor = 1;
                actionObj->target = 0;
            } else if (anim_class == "VJ") {
                actionObj = new Graph::Action();
                actionObj->type = "cunnilingus";
                actionObj->actor = 0;
                actionObj->target = 1;
            } else if (anim_class == "HJ" || anim_class == "ApHJ") {
                actionObj = new Graph::Action();
                actionObj->type = "handjob";
                actionObj->actor = 1;
                actionObj->target = 0;
            } else if (anim_class == "Cr") {
                actionObj = new Graph::Action();
                actionObj->type = "rubbingclitoris";
                actionObj->actor = 0;
                actionObj->target = 1;
            } else if (anim_class == "Pf1" || anim_class == "Pf2") {
                actionObj = new Graph::Action();
                actionObj->type = "vaginalfingering";
                actionObj->actor = 0;
                actionObj->target = 1;
            } else if (anim_class == "BoJ") {
                actionObj = new Graph::Action();
                actionObj->type = "boobjob";
                actionObj->actor = 1;
                actionObj->target = 0;
                actionObj = new Graph::Action();
                actionObj->type = "footjob";
                actionObj->actor = 1;
                actionObj->target = 0;
            } else if (anim_class == "BoF") {
                actionObj = new Graph::Action();
                actionObj->type = "suckingnipples";
                actionObj->actor = 0;
                actionObj->target = 1;
            } else if (anim_class == "Po") {
                actionObj = new Graph::Action();
                actionObj->type = "masturbation";
                actionObj->actor = 0;
                actionObj->target = 0;
            } else if (anim_class == "DHJ") {
                actionObj = new Graph::Action();
                actionObj->type = "handjob";
                actionObj->actor = 1;
                actionObj->target = 0;
                actionObj2 = new Graph::Action();
                actionObj2->type = "handjob";
                actionObj2->actor = 1;
                actionObj2->target = 2;
            } else if (anim_class == "VBJ") {
                actionObj = new Graph::Action();
                actionObj->type = "blowjob";
                actionObj->actor = 1;
                actionObj->target = 0;
                actionObj2 = new Graph::Action();
                actionObj2->type = "cunnilingus";
                actionObj2->actor = 0;
                actionObj2->target = 1;
            } else if (anim_class == "VHJ") {
                actionObj = new Graph::Action();
                actionObj->type = "handjob";
                actionObj->actor = 1;
                actionObj->target = 0;
                actionObj2 = new Graph::Action();
                actionObj2->type = "cunnilingus";
                actionObj2->actor = 0;
                actionObj2->target = 1;
            }
            if (actionObj) {
                actionObj->performer = actionObj->actor;
                actionObj->attributes = Graph::LookupTable::GetActionAttributesByType(actionObj->type);
                node->actions.push_back(actionObj);
            }
            if (actionObj2) {
                actionObj2->performer = actionObj2->actor;
                actionObj2->attributes = Graph::LookupTable::GetActionAttributesByType(actionObj2->type);
                node->actions.push_back(actionObj2);
            }
        }

        Graph::LookupTable::addNode(node);

        return j_obj;
    }

    void BuildDB() {
        const auto timer_start = std::chrono::high_resolution_clock::now();
        const fs::path root_path("Data\\Meshes\\0SA\\mod\\0Sex\\scene");

        if (!fs::exists(root_path)) {
            logger::info("scene folder not found");
            return;
        }

        auto j_root = json::array();
        for (auto const& mod : fs::directory_iterator(root_path)) {
            if (!mod.is_directory()) continue;

            auto& mod_path = mod.path();
            for (auto& pos : fs::directory_iterator(mod_path)) {
                if (!pos.is_directory()) continue;

                auto& pos_path = pos.path();
                auto posName = pos_path.filename().string();
                if (posName == "_TOG") continue;

                for (auto& cls : fs::directory_iterator(pos_path)) {
                    if (!cls.is_directory()) continue;

                    auto& cls_path = cls.path();
                    for (auto& file : fs::directory_iterator(cls_path)) {
                        if (!file.exists() || !file.is_regular_file()) continue;

                        auto& file_path = file.path();

                        auto fileExt = file_path.extension().string();
                        if (fileExt != ".xml") continue;

                        auto file_path_str = file_path.string();

                        pugi::xml_document xml_doc;
                        auto xml_result = xml_doc.load_file(file_path_str.c_str());

                        if (!xml_result) {
                            logger::error("load failed: {} [{}]", file_path_str, xml_result.description());
                            continue;
                        }

                        auto j_build = BuildJson(xml_doc, mod_path, cls_path);
                        j_root.push_back(j_build);
                    }
                }
            }
        }

        const auto db_path = util::database_path();
        std::ofstream db_file(*db_path);
        db_file << std::setw(2) << j_root << std::endl;

        const auto timer_finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timer_elapsed = timer_finish - timer_start;
        logger::info("Build time: {}s", timer_elapsed.count());
    }
}  // namespace PapyrusDatabase