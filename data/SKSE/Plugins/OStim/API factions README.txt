During a scene actors are added to a lot of different factions.
These can be used to condition functions for various purposes.

The following factions are hard coded into OStim:

OStimActorCountFaction
the faction rank is equal to the amount of actors in the scene

OStimExcitementFaction
the faction rank is equal to the actors current excitement (rounded down)

OStimSchlongifiedFaction
if the actor is in the faction they have a penis (so are either male or a futa)
if the actor is not in the faction they do not have a penis
supports both SoS and TNG
does not work for actors that aren't currently in a scene

OStimTimesClimaxedFaction
the faction rank is equal to the amount of times the actor had an orgasm in the current scene
will reset after scene, so this is not an indicator for how many climaxes the actor had in the entire playthrough

OStimTImeUntilClimaxFaction
the faction rank is equal to the amount of seconds it will take this actor to reach an orgasm at their current stimulation increase (rounded down)
if the actor takes more than 100 seconds to reach the climax the faction rank will be 101, it does not go higher than that
if the actor cannot reach a climax in the current scene (either because they are not getting excitement or they can't go past a certain threshold) the faction rank will be -1



in addition to that actions can have factions defined their .json files to indicate if the actor is current the actor, target and/or performer of that action
to see which actions put them in which factions have a look at the action .json files