#include "Core/ThreadManager.h"
#include "UI/Align/AlignMenu.h"

#include "Util/Constants.h"

namespace OStim {

    ThreadManager::ThreadManager() {
        m_excitementThread = std::thread([&]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(Constants::LOOP_TIME_MILLISECONDS));
                if (!RE::UI::GetSingleton()->GameIsPaused()) {
                    std::shared_lock<std::shared_mutex> lock(m_threadMapMtx);
                    for (auto& it : m_threadMap) {
                        it.second->loop();
                    }
                }
            }
        });
        m_excitementThread.detach();
    }

    void ThreadManager::TrackThread(ThreadId id, RE::TESObjectREFR* furniture, std::vector<RE::Actor*> actors) {
        Thread* t = new Thread(id, furniture, actors);
        std::unique_lock<std::shared_mutex> lock(m_threadMapMtx);
        m_threadMap.insert(std::make_pair(id, t));
        m_threadMap[id]->initContinue();
        auto log = RE::ConsoleLog::GetSingleton();
        if (log) {
            log->Print(("Tracking " + std::to_string(id)).c_str());
        }
    }

    Thread* ThreadManager::GetThread(ThreadId a_id) {
        std::shared_lock<std::shared_mutex> lock(m_threadMapMtx);
        auto it = m_threadMap.find(a_id);
        if (it == m_threadMap.end()) {
            return nullptr;
        }
        return it->second;
    }

    void ThreadManager::UnTrackThread(ThreadId a_id) {
        std::unique_lock<std::shared_mutex> lock(m_threadMapMtx);
        auto it = m_threadMap.find(a_id);
        if (it != m_threadMap.end()) {
            Thread* thread = it->second;
            UI::Align::AlignMenu::HandleThreadRemoved(thread);
            m_threadMap.erase(a_id);
            thread->close();
            delete thread;
            auto log = RE::ConsoleLog::GetSingleton();
            if (log) {
                log->Print(("Found scene: erasing " + std::to_string(a_id)).c_str());
            }
        }
    }

    void ThreadManager::UntrackAllThreads() {
        // this is a force close due to the user loading another save
        // so no need to free actors etc. here
        std::unique_lock<std::shared_mutex> lock(m_threadMapMtx);
        for (auto& entry : m_threadMap) {
            delete entry.second;
        }
        m_threadMap.clear();
    }

    bool ThreadManager::AnySceneRunning() {
        return m_threadMap.size() > 0;
    }

    ThreadActor* ThreadManager::findActor(RE::Actor* actor) {
        for (auto&[id, thread] : m_threadMap) {
            ThreadActor* threadActor = thread->GetActor(actor);
            if (threadActor) {
                return threadActor;
            }
        }

        return nullptr;
    }

    // serialized the currently running threads so that the actors can be properly set free on game load
    std::vector<Serialization::OldThread> ThreadManager::serialize() {
        std::vector<Serialization::OldThread> oldThreads;

        for (auto& it : m_threadMap) {
            oldThreads.push_back(it.second->serialize());
        }

        return oldThreads;
    }
}  // namespace OStim