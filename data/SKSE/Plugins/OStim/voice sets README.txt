Voice sets define what sounds are made by an actor during a scene.

OStim will always first check if there is a voice set for the actor base. If there isn't it will check for the voice type.
If that fails too it will use the default voice set for either males for females depending on the actors sex.

Since OStim plays the sounds from an SKSE plugin it can play them from sound descriptors directly, there is no need to create sound markers.

field summary:

voice set:
"target" (object): the target for this voice set, can be either an actor base or a voice type (see form)
	There are two exceptions, which are the form ids 0 and 1 for the default voice set for males and females, respectively
"moan" (object): the sound descriptor of the moaning sound (see form)
"moanMuffled" (object): the sound descriptor of the muffled moaning sound (see form)
	the muffled moaning sound is used instead of the regular one when an actor is muffled (this usually happens when they are performing oral actions)
"moanExpression" (string): the event expression the actor makes when moaning (by default "moan") (see facial expressions README on how event expressions work)
"climax" (object): the sound descriptor of the climax sound (see form)
"climaxMuffled" (object) the sound descriptor for the muffled climax sound (see form)
	the muffled climax sound is used instead of the regular one when an actor is muffled (this usually happens when they are performing oral actions)
"climaxExpression" (string): the event expression the actor makes when climaxing (by default "climax") (see facial expressions README on how event expressions work)

form:
"mod" (string): the name of the mod this form is defined in, including the file extension
"formid" (string): the form id of the form in hexadecimal notation
	note: json doesn't support hexadecimal notation, which is why this is a string and not an int