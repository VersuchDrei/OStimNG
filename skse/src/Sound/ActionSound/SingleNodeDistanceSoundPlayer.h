#pragma once

#include "SoundPlayer.h"

#include <math.h>

namespace Sound {
    class SingleNodeDistanceSoundPlayer : public SoundPlayer {
    public:
        inline SingleNodeDistanceSoundPlayer(GameAPI::GameSound sound, GameAPI::GameActor actor, GameAPI::GameActor target, RE::NiAVObject* actorNode, RE::NiAVObject* targetNode)
            : SoundPlayer(sound, actor, target), actorNode{actorNode}, targetNode{targetNode} {
            lastDistance = calculateDistance();
        };

        virtual void loop();

    private:
        RE::NiAVObject* actorNode;
        RE::NiAVObject* targetNode;

        int countDown = 10;
        float lastDistance = 0;
        float in = true;

        inline float calculateDistance() {
            return powf((actorNode->local.translate.x - targetNode->local.translate.x) * 1000, 2) +
                   powf((actorNode->local.translate.y - targetNode->local.translate.y) * 1000, 2) +
                   powf((actorNode->local.translate.z - targetNode->local.translate.z) * 1000, 2);
        }
    };
}