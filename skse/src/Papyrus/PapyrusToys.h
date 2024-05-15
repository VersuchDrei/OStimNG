#pragma once

#include "SexToys/ToyTable.h"

namespace PapyrusToys {
	using VM = RE::BSScript::IVirtualMachine;

	std::vector<std::string> GetToySlots(RE::StaticFunctionTag*) {
        std::set<std::string> slotSet = Toys::ToyTable::getSingleton()->getToySlots();
        std::vector<std::string> slotVec(slotSet.begin(), slotSet.end());
        return slotVec;
	}

	std::vector<std::string> GetToys(RE::StaticFunctionTag*) {
		std::vector<std::string> toys;
        for (Toys::ToyWrapper& toy : *Toys::ToyTable::getSingleton()->getToys()) {
            toys.push_back(toy.getID());
            toys.push_back(toy.getName());
		}
		return toys;
	}


	int GetSynchronizationType(RE::StaticFunctionTag*) {
        return static_cast<int>(Toys::ToyTable::getSingleton()->getSettings()->synchronizationType);
	}

	void SetSynchronizationType(RE::StaticFunctionTag*, int type) {
		Toys::ToyTable::getSingleton()->getSettings()->synchronizationType = static_cast<Toys::Settings::SynchronizationType>(type);
	}

#pragma region toy settings
	bool GetToyEnabled(RE::StaticFunctionTag*, std::string toy) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return false;
		}

		return wrapper->getSettings()->enabled;
	}

	void SetToyEnabled(RE::StaticFunctionTag*, std::string toy, bool enabled) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        wrapper->getSettings()->enabled = enabled;
	}

	int GetToySynchronizationType(RE::StaticFunctionTag*, std::string toy) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return -1;
        }

        return static_cast<int>(wrapper->getSettings()->synchronizationType);
	}

	void SetToySynchronizationType(RE::StaticFunctionTag*, std::string toy, int type) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        wrapper->getSettings()->synchronizationType = static_cast<Toys::Settings::SynchronizationType>(type);
	}

	bool GetToyClimax(RE::StaticFunctionTag*, std::string toy) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return false;
		}

		return wrapper->getSettings()->climax;
	}

	void SetToyClimax(RE::StaticFunctionTag*, std::string toy, bool climax) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        wrapper->getSettings()->climax = climax;
	}

	float GetToyClimaxMagnitude(RE::StaticFunctionTag*, std::string toy) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return 0.0f;
		}

		return wrapper->getSettings()->climaxMagnitude;
	}

	void SetToyClimaxMagnitude(RE::StaticFunctionTag*, std::string toy, float magnitude) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        wrapper->getSettings()->climaxMagnitude = magnitude;
	}
#pragma endregion

#pragma region slot settings
    int GetSlotSynchronizationType(RE::StaticFunctionTag*, std::string toy, std::string slot) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return -1;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return -1;
        }

        return static_cast<int>(settings->synchronizationType);
    }

    void SetSlotSynchronizationType(RE::StaticFunctionTag*, std::string toy, std::string slot, int type) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return;
        }

        settings->synchronizationType = static_cast<Toys::Settings::SynchronizationType>(type);
    }

    float GetSlotMinScale(RE::StaticFunctionTag*, std::string toy, std::string slot) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return -1.0f;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return -1.0f;
        }

        return settings->minScale;
    }

    void SetSlotMinScale(RE::StaticFunctionTag*, std::string toy, std::string slot, float scale) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return;
        }

        settings->minScale = scale;
    }

    float GetSlotMaxScale(RE::StaticFunctionTag*, std::string toy, std::string slot) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return -1.0f;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return -1.0f;
        }

        return settings->maxScale;
    }

    void SetSlotMaxScale(RE::StaticFunctionTag*, std::string toy, std::string slot, float scale) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return;
        }

        settings->maxScale = scale;
    }

    float GetSlotMinMagnitude(RE::StaticFunctionTag*, std::string toy, std::string slot) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return -1.0f;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return -1.0f;
        }

        return settings->minMagnitude;
    }

    void SetSlotMinMagnitude(RE::StaticFunctionTag*, std::string toy, std::string slot, float scale) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return;
        }

        settings->minMagnitude = scale;
    }

    float GetSlotMaxMagnitude(RE::StaticFunctionTag*, std::string toy, std::string slot) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return -1.0f;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return -1.0f;
        }

        return settings->maxMagnitude;
    }

    void SetSlotMaxMagnitude(RE::StaticFunctionTag*, std::string toy, std::string slot, float scale) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return;
        }

        settings->maxMagnitude = scale;
    }

    bool GetSlotDoPeaks(RE::StaticFunctionTag*, std::string toy, std::string slot) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return false;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return false;
        }

        return settings->doPeaks;
    }

    void SetSlotDoPeaks(RE::StaticFunctionTag*, std::string toy, std::string slot, bool doPeaks) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return;
        }

        settings->doPeaks = doPeaks;
    }

    float GetSlotMinPeak(RE::StaticFunctionTag*, std::string toy, std::string slot) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return -1.0f;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return -1.0f;
        }

        return settings->minPeak;
    }

    void SetSlotMinPeak(RE::StaticFunctionTag*, std::string toy, std::string slot, float peak) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return;
        }

        settings->minPeak = peak;
    }

    float GetSlotMaxPeak(RE::StaticFunctionTag*, std::string toy, std::string slot) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return -1.0f;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return -1.0f;
        }

        return settings->maxPeak;
    }

    void SetSlotMaxPeak(RE::StaticFunctionTag*, std::string toy, std::string slot, float peak) {
        Toys::ToyWrapper* wrapper = Toys::ToyTable::getSingleton()->getToy(toy);
        if (!wrapper) {
            return;
        }

        Toys::Settings::SlotSettings* settings = wrapper->getSettings()->getSlotSettingsUnsafe(slot);
        if (!settings) {
            return;
        }

        settings->maxPeak = peak;
    }
#pragma endregion

	bool Bind(VM* a_vm) {
		const auto obj = "OToys"sv;

		BIND(GetToySlots);
        BIND(GetToys);

		BIND(GetSynchronizationType);
        BIND(SetSynchronizationType);

		BIND(GetToyEnabled);
        BIND(SetToyEnabled);
        BIND(GetToySynchronizationType);
        BIND(SetToySynchronizationType);
        BIND(GetToyClimax);
        BIND(SetToyClimax);
        BIND(GetToyClimaxMagnitude);
        BIND(SetToyClimaxMagnitude);

        BIND(GetSlotSynchronizationType);
        BIND(SetSlotSynchronizationType);
        BIND(SetSlotMinScale);
        BIND(GetSlotMinScale);
        BIND(SetSlotMinMagnitude);
        BIND(GetSlotMinMagnitude);
        BIND(SetSlotMaxMagnitude);
        BIND(GetSlotMaxMagnitude);
        BIND(SetSlotDoPeaks);
        BIND(GetSlotDoPeaks);
        BIND(GetSlotMinPeak);
        BIND(SetSlotMinPeak);
        BIND(GetSlotMaxPeak);
        BIND(SetSlotMaxPeak);

		return true;
	}
}