#pragma once

#include "../../Model/Setting.h"

#include "PluginInterface/Settings/Setting.h"

namespace Settings {
    namespace PluginSettings {
        class PluginSetting : public Setting {
        public:
            PluginSetting(OStim::Setting* setting, bool disposable);
            ~PluginSetting();

            virtual bool isDisposable();

            virtual std::string getName();
            virtual std::string getTooltip();
            virtual SettingType getType();
            virtual bool isEnabled();

            // toggle options
            virtual bool isActivatedByDefault();
            virtual bool isActivated();
            virtual bool toggle();

            // slider options
            virtual float getDefaultValue();
            virtual float getCurrentValue();
            virtual float getValueStep();
            virtual float getMinValue();
            virtual float getMaxValue();
            virtual bool setValue(float value);

            // menu options
            virtual dropDownIndex getDefaultIndex();
            virtual dropDownIndex getCurrentIndex();
            virtual std::string getCurrentOption();
            virtual std::vector<std::string> getOptions();
            virtual bool setIndex(dropDownIndex index);

        private:
            OStim::Setting* setting;
            bool disposable;
        };
    }
}