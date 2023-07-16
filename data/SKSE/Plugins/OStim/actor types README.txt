Actor types are used to limit which animations an actor can play.
For example can you create an actor type "armbinder" for actors that are waring armbinders.
If you then add the same actor type to animations actors of that type can only play those animations.

Actor types are "abusing" perks to check for conditions. OStim is using the perks condition functions to check if the actor is of that type.
Actors do not need to actually have the perk in order for that to work. The perk just has to exist.
If more than one actor type applies for an actor the one where the perk has the higher value in the level field will be taken.
If both have the same value in the level field behavior is undefined.

field summary:
"condition" (object): the perk that is used as condition for the actor type (see form fields)

form fields:
"mod" (string): the name of the mod this form is defined in, including the file extension
"formid" (string): the form id of the form in hexadecimal notation
	note: json doesn't support hexadecimal notation, which is why this is a string and not an int