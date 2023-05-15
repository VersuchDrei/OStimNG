#include "SoundType.h"

#include "NodeDistanceSoundType.h"

namespace Sound {
    SoundType* SoundType::fromJson(std::string path, json json) {
        if (!json.contains("type")) {
            logger::warn("file {} does not have field 'sound.type' defined", path);
            return nullptr;
        }

        if (!json.contains("sound")) {
            logger::warn("file {} does not have field 'sound.sound' defined", path);
            return nullptr;
        }

        GameAPI::GameSound sound;
        sound.loadJson(path, json["sound"]);
        if (!sound) {
            return nullptr;
        }

        std::string type = json["type"];
        if (type == "nodedistance") {
            if (!json.contains("actorBone")) {
                logger::warn("file {} does not have field 'sound.actorBone' defined", path);
                return nullptr;
            }

            if (!json.contains("targetBone")) {
                logger::warn("file {} does not have field 'sound.targetBone' defined", path);
                return nullptr;
            }

            std::vector<std::string> actorBones;
            auto& aBones = json["actorBone"];
            if (aBones.is_array()) {
                for (auto& bone : aBones) {
                    actorBones.push_back(bone);
                }
            } else {
                actorBones.push_back(aBones);
            }

            std::vector<std::string> targetBones;
            auto& tBones = json["actorBone"];
            if (tBones.is_array()) {
                for (auto& bone : tBones) {
                    targetBones.push_back(bone);
                }
            } else {
                targetBones.push_back(tBones);
            }

            return new NodeDistanceSoundType(sound, actorBones, targetBones);
        }

        logger::warn("file {} has unknown sound type {}", path, type);
        return nullptr;
    }
}