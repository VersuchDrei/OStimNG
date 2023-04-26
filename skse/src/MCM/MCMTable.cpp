#include "MCMTable.h"

#include "Serial/Manager.h"
#include "Util/CompatibilityTable.h"
#include "Util.h"

namespace MCM {
    void MCMTable::setupForms() {
        for (auto& [formID, setting] : settings) {
            setting.setup(formID);
        }
        auto dataHandler = RE::TESDataHandler::GetSingleton();

        OStimKeySceneStart = dataHandler->LookupForm<RE::TESGlobal>(0xDE7, "OStim.esp");
        OStimKeySpeedUp = dataHandler->LookupForm<RE::TESGlobal>(0xDE8, "OStim.esp");
        OStimKeySpeedDown = dataHandler->LookupForm<RE::TESGlobal>(0xDE9, "OStim.esp");
        OStimKeyPullOut = dataHandler->LookupForm<RE::TESGlobal>(0xDEA, "OStim.esp");
        OStimKeyAutoMode = dataHandler->LookupForm<RE::TESGlobal>(0xDEB, "OStim.esp");

        OStimUsePapyrusUndressing = dataHandler->LookupForm<RE::TESGlobal>(0xDB0, "OStim.esp");
    }

    void MCMTable::resetValues() {
        undressingMask = 0x3D8BC39D;
        doPapyrusUndressing = false;
    }

    void MCMTable::restoreDefaults() {
        for (auto& [formID, setting] : settings) {
            setting.restoreDefault();
        }
        undressingMask = 0x3D8BC39D;
    }


    bool MCMTable::useFreeCam() {
        return settings[0xDDE].asBool();
    }

    int MCMTable::keyAlignment() {
        return settings[0xDE2].asInt();
    }

    int MCMTable::keySceneStart() {
        return static_cast<int>(OStimKeySceneStart->value);
    }

    int MCMTable::keySpeedUp() {
        return static_cast<int>(OStimKeySpeedUp->value);
    }

    int MCMTable::keySpeedDown() {
        return static_cast<int>(OStimKeySpeedDown->value);
    }

    int MCMTable::keyPullOut() {
        return static_cast<int>(OStimKeyPullOut->value);
    }

    int MCMTable::keyAutoMode() {
        return static_cast<int>(OStimKeyAutoMode->value);
    }

    int MCMTable::keyFreeCam() {
        return settings[0xDEC].asInt();
    }


    float MCMTable::freeCamSpeed() {
        return settings[0xDDF].asFloat();
    }

    float MCMTable::freeCamFOV() {
        return settings[0xDE0].asFloat();
    }

    bool MCMTable::supportImprovedCam() {
        return settings[0xDE6].asBool();
    }


    float MCMTable::getMaleSexExcitementMult() {
        return settings[0xDA2].asFloat();
    }

    float MCMTable::getFemaleSexExcitementMult() {
        return settings[0xDA3].asFloat();
    }

    float MCMTable::getExcitementDecayRate() {
        return settings[0xDB5].asFloat();
    }

    int MCMTable::getExcitementDecayGracePeriod() {
        return settings[0xDB4].asInt();
    }


    bool MCMTable::isScalingDisabled() {
        return settings[0xD94].asBool();
    }

    bool MCMTable::isSchlongBendingDisabled() {
        return settings[0xD97].asBool();
    }


    bool MCMTable::undressAtStart() {
        return settings[0xDAA].asBool();
    }

    bool MCMTable::removeWeaponsAtStart() {
        return settings[0xDAB].asBool();
    }

    bool MCMTable::undressMidScene() {
        return settings[0xDAC].asBool();
    }

    bool MCMTable::partialUndressing() {
        return settings[0xDAD].asBool();
    }

    uint32_t MCMTable::removeWeaponsWithSlot() {
        // the stripping function will treat this as a slotmask
        // so if we ever want to support multiple slots here in the future we just need to change the return value of this function
        return 1 << static_cast<int>(settings[0xDAE].asInt() - 30);
    }

    bool MCMTable::animateRedress() {
        return settings[0xDAF].asBool();
    }

    bool MCMTable::undressWigs() {
        return settings[0xDF8].asBool();
    }

    uint32_t MCMTable::getUndressingMask() {
        return undressingMask;
    }

    void MCMTable::setUndressingMask(uint32_t mask) {
        undressingMask = mask;
    }

    void MCMTable::setPapyusUndressing(bool doPapyrus) {
        doPapyrusUndressing = doPapyrus;
        OStimUsePapyrusUndressing->value = doPapyrus ? 1 : 0;
        logger::info("papyrus undressing is {}", doPapyrus ? "enabled" : "disabled");
    }

    bool MCMTable::usePapyrusUndressing() {
        return doPapyrusUndressing;
    }


    int MCMTable::getExpressionDurationMin() {
        return settings[0xDB2].asInt();
    }

    int MCMTable::getExpressionDurationMax() {
        return settings[0xDB3].asInt();
    }


    bool MCMTable::equipStrapOnIfNeeded() {
        return settings[0xDDB].asBool();
    }

    bool MCMTable::unequipStrapOnIfNotNeeded() {
        return settings[0xDDC].asBool();
    }

    bool MCMTable::unequipStrapOnIfInWay() {
        return settings[0xDDD].asBool();
    }


    bool MCMTable::useSoSSex() {
        return Compatibility::CompatibilityTable::sosInstalled() && settings[0xE04].asBool();
    }

    bool MCMTable::futaUseMaleExcitement() {
        return Compatibility::CompatibilityTable::sosInstalled() && settings[0xE04].asBool() && settings[0xE05].asBool();
    }


    bool MCMTable::groupAlignmentBySex() {
        return settings[0xDE3].asBool();
    }

    bool MCMTable::groupAlignmentByHeight() {
        return settings[0xDE4].asBool();
    }

    bool MCMTable::groupAlignmentByHeels() {
        return settings[0xDE5].asBool();
    }

    float MCMTable::bedRealignment() {
        return settings[0xDFE].asFloat();
    }

    float MCMTable::bedOffset() {
        return settings[0xDFF].asFloat();
    }


    void MCMTable::exportSettings() {
        const auto settings_path = util::settings_path();
        if (!fs::exists(*settings_path)) {
            logger::warn("settings file doesn't exist or no access");
            return;
        }

        std::ifstream ifs(*settings_path);
        json json = json::parse(ifs, nullptr, false);

        if (json.is_discarded()) {
            logger::warn("settings file is malformed");
            return;
        }

        for (auto& [formID, setting] : settings) {
            setting.exportSetting(json);
        }

        json["SetUndressingSlotMask"] = undressingMask;

        Serialization::exportSettings(json);

        std::ofstream file(*settings_path);
        file << std::setw(2) << json << std::endl;
    }

    void MCMTable::importSettings() {
        const auto settings_path = util::settings_path();
        if (!fs::exists(*settings_path)) {
            logger::warn("settings file doesn't exist or no access");
            return;
        }

        std::ifstream ifs(*settings_path);
        json json = json::parse(ifs, nullptr, false);

        if (json.is_discarded()) {
            logger::warn("settings file is malformed");
            return;
        }

        for (auto& [formID, setting] : settings) {
            setting.importSetting(json);
        }

        if (json.contains("SetUndressingSlotMask")) {
            undressingMask = json["SetUndressingSlotMask"];
        } else {
            undressingMask = 0x3D8BC39D;
        }

        Serialization::importSettings(json);
    }
}