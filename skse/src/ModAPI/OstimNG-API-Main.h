#pragma once 

#include "Core/ThreadStarter/ThreadStarter.h"
#include "Core/ThreadManager.h"
#include "OstimNG-API-Scene.h"
#include "Graph/GraphTable.h"
#include "Furniture/Furniture.h"
#include "MCM/MCMTable.h"
namespace OstimNG_API
{
    namespace Scene
    {
        class SceneInterface : ISceneInterface
        {
            public: 

            static SceneInterface* GetSingleton() noexcept
            {
                static SceneInterface singleton; 
                return std::addressof(singleton); 
            }

            APIResult StartScene(std::string_view pluginName,RE::TESObjectREFR* furniture,const std::string& startingAnimation,std::vector<RE::Actor*> actors,int *threadID) noexcept override;
            APIResult StartScene(std::string_view pluginName, const std::string& startingAnimation, std::vector<RE::Actor*> actors, int *threadID) noexcept override; 
            
            APIResult StopScene(std::string_view pluginName, int threadID) noexcept override; 
            APIResult SetAutoMode(std::string_view pluginName, int threadID, bool autoMode) noexcept override; 


            APIResult TryGetAutoMode(std::string_view pluginName, int threadID, bool* autoMode) noexcept override; 
        }; 
    }
}