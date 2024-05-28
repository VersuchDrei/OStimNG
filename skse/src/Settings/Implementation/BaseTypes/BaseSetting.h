#pragma once

#include "../../Model/Setting.h"
#include "CommonSettingParams.h"

namespace Settings {
    namespace BaseTypes {
        class BaseSetting : public Setting {
        public:
            BaseSetting(CommonSettingParams params, SettingType type, float defaultValue);

            virtual bool isDisposable();

            virtual std::string getName();
            virtual std::string getTooltip();
            virtual SettingType getType();
            virtual bool isEnabled();

            virtual bool isActivatedByDefault();
            virtual bool isActivated();
            virtual bool toggle();

            virtual float getDefaultValue();
            virtual float getCurrentValue();
            virtual float getValueStep();
            virtual float getMinValue();
            virtual float getMaxValue();
            virtual bool setValue(float value);

            virtual dropDownIndex getDefaultIndex();
            virtual dropDownIndex getCurrentIndex();
            virtual std::string getCurrentOption();
            virtual std::vector<std::string> getOptions();
            virtual bool setIndex(dropDownIndex index);

            virtual std::string getDefaultText();
            virtual std::string getCurrentText();
            virtual bool setText(std::string text);

            virtual bool click();

        protected:
            inline bool shouldRedraw() { return alwaysRedraw; }

        private:
            std::string name;
            std::string tooltip;
            SettingType type;
            std::function<bool()> enabled;
            bool alwaysRedraw;
            float defaultValue;
        };
    }
}