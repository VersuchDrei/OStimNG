#include "MCMTable.h"

#include "Serial/Manager.h"
#include "Util.h"

namespace MCM {
    void MCMTable::setupForms() {
        auto dataHandler = RE::TESDataHandler::GetSingleton();

        OStimKeyAlignment = dataHandler->LookupForm<RE::TESGlobal>(0xDE2, "OStim.esp");
        OStimKeySceneStart = dataHandler->LookupForm<RE::TESGlobal>(0xDE7, "OStim.esp");
        OStimKeySpeedUp = dataHandler->LookupForm<RE::TESGlobal>(0xDE8, "OStim.esp");
        OStimKeySpeedDown = dataHandler->LookupForm<RE::TESGlobal>(0xDE9, "OStim.esp");
        OStimKeyPullOut = dataHandler->LookupForm<RE::TESGlobal>(0xDEA, "OStim.esp");
        OStimKeyAutoMode = dataHandler->LookupForm<RE::TESGlobal>(0xDEB, "OStim.esp");
        OStimKeyFreeCam = dataHandler->LookupForm<RE::TESGlobal>(0xDEC, "OStim.esp");

        OStimUseFreeCam = dataHandler->LookupForm<RE::TESGlobal>(0xDDE, "OStim.esp");
        OStimFreeCamSpeed = dataHandler->LookupForm<RE::TESGlobal>(0xDDF, "OStim.esp");
        OStimFreeCamFOV = dataHandler->LookupForm<RE::TESGlobal>(0xDE0, "OStim.esp");
        OStimImprovedCamSupport = dataHandler->LookupForm<RE::TESGlobal>(0xDE6, "OStim.esp");

        maleExcitementMultSetting = dataHandler->LookupForm<RE::TESGlobal>(0xDA2, "OStim.esp");
        femaleExcitementMultSetting = dataHandler->LookupForm<RE::TESGlobal>(0xDA3, "OStim.esp");
        OStimExcitementDecayRate = dataHandler->LookupForm<RE::TESGlobal>(0xDB5, "OStim.esp");
        OStimExcitementDecayGracePeriod = dataHandler->LookupForm<RE::TESGlobal>(0xDB4, "OStim.esp");

        OStimDisableScaling = dataHandler->LookupForm<RE::TESGlobal>(0xD94, "OStim.esp");
        OStimDisableSchlongBending = dataHandler->LookupForm<RE::TESGlobal>(0xD97, "OStim.esp");

        OStimUndressAtStart = dataHandler->LookupForm<RE::TESGlobal>(0xDAA, "OStim.esp");
        OStimRemoveWeaponsAtStart = dataHandler->LookupForm<RE::TESGlobal>(0xDAB, "OStim.esp");
        OStimUndressMidScene = dataHandler->LookupForm<RE::TESGlobal>(0xDAC, "OStim.esp");
        OStimPartialUndressing = dataHandler->LookupForm<RE::TESGlobal>(0xDAD, "OStim.esp");
        OStimRemoveWeaponsWithSlot = dataHandler->LookupForm<RE::TESGlobal>(0xDAE, "OStim.esp");
        OStimAnimateRedress = dataHandler->LookupForm<RE::TESGlobal>(0xDAF, "OStim.esp");
        OStimUsePapyrusUndressing = dataHandler->LookupForm<RE::TESGlobal>(0xDB0, "OStim.esp");

        OStimExpressionDurationMin = dataHandler->LookupForm<RE::TESGlobal>(0xDB2, "OStim.esp");
        OStimExpressionDurationMax = dataHandler->LookupForm<RE::TESGlobal>(0xDB3, "OStim.esp");

        OStimEquipStrapOnIfNeeded = dataHandler->LookupForm<RE::TESGlobal>(0xDDB, "OStim.esp");
        OStimUnequipStrapOnIfNotNeeded = dataHandler->LookupForm<RE::TESGlobal>(0xDDC, "OStim.esp");
        OStimUnequipStrapOnIfInWay = dataHandler->LookupForm<RE::TESGlobal>(0xDDD, "OStim.esp");

        OStimAlignmentGroupBySex = dataHandler->LookupForm<RE::TESGlobal>(0xDE3, "OStim.esp");
        OStimAlignmentGroupByHeight = dataHandler->LookupForm<RE::TESGlobal>(0xDE4, "OStim.esp");
        OStimAlignmentGroupByHeels = dataHandler->LookupForm<RE::TESGlobal>(0xDE5, "OStim.esp");
    }

    void MCMTable::resetDefaults() {
        undressingMask = 0x3D8BC39D;
        doPapyrusUndressing = false;
    }


    bool MCMTable::useFreeCam() {
        return OStimUseFreeCam->value != 0;
    }

    int MCMTable::keyAlignment() {
        return static_cast<int>(OStimKeyAlignment->value); }

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
        return static_cast<int>(OStimKeyFreeCam->value);
    }


    float MCMTable::freeCamSpeed() {
        return OStimFreeCamSpeed->value;
    }

    float MCMTable::freeCamFOV() {
        return OStimFreeCamFOV->value;
    }

    bool MCMTable::supportImprovedCam() {
        return OStimImprovedCamSupport->value;
    }


    float MCMTable::getMaleSexExcitementMult() {
        return maleExcitementMultSetting->value;
    }

    float MCMTable::getFemaleSexExcitementMult() {
        return femaleExcitementMultSetting->value;
    }

    float MCMTable::getExcitementDecayRate() {
        return OStimExcitementDecayRate->value;
    }

    int MCMTable::getExcitementDecayGracePeriod() {
        return OStimExcitementDecayGracePeriod->value;
    }


    bool MCMTable::isScalingDisabled() {
        return OStimDisableScaling->value != 0;
    }

    bool MCMTable::isSchlongBendingDisabled() {
        return OStimDisableSchlongBending->value != 0;
    }


    bool MCMTable::undressAtStart() {
        return OStimUndressAtStart->value != 0;
    }

    bool MCMTable::removeWeaponsAtStart() {
        return OStimRemoveWeaponsAtStart->value != 0;
    }

    bool MCMTable::undressMidScene() {
        return OStimUndressMidScene->value != 0;
    }

    bool MCMTable::partialUndressing() {
        return OStimPartialUndressing->value != 0;
    }

    uint32_t MCMTable::removeWeaponsWithSlot() {
        // the stripping function will treat this as a slotmask
        // so if we ever want to support multiple slots here in the future we just need to change the return value of this function
        return 1 << static_cast<int>(OStimRemoveWeaponsWithSlot->value - 30);
    }

    bool MCMTable::animateRedress() {
        return OStimAnimateRedress->value != 0;
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
        return OStimExpressionDurationMin->value;
    }

    int MCMTable::getExpressionDurationMax() {
        return OStimExpressionDurationMax->value;
    }


    bool MCMTable::equipStrapOnIfNeeded() {
        return OStimEquipStrapOnIfNeeded->value != 0;
    }

    bool MCMTable::unequipStrapOnIfNotNeeded() {
        return OStimUnequipStrapOnIfNotNeeded->value != 0;
    }

    bool MCMTable::unequipStrapOnIfInWay() {
        return OStimUnequipStrapOnIfInWay->value != 0;
    }


    bool MCMTable::groupAlignmentBySex() {
        return OStimAlignmentGroupBySex->value != 0;
    }

    bool MCMTable::groupAlignmentByHeight() {
        return OStimDisableScaling->value != 0 && OStimAlignmentGroupByHeight->value != 0;
    }

    bool MCMTable::groupAlignmentByHeels() {
        return OStimAlignmentGroupByHeels->value != 0;
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

        json["keyAlignment"] = OStimKeyAlignment->value;
        json["SetFreeCamToggleKey"] = OStimKeyFreeCam->value;

        json["SetUseFreeCam"] = OStimUseFreeCam->value;
        json["SetCameraSpeed"] = OStimFreeCamSpeed->value;
        json["SetFreeCamFOV"] = OStimFreeCamFOV->value;
        json["SetClipinglessFirstPerson"] = OStimImprovedCamSupport->value;

        json["SetsexExcitementMult"] = maleExcitementMultSetting->value;
        json["SetFemaleSexExcitementMult"] = femaleExcitementMultSetting->value;
        json["excitementDecayRate"] = OStimExcitementDecayRate->value;
        json["excitementDecayGracePeriod"] = OStimExcitementDecayGracePeriod->value;

        json["SetAlwaysUndressAtStart"] = OStimUndressAtStart->value;
        json["SetRemoveWeaponsAtStart"] = OStimRemoveWeaponsAtStart->value;
        json["SetUndressIfNeed"] = OStimUndressMidScene->value;
        json["SetPartialUndressing"] = OStimPartialUndressing->value;
        json["SetRemoveWeaponsWithSlot"] = OStimRemoveWeaponsWithSlot->value;
        json["SetAnimateRedress"] = OStimAnimateRedress->value;
        json["SetUndressingSlotMask"] = undressingMask;

        json["SetExpressionDurationMin"] = OStimExpressionDurationMin->value;
        json["SetExpressionDurationMax"] = OStimExpressionDurationMax->value;

        json["equipStrapOnIfNeeded"] = OStimEquipStrapOnIfNeeded->value;
        json["unequipStrapOnIfNotNeeded"] = OStimUnequipStrapOnIfNotNeeded->value;
        json["unequipStrapOnIfInWay"] = OStimUnequipStrapOnIfInWay->value;

        json["SetScaling"] = OStimDisableScaling->value;
        json["SetSchlongBending"] = OStimDisableSchlongBending->value;
        json["alignmentGroupBySex"] = OStimAlignmentGroupBySex->value;
        json["alignmentGroupByHeight"] = OStimAlignmentGroupByHeight->value;
        json["alignmentGroupByHeels"] = OStimAlignmentGroupByHeels->value;

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

        importSetting(json, OStimKeyAlignment, "keyAlignment", 38);
        importSetting(json, OStimKeyFreeCam, "SetFreeCamToggleKey", 181);

        importSetting(json, OStimUseFreeCam, "SetUseFreeCam", 1);
        importSetting(json, OStimFreeCamSpeed, "SetCameraSpeed", 3);
        importSetting(json, OStimFreeCamFOV, "SetFreeCamFOV", 45);
        importSetting(json, OStimImprovedCamSupport, "SetClipinglessFirstPerson", 0);

        importSetting(json, maleExcitementMultSetting, "SetsexExcitementMult", 1);
        importSetting(json, femaleExcitementMultSetting, "SetFemaleSexExcitementMult", 1);
        importSetting(json, OStimExcitementDecayRate, "excitementDecayRate", 0);
        importSetting(json, OStimExcitementDecayGracePeriod, "excitementDecayGracePeriod", 0);

        importSetting(json, OStimUndressAtStart, "SetAlwaysUndressAtStart", 0);
        importSetting(json, OStimRemoveWeaponsAtStart, "SetRemoveWeaponsAtStart", 1);
        importSetting(json, OStimUndressMidScene, "SetUndressIfNeed", 1);
        importSetting(json, OStimPartialUndressing, "SetPartialUndressing", 1);
        importSetting(json, OStimRemoveWeaponsWithSlot, "SetRemoveWeaponsWithSlot", 32);
        importSetting(json, OStimAnimateRedress, "SetAnimateRedress", 0);
        if (json.contains("SetUndressingSlotMask")) {
            undressingMask = json["SetUndressingSlotMask"];
        } else {
            undressingMask = 0x3D8BC39D;
        }

        importSetting(json, OStimExpressionDurationMin, "SetExpressionDurationMin", 1000);
        importSetting(json, OStimExpressionDurationMax, "SetExpressionDurationMax", 3000);

        importSetting(json, OStimEquipStrapOnIfNeeded, "equipStrapOnIfNeeded", 1);
        importSetting(json, OStimUnequipStrapOnIfNotNeeded, "unequipStrapOnIfNotNeeded", 0);
        importSetting(json, OStimUnequipStrapOnIfInWay, "unequipStrapOnIfInWay", 1);

        importSetting(json, OStimDisableScaling, "SetScaling", 0);
        importSetting(json, OStimDisableSchlongBending, "SetSchlongBending", 0);
        importSetting(json, OStimAlignmentGroupBySex, "alignmentGroupBySex", 1);
        importSetting(json, OStimAlignmentGroupByHeight, "alignmentGroupByHeight", 0);
        importSetting(json, OStimAlignmentGroupByHeels, "alignmentGroupByHeels", 0);

        Serialization::importSettings(json);
    }

    void MCMTable::importSetting(json json, RE::TESGlobal* setting, std::string key, float fallback) {
        if (json.contains(key)) {
            setting->value = json[key];
        } else {
            setting->value = fallback;
        }
    }
}