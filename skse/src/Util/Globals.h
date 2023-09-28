#pragma once

namespace Util {
    class Globals {
    public:
        static void setupForms();
        static void resetSaveGameValues();

        static void setSceneIntegrityVerified(bool verified);
        inline static bool isSceneIntegrityVerified() { return sceneIntegrityVerified; }

        static void setPapyrusUndressing(bool doPapyrus);
        inline static bool usePapyrusUndressing() { return doPapyrusUndressing; }

    private:
        inline static bool sceneIntegrityVerified = false;

        inline static bool doPapyrusUndressing = false;
        inline static RE::TESGlobal* OStimUsePapyrusUndressing;
    };
}