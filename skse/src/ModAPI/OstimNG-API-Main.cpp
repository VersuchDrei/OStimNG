#include "OstimNG-API-Main.h"

namespace OstimNG_API
{
    namespace Scene
    {
        APIResult SceneInterface::StartScene(std::string_view pluginName,
                                                                 RE::TESObjectREFR* furniture,
                                                                 std::vector<RE::Actor*> actors) noexcept {
            using Result = OstimNG_API::Scene::APIResult; 

            return Result::OK; 
        }
    }

}


