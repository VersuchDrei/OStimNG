#include "ThreadInterfaceImpl.h"

#include "Core/ThreadManager.h"

namespace Interface {
    uint32_t ThreadInterfaceImpl::getVersion() {
        return 1;
    }


    OStim::Thread* ThreadInterfaceImpl::getThread(int32_t threadID) {
        return Threading::ThreadManager::GetSingleton()->GetThread(threadID);
    }


    void ThreadInterfaceImpl::registerThreadStartListener(OStim::ThreadEventListener* listener) {
        Threading::ThreadManager::GetSingleton()->registerThreadStartListener([listener](Threading::Thread* thread) { listener->listen(thread); });
    }

    void ThreadInterfaceImpl::registerSpeedChangedListener(OStim::ThreadEventListener* listener) {
        Threading::ThreadManager::GetSingleton()->registerSpeedChangedListener([listener](Threading::Thread* thread) { listener->listen(thread); });
    }

    void ThreadInterfaceImpl::registerNodeChangedListener(OStim::ThreadEventListener* listener) {
        Threading::ThreadManager::GetSingleton()->registerNodeChangedListener([listener](Threading::Thread* thread) { listener->listen(thread); });
    }

    void ThreadInterfaceImpl::registerClimaxListener(OStim::ThreadActorEventListener* listener) {
        Threading::ThreadManager::GetSingleton()->registerClimaxListener([listener](Threading::Thread* thread, Threading::ThreadActor* actor) { listener->listen(thread, actor); });
    }

    void ThreadInterfaceImpl::registerThreadStopListener(OStim::ThreadEventListener* listener) {
        Threading::ThreadManager::GetSingleton()->registerThreadEndListener([listener](Threading::Thread* thread) { listener->listen(thread); });
    }
}