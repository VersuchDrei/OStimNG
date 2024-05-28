#include "SettingTable.h"

#include "AddonPage.h"

#include "SexToys/Menu/ToySettingMenu.h"

namespace Settings {
    SettingTable::SettingTable() {
        addPage(Toys::Menu::ToySettingMenu::getSingleton());
        addPage(AddonPage::getSingleton());
    }


    void SettingTable::menuOpenend() {
        for (SettingPage* page : pages) {
            page->onMenuOpened();
        }
    }

    SettingPage* SettingTable::getPage(int index) {
        if (index < 0 || index >= pages.size()) {
            return nullptr;
        }

        return pages[index];
    }
}