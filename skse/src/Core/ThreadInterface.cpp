#include "Core/ThreadInterface.h"
#include "Core/ThreadManager.h"

namespace Interfaces {
    Threading::Thread* ThreadInterface::GetThread(int64_t threadId) {
        auto threadManager = Threading::ThreadManager::GetSingleton();
		return threadManager->GetThread(threadId);
	}
}