#include "SingleNodeDistanceSoundPlayer.h"

namespace Sound {
    void SingleNodeDistanceSoundPlayer::loop() {
        if (countDown-- > 0) {
            return;
        }
        countDown = 10;

        float nextDistance = calculateDistance();

        logger::info("in: {}, last distance: {}, next distance: {}", in, lastDistance, nextDistance);

        if (nextDistance < lastDistance) {
            if (!in) {
                in = true;
            }
        } else if (nextDistance > lastDistance) {
            if (in) {
                in = false;
                play();
            }
        }

        lastDistance = nextDistance;
    }
}