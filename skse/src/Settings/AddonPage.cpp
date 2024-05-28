#include "AddonPage.h"

namespace Settings {
    void AddonPage::onMenuOpened() {
    }

    std::string AddonPage::getName() {
        return "$ostim_page_addons";
    }

    SettingDisplayOrder AddonPage::getDisplayOrder() {
        return SettingDisplayOrder::TOP_TO_BOTTOM;
    }


    settingGroupIndex AddonPage::getGroupCount() {
        return static_cast<settingGroupIndex>(groups.size());
    }

    SettingGroup* AddonPage::getGroup(settingGroupIndex index) {
        if (index < 0 || index >= groups.size()) {
            return nullptr;
        }

        return groups[index];
    }
}