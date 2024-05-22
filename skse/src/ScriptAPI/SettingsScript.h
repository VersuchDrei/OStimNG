#pragma once

namespace SettingsScript {
    unsigned int getSettingPageCount();
    std::string getSettingPageName(unsigned int page);

    unsigned int getSettingGroupCount(unsigned int page);
    std::string getSettingGroupName(unsigned int page, unsigned int group);

    unsigned int getSettingCount(unsigned int page, unsigned int group);
    std::string getSettingName(unsigned int page, unsigned int group, unsigned int setting);
    unsigned int getSettingType(unsigned int page, unsigned int group, unsigned int setting);
    bool isSettingEnabled(unsigned int page, unsigned int group, unsigned int setting);

    bool isSettingActivatedByDefault(unsigned int page, unsigned int group, unsigned int setting);
    bool isSettingActivated(unsigned int page, unsigned int group, unsigned int setting);
    bool toggleSetting(unsigned int page, unsigned int group, unsigned int setting);

    float getDefaultSettingValue(unsigned int page, unsigned int group, unsigned int setting);
    float getCurrentSettingValue(unsigned int page, unsigned int group, unsigned int setting);
    float getMinSettingValue(unsigned int page, unsigned int group, unsigned int setting);
    float getMaxSettingValue(unsigned int page, unsigned int group, unsigned int setting);
    bool setSettingValue(unsigned int page, unsigned int group, unsigned int setting, float value);

    unsigned int getDefaultSettingIndex(unsigned int page, unsigned int group, unsigned int setting);
    unsigned int getCurrentSettingIndex(unsigned int page, unsigned int group, unsigned int setting);
    std::string getCurrentSettingOption(unsigned int page, unsigned int group, unsigned int setting);
    std::vector<std::string> getSettingOptions(unsigned int page, unsigned int group, unsigned int setting);
    bool setSettingIndex(unsigned int page, unsigned int group, unsigned int setting, unsigned int index);
}