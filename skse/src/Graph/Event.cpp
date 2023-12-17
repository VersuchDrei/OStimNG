#include "Event.h"

namespace Graph {
    GameAPI::GameSound Event::getSound() {
        if (sound) {
            return sound;
        }
        if (supertype) {
            return supertype->getSound();
        }
        return {};
    }

    float Event::getCameraShakeDuration() {
        if (cameraShakeDuration != 0) {
            return cameraShakeDuration;
        }
        if (supertype) {
            return supertype->getCameraShakeDuration();
        }
        return 0.0f;
    }

    float Event::getCameraShakeStrength() {
        if (cameraShakeStrength != 0) {
            return cameraShakeStrength;
        }
        if (supertype) {
            return supertype->getCameraShakeStrength();
        }
        return 0.0f;
    }

    float Event::getControllerRumbleDuration() {
        if (controllerRumbleDuration != 0) {
            return controllerRumbleDuration;
        }
        if (supertype) {
            return supertype->getControllerRumbleDuration();
        }
        return 0.0f;
    }

    float Event::getControllerRumbleStrength() {
        if (controllerRumbleStrength != 0) {
            return controllerRumbleStrength;
        }
        if (supertype) {
            return supertype->getControllerRumbleStrength();
        }
        return 0.0f;
    }


    float Event::getActorStimulation() {
        if (actor.stimulation != 0) {
            return actor.stimulation;
        }
        if (supertype) {
            return supertype->getActorStimulation();
        }
        return 0.0f;
    }

    float Event::getActorMaxStimulation() {
        if (actor.maxStimulation != 100) {
            return actor.stimulation;
        }
        if (supertype) {
            return supertype->getActorMaxStimulation();
        }
        return 100.0f;
    }

    float Event::getTargetStimulation() {
        if (target.stimulation != 0) {
            return target.stimulation;
        }
        if (supertype) {
            return supertype->getTargetStimulation();
        }
        return 0.0f;
    }

    float Event::getTargetMaxStimulation() {
        if (target.maxStimulation != 100) {
            return target.stimulation;
        }
        if (supertype) {
            return supertype->getTargetMaxStimulation();
        }
        return 100.0f;
    }

    float Event::getPerformerStimulation() {
        if (performer.stimulation != 0) {
            return performer.stimulation;
        }
        if (supertype) {
            return supertype->getPerformerStimulation();
        }
        return 0.0f;
    }

    float Event::getPerformerMaxStimulation() {
        if (performer.maxStimulation != 100) {
            return performer.stimulation;
        }
        if (supertype) {
            return supertype->getPerformerMaxStimulation();
        }
        return 100.0f;
    }


    float Event::getActorReactionDelay() {
        if (actor.reactionDelay != 0) {
            return actor.reactionDelay;
        }
        if (supertype) {
            return supertype->getActorReactionDelay();
        }
        return 0.0f;
    }

    float Event::getTargetReactionDelay() {
        if (target.reactionDelay != 0) {
            return target.reactionDelay;
        }
        if (supertype) {
            return supertype->getTargetReactionDelay();
        }
        return 0.0f;
    }

    float Event::getPerformerReactionDelay() {
        if (performer.reactionDelay != 0) {
            return performer.reactionDelay;
        }
        if (supertype) {
            return supertype->getPerformerReactionDelay();
        }
        return 0.0f;
    }


    bool Event::isChildOf(Event* other) {
        if (this == other) {
            return true;
        }
        if (supertype) {
            return supertype->isChildOf(other);
        }
        return false;
    }
}