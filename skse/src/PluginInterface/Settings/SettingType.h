#pragma once

namespace OStim {
    enum SettingType : uint32_t {
        TOGGLE = 0,
        SLIDER = 1,
        DROP_DOWN = 2,
        TEXT_INPUT = 3, // will need UIExtensions until we implement our own text box
        BUTTON = 4
    };
}