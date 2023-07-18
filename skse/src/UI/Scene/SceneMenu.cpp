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

	SceneMenu::SceneMenu() : Super(MENU_NAME) {
        Locker locker(_lock);
        auto optionBoxes = GetOptionBoxes(uiMovie);
        RE::GFxFunctionHandler* fn = new doSendTransitionRequest;
        RE::GFxValue dst;
        _view->CreateFunction(&dst, fn);
        optionBoxes.SetMember("doSendTransitionRequest", dst);
	}

	void SceneMenu::Show() {
        OStimMenu::Show();
        ApplyPositions();
	}

    void SceneMenu::SendControl(int32_t control) {
        Locker locker(_lock);
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
        Locker locker(_lock);
        RE::GFxValue menuValues;
        _view->CreateArray(&menuValues);
        MenuData menuData;
        BuildMenuData(menuData);
        menuData.loadValues(_view, menuValues);
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
        Locker locker(_lock);
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
        Locker locker(_lock);
        auto boxes = GetOptionBoxes();
        boxes.Invoke("SpeedUp");
    }

    void SceneMenu::SpeedDown() {
        Locker locker(_lock);
        auto boxes = GetOptionBoxes();
        boxes.Invoke("SpeedDown");
    }
    
    void SceneMenu::ChangeAnimation(std::string nodeId) {
        SKSE::GetTaskInterface()->AddTask([nodeId]() {
            UI::UIState::GetSingleton()->currentThread->Navigate(nodeId);
        });
    }

    
}