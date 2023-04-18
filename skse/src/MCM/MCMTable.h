#pragma once

#include "MCMSetting.h"

namespace MCM {
    class MCMTable {
    public:
        static void setupForms();
        static void resetValues();
        static void restoreDefaults();

        static int keyAlignment();
        static int keySceneStart();
        static int keySpeedUp();
        static int keySpeedDown();
        static int keyPullOut();
        static int keyAutoMode();
        static int keyFreeCam();

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
        static bool undressWigs();
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
        inline static std::unordered_map<uint32_t, MCMSetting> settings {
            {0xDE2, {38, "keyAlignment"}},
            {0xDEC, {181, "SetFreeCamToggleKey"}},

            {0xDA1, {1, "SetUseIntroScenes"}},

            {0xDA6, {1, "SetEnableFurniture"}},
            {0xDA7, {1, "SetSelectFurniture"}},
            {0xDA8, {15, "SetFurnitureSearchDistance"}},
            {0xDA4, {1, "SetResetClutter"}},
            {0xDA5, {5, "SetResetClutterRadius"}},

            {0xDA9, {1, "SetAutoClimaxAnims"}},

            {0xDDE, {1, "SetUseFreeCam"}},
            {0xDDF, {3, "SetCameraSpeed"}},
            {0xDE0, {45, "SetFreeCamFOV"}},
            {0xDE6, {0, "SetClipinglessFirstPerson"}},

            {0xDA2, {1, "SetsexExcitementMult"}},
            {0xDA3, {1, "SetFemaleSexExcitementMult"}},
            {0xDB5, {0.5, "excitementDecayRate"}},
            {0xDB4, {5000, "excitementDecayGracePeriod"}},

            {0xDA9, {1, "autoClimaxAnimations"}},
            {0xDF9, {1, "SetEndOnOrgasm"}},
            {0xDFA, {0, "SetEndOnSubOrgasm"}},
            {0xDFB, {0, "SetEndOnBothOrgasm"}},
            {0xDFC, {1, "SetSlowMoOrgasms"}},
            {0xDFD, {1, "SetBlurOrgasms"}},

            {0xDAA, {0, "SetAlwaysUndressAtStart"}},
            {0xDAB, {1, "SetRemoveWeaponsAtStart"}},
            {0xDAC, {1, "SetUndressIfNeed"}},
            {0xDAD, {1, "SetPartialUndressing"}},
            {0xDAE, {32, "SetRemoveWeaponsWithSlot"}},
            {0xDAF, {0, "SetAnimateRedress"}},
            {0xDF8, {0, "undressWigs"}},

            {0xDB2, {1000, "SetExpressionDurationMin"}},
            {0xDB3, {3000, "SetExpressionDurationMax"}},

            {0xDEE, {0, "SetOnlyGayAnimsInGayScenes"}},  // intended sex only
            {0XDEF, {0, "PlayerAlwaysDomStraight"}},
            {0XDF0, {0, "PlayerAlwaysSubStraight"}},
            {0XDF1, {0, "PlayerAlwaysDomGay"}},
            {0XDF2, {0, "PlayerAlwaysSubGay"}},
            {0XDF3, {0, "playerSelectRoleStraight"}},
            {0XDF4, {1, "playerSelectRoleGay"}},
            {0XDF5, {0, "playerSelectRoleThreesome"}},
            {0xDDB, {1, "equipStrapOnIfNeeded"}},
            {0xDDC, {0, "unequipStrapOnIfNotNeeded"}},
            {0xDDD, {1, "unequipStrapOnIfInWay"}},
            
            {0xD94, {0, "SetScaling"}},
            {0xD97, {0, "SetSchlongBending"}},
            {0xDE3, {1, "alignmentGroupBySex"}},
            {0xDE4, {0, "alignmentGroupByHeight"}},
            {0xDE5, {1, "alignmentGroupByHeels"}}
        };

        inline static RE::TESGlobal* OStimKeySceneStart;
        inline static RE::TESGlobal* OStimKeySpeedUp;
        inline static RE::TESGlobal* OStimKeySpeedDown;
        inline static RE::TESGlobal* OStimKeyPullOut;
        inline static RE::TESGlobal* OStimKeyAutoMode;

        inline static uint32_t undressingMask = 0x3D8BC39D;

        inline static bool doPapyrusUndressing = false;
        inline static RE::TESGlobal* OStimUsePapyrusUndressing;
    };
}