#include "ToyTable.h"

namespace Toys {
    void ToyTable::addToySlots(std::vector<std::string>& slots) {
        for (std::string slot : slots) {
            toySlots.insert(slot);
        }
    }


    void ToyTable::reloadToys() {
        if (isReloading) {
            return;
        }

        isReloading = true;
        toys.clear();
        std::vector<OStim::SexToyReloadListener*> listeners = reloadListeners;
        std::thread([listeners]() {
            for (OStim::SexToyReloadListener* listener : listeners) {
                listener->reloadToys();
            }
            isReloading = false;
            // TODO inform thread
        }).detach();
    }

    ToyWrapper* ToyTable::getToy(std::string id) {
        if (isReloading) {
            return nullptr;
        }

        for (ToyWrapper& toy : toys) {
            if (std::string(toy.getID()) == id) {
                return &toy;
            }
        }

        return nullptr;
    }
}