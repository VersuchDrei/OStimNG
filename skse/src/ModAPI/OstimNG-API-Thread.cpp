#include "OstimNG-API-Thread.h"
#include "Core/ThreadManager.h"
#include "Graph/GraphTable.h"
#include "UI/Scene/SceneOptions.h"
#include "UI/UIState.h"
#include "MCM/MCMTable.h"

namespace OstimNG_API::Thread
{
    class ThreadInterface : public IThreadInterface
    {
    private:
        struct CallbackRegistration
        {
            ThreadEventCallback callback;
            void* userData;
        };
        std::vector<CallbackRegistration> eventCallbacks;
        
        struct ControlCallbackRegistration
        {
            ControlEventCallback callback;
            void* userData;
        };
        std::vector<ControlCallbackRegistration> controlCallbacks;
        
        std::mutex callbackMutex;

    public:
        static ThreadInterface* GetSingleton() noexcept
        {
            static ThreadInterface singleton;
            return std::addressof(singleton);
        }

        void NotifyEvent(ThreadEvent eventType, uint32_t threadID)
        {
            std::lock_guard<std::mutex> lock(callbackMutex);
            for (auto& registration : eventCallbacks)
            {
                registration.callback(eventType, threadID, registration.userData);
            }
        }

        void NotifyControlInput(Controls controlType, uint32_t threadID)
        {
            std::lock_guard<std::mutex> lock(callbackMutex);
            for (auto& registration : controlCallbacks)
            {
                registration.callback(controlType, threadID, registration.userData);
            }
        }

        uint32_t GetPlayerThreadID() noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->getPlayerThread();
            return thread ? thread->m_threadId : 0;
        }

        bool IsThreadValid(uint32_t threadID) noexcept override
        {
            return Threading::ThreadManager::GetSingleton()->GetThread(threadID) != nullptr;
        }

        const char* GetCurrentSceneID(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return nullptr;

            auto state = UI::UIState::GetSingleton();
            auto currentNode = state->currentNode;

            return currentNode ? currentNode->scene_id.c_str() : nullptr;
        }

        uint32_t GetActorCount(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return 0;

            return static_cast<uint32_t>(thread->getActors().size());
        }

        uint32_t GetActors(uint32_t threadID, ActorData* buffer, uint32_t bufferSize) noexcept override
        {
            if (!buffer || bufferSize == 0) return 0;

            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return 0;

            uint32_t count = 0;
            for (auto& [index, actor] : thread->getActors())
            {
                if (count >= bufferSize) break;

                buffer[count].formID = actor.getActor().form->GetFormID();
                buffer[count].excitement = actor.getExcitement();
                buffer[count].isFemale = actor.isFemale();
                buffer[count].hasSchlong = actor.hasSchlong();
                buffer[count].timesClimaxed = actor.getTimesClimaxed();
                count++;
            }

            return count;
        }

        uint32_t GetNavigationCount(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return 0;

            auto state = UI::UIState::GetSingleton();
            auto currentNode = state->currentNode;

            if (!currentNode || currentNode->isTransition || thread->isInSequence() ||
                (thread->getThreadFlags() & Threading::ThreadFlag::NO_PLAYER_CONTROL))
            {
                return 0;
            }

            std::vector<Trait::ActorCondition> conditions = thread->getActorConditions();
            uint32_t count = 0;

            for (auto& nav : currentNode->navigations)
            {
                if (nav.fulfilledBy(conditions) &&
                    thread->getFurnitureType()->isChildOf(nav.nodes.back()->furnitureType))
                {
                    count++;
                }
            }

            return count;
        }

        uint32_t GetNavigationOptions(uint32_t threadID, NavigationData* buffer, uint32_t bufferSize) noexcept override
        {
            if (!buffer || bufferSize == 0) return 0;

            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return 0;

            auto state = UI::UIState::GetSingleton();
            auto currentNode = state->currentNode;

            if (!currentNode || currentNode->isTransition || thread->isInSequence() ||
                (thread->getThreadFlags() & Threading::ThreadFlag::NO_PLAYER_CONTROL))
            {
                return 0;
            }

            std::vector<Trait::ActorCondition> conditions = thread->getActorConditions();
            uint32_t count = 0;

            for (auto& nav : currentNode->navigations)
            {
                if (count >= bufferSize) break;

                if (nav.fulfilledBy(conditions) &&
                    thread->getFurnitureType()->isChildOf(nav.nodes.back()->furnitureType))
                {
                    buffer[count].sceneId = nav.nodes.front()->scene_id;
                    buffer[count].destinationId = nav.nodes.back()->scene_id;
                    buffer[count].icon = nav.icon;
                    buffer[count].description = nav.getDescription(thread);
                    buffer[count].border = nav.border;
                    buffer[count].isTransition = nav.isTransition;
                    count++;
                }
            }

            return count;
        }

        APIResult NavigateToScene(uint32_t threadID, const char* sceneID) noexcept override
        {
            if (!sceneID) {
                SKSE::log::warn("NavigateToScene: Invalid sceneID (nullptr)");
                return APIResult::Invalid;
            }

            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) {
                SKSE::log::warn("NavigateToScene: Thread not found for ID {}", threadID);
                return APIResult::Invalid;
            }

            SKSE::GetTaskInterface()->AddTask([threadID, sceneID = std::string(sceneID)]()
            {
                auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
                if (thread)
                {
                    thread->Navigate(sceneID);
                }
                else
                {
                    SKSE::log::warn("NavigateToScene (Task): Thread {} no longer exists", threadID);
                }
            });

            return APIResult::OK;
        }

        bool IsTransition(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return false;

            auto state = UI::UIState::GetSingleton();
            auto currentNode = state->currentNode;

            return currentNode ? currentNode->isTransition : false;
        }

        bool IsInSequence(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return false;

            return thread->isInSequence();
        }

        bool IsAutoMode(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return false;

            return thread->isInAutoMode();
        }

        bool IsPlayerControlDisabled(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return false;

            return (thread->getThreadFlags() & Threading::ThreadFlag::NO_PLAYER_CONTROL) != 0;
        }

        void RegisterEventCallback(ThreadEventCallback callback, void* userData) noexcept override
        {
            if (!callback) return;

            std::lock_guard<std::mutex> lock(callbackMutex);
            eventCallbacks.push_back({callback, userData});
            SKSE::log::info("Registered thread event callback");
        }

        void UnregisterEventCallback(ThreadEventCallback callback) noexcept override
        {
            if (!callback) return;

            std::lock_guard<std::mutex> lock(callbackMutex);
            eventCallbacks.erase(
                std::remove_if(eventCallbacks.begin(), eventCallbacks.end(),
                    [callback](const CallbackRegistration& reg) { return reg.callback == callback; }),
                eventCallbacks.end());
            SKSE::log::info("Unregistered thread event callback");
        }

        void RegisterControlCallback(ControlEventCallback callback, void* userData) noexcept override
        {
            if (!callback) return;

            std::lock_guard<std::mutex> lock(callbackMutex);
            controlCallbacks.push_back({callback, userData});
            SKSE::log::info("Registered control event callback");
        }

        void UnregisterControlCallback(ControlEventCallback callback) noexcept override
        {
            if (!callback) return;

            std::lock_guard<std::mutex> lock(callbackMutex);
            controlCallbacks.erase(
                std::remove_if(controlCallbacks.begin(), controlCallbacks.end(),
                    [callback](const ControlCallbackRegistration& reg) { return reg.callback == callback; }),
                controlCallbacks.end());
            SKSE::log::info("Unregistered control event callback");
        }

        void SetExternalUIEnabled(bool enabled) noexcept override
        {
            UI::UIState::SetExternalUIEnabled(enabled);
            SKSE::log::info("External UI {}", enabled ? "enabled" : "disabled");
        }

        void GetKeyData(KeyData* outData) noexcept override
        {
            if (!outData) return;
            outData->keyUp = MCM::MCMTable::keyUp();
            outData->keyDown = MCM::MCMTable::keyDown();
            outData->keyLeft = MCM::MCMTable::keyLeft();
            outData->keyRight = MCM::MCMTable::keyRight();
            outData->keyYes = MCM::MCMTable::keyYes();
            outData->keyEnd = MCM::MCMTable::keyEnd();
            outData->keyToggle = MCM::MCMTable::keyToggle();
            outData->keySearch = MCM::MCMTable::keySearch();
            outData->keyAlignment = MCM::MCMTable::keyAlignment();
            outData->keySceneStart = MCM::MCMTable::keySceneStart();
            outData->keyNpcSceneStart = MCM::MCMTable::keyNpcSceneStart();
            outData->keySpeedUp = MCM::MCMTable::keySpeedUp();
            outData->keySpeedDown = MCM::MCMTable::keySpeedDown();
            outData->keyPullOut = MCM::MCMTable::keyPullOut();
            outData->keyAutoMode = MCM::MCMTable::keyAutoMode();
            outData->keyFreeCam = MCM::MCMTable::keyFreeCam();
            outData->keyHideUI = MCM::MCMTable::keyHideUI();
        }

        // --- Navigation ---

        const char* GetCurrentNodeName(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return nullptr;

            auto currentNode = thread->getCurrentNodeInternal();
            return currentNode ? currentNode->scene_name.c_str() : nullptr;
        }

        int32_t GetCurrentSpeed(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return 0;

            return thread->getCurrentSpeed();
        }

        int32_t GetMaxSpeed(uint32_t threadID) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return 0;

            auto currentNode = thread->getCurrentNodeInternal();
            if (!currentNode || currentNode->speeds.empty()) return 0;

            return static_cast<int32_t>(currentNode->speeds.size()) - 1;
        }

        APIResult SetSpeed(uint32_t threadID, int32_t speed) noexcept override
        {
            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return APIResult::Invalid;

            auto currentNode = thread->getCurrentNodeInternal();
            if (!currentNode) return APIResult::Invalid;

            if (speed < 0 || speed >= static_cast<int32_t>(currentNode->speeds.size()))
                return APIResult::Failed;

            SKSE::GetTaskInterface()->AddTask([threadID, speed]()
            {
                auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
                if (thread)
                {
                    thread->SetSpeed(speed);
                }
            });

            return APIResult::OK;
        }

        // --- Alignment ---

        bool GetActorAlignment(uint32_t threadID, uint32_t actorIndex, ActorAlignmentData* outData) noexcept override
        {
            if (!outData) return false;

            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return false;

            if (actorIndex >= thread->getActors().size()) return false;

            auto alignment = thread->getActorAlignment(static_cast<int>(actorIndex));
            outData->offsetX = alignment.offsetX;
            outData->offsetY = alignment.offsetY;
            outData->offsetZ = alignment.offsetZ;
            outData->scale = alignment.scale;
            outData->rotation = alignment.rotation;
            outData->sosBend = alignment.sosBend;

            return true;
        }

        APIResult SetActorAlignment(uint32_t threadID, uint32_t actorIndex, const ActorAlignmentData* data) noexcept override
        {
            if (!data) return APIResult::Invalid;

            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return APIResult::Invalid;

            if (actorIndex >= thread->getActors().size()) return APIResult::Failed;

            Alignment::ActorAlignment alignment;
            alignment.offsetX = data->offsetX;
            alignment.offsetY = data->offsetY;
            alignment.offsetZ = data->offsetZ;
            alignment.scale = data->scale;
            alignment.rotation = data->rotation;
            alignment.sosBend = data->sosBend;

            int idx = static_cast<int>(actorIndex);
            SKSE::GetTaskInterface()->AddTask([threadID, idx, alignment]()
            {
                auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
                if (thread)
                {
                    thread->updateActorAlignment(idx, alignment);
                }
            });

            return APIResult::OK;
        }

        // --- Search ---

        uint32_t SearchScenes(const char* query, SceneSearchResult* buffer, uint32_t bufferSize) noexcept override
        {
            if (!query || !buffer || bufferSize == 0) return 0;

            std::string name(query);
            std::vector<Graph::Node*> results;
            Graph::GraphTable::searchNodesByName(name, results);

            // Sort results alphabetically by scene name
            std::sort(results.begin(), results.end(), [](Graph::Node* a, Graph::Node* b) {
                return a->scene_name < b->scene_name;
            });

            auto thread = UI::UIState::GetSingleton()->currentThread;

            uint32_t count = 0;
            for (auto* node : results)
            {
                if (count >= bufferSize) break;

                if (node->actors.size() != thread->getActorCount()) continue;
                if (!thread->getFurnitureType()->isChildOf(node->furnitureType)) continue;
                if (node->isTransition) continue;

                buffer[count].sceneId = node->scene_id;
                buffer[count].name = node->scene_name;
                buffer[count].actorCount = node->numActors;
                count++;
            }

            return count;
        }

        bool GetSceneInfo(const char* sceneID, SceneSearchResult* outInfo) noexcept override
        {
            if (!sceneID || !outInfo) return false;

            auto node = Graph::GraphTable::getNodeById(sceneID);
            if (!node) return false;

            outInfo->sceneId = node->scene_id;
            outInfo->name = node->scene_name;
            outInfo->actorCount = node->numActors;

            return true;
        }

        APIResult NavigateToSearchResult(uint32_t threadID, const char* sceneID) noexcept override
        {
            if (!sceneID) {
                SKSE::log::warn("NavigateToSearchResult: Invalid sceneID (nullptr)");
                return APIResult::Invalid;
            }

            auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) {
                SKSE::log::warn("NavigateToSearchResult: Thread not found for ID {}", threadID);
                return APIResult::Invalid;
            }

            auto node = Graph::GraphTable::getNodeById(sceneID);
            if (!node) {
                SKSE::log::warn("NavigateToSearchResult: Scene node not found for ID {}", sceneID);
                return APIResult::Failed;
            }

            SKSE::GetTaskInterface()->AddTask([threadID, nodePtr = node]()
            {
                auto thread = Threading::ThreadManager::GetSingleton()->GetThread(threadID);
                if (thread)
                {
                    thread->ChangeNode(nodePtr);
                }
                else
                {
                    SKSE::log::warn("NavigateToSearchResult (Task): Thread {} no longer exists", threadID);
                }
            });

            return APIResult::OK;
        }

        // --- Options Menu ---

        void RebuildOptionsTree() noexcept override
        {
            UI::Scene::SceneOptions::GetSingleton()->BuildPageTree();
        }

        uint32_t GetOptionsItemCount() noexcept override
        {
            UI::Scene::MenuData menuData;
            UI::Scene::SceneOptions::GetSingleton()->BuildMenuData(menuData);

            // First item is always the back/exit button, exclude it
            return menuData.options.size() > 1
                ? static_cast<uint32_t>(menuData.options.size() - 1)
                : 0;
        }

        uint32_t GetOptionsItems(OptionsMenuItem* buffer, uint32_t bufferSize) noexcept override
        {
            if (!buffer || bufferSize == 0) return 0;

            UI::Scene::MenuData menuData;
            UI::Scene::SceneOptions::GetSingleton()->BuildMenuData(menuData);

            // Include first item (back/exit button)
            uint32_t count = 0;
            for (size_t i = 0; i < menuData.options.size() && count < bufferSize; i++)
            {
                buffer[count].id = menuData.options[i].nodeId;
                buffer[count].title = menuData.options[i].title;
                buffer[count].icon = menuData.options[i].imagePath;
                buffer[count].border = menuData.options[i].border;
                buffer[count].description = menuData.options[i].description;
                count++;
            }

            return count;
        }

        bool SelectOptionsItem(int32_t index) noexcept override
        {
            auto result = UI::Scene::SceneOptions::GetSingleton()->Handle(index);
            return result != UI::Scene::HandleResult::kExit;
        }

        bool IsOptionsAtRoot() noexcept override
        {
            return UI::Scene::SceneOptions::GetSingleton()->isAtRoot();
        }
    };

    void NotifyEvent(ThreadEvent eventType, uint32_t threadID)
    {
        ThreadInterface::GetSingleton()->NotifyEvent(eventType, threadID);
    }

    void NotifyControlInput(Controls controlType, uint32_t threadID)
    {
        ThreadInterface::GetSingleton()->NotifyControlInput(controlType, threadID);
    }
}

OstimNG_API::Thread::IThreadInterface* g_ostimThreadInterface = nullptr;

extern "C" DLLEXPORT OstimNG_API::Thread::IThreadInterface* SKSEAPI RequestPluginAPI_Thread(
    const OstimNG_API::Thread::InterfaceVersion version,
    const char* pluginName,
    REL::Version pluginVersion)
{
    if (!pluginName)
    {
        SKSE::log::warn("OStim::RequestPluginAPI_Thread called with nullptr plugin name");
        return nullptr;
    }

    SKSE::log::info("OStim::RequestPluginAPI_Thread called, InterfaceVersion {}, Plugin: {}, Version: {}",
        static_cast<uint8_t>(version), pluginName, pluginVersion);

    switch (version)
    {
        case OstimNG_API::Thread::InterfaceVersion::V1:
            return OstimNG_API::Thread::ThreadInterface::GetSingleton();
        default:
            SKSE::log::warn("OStim::RequestPluginAPI_Thread called with unsupported version");
            return nullptr;
    }
}
