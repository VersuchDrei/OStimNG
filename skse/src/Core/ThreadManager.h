#pragma once
#include "Core/Singleton.h"
#include "Core/Thread.h"
#include "GameAPI/GameActor.h"
#include "Serial/OldThread.h"
#include <shared_mutex>

namespace OStim {    

    class ThreadManager final : public ISingleton<ThreadManager> {
        
    public:
        ThreadManager();
        void TrackThread(ThreadId id, RE::TESObjectREFR* furniture, std::vector<RE::Actor*> actors);
        Thread* GetThread(ThreadId a_id);
        Thread* getPlayerThread();
        void UnTrackThread(ThreadId a_id);
        void queueThreadStop(ThreadId threadID);
        void UntrackAllThreads();
        bool AnySceneRunning();
        bool playerThreadRunning();

        Thread* findThread(GameAPI::GameActor actor);

        ThreadActor* findActor(GameAPI::GameActor actor);

        std::vector<Serialization::OldThread> serialize();

    private:
        using ThreadMap = std::unordered_map<ThreadId, Thread*>;
        std::shared_mutex m_threadMapMtx;
        ThreadMap m_threadMap;
        std::thread m_excitementThread;
        std::vector<ThreadId> threadStopQueue;

        void stopThreadNoLock(ThreadId threadID);
    };
}  // namespace OStim