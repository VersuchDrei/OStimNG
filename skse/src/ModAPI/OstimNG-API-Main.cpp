#include "OstimNG-API-Main.h"

namespace OstimNG_API
{
    namespace Scene
    {
        APIResult SceneInterface::StartScene(std::string_view pluginName,
                                                                 RE::TESObjectREFR* furniture,
                                                                 std::vector<RE::Actor*> actors) noexcept {
            using Result = OstimNG_API::Scene::APIResult;

            if (actors.size() < 2) return Result::Invalid; 
            

            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;

            Actor* thirdActor = (actors.size() > 2) ? actors[2] : nullptr; 

            
            bool callResult = false;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                auto args = RE::MakeFunctionArguments<RE::Actor*, RE::Actor*, RE::Actor*>(
                    std::move(actors[0]), std::move(actors[1]), std::move(thirdActor));
                callResult = vm->DispatchStaticCall("OSKSE", "StartScene", args, callback);
            }

            auto apiResult = (callResult) ? Result::OK : Result::Failed; 
            return Result::OK; 
        }
    }

}


