#pragma once

#include "Core/Core.h"
#include "Core/ThreadManager.h"
#include "Trait/TraitTable.h"
#include "Util/CompatibilityTable.h"
#include "Util/StringUtil.h"
#include "Util/VectorUtil.h"

namespace PapyrusThreadActor {
    using VM = RE::BSScript::IVirtualMachine;

    float GetExcitement(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            return threadActor->getExcitement();
        }

        return -1;
    }

    void SetExcitement(RE::StaticFunctionTag*, RE::Actor* actor, float excitement) {
        if (excitement < 0) {
            excitement = 0;
        } else if (excitement > 100) {
            excitement = 100;
        }

        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->setExcitement(excitement);
        }
    }

    void ModifyExcitement(RE::StaticFunctionTag*, RE::Actor* actor, float excitement, bool respectMultiplier) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->addExcitement(excitement, respectMultiplier);
        }
    }

    void Climax(RE::StaticFunctionTag*, RE::Actor* actor, bool climaxAnimation) {
        // TODO: climax animations
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->orgasm();
        }
    }

    int GetTimesClimaxed(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            return threadActor->getTimexClimaxed();
        }
        return 0;
    }


    float PlayExpression(RE::StaticFunctionTag*, RE::Actor* actor, std::string expression) {
        StringUtil::toLower(&expression);
        std::vector<Trait::FacialExpression*>* expressions = Trait::TraitTable::getExpressionsForEvent(expression);
        if (!expressions) {
            return -1;
        }

        Trait::FacialExpression* eventExpression = VectorUtil::randomElement(expressions);

        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->setEventExpression(eventExpression);
        }

        return eventExpression->getDuration(actor);
    }
    
    void ClearExpression(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->clearEventExpression();
        }
    }

    bool HasExpressionOverride(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            return threadActor->hasExpressionOverride();
        }
        return false;
    }


    void Mute(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->mute();
        }
    }

    void Unmute(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->mute();
        }
    }

    bool IsMuted(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            return threadActor->isMuted();
        }
        return false;
    }


    void Undress(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->undress();
        }
    }

    void Redress(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->redress();
        }
    }

    void UndressPartial(RE::StaticFunctionTag*, RE::Actor* actor, int mask) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->undressPartial(mask);
        }
    }

    void RedressPartial(RE::StaticFunctionTag*, RE::Actor* actor, int mask) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->redressPartial(mask);
        }
    }

    void RemoveWeapons(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->removeWeapons();
        }
    }

    void AddWeapons(RE::StaticFunctionTag*, RE::Actor* actor) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->addWeapons();
        }
    }

    bool EquipObject(RE::StaticFunctionTag*, RE::Actor* actor, std::string type) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            return threadActor->equipObject(type);
        }

        return false;
    }

    void UnequipObject(RE::StaticFunctionTag*, RE::Actor* actor, std::string type) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->unequipObject(type);
        }
    }

    bool IsObjectEquipped(RE::StaticFunctionTag*, RE::Actor* actor, std::string type) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            return threadActor->isObjectEquipped(type);
        }

        return false;
    }

    bool SetObjectVariant(RE::StaticFunctionTag*, RE::Actor* actor, std::string type, std::string variant, float duration) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            return threadActor->setObjectVariant(type, variant, duration * 1000);
        }

        return false;
    }

    void UnsetObjectVariant(RE::StaticFunctionTag*, RE::Actor* actor, std::string type) {
        OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
        if (threadActor) {
            threadActor->unsetObjectVariant(type);
        }
    }


    bool IsInOStim(RE::StaticFunctionTag*, RE::Actor* actor) {
        return OStim::ThreadManager::GetSingleton()->findActor(actor);
    }

    bool HasSchlong(RE::StaticFunctionTag*, RE::Actor* actor) {
        return Compatibility::CompatibilityTable::hasSchlong(actor);
    }

    bool VerifyActors(RE::StaticFunctionTag*, std::vector<RE::Actor*> actors) {
        for (RE::Actor*& actor : actors) {
            if (!OStim::isEligible(actor)) {
                return false;
            }
        }
        return true;
    }

    std::vector<RE::Actor*> SortActors(RE::StaticFunctionTag*, std::vector<RE::Actor*> actors, int playerIndex) {
        std::stable_sort(actors.begin(), actors.end(), [&](RE::Actor* actorA, RE::Actor* actorB) {
            return Compatibility::CompatibilityTable::hasSchlong(actorA) && !Compatibility::CompatibilityTable::hasSchlong(actorB);
        });

        if (playerIndex >= 0 && playerIndex < actors.size()) {
            RE::Actor* player = RE::PlayerCharacter::GetSingleton();
            int currentPlayerIndex = VectorUtil::getIndex(actors, player);
            if (currentPlayerIndex >= 0) {
                if (currentPlayerIndex < playerIndex) {
                    while (currentPlayerIndex < playerIndex) {
                        actors[currentPlayerIndex] = actors[currentPlayerIndex + 1];
                        currentPlayerIndex++;
                    }
                    actors[playerIndex] = player;
                } else if (currentPlayerIndex > playerIndex) {
                    while (currentPlayerIndex > playerIndex) {
                        actors[currentPlayerIndex] = actors[currentPlayerIndex - 1];
                        currentPlayerIndex--;
                    }
                    actors[playerIndex] = player;
                }
            }
        }

        return actors;
    }

    bool Bind(VM* a_vm) {
        const auto obj = "OActor"sv;

        BIND(GetExcitement);
        BIND(SetExcitement);
        BIND(ModifyExcitement);
        BIND(Climax);
        BIND(GetTimesClimaxed);

        BIND(PlayExpression);
        BIND(ClearExpression);
        BIND(HasExpressionOverride);

        BIND(Mute);
        BIND(Unmute);
        BIND(IsMuted);

        BIND(Undress);
        BIND(Redress);
        BIND(UndressPartial);
        BIND(RedressPartial);
        BIND(RemoveWeapons);
        BIND(AddWeapons);

        BIND(EquipObject);
        BIND(UnequipObject);
        BIND(IsObjectEquipped);
        BIND(SetObjectVariant);
        BIND(UnsetObjectVariant);

        BIND(IsInOStim);
        BIND(HasSchlong);
        BIND(VerifyActors);
        BIND(SortActors);

        return true;
    }
}  // namespace PapyrusThreadActor