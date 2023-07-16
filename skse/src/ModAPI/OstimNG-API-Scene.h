#pragma once 
using namespace RE; 

namespace OstimNG_API::Scene
{
    
    enum class InterfaceVersion : uint8_t
    {
        V1

    };

    enum class APIResult : uint8_t
    {
        // Scene successfully started
        OK,

        // The arguments were invalid
        Invalid,

        // Failed 
        Failed
    };
    class ISceneInterface
    {
        public: 

         

        


        [[nodiscard]] virtual APIResult StartScene(std::string_view pluginName,RE::TESObjectREFR* furniture,std::string startingAnimation,std::vector<RE::Actor*> actors, int& outThreadID) noexcept = 0 ;
        [[nodiscard]] virtual APIResult StopScene(std::string_view pluginName, int threadID) noexcept = 0; 

        [[nodiscard]] virtual APIResult SetAutoMode(std::string_view pluginName, int threadID, bool autoMode) noexcept = 0;

        [[nodiscard]] virtual APIResult TryGetMetadata(std::string_view pluginName, int threadID, std::vector<std::string> &tags) noexcept = 0; 
        [[nodiscard]] virtual APIResult TryGetAutoMode(std::string_view pluginName, int threadID, bool& autoMode) noexcept = 0; 
        
    };

    
    using _RequestPluginAPI_Scene = ISceneInterface* (*)(InterfaceVersion a_interfaceVersion, const char* a_pluginName, REL::Version a_pluginVersion); 

    ISceneInterface* GetAPI();

}

extern OstimNG_API::Scene::ISceneInterface* g_ostimSceneInterface;