Facial expressions are the expressions actors make during scenes. (duh)

there are sadly a few things with similar names here because I didn't think properly about it when I first created it :c (sorry)
so when it comes to the term "actor" there are two things it can mean depending on context
1) a Skyrim actor (as in the reference to the PC or an NPC)
2) an action actor, this refers to the actor property of a record of the "actions" list in the scene file (which is just an int)


Expressions are split up into four layers.
1) The underlying expression, this is the basic expression actors make which is based on an animations metadata defined.
2) The looking override, this can be defined in an animations metadata to make the actors look into a certain diretion.
3) The event expression, a short term expression override which is for example used to open an actors mouth when they moan.
4) The override expression, this is used to open an actors mouth in oral actions and is usually defined in an actions "expressionOverride" field.

Higher layers overwrite lower layers, but only in the slots they're using
The slots are:
- the emotional expression
- the phonemes (mouth)
- eyeball modifiers (as in looking direction)
- eyelid modifiers (as in closing the eyes)
- eyebrow modifiers
So for example if a moan expression only contains phonemes and eyelid modifiers the emotional expression and eyeball and brow modifiers of the underlying expression will still be used.


Expressions will be chosen on a variety of conditions. OStim will randomly select a new one every few seconds (duration is based on MCM settings).

For the underlying expression the list of actions of an animation will be parsed from top to bottom.
It will always check the actor against the actions target first and then agains the actions actor, until one is found that has expressions defined.
(this is usually the first one unless you use custom actions)

For event expressions (moaning, climax or script caused events) an expression for this event will be used.

For override expressions (open mouth / tongue for oral actions) an expression for the set defined in the actions .json file will be used.
One exception is the "default" set, which is the expression that is used if no other expression qualifies for the actor.


field summary:

expression:
"actionActors" (list<string>): a list of actions that cause this expression if the Skyrim actor is the action actor
"actionTargets" (list<string>): a list of actions that cause this expression if the Skyrim actor is the action target
"events" (list<string>): a list of events that cause this expression
	vanilla events are "moan" and "climax"
"sets" (list<string>): a list of sets that contain this expression
	vanilla sets are "openmouth", "tongue" and "default"
"female" (object): the expression for a female Skyrim actor (see gender expression)
"male" (object"): the expression for a male Skyrim actor (see gender expression)

gender expression:
"duration" (float): the duration in seconds this expression will stay when used as an event expression, does not affect underlying or override expressions
"expression" (object): properties of the emotional expression to make (see properties)
"modifiers" (list<object>): a list of properties of eye modifiers to make (see properties)
"phonemes" (list<object>): a list of properties of mouth phonemes to make (see properties)
"phonemeObjecs" (list<string>): a list of equip object types to equip when playing this expressions phonemes
	these will get unequipped if another expression overwrites the phonemes and equipped again when the override ends
"phonemeObjectThreshold" (float): an excitement threshold for the phoneme objects, if set phoneme objects will only be equipped if the actor has at least this amount of excitement

properties:
"type" (int): the index of the expression/modifier/phoneme type
"baseValue" (float): the base strength of the expression (expression strength after all modifiers will be clamped to 0 to 100)
"variance" (float): a variance for the base value (can not be negative, instead use a lower baseValue and a positive variance to counter it)
	this can be used to make the expression not always look the same
	the expression strength will be (a random number between (baseValue) and (baseValue + variance) + (speed and expression multipliers)
"speedMultiplier" (float): a multiplier for the animation speed to be added to the base value (can be negative)
"excitementMultiplier" (float): a multiplier for the actors excitement to be added to the base value (can be negative)
"faction" (object): a faction whose faction rank to add to the base value
"factionFallback" (int): a fallback value for when the actor is not a member of the faction
	if no faction was provided this value is ignored
"delay" (float): a delay in seconds before this expression/modifier/phoneme will be applied
	this can be used to not make the entire face move at once, so it doesn't look like the actor is just changing from one expression to another (even though they technically are)
"delayVariance" (float): a variance for the delay which will get randomized 
	this can be used to make the expression change not always look the same and make the actor look even more life like
	the delay will be a random number between (delay) and (delay + delay variance)

form fields:
"mod" (string): the name of the mod this form is defined in, including the file extension
"formid" (string): the form id of the form in hexadecimal notation
	note: json doesn't support hexadecimal notation, which is why this is a string and not an int


F.A.Q.

Q: Where can I see which index corresponds to which expression?
A: You can look it up in this steam guide: https://steamcommunity.com/sharedfiles/filedetails/?l=german&id=187155077