#include "UI/Scene/SceneMenu.h"
#include "UI/UIState.h"
#include <Graph/GraphTable.h>

namespace UI::Scene {    

    SceneMenu::SceneMenu() : Super(MENU_NAME) {}

    void SceneMenu::PostRegister() {
        QueueUITask([this]() {
            Locker locker(_lock);
            RE::GFxValue optionBoxes;
            GetOptionBoxes(optionBoxes);
            RE::GFxFunctionHandler* fn = new doSendTransitionRequest;
            RE::GFxValue dst;
            _view->CreateFunction(&dst, fn);
            optionBoxes.SetMember("doSendTransitionRequest", dst);
        });
    }

	void SceneMenu::Show() {
        OStimMenu::Show();
        ApplyPositions();
	}

    void SceneMenu::SendControl(int32_t control) {
        QueueUITask([this, control]() {
            Locker locker(_lock);
            RE::GFxValue optionBoxes;
            GetOptionBoxes(optionBoxes);
            const RE::GFxValue val{ control };
            optionBoxes.Invoke("HandleKeyboardInput", nullptr, &val, 1); 
        });
    }

    void SceneMenu::Handle(UI::Controls control)
    {
        switch (control) {
            case Up: {
                SendControl(0);
            } break;
            case Down: {
                SendControl(1);
            } break;
            case Left: {
                SendControl(2);
            } break;
            case Right: {
                SendControl(3);
            } break;
            case Yes: { 
                SendControl(4);
            } break;
        }
    }

    void SceneMenu::ApplyPositions() {
        QueueUITask([this]() {
            RE::GFxValue root;
            GetRoot(root);
            if (!root.IsObject())
                return;

            auto controlPositions = &UI::Settings::positionSettings.ScenePositions.ControlPosition;
            const RE::GFxValue controlX = RE::GFxValue{ controlPositions->xPos - 25 };
            const RE::GFxValue controlY = RE::GFxValue{ controlPositions->yPos - 50 };
            const RE::GFxValue controlXScale = RE::GFxValue{ controlPositions->xScale };
            const RE::GFxValue controlYScale = RE::GFxValue{ controlPositions->yScale };
            RE::GFxValue controlPosArray[4]{ controlX, controlY, controlXScale, controlYScale };

            RE::GFxValue alignmentInfo;
            root.GetMember("optionBoxesContainer", &alignmentInfo);
            alignmentInfo.Invoke("setPosition", nullptr, controlPosArray, 4);
        });
    }

    void SceneMenu::UpdateMenuData() {
        QueueUITask([this]() {
            Locker locker(_lock);
            RE::GFxValue menuValues;
            _view->CreateArray(&menuValues);
            MenuData menuData;
            BuildMenuData(menuData);
            menuData.loadValues(_view, menuValues);
            RE::GFxValue optionBoxes;
            GetOptionBoxes(optionBoxes);
            optionBoxes.Invoke("AssignData", nullptr, &menuValues, 1);
        });
    }

    void SceneMenu::BuildMenuData(MenuData& menuData) {
        auto state = UI::UIState::GetSingleton();
        auto currentNode = state->currentNode;
        if (!state->currentNode)
            return;
        if (currentNode->isTransition || state->currentThread->areNodesQueued()) {
            menuData.options.clear();
        } else {
            logger::info("before building conditions");
            std::vector<Trait::ActorCondition> conditions = state->currentThread->getActorConditions();
            logger::info("after building conditions");
            for (auto& nav : currentNode->navigations) {
                if (nav.fulfilledBy(conditions) && state->currentThread->getFurnitureType()->isChildOf(nav.nodes.back()->furnitureType)) {
                    menuData.options.push_back(
                        {.nodeId = nav.nodes.front()->scene_id,
                         .title = nav.nodes.back()->scene_name,
                         .imagePath = nav.icon,
                         .border = nav.border,
                         .description = nav.getDescription(state->currentThread)});
                }
            }
        }
    }
    void SceneMenu::HideSpeed() {

    }
    void SceneMenu::UpdateSpeed() {
        QueueUITask([this]() {
            Locker locker(_lock);
            auto thread = UI::UIState::GetSingleton()->currentThread;
            if (!thread) {
                return;
            }
            auto node = UI::UIState::GetSingleton()->currentNode;
            if (!node) {
                return;
            }
            RE::GFxValue optionBoxes;
            GetOptionBoxes(optionBoxes);
            if (node->speeds.size() > 1) {
                auto speed = thread->getCurrentSpeed();
                auto& speedObj = node->speeds[speed];
                const std::string speedStr = std::to_string(speedObj.displaySpeed);
                logger::info("{}"sv, speedStr);
                RE::GFxValue args[3]{ RE::GFxValue{ speedStr }, speed != (node->speeds.size() - 1), speed != 0 };
                optionBoxes.Invoke("ShowSpeed", nullptr, args, 3);
            }
            else {
                optionBoxes.Invoke("HideSpeed");
            }
        });
    }

    void SceneMenu::SpeedUp() {
        QueueUITask([this]() {
            Locker locker(_lock);
            RE::GFxValue optionBoxes;
            GetOptionBoxes(optionBoxes);
            optionBoxes.Invoke("SpeedUp");
        });
    }

    void SceneMenu::SpeedDown() {
        QueueUITask([this]() {
            Locker locker(_lock);
            RE::GFxValue optionBoxes;
            GetOptionBoxes(optionBoxes);
            optionBoxes.Invoke("SpeedDown");
        });
    }
    
    void SceneMenu::ChangeAnimation(std::string nodeId) {
        SKSE::GetTaskInterface()->AddTask([nodeId]() {
            UI::UIState::GetSingleton()->currentThread->Navigate(nodeId);
        });
    }    

    void SceneMenu::GetOptionBoxes(RE::GFxValue& optionBoxes) {
        RE::GFxValue root;
        GetRoot(root);
        RE::GFxValue optionBoxesContainer;
        root.GetMember("optionBoxesContainer", &optionBoxesContainer);
        optionBoxesContainer.GetMember("optionBoxes", &optionBoxes);
    }
}