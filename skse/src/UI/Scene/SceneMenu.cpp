#include "UI/Scene/SceneMenu.h"
#include "UI/UIState.h"
#include <Graph/LookupTable.h>

namespace UI::Scene {

    inline RE::GFxValue GetRoot() {
        RE::GFxValue root;
        RE::GPtr<RE::IMenu> sceneMenu = RE::UI::GetSingleton()->GetMenu(SceneMenu::MENU_NAME);
        assert(sceneMenu && sceneMenu->uiMovie);

        sceneMenu->uiMovie->GetVariable(&root, "_root");
        return root;
    }

    inline RE::GFxValue GetOptionBoxes() {

        auto root = GetRoot();
        RE::GFxValue optionBoxesContainer;
        root.GetMember("optionBoxesContainer", &optionBoxesContainer);
        RE::GFxValue optionBoxes;
        optionBoxesContainer.GetMember("optionBoxes", &optionBoxes);
        return optionBoxes;
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

        scaleformManager->LoadMovieEx(this, MENU_PATH, [](RE::GFxMovieDef* a_def) -> void {
            a_def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<Logger>().get());
        });

        view = menu->uiMovie;        
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

    void SceneMenu::PostRegister() {
        auto optionBoxes = GetOptionBoxes();
        RE::GFxFunctionHandler* fn = new doSendTransitionRequest;
        RE::GFxValue dst;
        view->CreateFunction(&dst, fn);
        optionBoxes.SetMember("doSendTransitionRequest", dst);
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
                    .nodeId = nav.destination->scene_id,
                    .title = nav.destination->scene_name,
                    .imagePath = nav.icon,
                    .border = nav.border,
                    .description = nav.destination->scene_name
                    });
            }
        }
    }
    
    void SceneMenu::ChangeAnimation(std::string nodeId) {
        UI::UIState::GetSingleton()->currentThread->Navigate(nodeId);
    }

    
}