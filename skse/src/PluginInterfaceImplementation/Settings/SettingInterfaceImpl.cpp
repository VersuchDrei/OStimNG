#include "SettingInterfaceImpl.h"

#include "Settings/Implementation/Plugin/PluginSettingGroup.h"
#include "Settings/Implementation/Plugin/PluginSettingPage.h"
#include "Settings/AddonPage.h"

namespace Interface {
    uint32_t SettingInterfaceImpl::getVersion() {
        return 1;
    }

    void SettingInterfaceImpl::addGroup(OStim::SettingGroup* group) {
        if (group->isDisposable()) {
            return;
        }

        Settings::AddonPage::getSingleton()->addGroup(new Settings::PluginSettings::PluginSettingGroup(group, false));
    }

    void SettingInterfaceImpl::addPage(OStim::SettingPage* page) {
        // TODO add page to stuff
    }
}