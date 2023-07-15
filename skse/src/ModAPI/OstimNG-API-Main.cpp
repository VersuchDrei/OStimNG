#include "OstimNG-API-Main.h"
using Result = OstimNG_API::Scene::APIResult;

namespace OstimNG_API
{
    namespace Scene
    {
        APIResult SceneInterface::StartScene(std::string_view pluginName,
                                                                 RE::TESObjectREFR* furniture,std::string startingAnimation,
                                                                 std::vector<RE::Actor*> actors, int& outThreadID) noexcept {
            
            outThreadID = -1; 
            if (actors.size() < 2) return Result::Invalid; 
            
            
             OStim::ThreadStartParams params;

             params.actors = GameAPI::GameActor::convertVector(actors);
             params.startingNode = Graph::GraphTable::getNodeById(startingAnimation);
             params.furniture = furniture;

             outThreadID = OStim::startThread(params); 

             if (outThreadID == -1) return Result::Failed; 

             return Result::OK; 
        }
        APIResult SceneInterface::StopScene(std::string_view pluginName, int threadID) noexcept 
        {
             OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
             if (!thread) return APIResult::Invalid;

             thread->stopFaded();
             return Result::OK; 
        }

        APIResult SceneInterface::SetAutoMode(std::string_view pluginName, int threadID, bool autoMode) noexcept {
             OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
             if (!thread) return Result::Invalid;

             autoMode ? thread->startAutoMode() : thread->stopAutoMode();
             return Result::OK; 
        }

        APIResult SceneInterface::TryGetMetadata(std::string_view pluginName, int threadID,
                                                 std::vector<std::string>& tags) noexcept {
             OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
             if (!thread) return Result::Invalid;

             tags = thread->getMetadata();
             return Result::OK; 
        }
        APIResult SceneInterface::TryGetAutoMode(std::string_view pluginName, int threadID, bool& autoMode) noexcept {
            OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
            if (!thread) return Result::Invalid;

            autoMode = thread->isInAutoMode(); 

            return Result::OK; 
        }
    }  // namespace Scene
}


