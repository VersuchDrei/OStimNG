#pragma once 

#include "OstimNG-API-Scene.h"
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


             APIResult StartScene(std::string_view pluginName, RE::TESObjectREFR* furniture, std::vector<RE::Actor*> actors) noexcept override; 
        }; 
    }
}