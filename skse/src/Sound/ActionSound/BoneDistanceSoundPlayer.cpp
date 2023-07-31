#include "BoneDistanceSoundPlayer.h"

#include "Util/Constants.h"

namespace Sound {
    void BoneDistanceSoundPlayer::loop() {
        // the smooth transitions from animations can sometimes bork up calculcations
        // so we have to wait for half a second before doing anything
        if (initialCooldown > 0) {
            initialCooldown -= Constants::LOOP_TIME_MILLISECONDS;
            return;
        }

        if (--skips > 0) {
            return;
        }
        skips = 3;

        float nextDistance = calculateDistance();

        if (nextDistance < lastDistance) {
            if (!in) {
                in = true;
                if (lastDistance > maxDistance) {
                    maxDistance = lastDistance;
                    if (minDistance != 0) {
                        calculateThreshold();
                    }
                }
                if (inverse && lastDistance > distanceThreshold) {
                    trigger();
                }
            }
        } else if (nextDistance > lastDistance) {
            if (in) {
                in = false;
                if (minDistance == 0 || lastDistance < minDistance) {
                    minDistance = lastDistance;
                    // in starts at false, so by the time we are here max distance has been set and we don't need to check it against 0
                    calculateThreshold();
                }
                if (!inverse && lastDistance < distanceThreshold) {
                    trigger();
                }
            }
        }

        lastDistance = nextDistance;
        timeSinceLastTrigger += Constants::LOOP_TIME_MILLISECONDS;
    }

    void BoneDistanceSoundPlayer::calculateThreshold() {
        distanceThreshold = minDistance + (maxDistance - minDistance) * (inverse ? 0.9 : 0.1);
    }

    void BoneDistanceSoundPlayer::trigger() {
        if (timeSinceLastTrigger < minInterval || (maxInterval != 0 && timeSinceLastTrigger > maxInterval)) {
            timeSinceLastTrigger = 0;
            return;
        }

        play();
        timeSinceLastTrigger = 0;
    }
}