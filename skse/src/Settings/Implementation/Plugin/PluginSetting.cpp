#include "PluginSetting.h"

namespace Settings {
    namespace PluginSettings {
        PluginSetting::PluginSetting(OStim::Setting* setting, bool disposable)
            : setting{setting}, disposable{disposable} {}

        PluginSetting::~PluginSetting() {
            if (setting->isDisposable()) {
                delete setting;
            }
        }


        bool PluginSetting::isDisposable() {
            return disposable;
        }


        std::string PluginSetting::getName() {
            return std::string(setting->getName());
        }

        std::string PluginSetting::getTooltip() {
            return std::string(setting->getTooltip());
        }

        SettingType PluginSetting::getType() {
            return static_cast<SettingType>(setting->getType());
        }

        bool PluginSetting::isEnabled() {
            return setting->isEnabled();
        }


        bool PluginSetting::isActivatedByDefault() {
            return setting->isActivatedByDefault();
        }

        bool PluginSetting::isActivated() {
            return setting->isActivated();
        }

        bool PluginSetting::toggle() {
            return setting->toggle();
        }


        float PluginSetting::getDefaultValue() {
            return setting->getDefaultValue();
        }

        float PluginSetting::getCurrentValue() {
            return setting->getCurrentValue();
        }

        float PluginSetting::getValueStep() {
            return setting->getValueStep();
        }

        float PluginSetting::getMinValue() {
            return setting->getMinValue();
        }

        float PluginSetting::getMaxValue() {
            return setting->getMaxValue();
        }

        bool PluginSetting::setValue(float value) {
            return setting->setValue(value);
        }


        dropDownIndex PluginSetting::getDefaultIndex() {
            return setting->getDefaultIndex();
        }

        dropDownIndex PluginSetting::getCurrentIndex() {
            return setting->getCurrentIndex();
        }

        std::string PluginSetting::getCurrentOption() {
            return std::string(setting->getCurrentOption());
        }

        std::vector<std::string> PluginSetting::getOptions() {
            std::vector<const char*> cOptions = setting->getOptions();
            std::vector<std::string> options;
            for (const char* cOption : cOptions) {
                options.push_back(std::string(cOption));
            }
            return options;
        }

        bool PluginSetting::setIndex(dropDownIndex index) {
            return setting->setIndex(index);
        }
    }
}