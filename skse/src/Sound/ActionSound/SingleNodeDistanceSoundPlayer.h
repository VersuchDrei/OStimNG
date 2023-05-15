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

        float lastDistance = 0;
        float in = true;

        inline float calculateDistance() {
            return powf((actorNode->world.translate.x - targetNode->world.translate.x) * 1000, 2) +
                   powf((actorNode->world.translate.y - targetNode->world.translate.y) * 1000, 2) +
                   powf((actorNode->world.translate.z - targetNode->world.translate.z) * 1000, 2);
        }
    };
}