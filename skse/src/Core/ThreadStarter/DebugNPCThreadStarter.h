#pragma once

#include "ThreadStartParams.h"

namespace Threading {
    void startDebugNPCThread(ThreadStartParams params);

    namespace {
        void handleActorAdding(ThreadStartParams params);
        void addActor(ThreadStartParams params, GameAPI::GameActor actor);
    }
}