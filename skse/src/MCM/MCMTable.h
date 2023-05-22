#pragma once

#include "MCMSetting.h"

namespace MCM {
    class MCMTable {
    public:
        static void setupForms();
        static void resetValues();
        static void restoreDefaults();

        inline static bool resetPosition() { return settings[0xE16].asBool(); }

        static int keyAlignment();
        static int keySceneStart();
        static int keySpeedUp();
        static int keySpeedDown();
        static int keyPullOut();
        static int keyAutoMode();
        static int keyFreeCam();

        static bool useRumble();

        static bool useFreeCam();
        static float freeCamSpeed();
        static float freeCamFOV();
        static bool supportImprovedCam();
        static bool useScreenShake();

        static float getMaleSexExcitementMult();
        static float getFemaleSexExcitementMult();
        static float getExcitementDecayRate();
        static int getExcitementDecayGracePeriod();

        static bool getSlowMotionOnOrgasm();
        static bool getBlurOnOrgasm();

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

        static int getMoanIntervalMin();
        static int getMoanIntervalMax();
        static float getMoanVolume();
        static float getSoundVolume();

        static bool equipStrapOnIfNeeded();
        static bool unequipStrapOnIfNotNeeded();
        static bool unequipStrapOnIfInWay();

        inline static bool useSoSSex() { return settings[0xE04].asBool(); }
        inline static bool futaUseMaleExcitement() { return settings[0xE05].asBool(); }
        inline static bool futaUseMaleClimax() { return settings[0xE17].asBool(); }

        static bool groupAlignmentBySex();
        static bool groupAlignmentByHeight();
        static bool groupAlignmentByHeels();
        
        static float bedRealignment();
        static float bedOffset();

        static void exportSettings();
        static void importSettings();

    private:
        inline static std::unordered_map<uint32_t, MCMSetting> settings{
            {0xE16, {1, "SetResetPosition"}},

            {0xDE2, {38, "keyAlignment"}},
            {0xDEC, {181, "SetFreeCamToggleKey"}},

            {0xE11, {1, "SetUseRumble"}},

            {0xDA1, {1, "SetUseIntroScenes"}},

            {0xDA6, {1, "SetEnableFurniture"}},
            {0xDA7, {1, "SetSelectFurniture"}},
            {0xDA8, {15, "SetFurnitureSearchDistance"}},
            {0xDA4, {1, "SetResetClutter"}},
            {0xDA5, {5, "SetResetClutterRadius"}},
            {0xDFE, {0, "SetBedRealignment"}},
            {0xDFF, {3, "bedOffset"}},

            {0xDDE, {1, "SetUseFreeCam"}},
            {0xDDF, {3, "SetCameraSpeed"}},
            {0xDE0, {45, "SetFreeCamFOV"}},

            {0xDE6, {0, "SetClipinglessFirstPerson"}},
            {0xE10, {0, "SetUseScreenShake"}},
            {0xE12, {0, "SetForceFirstPerson"}}, // on scene end

            {0xDA2, {1, "SetsexExcitementMult"}},
            {0xDA3, {1, "SetFemaleSexExcitementMult"}},
            {0xDB5, {0.5, "excitementDecayRate"}},
            {0xDB4, {5000, "excitementDecayGracePeriod"}},

            {0xE00, {1, "enablePlayerBar"}},
            {0xE01, {1, "enableNpcBar"}},
            {0xE02, {0, "SetAutoHideBar"}},
            {0xE03, {0, "SetMatchColorToGender"}},

            {0xDA9, {1, "SetAutoClimaxAnims"}},
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

            {0xE0A, {2500, "moanIntervalMin"}},
            {0xE0B, {5000, "moanIntervalMax"}},
            {0xE0C, {1, "moanVolume"}},
            {0xE0D, {1, "soundVolume"}},

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

            {0xE04, {1, "useSoSSex"}},
            {0xE05, {0, "futaUseMaleExcitement"}},
            {0xE17, {1, "futaUseMaleClimax"}},
            
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