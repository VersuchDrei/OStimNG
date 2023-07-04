#pragma once

#include "SoundPlayer.h"

namespace Sound {
    class BoneDistanceSoundPlayer : public SoundPlayer{
    public:
        inline BoneDistanceSoundPlayer(GameAPI::GameSound sound, bool muteWithActor, bool muteWithTarget, OStim::ThreadActor* actor, OStim::ThreadActor* target, bool inverse, int minInverval, int maxInterval)
            : SoundPlayer(sound, muteWithActor, muteWithTarget, actor, target), inverse{inverse}, minInterval{minInterval}, maxInterval{maxInterval} {}

        virtual void loop();

    protected:
        virtual float calculateDistance() = 0;

    private:
        bool inverse;
        int minInterval;
        int maxInterval;

        int initialCooldown = 1000;
        int skips = 3;

        float minDistance = 0;
        float maxDistance = 0;
        float distanceThreshold = 0;

        float lastDistance = 0;
        float in = false;
        int timeSinceLastTrigger = 0;

        void calculateThreshold();
        void trigger();
    };
}