Events can be fired by annotations or Papyrus scripts to modify threads and listened to by Papyrus scripts.
Events themselves can define a lot of properties for the actors involved.

there are sadly a few things with similar names here because I didn't think properly about it when I first created it :c (sorry)
so when it comes to the term "actor" there are two things it can mean depending on context
1) a Skyrim actor (as in the reference to the PC or an NPC)
2) an event actor, this refers to the actor property of an event that is fired


field summary:
note: all string values are converted to lower case when parsed, so capitalization does not matter

event fields:
"actor" (object): a collection of attributes for the event actor (see actor fields)
"target" (object): a collection of attributes for the event target (see actor fields)
"performer" (object): a collection of attributes for the event performer (see actor fields)
"sound" (object): the sound descriptor of the sound that plays whenever this event gets fired (see form)
"cameraShakeStrength" (float): if set the camera shakes this strong when the event is called (only accepts values between 0 and 1)
"cameraShakeDuration" (float): if set the camera shakes for this long when the event is called
"controllerRumbleStrength" (float): if set the controller rubles this strong when the event is called (only accepts calues between 0 and 1) 
"controllerRumbleDuration" (float): if set the controller rumbles for this long when the event is called
"tags" (list<string>): a list of tags for this event, commonly used are "playful", "sensual" and "sexual"
	these can be used by addons to filter events

actor fields:
"stimulation" (float): the amount of stimulation this event generates for this Skyrim actor
"maxStimulation" (float): a stimulation threshold after which this event will no longer generate any stimulation
	this for example can be used by events like spanking to prevent the Skyrim actors climaxing from spanking alone
"stimulationFaction" (object): a faction that the actor can be put in to modify the stimulation they receive from this event (see form fields)
"maxStimulationFaction" (object): a faction that the actor can be put in to modify the maximum stimulation they can receive from this event (see form fields)

form:
"mod" (string): the name of the mod this form is defined in, including the file extension
"formid" (string): the form id of the form in hexadecimal notation
	note: json doesn't support hexadecimal notation, which is why this is a string and not an int