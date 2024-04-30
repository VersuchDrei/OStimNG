#include "ToyHandler.h"

#include "Core/Thread.h"
#include "Util/Constants.h"

namespace Toys {
    ToyHandler::ToyHandler(OStim::ThreadActor* actor, std::string slot, ToyWrapper* toy)
        : actor{actor},
          settings{toy->getSettings()->getSlotSettings(slot)},
          toy{toy},
          updateInterval{toy->getUpdateInterval()} {

    }

    void ToyHandler::loop() {
        updateCooldown -= Constants::LOOP_TIME_MILLISECONDS;
    }

    void ToyHandler::update(uint16_t peakInterval) {
        if (updateCooldown > 0) {
            return;
        }

        updateCooldown = updateInterval;

        float baseline = 0.0f;
        float peak = 0.0f;

        float currentValue = 0.0f;
        switch (settings->scalingType) {
        case Settings::ScalingType::STIMULATION_RATE:
            currentValue = actor->getBaseExcitementInc() * (1 + actor->getThread()->getRelativeSpeed());
            break;
        case Settings::ScalingType::STIMULATION_VALUE:
            currentValue = actor->getExcitement();
            break;
        case Settings::ScalingType::SPEED:
            currentValue = actor->getThread()->getRelativeSpeed();
            break;
        default:
            currentValue = settings->maxScale;
            break;
        }

        if (currentValue <= settings->minScale) {
            baseline = settings->minMagnitude;
            peak = settings->doPeaks ? settings->minPeak : -1.0f;
        } else if (currentValue >= settings->maxScale) {
            baseline = settings->maxMagnitude;
            peak = settings->doPeaks ? settings->maxPeak : -1.0f;
        } else {
            float scale = (currentValue - settings->minScale) / (settings->maxScale - settings->minScale);
            baseline = settings->minMagnitude + (settings->maxMagnitude - settings->minMagnitude) * scale;
            peak = settings->doPeaks ? settings->minPeak + (settings->maxPeak - settings->minPeak) * scale : -1.0f;
        }

        toy->update(baseline, peak, settings->doPeaks ? peakInterval : -1);
    }

    void ToyHandler::stop() {
        toy->stop();
    }
}