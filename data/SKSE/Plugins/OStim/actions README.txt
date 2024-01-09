Actions are what's used by animation metadata to describe what is going on.
Actions themselves can define a lot of properties for the actors involved.

there are sadly a few things with similar names here because I didn't think properly about it when I first created it :c (sorry)
so when it comes to the term "actor" there are two things it can mean depending on context
1) a Skyrim actor (as in the reference to the PC or an NPC)
2) an action actor, this refers to the actor property of a record of the "actions" list in the scene file (which is just an int)


field summary:
note: all string values are converted to lower case when parsed, so capitalization does not matter

action fields:
"info" (string): this field does not get parsed, it's just there to help understanding how to properly use this action
"aliases" (list<string>): a list of aliases for this action, using an alias in a scene file will result in it being treated as if this action was used
"actor" (object): a collection of attributes for the action actor (see actor fields)
"target" (object): a collection of attributes for the action target (see actor fields)
"performer" (object): a collection of attributes for the action performer (see actor fields)
"sounds" (list<object>): a list of sounds to play during this action (see sound fields)
"tags" (list<string>): a list of tags for this action, commonly used are "oral", playful", "seductive", "sensual" and "sexual"
	these can be used by addons to filter actions

actor fields:
"info" (string): this field does not get parsed, it's just there to help understanding how to properly use this action
"stimulation" (float): the amount of stimulation this action generates every second for this Skyrim actor
	in an animation with multiple stimulating actions the Skyrim actor will get stimulation equal to the highest stimulation defined for them plus 10% of all other stimulation defined for them
"maxStimulation" (float): a stimulation threshold after which this action will no longer generate any stimulation
	this for example can be used by actions like kissing to prevent the Skyrim actors climaxing from kissing alone
"stimulationFaction" (object): a faction that the actor can be put in to modify the stimulation they receive from this action (see form fields)
"maxStimulationFaction" (object): a faction that the actor can be put in to modify the maximum stimulation they can receive from this action (see form fields)
"fullStrip" (bool): When true executing this action will fully strip this Skyrim actor (if fully undress mid scene is enabled in the MCM)
"moan" (bool): When true the Skyrim actor will do moaning sounds when this action is played.
"talk" (bool): When true the Skyrim actor will talk when this action is played.
	This requires add-ons to add dialogue for this actor as OStim does not come with any dialogue on its own.
"muffled" (bool): When true the Skyirm actor will only do muffled sounds when moaning and do no talking.
	this is mainly used for when an actor has their mouth full (usually by performing an oral action)
"expressionOverride" (string): when set executing this action will override this Skyrim actors facial expression with one for this set (see facial expression README for how to define an expression set)
	this can be used to open the mouth and/or stick out the tongue for oral actions
"requirements" (list<string>): a list of requirements for this Skyrim actor, if they are not met an animation containing this action will not show up in navigation
	possible values are "anus", "breast", "foot", "hand", "mouth", "nipple", "penis", "testicles" and "vagina"
"strippingSlots" (list<int>): a list of biped slots to strip on this Skyrim actor when executing this scene (if partial undressing is enabled in the MCM)
"faction" (object/list<object>): a faction or list of factions the actor is put in while participating in this role in this action (see form fields)
	this is mainly useful for condition functions for addons / voice sets
"ints" (map<string, int>): A map of custom ints that can be used by addons
"intLists" (map<string, list<int>>): a map of custom int lists that can be used by addons
"floats" (map<string, float>): a map of custom floats that can be used by addons
	known ones are: "stamina" for the actions stamina cost for OEndurance
"floatLists" (map<string, list<float>>): a map of custom float lists that can be used by addons
"strings" (map<string, string>): a map of custom strings that can be used by addons
"stringLists" (map<string, list<string>>): a map of custom string lists that can be used by addons
	known ones are: "cum" for the cum overlay slots for OCum

form fields:
"mod" (string): the name of the mod this form is defined in, including the file extension
"formid" (string): the form id of the form in hexadecimal notation
	note: json doesn't support hexadecimal notation, which is why this is a string and not an int

sound fields:
"type" (string): the type of the sound, depending on type additional fields may be required (see sound types)
"sound" (object): the sound descriptor of the sound (see form fields)
"muteWithActor" (bool): when set to true the sound will not play while the actor is muted
"muteWithTarget" (bool): when set to true the sound will not play while the target is muted
"playerThreadOnly" (bool): when set to true this sound will not play in NPCxNPC threads

sound types:

"bonedistance":
	the sound will be played whenever the given bones are closest to each other
"actorBone" (string/list<string>): the bone / list of bones of the actor to check the distance against
"targetBone" (string/list<string>): the bone / list of bones of the target to check the distance against
"inverse" (bool): if true the sound will be played when the bones are furthest apart rather than closest together
"minInterval" (int): if set the amount of miliseconds that two sound triggers will need to be apart in order for the sound to be played
	this can be used to not play the sound at fast speeds
"maxInterval" (int): if set the amount of miliseconds that the time between two triggers cannot exceed in order for the sound to be played
	this can be used to not play the sound at slow speeds

"loop":
	the sound will be played on a loop
"delay" (int): the time in milliseconds before a new sound is played
	this timer starts after the previous sound is done playing, not when it starts playing


F.A.Q.

Q: What are action actor, target and performer?
A: Action actor and target are the Skyrim actors involved in this action. Action performer is the one currently performing the action.

Q: Why are action actor and performer not the same thing?
A: For the sake of tag integrity action actor and target shouldn't swap depending on who is actively performing.
For example for classic missionary sex the male is the action actor and the female is the target.
In a cowgirl animation the female is usually performing, but if we make her the actor of the action it is hard for addons to figure out who is penetrating who.
So in such a case the male is still going to be the action actor and the female the target, but now the female is also the performer.
Who is action actor and who is target should be designed around consensual vanilla sex. The one with the penis is not automatically always the action actor.
So for vaginalsex the male is the action actor and female the target and for blowjob the female is the actor and male is the target, etc.

Q: How can addons access the custom actor data defined in "ints", "intlists" etc.?
A: All of those can be accessed with a variety of functions on the OMetadata script. See OMetadata.psc for documentation on those functions.