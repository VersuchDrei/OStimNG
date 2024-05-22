#pragma once

#include "../Settings/SlotSettings.h"
#include "ToyInfo.h"

#include "Settings/Model/SettingPage.h"
#include "Util/Singleton.h"

namespace Toys {
    namespace Menu {
        class ToySettingMenu : public ::Settings::SettingPage {
        public:
            ToySettingMenu();

            virtual void onMenuOpened();
            virtual std::string getName();
            virtual ::Settings::SettingDisplayOrder getDisplayOrder();

            virtual settingGroupIndex getGroupCount();
            virtual ::Settings::SettingGroup* getGroup(settingGroupIndex index);

        private:
            inline static int defaultToy = 0;
            inline static int defaultSlot = 0;

            std::vector<ToyInfo> toys;
            std::vector<std::string> slots;

            int toy = defaultToy;
            int slot = defaultSlot;
            Settings::SlotSettings* slotSettings = nullptr;

            std::vector<::Settings::SettingGroup*> toySettingGroups;

            void selectToy(int toy);
            void selectSlot(int slot);
            void loadSettings();
        };
    }
}