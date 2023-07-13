#include "ThreadStarter.h"

#include "NPCThreadStarter.h"
#include "PlayerThreadStarter.h"

#include "Core/Core.h"

#include "Furniture/Furniture.h"

namespace OStim {
    int startThread(ThreadStartParams& params) {
        if (params.actors.empty()) {
            return -1;
        }

        bool hasPlayer = false;
        std::set<GameAPI::GameActor> actorSet;

        for (GameAPI::GameActor actor : params.actors) {
            if (!isEligible(actor)) {
                logger::info("actor {} is not eligible for OStim", actor.getName());
                return -1;
            }

            if (!actor.isLoaded()) {
                logger::info("actor {} is not loaded", actor.getName());
            }

            auto iter = actorSet.find(actor);
            if (iter != actorSet.end()) {
                logger::info("duplicate actor in list: {}", actor.getName());
                return -1;
            }
            actorSet.insert(actor);

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