Actor properties are used to modify OStims behavior based on the actors involved.

Actor properties are "abusing" perks to check for conditions. OStim is using the perks condition functions to check if the actor qualifies.
Actors do not need to actually have the perk in order for that to work. The perk just has to exist.
If more than one actor property of the same type qualifies for an actor the one where the perk has the higher value in the level field will be taken.
If both have the same value in the level field behavior is undefined.
field summary:
"condition" (object): the perk that is used as condition for the actor properties (see form fields)
"type" (string): the type of the actor, actor types are used to limit which animations an actor can play
	for example can you create an actor type "armbinder" for actors that are waring armbinders
	if you then add the same actor type to animations actors of that type can only play those animations
"skeleton" (float): the height of the actors skeleton, this will be used for scale calculations
	WARNING: this is not yet implemented, it is just listed to explain why one of the vanilla files contains this field
	this is only useful for custom races with custom skeletons, so you most likely won't need it
"expression" (string): an expression override to use during the entire scene
	this expression override overrules any other expressions
	this can for example be used to keep an actors mouth open during OStim scenes if they are wearing a gag
"mute" (bool): if true the actor won't make any sounds for the entire scene
"muffle" (bool): if true the actor will only make muffled sounds for the entire scene
	this again can be used for wearing a gag
"requirements" (map<string, bool>): a list of requirements this actor fulfills / doesn't fulfill
	a higher priority requirement set to false will overwrite a lower priority set to true
	if a higher priority list doesn't containt the requirement the lower one will take place
"equipObjects" (map<string, bool>): a list of equip objects to equip / not equip on the actor
	these objects will be equipped at scene start, but it will not prevent unequipping it by annotations or Papyrus calls
	similarly setting one to false will overwrite a lower priority setting to equip it, but will not prevent later equipping of it by annotations or Papyrus calls


form fields:
"mod" (string): the name of the mod this form is defined in, including the file extension
"formid" (string): the form id of the form in hexadecimal notation
	note: json doesn't support hexadecimal notation, which is why this is a string and not an int