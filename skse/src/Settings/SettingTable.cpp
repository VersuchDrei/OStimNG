#include "SettingTable.h"

namespace Settings {
    SettingPage* SettingTable::getPage(int index) {
        if (index < 0 || index >= pages.size()) {
            return nullptr;
        }

        return pages[index];
    }

    Setting* SettingTable::getSetting(int pageIndex, int settingIndex) {
        SettingPage* page = getPage(pageIndex);
        if (!page) {
            return nullptr;
        }

        settingGroupIndex count = page->getGroupCount();
        for (settingGroupIndex i = 0; i < count; i++) {
            SettingGroup* group = page->getGroup(i);
            if (settingIndex < group->getSettingCount()) {
                return group->getSetting(settingIndex);
            }

            settingIndex -= group->getSettingCount();
        }

        return nullptr;
    }
}