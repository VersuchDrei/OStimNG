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
            if (actors.size() < 1) return Result::Invalid; 
            
            
             OStim::ThreadStartParams params;

             params.actors = GameAPI::GameActor::convertVector(actors);
             params.startingNode = Graph::GraphTable::getNodeById(startingAnimation);
             params.furniture = furniture;

             outThreadID = OStim::startThread(params); 

             if (outThreadID == -1) return Result::Failed; 

             return Result::OK; 
        }

        
        APIResult SceneInterface::StartScene(std::string_view pluginName, std::string startingAnimation,
                                             std::vector<RE::Actor*> actors, int& outThreadID) noexcept {
             if (actors.size() < 1) return Result::Invalid; 
             
             auto furnitureList = Furniture::findFurniture(actors.size(), actors[0], MCM::MCMTable::furnitureSearchDistance(), 96);
             
             auto* furniture = (furnitureList.size() > 1) ? furnitureList[0] : nullptr; 

             return StartScene(pluginName, furniture, startingAnimation, actors, outThreadID); 
        }

        APIResult SceneInterface::StopScene(std::string_view pluginName, int threadID) noexcept {
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




extern "C" DLLEXPORT OstimNG_API::Scene::SceneInterface* SKSEAPI RequestPluginAPI_Scene(const OstimNG_API::Scene::InterfaceVersion version, const char* pluginName, REL::Version pluginVersion)
{
    const auto api = OstimNG_API::Scene::SceneInterface::GetSingleton(); 

    if (pluginName == nullptr) 
    {
        SKSE::log::info("Ostim::RequestPluginAPI_Scene called with a nullptr plugin name");
        return nullptr;
    }

    SKSE::log::info("Ostim::RequestPluginAPI_Scene called, InterfaceVersion {} (Plugin name: {}, version: {}", static_cast<uint8_t>(version) + 1, pluginName, pluginVersion);
    return api; 
}
