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

        if (params.threadID > 10 || params.threadID == 0 && !hasPlayer) {
            return -1;
        }

        if (hasPlayer) {
            params.threadID = 0;
            startPlayerThread(params);
            return 0;
        } else {
            return startNPCThread(params);
        }
    }
}