#include "MCMSetting.h"

namespace MCM {
    void MCMSetting::setup() {
        globalVariable = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(formID, "OStim.esp");
    }

    void MCMSetting::exportSetting(json& json) {
        json[exportValue] = globalVariable->value;
    }

    void MCMSetting::importSetting(json& json) {
        if (json.contains(exportValue)) {
            globalVariable->value = json[exportValue];
        } else {
            globalVariable->value = defaultValue;
        }
    }
}