#pragma once

#include "PluginInterface.h"

namespace OStim {
    class InterfaceMap {
    public:
        virtual PluginInterface* queryInterface(const char* name) = 0;
        virtual bool addInterface(const char* name, PluginInterface* pluginInterface) = 0;
        virtual PluginInterface* removeInterface(const char* name) = 0;
    };

    // this was only added in version 0x07050013, so it might fail if an older OStim is installed
    inline static InterfaceMap* getInferfaceMap() {
        const HMODULE ostim = GetModuleHandleA("OStim.dll");
        if (!ostim) {
            return nullptr;
        }

        const auto requestInterfaceMap = reinterpret_cast<InterfaceMap* (*)()>(reinterpret_cast<void*>(GetProcAddress(ostim, "requestInterfaceMap")));
        if (!requestInterfaceMap) {
            return nullptr;
        }

        return requestInterfaceMap();
    }
}