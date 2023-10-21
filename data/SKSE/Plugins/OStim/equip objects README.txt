Equip objects are armor items that can be equipped on actors in OStim scenes.
OStim will handle equipping and unequipping of these objects.
So you do not have to worry about removing them in the ostim_end event, as OStim will automaically remove all equip objects at scene end.

Equip objects should be marked as non playable and not otherwise used by any mod to not create any conflicts.


field summary:

equip object:
"type" (string): the type of the equip object, types used by OStim are "strapon" and "tongue", but addons can equip any custom type you want
"name" (string): the name of the equip object, this is the name that will be displayed in the MCM when selecting equip objects
"default" (bool): if true this equip object will be considered the default one for all actors that don't have anything else defined
	if no default object exists the default is random
	if more than one default object exists they will overwrite each other and lead to unpredictable behavior
"mod" (string): the file name of the mod the armor item is defined in, including file extension
"formid" (string): the form id of the armor item in hexadecimal notation, excluding the load order id (leading zeroes are optional)
"variants" (map<string, object>): a map of variants for this equip objects, with the keys being the variants ids (see variant)
	note: variant names are converted to lowercase when parsed, so capitalization does not matter

variant:
"mod" (string): the file name of the mod the armor item is defined in, including file extension
"formid" (string): the form id of the armor item in hexadecimal notation, excluding the load order id (leading zeroes are optional)


F.A.Q.

Q: How can I equip an equip object in my addon?
A: Equip objects can be equipped and unequipped in the OActor script. See OActor.psc for documentation.

Q: How can I change an equip objects variant in my addon?
A: Equip object variants can be set in the OActor script. See OActor.psc for documentation.