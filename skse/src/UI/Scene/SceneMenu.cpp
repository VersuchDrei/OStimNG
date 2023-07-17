#include "UI/Scene/SceneMenu.h"
#include "UI/UIState.h"
#include <Graph/GraphTable.h>

namespace UI::Scene {

    inline RE::GFxValue GetRoot() {
        RE::GPtr<RE::IMenu> sceneMenu = RE::UI::GetSingleton()->GetMenu(SceneMenu::MENU_NAME);
        assert(sceneMenu && sceneMenu->uiMovie);

        RE::GFxValue root;
        sceneMenu->uiMovie->GetVariable(&root, "_root");
        return root;
    }

    inline RE::GFxValue GetRoot(RE::GPtr<RE::GFxMovieView> uiMovie) {
        RE::GFxValue root;
        uiMovie->GetVariable(&root, "_root");
        return root;
    }

    
    inline RE::GFxValue GetOptionBoxes(RE::GPtr<RE::GFxMovieView> uiMovie){
        auto root = GetRoot(uiMovie);
        RE::GFxValue optionBoxesContainer;
        root.GetMember("optionBoxesContainer", &optionBoxesContainer);
        RE::GFxValue optionBoxes;
        optionBoxesContainer.GetMember("optionBoxes", &optionBoxes);
        return optionBoxes;
    }
    inline RE::GFxValue GetOptionBoxes() {

        RE::GPtr<RE::IMenu> sceneMenu = RE::UI::GetSingleton()->GetMenu(SceneMenu::MENU_NAME);
        return GetOptionBoxes(sceneMenu->uiMovie);  

    }

	SceneMenu::SceneMenu() : Super() {
        auto scaleformManager = RE::BSScaleformManager::GetSingleton();
        
        inputContext = Context::kNone;

        auto menu = static_cast<Super*>(this);
        menu->depthPriority = 0;

        menuFlags.set(
            RE::UI_MENU_FLAGS::kAlwaysOpen,
            RE::UI_MENU_FLAGS::kRequiresUpdate,
            RE::UI_MENU_FLAGS::kAllowSaving);

        if (uiMovie) {
            uiMovie->SetMouseCursorCount(0);  // disable input            
        }

        scaleformManager->LoadMovieEx(menu, MENU_PATH, [](RE::GFxMovieDef* a_def) -> void {
            a_def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<Logger>().get());
        });

        view = menu->uiMovie;

        auto optionBoxes = GetOptionBoxes(uiMovie);
        RE::GFxFunctionHandler* fn = new doSendTransitionRequest;
        RE::GFxValue dst;
        view->CreateFunction(&dst, fn);
        optionBoxes.SetMember("doSendTransitionRequest", dst);
	}

	void SceneMenu::Register() {
        auto ui = RE::UI::GetSingleton();
        if (ui) {
            ui->Register(MENU_NAME, Creator);
            logger::info("Registered {}", MENU_NAME);           

            RE::GPtr<RE::IMenu> alignMenu = RE::UI::GetSingleton()->GetMenu(MENU_NAME);     

            auto msgQ = RE::UIMessageQueue::GetSingleton();
            if (msgQ) {
                msgQ->AddMessage(SceneMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
            }
        }
	}

	void SceneMenu::Show() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(SceneMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
        }
        UI::Settings::LoadSettings();
        ApplyPositions();
	}

	void SceneMenu::Hide() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(SceneMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        }
	}

	void SceneMenu::Update() {
	}

    void SceneMenu::AdvanceMovie(float a_interval, std::uint32_t a_currentTime) {
        SceneMenu::Update();
        RE::IMenu::AdvanceMovie(a_interval, a_currentTime);
    }

    void SceneMenu::SendControl(int32_t control) {

        auto optionBoxes = GetOptionBoxes();
        const RE::GFxValue val{ control };
        optionBoxes.Invoke("HandleKeyboardInput", nullptr, &val, 1);
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
        auto root = GetRoot();
        if (!root.IsObject())
            return;

        auto controlPositions = &UI::Settings::positionSettings.ScenePositions.ControlPosition;
        const RE::GFxValue controlX = RE::GFxValue{ controlPositions->xPos };
        const RE::GFxValue controlY = RE::GFxValue{ controlPositions->yPos };
        const RE::GFxValue controlXScale = RE::GFxValue{ controlPositions->xScale };
        const RE::GFxValue controlYScale = RE::GFxValue{ controlPositions->yScale };
        RE::GFxValue controlPosArray[4]{ controlX, controlY, controlXScale, controlYScale };

        RE::GFxValue alignmentInfo;
        root.GetMember("optionBoxesContainer", &alignmentInfo);
        alignmentInfo.Invoke("setPosition", nullptr, controlPosArray, 4);
    }

    void SceneMenu::UpdateMenuData() {
        RE::GFxValue menuValues;
        view->CreateArray(&menuValues);
        MenuData menuData;
        BuildMenuData(menuData);
        menuData.loadValues(menuValues);
        auto optionBoxes = GetOptionBoxes();
        optionBoxes.Invoke("AssignData", nullptr, &menuValues, 1);


        //TEMP Bars Stuff
        /*
        RE::GFxValue barsData;
        view->CreateObject(&barsData);
        RE::GFxValue actorsData;
        view->CreateArray(&actorsData);
        RE::GFxValue actorData;
        view->CreateObject(&actorData);
        
        RE::GFxValue actorData2;
        view->CreateObject(&actorData2);
        
        actorsData.PushBack(actorData);
        actorsData.PushBack(actorData2);

        barsData.SetMember("actors", actorsData);
        RE::GFxValue barsMgr;
        root.GetMember("bars", &barsMgr);
        barsMgr.Invoke("AssignData", nullptr, &barsData, 1);
        */
    }

    void SceneMenu::BuildMenuData(MenuData& menuData) {
        // Do this when we have the node edges defined in c++
        auto state = UI::UIState::GetSingleton();
        auto currentNode = state->currentNode;
        if (!state->currentNode)
            return;
        if (currentNode->isTransition) {
            menuData.options.clear();
        } else {
            for (auto& nav : currentNode->navigations) {
                menuData.options.push_back({
                    .nodeId = nav.isTransition? nav.transitionNode->scene_id: nav.destination->scene_id,
                    .title = nav.destination->scene_name,
                    .imagePath = nav.icon,
                    .border = nav.border,
                    .description = nav.destination->scene_name
                    });
            }
        }
    }
    void SceneMenu::HideSpeed() {

    }
    void SceneMenu::UpdateSpeed() {
        auto thread = UI::UIState::GetSingleton()->currentThread;
        if (!thread) {
            return;
        }
        auto node = UI::UIState::GetSingleton()->currentNode;
        if (!node) {
            return;
        }
        auto boxes = GetOptionBoxes();
        if (node->speeds.size() > 1) {
            auto speed = thread->getCurrentSpeed();
            auto& speedObj = node->speeds[speed];
            const std::string speedStr = std::to_string(speedObj.playbackSpeed);
            logger::info("{}"sv, speedStr);
            RE::GFxValue args[3]{ RE::GFxValue{ speedStr }, speed != (node->speeds.size() - 1), speed != 0};
            boxes.Invoke("ShowSpeed", nullptr, args, 3);
        }
        else {
            boxes.Invoke("HideSpeed");
        }
    }

    void SceneMenu::SpeedUp() {
        auto boxes = GetOptionBoxes();
        boxes.Invoke("SpeedUp");
    }

    void SceneMenu::SpeedDown() {
        auto boxes = GetOptionBoxes();
        boxes.Invoke("SpeedDown");
    }
    
    void SceneMenu::ChangeAnimation(std::string nodeId) {
        SKSE::GetTaskInterface()->AddTask([nodeId]() {
            UI::UIState::GetSingleton()->currentThread->Navigate(nodeId);
        });
    }

    
}