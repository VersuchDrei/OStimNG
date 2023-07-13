#pragma once

#include "ThreadStartParams.h"
#include "GameAPI/GameActor.h"

namespace OStim {
    void startPlayerThread(ThreadStartParams params);

    bool getInDialogue(std::vector<GameAPI::GameActor> actors);
    void handleFurniture(ThreadStartParams params);
    void addFurniture(ThreadStartParams params, RE::TESObjectREFR* furniture);
    void handleActorSorting(ThreadStartParams params);
    void sortActors(ThreadStartParams params, int playerIndex);
    void handleStartingNode(ThreadStartParams params);
    void startInner(ThreadStartParams params);
}