#include "NodeDistanceSoundType.h"

#include "SingleNodeDistanceSoundPlayer.h"

#include "Core/ThreadActor.h"

namespace Sound {
    SoundPlayer* NodeDistanceSoundType::create(OStim::ThreadActor* actor, OStim::ThreadActor* target) {
        std::vector<RE::NiAVObject*> actorNodes;
        for (std::string& bone : actorBones) {
            RE::NiAVObject* node = actor->getActor()->GetNodeByName(bone);
            if (node) {
                actorNodes.push_back(node);
            }
        }

        if (actorNodes.empty()) {
            return nullptr;
        }

        std::vector<RE::NiAVObject*> targetNodes;
        for (std::string& bone : targetBones) {
            RE::NiAVObject* node = target->getActor()->GetNodeByName(bone);
            if (node) {
                targetNodes.push_back(node);
            }
        }

        if (targetNodes.empty()) {
            return nullptr;
        }

        if (actorNodes.size() == 1 && targetNodes.size() == 1) {
            return new SingleNodeDistanceSoundPlayer(sound, actor->getActor(), actorNodes[0], targetNodes[0]);
        }

        return nullptr;
    }
}