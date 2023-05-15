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

    VoiceSet* SoundTable::getVoiceSet(RE::Actor* actor) {
        auto iter = voiceSets.find(actor->GetActorBase()->formID);
        if (iter != voiceSets.end()) {
            return &iter->second;
        }

        if (actor->GetActorBase()->voiceType) {
            iter = voiceSets.find(actor->GetActorBase()->voiceType->formID);
            if (iter != voiceSets.end()) {
                return &iter->second;
            }
        }

        iter = voiceSets.find(actor->GetActorBase()->race->formID);
        if (iter != voiceSets.end()) {
            return &iter->second;
        }

        if (actor->HasKeyword(Util::LookupTable::ActorTypeNPC)) {
            iter = voiceSets.find(actor->GetActorBase()->GetSex() == RE::SEX::kFemale ? 1 : 0);
            if (iter != voiceSets.end()) {
                return &iter->second;
            }
        }

        return nullptr;
    }
}