#include "SingleNodeDistanceSoundPlayer.h"

namespace Sound {
    void SingleNodeDistanceSoundPlayer::loop() {
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