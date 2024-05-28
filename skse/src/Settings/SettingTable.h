#pragma once

#include "Model/SettingPage.h"

#include "Util/Singleton.h"

namespace Settings {
    class SettingTable : public Util::Singleton<SettingTable> {
    public:
        SettingTable();

        void menuOpenend();
        inline void addPage(SettingPage* page) { pages.push_back(page); }
        inline size_t getPageCount() { return pages.size(); }
        SettingPage* getPage(int index);

    private:
        std::vector<SettingPage*> pages;
    };
}