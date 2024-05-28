#pragma once

#include "Model/SettingPage.h"

#include "Util/Singleton.h"

namespace Settings {
    class AddonPage : public SettingPage, public Util::Singleton<AddonPage> {
    public:
        inline void addGroup(SettingGroup* group) { groups.push_back(group); }

        virtual void onMenuOpened();
        virtual std::string getName();
        virtual SettingDisplayOrder getDisplayOrder();

        virtual settingGroupIndex getGroupCount();
        virtual SettingGroup* getGroup(settingGroupIndex index);

    private:
        std::vector<SettingGroup*> groups;
    };
}