#pragma once

namespace MCM {
    struct MCMSetting {
        uint32_t formID;
        RE::TESGlobal* globalVariable;
        float defaultValue;
        std::string exportValue;

        void setup();
        void exportSetting(json& json);
        void importSetting(json& json);
    };
}