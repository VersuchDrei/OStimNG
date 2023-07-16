#include "SoundTable.h"

#include "Util/JsonFileLoader.h"
#include "Util/JsonUtil.h"
#include "Util/LookupTable.h"

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

            if (json.contains("moan")) {
                voiceSet.moan.loadJson(path, json["moan"]);
            }
            if (json.contains("moanMuffled")) {
                voiceSet.moanMuffled.loadJson(path, json["moanMuffled"]);
            }
            if (json.contains("moanExpression")) {
                voiceSet.moanExpression = json["moanExpression"];
            }

            if (json.contains("climax")) {
                voiceSet.climax.loadJson(path, json["climax"]);
            }
            if (json.contains("climaxMuffled")) {
                voiceSet.climaxMuffled.loadJson(path, json["climaxMuffled"]);
            }
            if (json.contains("climaxExpression")) {
                voiceSet.climaxExpression = json["climaxExpression"];
            }

            voiceSets[formID] = voiceSet;
        });
    }

    VoiceSet* SoundTable::getVoiceSet(GameAPI::GameActor actor) {
        auto iter = voiceSets.find(actor.getBaseFormID());
        if (iter != voiceSets.end()) {
            return &iter->second;
        }

        if (actor.form->GetActorBase()->voiceType) {
            iter = voiceSets.find(actor.getVoice().getFormID());
            if (iter != voiceSets.end()) {
                return &iter->second;
            }
        }

        iter = voiceSets.find(actor.getRace().getFormID());
        if (iter != voiceSets.end()) {
            return &iter->second;
        }

        if (actor.isHuman()) {
            iter = voiceSets.find(actor.isSex(GameAPI::GameSex::FEMALE) ? 1 : 0);
            if (iter != voiceSets.end()) {
                return &iter->second;
            }
        }

        return nullptr;
    }
}