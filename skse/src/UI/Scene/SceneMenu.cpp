#include "UI/Scene/SceneMenu.h"

namespace UI::Scene {

    inline RE::GFxValue GetRoot() {
        RE::GFxValue root;
        RE::GPtr<RE::IMenu> alignMenu = RE::UI::GetSingleton()->GetMenu(SceneMenu::MENU_NAME);
        if (!alignMenu || !alignMenu->uiMovie) {
            return root;
        }
        alignMenu->uiMovie->GetVariable(&root, "_root");
        return root;
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
            SceneMenu::Show();

            RE::GPtr<RE::IMenu> alignMenu = RE::UI::GetSingleton()->GetMenu(MENU_NAME);            
        }
	}

	void SceneMenu::Show() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(SceneMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
        }
	}

	void SceneMenu::Hide() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(SceneMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        }
	}

    void SceneMenu::SetThread(OStim::Thread* thread) {
        currentThread = thread;
        currentNode = thread->getCurrentNode();
        UpdateMenuData();
    }

    void SceneMenu::NodeChanged(OStim::Thread* thread, Graph::Node* node)
    {
        if (&thread != &currentThread) return;
        UpdateMenuData();
    }

	void SceneMenu::Update() {
	}

    void SceneMenu::AdvanceMovie(float a_interval, std::uint32_t a_currentTime) {
        SceneMenu::Update();
        RE::IMenu::AdvanceMovie(a_interval, a_currentTime);
    }

    void SceneMenu::SendControl(int32_t control) {

        auto root = GetRoot();
        RE::GFxValue optionBoxes;
        root.GetMember("optionBoxes", &optionBoxes);
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

    void SceneMenu::UpdateMenuData() {
        RE::GFxValue menuValues;
        view->CreateArray(&menuValues);
        auto menuData = BuildMenuData();
        menuData.loadValues(menuValues);
        auto root = GetRoot();
        RE::GFxValue optionBoxes;
        root.GetMember("optionBoxes", &optionBoxes);
        optionBoxes.Invoke("AssignData", nullptr, &menuValues, 1);
    }

    UI::Scene::SceneMenu::MenuData SceneMenu::BuildMenuData() {
        // Do this when we have the node edges defined in c++
        MenuData ret;
        OptionData val1{"AAAA","BBBB","Ostim/logo.dds","desc"};
        OptionData val2{ "CCCC","","Ostim/logo.dds","desc2" };
        OptionData val3{ "CCCC","","Ostim/logo.dds","desc2" };
        OptionData val4{ "CCCC","","Ostim/logo.dds","desc2" };
        OptionData val5{ "CCCC","","SOS/logo.dds","desc2" };
        OptionData val6{ "CCCC","","SOS/logo.dds","desc2" };
        ret.options.push_back(val1);
        ret.options.push_back(val2);
        ret.options.push_back(val3);
        ret.options.push_back(val4);
        ret.options.push_back(val5);
        ret.options.push_back(val6);
        return ret;
    }

    void SceneMenu::UpdateInfoBox() {
        auto root = GetRoot();
        RE::GFxValue Current;
        Current.SetMember("NodeID", RE::GFxValue{ currentNode->scene_id.c_str()});
        Current.SetMember("Title", RE::GFxValue{ currentNode->scene_name.c_str()});
        Current.SetMember("CurrentSpeed", 0);
        Current.SetMember("MaxSpeed", 1);
        Current.SetMember("Animator", RE::GFxValue{ currentNode->sourceModule.c_str()});
        RE::GFxValue infoBox;
        root.GetMember("optionBoxes", &infoBox);
        infoBox.Invoke("AssignData", nullptr, &Current,1);
    }
    
}