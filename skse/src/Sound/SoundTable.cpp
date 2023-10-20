#include "SoundTable.h"

#include "GameAPI/GameCondition.h"
#include "Serial/Manager.h"
#include "Util/JsonFileLoader.h"
#include "Util/JsonUtil.h"
#include "Util/LookupTable.h"
#include "Util/MathUtil.h"

namespace Sound {
    const char* VOICE_SET_FILE_PATH{"Data/SKSE/Plugins/OStim/voice sets"};

    void SoundTable::setup() {
        // voice sets
        Util::JsonFileLoader::LoadFilesInFolder(VOICE_SET_FILE_PATH, [&](std::string path, std::string filename, json json) {
            if (!json.contains("target")) {
                logger::warn("file {} does not have field 'target' defined", path);
                return;
            }

            auto& target = json["target"];
            if (!target.contains("mod")) {
                logger::warn("file {} does not have field 'target.mod' defined", path);
                return;
            }
            if (!target.contains("formid")) {
                logger::warn("file {} does not have field 'target.formid' defined", path);
                return;
            }

            RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();

            std::string stringID = target["formid"];
            uint32_t formID = std::stoi(stringID, nullptr, 16);
            std::string file = target["mod"];
            if (const RE::TESFile* mod = dataHandler->LookupLoadedModByName(file)) {
                formID += mod->GetCompileIndex() << 24;
            } else if (const RE::TESFile* mod = dataHandler->LookupLoadedLightModByName(file)) {
                formID += mod->GetPartialIndex() << 12;
            }

            VoiceSet voiceSet;

            JsonUtil::loadString(json, voiceSet.name, "name", filename, "voice set", false);
            if (voiceSet.name.empty()) {
                voiceSet.name = filename;
            }

            if (json.contains("moan") && json["moan"].is_object() && json["moan"].contains("mod")) {
                // legacy syntax
                loadSoundSets(json, voiceSet.moan.sound, "moan", "moan", "moanExpression", filename, path);
                loadSoundSets(json, voiceSet.moan.soundMuffled, "moanMuffled", "moan", "moanExpression", filename, path);
                loadSoundSets(json, voiceSet.climax.sound, "climax", "climax", "climaxExpression", filename, path);
                loadSoundSets(json, voiceSet.climax.soundMuffled, "climaxMuffled", "climax", "climaxExpression", filename, path);
            } else {
                loadReactionSet(json, voiceSet.moan, "moan", filename, path);
                loadReactionSet(json, voiceSet.climax, "climax", filename, path);
                loadReactionSet(json, voiceSet.climaxCommentSelf, "climaxCommentSelf", filename, path);
                loadReactionSet(json, voiceSet.climaxCommentOther, "climaxCommentOther", filename, path);

                loadReactionMap(json, voiceSet.eventActorReactions, "eventActorReactions", filename, path);
                loadReactionMap(json, voiceSet.eventTargetReactions, "eventTargetReactions", filename, path);
                loadReactionMap(json, voiceSet.eventPerformerReactions, "eventPerformerReactions", filename, path);

                JsonUtil::loadGameRecord(json, voiceSet.postSceneDialogue, "postSceneDialogue", filename, "voice set", path, false);
            }

            //TODO event reactions

            voiceSets[formID] = voiceSet;
        });
    }

    VoiceSet SoundTable::getVoiceSet(GameAPI::GameActor actor) {
        RE::FormID selection = Serialization::getVoiceSet(actor.getBaseFormID());
        if (selection != 0) {
            auto iter = voiceSets.find(selection);
            if (iter != voiceSets.end()) {
                logger::info("voice set found for actor {} by user selection", actor.getName());
                return iter->second;
            }
        }

        auto iter = voiceSets.find(actor.getBaseFormID());
        if (iter != voiceSets.end()) {
            logger::info("voice set found for actor {} by actor base", actor.getName());
            return iter->second;
        }

        if (actor.getVoice()) {
            iter = voiceSets.find(actor.getVoice().getFormID());
            if (iter != voiceSets.end()) {
                logger::info("voice set found for actor {} by voice type", actor.getName());
                return iter->second;
            }
        }

        iter = voiceSets.find(actor.getRace().getFormID());
        if (iter != voiceSets.end()) {
            logger::info("voice set found for actor {} by race", actor.getName());
            return iter->second;
        }

        if (actor.isHuman()) {
            logger::info("no voice set found for actor {}, using default", actor.getName());
            iter = voiceSets.find(actor.isSex(GameAPI::GameSex::FEMALE) ? 1 : 0);
            if (iter != voiceSets.end()) {
                return iter->second;
            }
        } else {
            logger::info("no voice set found for actor {}", actor.getName());
        }

        return {};
    }


    std::vector<std::string> SoundTable::getVoiceSetPairs() {
        std::vector<std::string> ret;
        ret.push_back("0");
        ret.push_back("default");

        std::vector<std::pair<std::string, RE::FormID>> voices;

        for (auto& [id, voiceSet] : voiceSets) {
            if (id > 1) {
                voices.push_back({voiceSet.name, id});
            }
        }

        std::sort(voices.begin(), voices.end(), [&](std::pair<std::string, RE::FormID> pairA, std::pair<std::string, RE::FormID> pairB) {
            return pairA.first.compare(pairB.first);
        });

        for (auto& [name, id] : voices) {
            ret.push_back(std::to_string(MathUtil::uintToInt(id)));
            ret.push_back(name);
        }

        return ret;
    }

    std::string SoundTable::getVoiceSetName(RE::FormID formID) {
        RE::FormID voiceID = Serialization::getVoiceSet(formID);

        if (voiceID == 0) {
            // for voice sets 0 is default male, but for serialization it is just default
            return "default";
        }

        auto iter = voiceSets.find(voiceID);
        if (iter != voiceSets.end()) {
            return iter->second.name;
        }

        return "default";
    }

    void SoundTable::setVoiceSet(RE::FormID formID, std::string voice) {
        RE::FormID voiceID = MathUtil::intToUint(std::stoi(voice));
        Serialization::setVoiceSet(formID, voiceID);
    }


    void SoundTable::loadSoundSets(json& json, std::vector<SoundSet>& soundSets, std::string propertyName, std::string defaultExpression, std::string expressionPropertyName, std::string& fileName, std::string& filePath) {
        if (!json.contains(propertyName)) {
            return;
        }

        JsonUtil::loadString(json, defaultExpression, expressionPropertyName, fileName, "voice set", false);

        if (json[propertyName].is_object()) {
            GameAPI::GameSound sound;
            sound.loadJson(filePath, json[propertyName]);
            if (sound) {
                soundSets.push_back({{}, sound, defaultExpression});
            }
        } else if (json[propertyName].is_array()) {
            int index = 0;
            for (auto& jsonSound : json[propertyName]) {
                if (jsonSound.contains("sound")) {
                    GameAPI::GameSound sound;
                    sound.loadJson(filePath, jsonSound["sound"]);

                    if (sound) {
                        GameAPI::GameCondition condition;
                        if (jsonSound.contains("condition")) {
                            condition.loadJson(filePath, jsonSound["condition"]);
                        }

                        std::string expression = defaultExpression;
                        if (jsonSound.contains("expression")) {
                            if (jsonSound["expression"].is_string()) {
                                expression = jsonSound["expression"];
                            } else {
                                logger::warn("property 'expression' of {} {} of voice type {} is not a string", propertyName, index, fileName);
                            }
                        }

                        int moanIntervalOverride = 0;
                        if (jsonSound.contains("moanIntervalOverride")) {
                            if (jsonSound["moanIntervalOverride"].is_number()) {
                                moanIntervalOverride = static_cast<int>(static_cast<float>(jsonSound["moanIntervalOverride"]) * 1000.0f);
                            } else {
                                logger::warn("property 'moanIntervalOverride' of {} {} of voice type {} is not a number", propertyName, index, fileName);
                            }
                        }

                        soundSets.push_back({condition, sound, expression, moanIntervalOverride});
                    }
                } else {
                    logger::warn("sound {} of sound list '{}' of voice type {} does not have field 'sound' defined", index, propertyName, fileName);
                }

                index++;
            }
        } else {
            logger::warn("property '{}' of voice set {} is malformed", propertyName, fileName);
        }
    }

    void SoundTable::loadDialogueSets(json& json, std::vector<DialogueSet>& dialogueSets, std::string propertyName, std::string& fileName, std::string& filePath) {
        if (!json.contains(propertyName)) {
            return;
        }

         if (json[propertyName].is_object()) {
            GameAPI::GameDialogue dialogue;
            dialogue.loadJson(filePath, json[propertyName]);
            if (dialogue) {
                dialogueSets.push_back({{}, dialogue, ""});
            }
        } else if (json[propertyName].is_array()) {
            int index = 0;
            for (auto& jsonDialogue : json[propertyName]) {
                if (jsonDialogue.contains("dialogue")) {
                    GameAPI::GameDialogue dialogue;
                    dialogue.loadJson(filePath, jsonDialogue["dialogue"]);

                    if (dialogue) {
                        GameAPI::GameCondition condition;
                        if (jsonDialogue.contains("condition")) {
                            condition.loadJson(filePath, jsonDialogue["condition"]);
                        }

                        std::string expression = "";
                        if (jsonDialogue.contains("expression")) {
                            if (jsonDialogue["expression"].is_string()) {
                                expression = jsonDialogue["expression"];
                            } else {
                                logger::warn("property 'expression' of {} {} of voice type {} is not a string", propertyName, index, fileName);
                            }
                        }

                        int dialogueThreshold = 0;
                        if (jsonDialogue.contains("dialogueThreshold")) {
                            if (jsonDialogue["dialogueThreshold"].is_number_integer()) {
                                dialogueThreshold = jsonDialogue["dialogueThreshold"];
                            } else {
                                logger::warn("property 'dialogueThreshold' of {} {} of voice type {} is not a number", propertyName, index, fileName);
                            }
                        }

                        int moanIntervalOverride = 0;
                        if (jsonDialogue.contains("moanIntervalOverride")) {
                            if (jsonDialogue["moanIntervalOverride"].is_number()) {
                                moanIntervalOverride = static_cast<int>(static_cast<float>(jsonDialogue["moanIntervalOverride"]) * 1000.0f);
                            } else {
                                logger::warn("property 'moanIntervalOverride' of {} {} of voice type {} is not a number", propertyName, index, fileName);
                            }
                        }

                        dialogueSets.push_back({condition, dialogue, expression, dialogueThreshold, moanIntervalOverride});
                    }
                } else {
                    logger::warn("dialogue {} of dialogue list '{}' of voice type {} does not have field 'dialogue' defined", index, propertyName, fileName);
                }

                index++;
            }
        } else {
            logger::warn("property '{}' of voice set {} is malformed", propertyName, fileName);
        }
    }

    void SoundTable::loadReactionSet(json& json, ReactionSet& reactionSet, std::string propertyName, std::string& fileName, std::string& filePath) {
        if (!json.contains(propertyName)) {
            return;
        }

        if (!json[propertyName].is_object()) {
            logger::warn("property '{}' of voice set {} is malformed", propertyName, fileName);
            return;
        }

        loadReactionSet(json[propertyName], reactionSet, fileName, filePath);
    }

    void SoundTable::loadReactionSet(json& json, ReactionSet& reactionSet, std::string& fileName, std::string& filePath) {
        loadSoundSets(json, reactionSet.sound, "sound", "", "soundExpression", fileName, filePath);
        loadSoundSets(json, reactionSet.soundMuffled, "soundMuffled", "", "soundExpression", fileName, filePath);
        loadDialogueSets(json, reactionSet.dialogue, "dialogue", fileName, filePath);
    }

    void SoundTable::loadReactionMap(json& json, std::unordered_map<std::string, ReactionSet>& reactionMap, std::string propertyName, std::string& fileName, std::string& filePath) {
        if (!json.contains(propertyName)) {
            return;
        }

        if (!json[propertyName].is_object()) {
            logger::warn("property '{}' of voice set {} is malformed", propertyName, fileName);
            return;
        }

        for (auto& [key, set] : json[propertyName].items()) {
            if (!set.is_object()) {
                logger::warn("property '{}.{}' of voice set {} is malformed", propertyName, key, fileName);
                continue;
            }

            ReactionSet reactionSet;
            loadReactionSet(set, reactionSet, fileName, filePath);

            reactionMap[key] = reactionSet;
        }
    }
}