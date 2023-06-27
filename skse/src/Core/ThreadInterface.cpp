#include "Core/ThreadInterface.h"
#include "Core/ThreadManager.h"

namespace Interfaces {
	OStim::Thread* ThreadInterface::GetThread(int64_t threadId) {
		auto threadManager = OStim::ThreadManager::GetSingleton();
		return threadManager->GetThread(threadId);
	}
}