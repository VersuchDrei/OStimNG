#pragma once

namespace MCM {
    class MCMTable {
    public:
        static void setupForms();
        static void resetDefaults();

        static bool useFreeCam();
        static float freeCamSpeed();
        static float freeCamFOV();
        static bool supportImprovedCam();

        static float getMaleSexExcitementMult();
        static float getFemaleSexExcitementMult();
        static float getExcitementDecayRate();
        static int getExcitementDecayGracePeriod();

        static bool isScalingDisabled();
        static bool isSchlongBendingDisabled();

        static bool undressAtStart();
        static bool removeWeaponsAtStart();
        static bool undressMidScene();
        static bool partialUndressing();
        static uint32_t removeWeaponsWithSlot();
        static bool animateRedress();
        static uint32_t getUndressingMask();
        static void setUndressingMask(uint32_t mask);

        // not really an MCM option but idk where else to put it
        static void setPapyusUndressing(bool doPapyrus);
        static bool usePapyrusUndressing();

        static int getExpressionDurationMin();
        static int getExpressionDurationMax();

        static bool equipStrapOnIfNeeded();
        static bool unequipStrapOnIfNotNeeded();
        static bool unequipStrapOnIfInWay();

        static bool groupAlignmentBySex();
        static bool groupAlignmentByHeight();
        static bool groupAlignmentByHeels();

        static void exportSettings();
        static void importSettings();

    private:
        inline static RE::TESGlobal* OStimKeyAlignment;

        inline static RE::TESGlobal* OStimUseFreeCam;
        inline static RE::TESGlobal* OStimFreeCamSpeed;
        inline static RE::TESGlobal* OStimFreeCamFOV;
        inline static RE::TESGlobal* OStimImprovedCamSupport;

        inline static RE::TESGlobal* maleExcitementMultSetting;
        inline static RE::TESGlobal* femaleExcitementMultSetting;
        inline static RE::TESGlobal* OStimExcitementDecayRate;
        inline static RE::TESGlobal* OStimExcitementDecayGracePeriod;

        inline static RE::TESGlobal* OStimDisableScaling;
        inline static RE::TESGlobal* OStimDisableSchlongBending;

        inline static RE::TESGlobal* OStimUndressAtStart;
        inline static RE::TESGlobal* OStimRemoveWeaponsAtStart;
        inline static RE::TESGlobal* OStimUndressMidScene;
        inline static RE::TESGlobal* OStimPartialUndressing;
        inline static RE::TESGlobal* OStimRemoveWeaponsWithSlot;
        inline static RE::TESGlobal* OStimAnimateRedress;
        inline static uint32_t undressingMask = 0x3D8BC39D;

        inline static bool doPapyrusUndressing = false;
        inline static RE::TESGlobal* OStimUsePapyrusUndressing;

        inline static RE::TESGlobal* OStimExpressionDurationMin;
        inline static RE::TESGlobal* OStimExpressionDurationMax;

        inline static RE::TESGlobal* OStimEquipStrapOnIfNeeded;
        inline static RE::TESGlobal* OStimUnequipStrapOnIfNotNeeded;
        inline static RE::TESGlobal* OStimUnequipStrapOnIfInWay;

        inline static RE::TESGlobal* OStimAlignmentGroupBySex;
        inline static RE::TESGlobal* OStimAlignmentGroupByHeight;
        inline static RE::TESGlobal* OStimAlignmentGroupByHeels;

        static void importSetting(json json, RE::TESGlobal* setting, std::string key, float fallback);
    };
}