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

        // A scene is already active
        AlreadyStarted,

        // The arguments were invalid
        Invalid,

        // Failed
        Failed
    };
    class ISceneInterface
    {
        public: 

        ISceneInterface* GetAPI(); 

        using _RequestPluginAPI_Scene = ISceneInterface* (*)(InterfaceVersion a_interfaceVersion, const char* a_pluginName, REL::Version a_pluginVersion); 


        [[nodiscard]] virtual APIResult StartScene(std::string_view pluginName, RE::TESObjectREFR* furniture, std::vector<RE::Actor*> actors) noexcept = 0 ;

        
    };
}

extern OstimNG_API::Scene::ISceneInterface* g_ostimSceneInterface;