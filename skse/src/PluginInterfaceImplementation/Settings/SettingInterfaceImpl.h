#pragma once

#include "PluginInterface/Settings/SettingInterface.h"
#include "Util/Singleton.h"

namespace Interface {
    class SettingInterfaceImpl : public OStim::SettingInterface, public Util::Singleton<SettingInterfaceImpl> {
    public:
        virtual uint32_t getVersion() override;
        virtual void addGroup(OStim::SettingGroup* group) override;
        virtual void addPage(OStim::SettingPage* page) override;
    };
}