#include "ThreadStarter.h"

#include "NPCThreadStarter.h"
#include "PlayerThreadStarter.h"

#include "Core/Core.h"

#include "Furniture/Furniture.h"
#include "GameAPI/Game.h"
#include "Util/VectorUtil.h"

namespace OStim {
    int startThread(ThreadStartParams& params) {
        if (params.actors.empty()) {
            return -1;
        }

        bool hasPlayer = false;
        // can't use an actual set because hash functions are weird with structs
        std::vector<GameAPI::GameActor> actorSet;

        for (GameAPI::GameActor actor : params.actors) {
            if (!isEligible(actor)) {
                GameAPI::Game::notification("actor " + actor.getName() + " is not eligible for OStim");
                return -1;
            }

            if (!actor.isLoaded()) {
                GameAPI::Game::notification("actor " + actor.getName() + " is not loaded");
                return -1;
            }

            if (VectorUtil::contains(actorSet, actor)) {
                GameAPI::Game::notification("duplicate actor in list: " + actor.getName());
                return -1;
            }
            actorSet.push_back(actor);

            hasPlayer |= actor.isPlayer();
        }

        std::vector<Trait::ActorCondition> conditions = Trait::ActorCondition::create(params.actors);
        if (params.startingSequence && !params.startingSequence->fulfilledBy(conditions)) {
            logger::info("actors don't fulfill requirements of sequence {}", params.startingSequence->id);
            params.startingSequence = nullptr;
        }

        if (params.startingNode && !params.startingNode->fulfilledBy(Trait::ActorCondition::create(params.actors))) {
            logger::info("actors don't fulfill requirements of scene {}", params.startingNode->scene_id);
            params.startingNode = nullptr;
        }

        if (hasPlayer) {
            startPlayerThread(params);
            return 0;
        } else {
            return startNPCThread(params);
        }
    }
}