#pragma once
#include "Core/Singleton.h"
#include "Core/Thread.h"
#include "Serial/OldThread.h"
#include <shared_mutex>

namespace OStim {    

    class ThreadManager final : public ISingleton<ThreadManager> {
        
    public:
        ThreadManager();
        void TrackThread(ThreadId id, RE::TESObjectREFR* furniture, std::vector<RE::Actor*> actors);
        Thread* GetThread(ThreadId a_id);
        void UnTrackThread(ThreadId a_id);
        void UntrackAllThreads();
        bool AnySceneRunning();

        ThreadActor* findActor(RE::Actor* actor);

        std::vector<Serialization::OldThread> serialize();

    private:
        using ThreadMap = std::unordered_map<ThreadId, Thread*>;
        std::shared_mutex m_threadMapMtx;
        ThreadMap m_threadMap;
        std::thread m_excitementThread;
    };
}  // namespace OStim