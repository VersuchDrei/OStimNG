#include "ToyThread.h"

#include "ToyTable.h"

namespace Toys {
    ToyThread::ToyThread(OStim::Thread* thread) : thread{thread} {
        thread->registerLoopListener([this]() { loop(); });
        thread->registerPeakListener([this](actionIndex action) { peak(action); });
        thread->registerNodeChangedListener([this]() { nodeChanged(); });
        thread->registerThreadEndListener([this]() { threadEnd(); });
    }

    void ToyThread::loop() {
        for (ToyGroup& group : groups) {
            group.loop();
        }
    }

    void ToyThread::peak(actionIndex action) {
        for (ToyGroup& group : groups) {
            if (group.isAction(action)) {
                group.peak();
            }
        }
    }

    void ToyThread::nodeChanged() {
        for (ToyGroup& group : groups) {
            group.stop();
        }

        groups.clear();

        
        std::vector<ToyWrapper>* toys = ToyTable::getToys();
        std::set<ToyWrapper*> toysInUse;

        for (auto& toy : *toys) {
            if (!toy.getSettings()->enabled) {
                toysInUse.insert(&toy);
            }
        }

        OStim::Thread* thread = this->thread;
        for (Graph::Action::Action& action : thread->getCurrentNode()->actions) {
            if (toysInUse.size() == toys->size()) {
                break;
            }

            std::vector<std::tuple<OStim::ThreadActor*, std::string, ToyWrapper*>> slotToys;
            action.roles.forEach([&slotToys, &thread, &action, &toys, &toysInUse](Graph::Role role, int index) {
                Graph::ActionActor* actionActor = action.attributes->roles.get(role);
                for (std::string slot : actionActor->toySlots) {
                    OStim::ThreadActor* actor = thread->GetActor(*action.roles.get(role));
                    for (ToyWrapper& toy : *toys) {
                        if (toysInUse.contains(&toy)) {
                            continue;
                        }

                        Settings::ToySettings* settings = toy.getSettings();
                        Settings::SlotSettings* slotSettings = settings->getSlotSettings(slot);

                        if (!slotSettings->enabled) {
                            continue;
                        }

                        Settings::SynchronizationType syncType = settings->synchronizationType;
                        if (syncType == Settings::SynchronizationType::INDIVIDUAL_SYNCHRONIZATION) {
                            syncType = settings->getSlotSettings(slot)->synchronizationType;
                        }

                        bool doToy = false;
                        switch (syncType) {
                            case Settings::SynchronizationType::PLAYER: {
                                if (actor) {
                                    doToy = actor->getActor().isPlayer();
                                }
                            } break;
                            case Settings::SynchronizationType::MALE: {
                                for (auto& [index, threadActor] : thread->getActors()) {
                                    if (threadActor.getActor().isSex(GameAPI::GameSex::MALE)) {
                                        doToy = actor == &threadActor;
                                        break;
                                    }
                                }
                            } break;
                            case Settings::SynchronizationType::FEMALE: {
                                for (auto& [index, threadActor] : thread->getActors()) {
                                    if (threadActor.getActor().isSex(GameAPI::GameSex::FEMALE)) {
                                        doToy = actor == &threadActor;
                                        break;
                                    }
                                }
                            } break;
                            case Settings::SynchronizationType::SCENE: {
                                doToy = true;
                            } break;
                        }

                        if (doToy) {
                            toysInUse.insert(&toy);
                            slotToys.push_back({actor, slot, &toy});
                        }
                    }
                }
            });

            if (!slotToys.empty()) {
                groups.push_back({&action, slotToys});
            }
        }
    }

    void ToyThread::threadEnd() {
        for (ToyGroup& group : groups) {
            group.stop();
        }

        delete this;
    }
}