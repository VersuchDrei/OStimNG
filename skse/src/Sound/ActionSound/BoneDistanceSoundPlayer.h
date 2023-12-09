#pragma once

#include "SoundPlayer.h"

namespace Sound {
    class BoneDistanceSoundPlayer : public SoundPlayer{
    public:
        inline BoneDistanceSoundPlayer(GameAPI::GameSound sound, bool muteWithActor, bool muteWithTarget, OStim::ThreadActor* actor, OStim::ThreadActor* target, bool inverse, int minInterval, int maxInterval)
            : SoundPlayer(sound, muteWithActor, muteWithTarget, actor, target), inverse{inverse}, minInterval{minInterval}, maxInterval{maxInterval} {}

        virtual void loop();

    protected:
        virtual float calculateDistance() = 0;

    private:
        bool inverse;
        int minInterval;
        int maxInterval;

        int initialCooldown = 1000;

        float minDistance = -1;
        float maxDistance = -1;
        float distanceThreshold = -1;
        bool inThreshold = false;

        int timeSinceLastTrigger = 0;

        void calculateThreshold();
        void trigger();
    };
}