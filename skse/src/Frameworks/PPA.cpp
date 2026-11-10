#include "PPA.h"

#include "Config/Config.h"
#include "Core/ThreadManager.h"
#include "PPA/AccuratePenetrationAPI.h"
#include "Util/StringUtil.h"

namespace Frameworks {
    namespace PPA {
        #pragma region Utility
        AccuratePenetration::API::InteractionAction toPPAAction(PPAAction action) {
            switch (action) {
                case PPAAction::ANUS:
                    return AccuratePenetration::API::InteractionAction::Anus;
                case PPAAction::HAND_L:
                    return AccuratePenetration::API::InteractionAction::HandL;
                case PPAAction::HAND_R:
                    return AccuratePenetration::API::InteractionAction::HandR;
                case PPAAction::HANDS:
                    return AccuratePenetration::API::InteractionAction::Hands;
                case PPAAction::MOUTH:
                    return AccuratePenetration::API::InteractionAction::Mouth;
                case PPAAction::NONE:
                    return AccuratePenetration::API::InteractionAction::None;
                case PPAAction::UNDEFINED:
                    return AccuratePenetration::API::InteractionAction::None;
                case PPAAction::VAGINA:
                    return AccuratePenetration::API::InteractionAction::Vagina;
                default:
                    return AccuratePenetration::API::InteractionAction::None;
            }
        }
        #pragma endregion

        namespace PPAActionAPI {
            PPAAction fromString(std::string action) {
                StringUtil::toLower(&action);

                if (action == "vagina") {
                    return PPAAction::VAGINA;
                } else if (action == "anus") {
                    return PPAAction::ANUS;
                } else if (action == "mouth") {
                    return PPAAction::MOUTH;
                } else if (action == "hands") {
                    return PPAAction::HANDS;
                } else if (action == "handr") {
                    return PPAAction::HAND_R;
                } else if (action == "handl") {
                    return PPAAction::HAND_L;
                } else if (action == "none") {
                    return PPAAction::NONE;
                }

                return PPAAction::UNDEFINED;
            }
        }

        
        const HMODULE ppa = GetModuleHandleW(AccuratePenetration::API::kPluginDLL);

        void __cdecl onAnimationUpdate(const AccuratePenetration::API::AnimationUpdateEvent* a_event, void* a_userData) {
            if (!a_event || a_event->apiVersion != AccuratePenetration::API::kVersion || a_event->size < sizeof(AccuratePenetration::API::AnimationUpdateEvent)) {
                return;
            }

            if (a_event->ending) {
                // Clear any state tracked for a_event->receiver.
                return;
            }

            if (a_event->actorCount == 0) {
                return;
            }

            Threading::Thread* thread = Threading::ThreadManager::GetSingleton()->findThread(GameAPI::GameActor(a_event->actors[0].actor.get().get()));
            if (!thread) {
                // no OStim thread running
                // might happen when user also has SL installed
                return;
            }

            for (std::uint32_t i = 0; i < a_event->actorCount; ++i) {
                Threading::ThreadActor* threadActor =thread->GetActor(i);
                if (!threadActor) {
                    // actor count mismatch
                    continue;
                }

                const auto setInteraction = reinterpret_cast<AccuratePenetration::API::SetActorInteractionFn>(GetProcAddress(ppa, AccuratePenetration::API::kSetActorInteractionFunctionName));
                if (!setInteraction) {
                    // something went horribly wrong
                    logger::error("cannot get PPA setInteraction function");
                    return;
                }

                if (threadActor->getGraphActor()->ppaAction == PPAAction::UNDEFINED || threadActor->getGraphActor()->ppaAction == PPAAction::NONE) {
                    setInteraction(a_event->actors[i].actor, AccuratePenetration::API::InteractionAction::None, RE::ActorHandle());
                } else {
                    Threading::ThreadActor* target = thread->GetActor(threadActor->getGraphActor()->ppaTarget);
                    if (target) {
                        setInteraction(a_event->actors[i].actor, toPPAAction(threadActor->getGraphActor()->ppaAction), target->getActor().form->GetHandle());
                    }
                }
            }
        }


        void setUp() {
            if (!Config::Config::autoPPA) {
                return;
            }

            constexpr auto dllPath = "Data/SKSE/Plugins/AccuratePenetration.dll";
            if (!std::filesystem::exists(dllPath)) {
                logger::info("PPA is not installed");
                return;
            }

            logger::info("setting up PPA");

            if (!ppa) {
                logger::info("PPA module not found");
                return;
            }

            auto getAPI = reinterpret_cast<AccuratePenetration::API::InterfaceV1*(__cdecl*)()>(GetProcAddress(ppa, AccuratePenetration::API::kGetAPIFunctionNameV1));
            if (!getAPI) {
                logger::info("PPA getAPI function not found");
                return;
            }

            const auto* api = getAPI();
            if (!api) {
                logger::info("PPA API not found");
                return;
            }

            if (api->version != AccuratePenetration::API::kVersion) {
                logger::info("PPA API verison mismatch");
                return;
            }

            if (!api->RegisterAnimationUpdateListener) {
                logger::info("PPA RegisterAnimationUpdateListener function not found");
                return;
            }

            // PPA doesn't update on scene change immediately
            // so this can't be done in the scene change code since it's too early, I tried
            // so we are left with using a PPA event

            // this does not look like the listener you'd want to use for this
            // from my testing it fires way more often than on scene change
            // but it's the only listener that PPA provides so here we go anyways
            api->RegisterAnimationUpdateListener(onAnimationUpdate, nullptr);

            logger::info("PPA integration was set up");
        }
    }
}