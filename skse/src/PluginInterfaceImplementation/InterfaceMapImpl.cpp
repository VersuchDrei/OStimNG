#include "InterfaceMapImpl.h"

#include "Settings/SettingInterfaceImpl.h"
#include "SexToys/SexToyInterfaceImpl.h"

namespace Interface {
    OStim::PluginInterface* InterfaceMapImpl::queryInterface(const char* name) {
        for (auto& [key, value]: interfaces) {
            if (_stricmp(key, name) == 0) {
                return value;
            }
        }

        return NULL;
    }

    bool InterfaceMapImpl::addInterface(const char* name, OStim::PluginInterface* pluginInterface) {
        if (queryInterface(name) != NULL) {
            return false;
        }

        interfaces.emplace(name, pluginInterface);
        return true;
    }

    OStim::PluginInterface* InterfaceMapImpl::removeInterface(const char* name) {
        for (auto it = interfaces.begin(); it != interfaces.end(); it++) {
            if (_stricmp(it->first, name) == 0) {
                OStim::PluginInterface* pluginInterface = it->second;
                interfaces.erase(it);
                return pluginInterface;
            }
        }

        return NULL;
    }

    void InterfaceMapImpl::setupInterfaces() {
        addInterface(OStim::SettingInterface::NAME, SettingInterfaceImpl::getSingleton());
        addInterface(OStim::SexToyInterface::NAME, SexToyInterfaceImpl::getSingleton());
    }
}