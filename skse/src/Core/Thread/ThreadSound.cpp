#include "Core/Thread.h"

namespace OStim {
    bool Thread::isAnyActorTalking() {
        for (auto& [index, actor] : m_actors) {
            if (actor.getActor().isTalking()) {
                return true;
            }
        }
        return false;
    }
}