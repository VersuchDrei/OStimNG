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


    float Event::getActorStimulation(GameAPI::GameActor actor) {
        if (this->actor.stimulationFaction && this->actor.stimulationFaction.contains(actor)) {
            return this->actor.stimulationFaction.getRank(actor) * 0.25f;
        }
        if (this->actor.stimulation != 0) {
            return this->actor.stimulation;
        }
        if (supertype) {
            return supertype->getActorStimulation(actor);
        }
        return 0.0f;
    }

    float Event::getActorMaxStimulation(GameAPI::GameActor actor) {
        if (this->actor.maxStimulationFaction && this->actor.maxStimulationFaction.contains(actor)) {
            return this->actor.maxStimulationFaction.getRank(actor);
        }
        if (this->actor.maxStimulation != 100) {
            return this->actor.stimulation;
        }
        if (supertype) {
            return supertype->getActorMaxStimulation(actor);
        }
        return 100.0f;
    }

    float Event::getTargetStimulation(GameAPI::GameActor actor) {
        if (target.stimulationFaction && target.stimulationFaction.contains(actor)) {
            return target.stimulationFaction.getRank(actor) * 0.25f;
        }
        if (target.stimulation != 0) {
            return target.stimulation;
        }
        if (supertype) {
            return supertype->getTargetStimulation(actor);
        }
        return 0.0f;
    }

    float Event::getTargetMaxStimulation(GameAPI::GameActor actor) {
        if (target.maxStimulationFaction && target.maxStimulationFaction.contains(actor)) {
            return target.maxStimulationFaction.getRank(actor);
        }
        if (target.maxStimulation != 100) {
            return target.stimulation;
        }
        if (supertype) {
            return supertype->getTargetMaxStimulation(actor);
        }
        return 100.0f;
    }

    float Event::getPerformerStimulation(GameAPI::GameActor actor) {
        if (performer.stimulationFaction && performer.stimulationFaction.contains(actor)) {
            return performer.stimulationFaction.getRank(actor) * 0.25f;
        }
        if (performer.stimulation != 0) {
            return performer.stimulation;
        }
        if (supertype) {
            return supertype->getPerformerStimulation(actor);
        }
        return 0.0f;
    }

    float Event::getPerformerMaxStimulation(GameAPI::GameActor actor) {
        if (performer.maxStimulationFaction && performer.maxStimulationFaction.contains(actor)) {
            return performer.maxStimulationFaction.getRank(actor);
        }
        if (performer.maxStimulation != 100) {
            return performer.stimulation;
        }
        if (supertype) {
            return supertype->getPerformerMaxStimulation(actor);
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