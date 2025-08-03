Voice sets define what sounds are made by an actor during a scene.

OStim will always first check if there is a voice set for the actor base. If there isn't it will check for the voice type.
If that fails too it will use the default voice set for either males for females depending on the actors sex.

Since OStim plays the sounds from an SKSE plugin it can play them from sound descriptors directly, there is no need to create sound markers.

If the actor is assigned a different voice in the MCM and/or is the player the following restrictions apply to dialogue:
- only the first response will be played (so optimally just don't make dialogue with multiple responses)
- the emotion defined in the Topic Info doesn't apply (so all your emotions should be defined in the expression .jsons rather than the CK)

field summary:

voice set:
"name" ($string): the name of the voice set, if none is set the file name will be taken, this name is displayed in the MCM when selecting voice sets
	$ the content of this field will be checked against translation files in data/Interface/translations
"target" (object): the target for this voice set, can be either an actor base or a voice type (see form)
	There are two exceptions, which are the form ids 0 and 1 for the default voice set for males and females, respectively
"aliases" (list<object>): a list of additional targets to be handled by this voice set (see form)
"voice" (object): the voice type with which dialogue of this voice set will be spoken
	if none is provided it will be derived from the target form (which should be good enough for 99% of cases)
	you most likely won't need to provide this, it is only necessary in rare edge cases
"moan" (object): the sounds and dialogues to play randomly during scenes (see reaction set)
"climax" (object): the sounds and dialogues to play when this actor climaxes (see raction set)
"climaxCommentSelf" (object): the sounds and dialogues to play after this actor climaxed, this gets played after the climax (see reaction set)
"climaxCommentOther" (object): the sounds and dialogues to play after another actor climaxed (see reaction set)
"eventActorReactions" (map<string, object>): a map of sounds and dialogues to play after events where this actor was the event actor (see reaction set)
"eventTargetReactions" (map<string, object>): a map of sounds and dialogues to play after events where this actor was the event target (see reaction set)
"eventPerformerReactions" (map<string, object>): a map of sounds and dialogues to play after events where this actor was the event reactions (see reaction set)
"postSceneDialogue" (object): the topic to play after the scene ended (see form)

form:
"mod" (string): the name of the mod this form is defined in, including the file extension
"formid" (string): the form id of the form in hexadecimal notation
	note: json doesn't support hexadecimal notation, which is why this is a string and not an int

reaction set:
"sound" (list<object>): the sounds to play in between dialogues (see sound set)
"soundMuffled" (list<object>): the sounds to play if the actor is muffled, this will replace regular sounds (see sound set)
"dialogue" (list<object>): the dialogues to play (see dialogue set)

sound set:
"condition" (object): the perk which condition functions evaluate if this sound can play
	if left empty this sound set will always play
"expression" (string): the expression set to play while playing the sound (optional) (see expressions README)
"sound": (object): the sound descriptor to play (see form)
"moanIntervalOverride" (float): overrides the moan interval from the players MCM (optional)

dialogue set:
"condition" (object): the perk which condition functions evaluate if this dialogue can play
"expression" (string): the expression set to play while playing the dialogue (optional) (see expressions README)
"dialogue": (object): the topic to play (see form)
"moanIntervalOverride" (float): overrides the moan interval from the players MCM (optional)
"dialogueThreshold" (int): the threshold for the dialogue countdown, higher numbers means dialogue plays more often, modifies the players MCM option (optional)