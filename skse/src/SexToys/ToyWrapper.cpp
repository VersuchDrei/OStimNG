#include "ToyWrapper.h"

#include "ToyTable.h"

namespace Toys {
    ToyWrapper::ToyWrapper(OStim::SexToy* toy)
        : toy{toy},
          id{std::string(toy->getID())},
          name{std::string(toy->getName())},
          settings{ToyTable::getSettings()->getToySettings(id)} {
    }


    void ToyWrapper::turnOn() {
        if (users <= 0) {
            if (settings->enabled) {
                toy->turnOn();
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
                toy->turnOff();
            }
            if (users > 0) {
                // flag toy as hard stopped
                users *= -1;
            }
        }
    }
}