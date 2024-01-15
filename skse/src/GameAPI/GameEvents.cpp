#include "GameEvents.h"

#include "GameUtil.h"
#include "GameTable.h"

namespace GameAPI {
    namespace GameEvents {
        void sendStartEvent(int threadID) {
            // legacy mod event
            if (threadID == 0) {
                GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_prestart", "", 0);
                GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_start", "", 0);
            }
            GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_thread_start", "", threadID);
        }

        void sendSceneChangedEvent(int threadID, std::string sceneID) {
            // legacy mod event
            if (threadID == 0) {
                GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_scenechanged", sceneID, 0);
                GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_scenechanged_" + sceneID, "", 0);
            }
            GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_thread_scenechanged", sceneID, threadID);
        }

        void sendSpeedChangedEvent(int threadID, std::string sceneID, int speed) {
            // legacy mod event
            if (threadID == 0) {
                GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_animationchanged", sceneID, speed);
            }

            GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_thread_speedchanged", std::to_string(speed), threadID);
        }

        void sendEndEvent(int threadID, std::string sceneID, std::vector<GameActor> actors) {
            // legacy mod event
            if (threadID == 0) {
                GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_end", "", -1);
                GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_totalend", "", 0);
            }
            GameUtil::sendModEvent(GameTable::getMainQuest(), "ostim_thread_end", "", threadID);
        }


        void sendOrgasmEvent(int threadID, std::string sceneID, int index, GameAPI::GameActor actor) {
            // legacy mod event
            if (threadID == 0) {
                GameUtil::sendModEvent(actor.form, "ostim_orgasm", sceneID, index);
            }

            GameUtil::sendModEvent(actor.form, "ostim_actor_orgasm", sceneID, threadID);
        }

        void sendOStimEvent(int threadID, std::string type, Graph::RoleMap<GameActor> actors) {
            // legacy mod event
            if (threadID == 0) {
                GameUtil::sendModEvent(actors.target.form, "ostim_spank", "", 0);
            }

            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                auto args = RE::MakeFunctionArguments<>(std::move(threadID), std::move(type), std::move(actors.actor.form), std::move(actors.target.form), std::move(actors.performer.form));
                vm->DispatchStaticCall("OSKSE", "SendOStimEvent", args, callback);
            }
        }
    }
}