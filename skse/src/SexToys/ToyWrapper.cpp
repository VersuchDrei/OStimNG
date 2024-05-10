#include "ToyWrapper.h"

#include "ToyTable.h"

namespace Toys {
    ToyWrapper::ToyWrapper(OStim::SexToy* toy)
        : toy{toy},
          id{std::string(toy->getID())},
          name{std::string(toy->getName())},
          settings{ToyTable::getSingleton()->getSettings()->getToySettings(id)} {
    }


    void ToyWrapper::turnOn() {
        if (users <= 0) {
            if (settings->enabled) {
                OStim::SexToy* toy = this->toy;
                std::thread([toy]() { toy->turnOn(); }).detach();
            }
            if (users < 0) {
                // hard stopped before, so need to unflag it
                users *= -1;
            }
        }
        users++;
    }

    void ToyWrapper::turnOff(bool overwrite) {
        if (users == 0) {
            // shouldn't happen, but just for safety
            return;
        }

        if (users < 0) {
            // hard stop happened before, so just decrease the absolute value of the negative number
            users++;
            return;
        }

        users--;
        if (users == 0 || overwrite) {
            if (settings->enabled) {
                OStim::SexToy* toy = this->toy;
                std::thread([toy]() { toy->turnOff(); }).detach();
            }
            if (users > 0) {
                // flag toy as hard stopped
                users *= -1;
            }
        }
    }

    void ToyWrapper::update(float baseline, float peak, uint16_t peakInterval) {
        OStim::SexToy* toy = this->toy;
        std::thread([toy, baseline, peak, peakInterval]() { toy->update(baseline, peak, peakInterval); }).detach();
    }

    void ToyWrapper::stop() {
        OStim::SexToy* toy = this->toy;
        std::thread([toy]() { toy->stop(); }).detach();
    }
}