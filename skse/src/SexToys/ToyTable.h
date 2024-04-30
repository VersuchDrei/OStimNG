#pragma once

#include "Settings/Settings.h"
#include "ToyWrapper.h"

#include "PluginInterface/SexToys/SexToyReloadListener.h"

namespace Toys {
    class ToyTable {
    public:
        inline static void addReloadListener(OStim::SexToyReloadListener* listener) { reloadListeners.push_back(listener); }

        static void addToySlots(std::vector<std::string>& slots);
        inline static std::set<std::string> getToySlots() { return toySlots; }

        static void reloadToys();
        inline static void addToy(OStim::SexToy* toy) { toys.push_back({toy}); }
        inline static std::vector<ToyWrapper>* getToys() { return &toys; }
        static ToyWrapper* getToy(std::string id);

        inline static Settings::Settings* getSettings() { return &settings; }

    private:
        inline static bool isReloading = false;
        inline static std::vector<OStim::SexToyReloadListener*> reloadListeners;

        inline static std::set<std::string> toySlots;
        inline static std::vector<ToyWrapper> toys;

        inline static Settings::Settings settings;
    };
}