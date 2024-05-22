#pragma once

#include "SettingType.h"

// all setters return a bool value indicating if the whole menu needs to be redrawn
namespace OStim {
    class Setting {
    public:
        virtual bool isDisposable() = 0;

        virtual const char* getName() = 0;
        virtual const char* getTooltip() = 0;
        virtual SettingType getType() = 0;
        virtual bool isEnabled() = 0;
        
        // toggle options
        virtual bool isActivatedByDefault() = 0;
        virtual bool isActivated() = 0;
        virtual bool toggle() = 0;

        // slider options
        virtual float getDefaultValue() = 0;
        virtual float getCurrentValue() = 0;
        virtual float getValueStep() = 0;
        virtual float getMinValue() = 0;
        virtual float getMaxValue() = 0;
        virtual bool setValue(float value) = 0;

        // menu options
        virtual uint32_t getDefaultIndex() = 0;
        virtual uint32_t getCurrentIndex() = 0;
        virtual const char* getCurrentOption() = 0;
        virtual std::vector<const char*> getOptions() = 0;
        virtual bool setIndex(uint32_t index) = 0;
    };
}