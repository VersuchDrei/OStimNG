;/* OActor
* * collection of methods to modify scene actors
* * all of these only affect actors that are currently in a scene
* * if you pass them an actor that is not in a scene nothing will happen
* * (except for a few functions where it is explicitly stated that they work on actors that are not in a scene)
*/;
ScriptName OActor

; ███████╗██╗  ██╗ ██████╗██╗████████╗███████╗███╗   ███╗███████╗███╗   ██╗████████╗
; ██╔════╝╚██╗██╔╝██╔════╝██║╚══██╔══╝██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝
; █████╗   ╚███╔╝ ██║     ██║   ██║   █████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║
; ██╔══╝   ██╔██╗ ██║     ██║   ██║   ██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║
; ███████╗██╔╝ ██╗╚██████╗██║   ██║   ███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║
; ╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝   ╚═╝   ╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝

;/* GetExcitement
* * returns the actors current excitement level
* *
* * @param: Act, the actor to get the excitement for
* *
* * @return: the current excitement of the actor
*/;
float Function GetExcitement(Actor Act) Global Native

;/* SetExcitement
* * sets the excitement of an actor
* * values less than 0 will round up to 0, values greater than 100 will round down to 100
* *
* * @param: Act, the actor to set the excitement for
* * @param: Excitement, the value to set the excitement to
*/;
Function SetExcitement(Actor Act, float Excitement) Global Native

;/* ModifyExcitement
* * modifies the excitement of the actor by the given value
* * if the result is less than 0 it will round up to 0, if it is greater than 100 it will round down to 100
* *
* * @param: Act, the actor to modify the excitement for
* * @param: Excitement, the value to modify the excitement by, negative values reduce excitement
* * @param: RespectMultiplier, if true the passed value will be multiplied by the actors excitement multiplier set in the MCM before being applied
*/;
Function ModifyExcitement(Actor Act, float Excitement, bool RespectMultiplier = false) Global Native

;/* Climax
* * causes the actor to have a climax
* *
* * @param: Act, the actor that should have the climax
* * @param: ClimaxAnimation, if true the actor will play a climax animation if the animation they are in has one
*/;
Function Climax(Actor Act, bool ClimaxAnimation = true) Global Native

;/* GetTimesClimaxed
* * returns the amount of climaxes the actor had in the current scene
* *
* * @param: Act, the actor to get the climax amount for
* *
* * @return: the amount of climaxes the actor had
*/;
int Function GetTimesClimaxed(Actor Act) Global Native


; ███████╗██╗  ██╗██████╗ ██████╗ ███████╗███████╗███████╗██╗ ██████╗ ███╗   ██╗███████╗
; ██╔════╝╚██╗██╔╝██╔══██╗██╔══██╗██╔════╝██╔════╝██╔════╝██║██╔═══██╗████╗  ██║██╔════╝
; █████╗   ╚███╔╝ ██████╔╝██████╔╝█████╗  ███████╗███████╗██║██║   ██║██╔██╗ ██║███████╗
; ██╔══╝   ██╔██╗ ██╔═══╝ ██╔══██╗██╔══╝  ╚════██║╚════██║██║██║   ██║██║╚██╗██║╚════██║
; ███████╗██╔╝ ██╗██║     ██║  ██║███████╗███████║███████║██║╚██████╔╝██║ ╚████║███████║
; ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

;/* PlayExpression
* * plays the facial expression event on the actor
* * this does not automatically reset the face again, for that you need to call ClearExpression
* *
* * @param: Act, the actor to play the expression on
* * @param: Expression, the name of the expression event to play
* *
* * @return: the duration defined in the events json file
*/;
float Function PlayExpression(Actor Act, string Expression) Global Native

;/* ClearExpression
* * resets the factial expression to the underlying expression based on the scenes actions, clearing all event expressions
* *
* * @param: Act, the actor to update the expression for
*/;
Function ClearExpression(Actor Act) Global Native

;/* HasExpressionOverride
* * checks if the actor has an expression override (e.g. is performing an oral action or has their mouth otherwise open)
* *
* * @param: Act, the actor to check for
* *
* * @return: true if the actors expression is overridden
*/;
bool Function HasExpressionOverride(Actor Act) Global Native


; ███████╗ ██████╗ ██╗   ██╗███╗   ██╗██████╗
; ██╔════╝██╔═══██╗██║   ██║████╗  ██║██╔══██╗
; ███████╗██║   ██║██║   ██║██╔██╗ ██║██║  ██║
; ╚════██║██║   ██║██║   ██║██║╚██╗██║██║  ██║
; ███████║╚██████╔╝╚██████╔╝██║ ╚████║██████╔╝
; ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝╚═════╝

;/* Mute
* * mutes an actor, preventing them from moaning and talking
* *
* * @param: Act, the actor to mute
*/;
Function Mute(Actor Act) Global Native

;/* Unmute
* * unmutes an actor, enabling them to moan and talk again
* *
* * @param: Act, the actor to unmute
*/;
Function Unmute(Actor Act) Global Native

;/* IsMuted
* * checks if an actor is muted
* *
* * @param: Act, the actor to check
* *
* * @return: true if the actor is muted, otherwise false
*/;
bool Function IsMuted(Actor Act) Global Native


; ██╗   ██╗███╗   ██╗██████╗ ██████╗ ███████╗███████╗███████╗██╗███╗   ██╗ ██████╗ 
; ██║   ██║████╗  ██║██╔══██╗██╔══██╗██╔════╝██╔════╝██╔════╝██║████╗  ██║██╔════╝ 
; ██║   ██║██╔██╗ ██║██║  ██║██████╔╝█████╗  ███████╗███████╗██║██╔██╗ ██║██║  ███╗
; ██║   ██║██║╚██╗██║██║  ██║██╔══██╗██╔══╝  ╚════██║╚════██║██║██║╚██╗██║██║   ██║
; ╚██████╔╝██║ ╚████║██████╔╝██║  ██║███████╗███████║███████║██║██║ ╚████║╚██████╔╝
;  ╚═════╝ ╚═╝  ╚═══╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚═╝╚═╝  ╚═══╝ ╚═════╝ 

;/* Undress
* * fully undresses the actor
* *
* * @param: Act, the actor to undress
*/;
Function Undress(Actor Act) Global Native

;/* Redress
* * redresses all items that were undressed during the current scene
* * 
* * @param: Act, the actor to redress
*/;
Function Redress(Actor Act) Global Native

;/* UndressPartial
* * undresses all items on the actor that overlap with the given slot mask
* * for slot mask values refer to: https://www.creationkit.com/index.php?title=Slot_Masks_-_Armor
* * 
* * @param: Act, the actor to undress
* * @param: Mask, the slot mask to check the items against
*/;
Function UndressPartial(Actor Act, int Mask) Global Native

;/* RedressPartial
* * redresses all items that were undressed during the current scene and overlap with the given slot mask
* * for slot mask values refer to: https://www.creationkit.com/index.php?title=Slot_Masks_-_Armor
* *
* * @param: Act, the actor to redress
* * @param: Mask, the slot mask to check the items against
*/;
Function RedressPartial(Actor Act, int Mask) Global Native

;/* RemoveWeapons
* * removes the weapons
* *
* * @param: Act, the actor to remove the weapons from
*/;
Function RemoveWeapons(Actor Act) Global Native

;/* AddWeapons
* * adds back the weapons that were removed during the current scene
* *
* * @param: Act, the actor to add the weapons to
*/;
Function AddWeapons(Actor Act) Global Native


; ███████╗ ██████╗ ██╗   ██╗██╗██████╗      ██████╗ ██████╗      ██╗███████╗ ██████╗████████╗███████╗
; ██╔════╝██╔═══██╗██║   ██║██║██╔══██╗    ██╔═══██╗██╔══██╗     ██║██╔════╝██╔════╝╚══██╔══╝██╔════╝
; █████╗  ██║   ██║██║   ██║██║██████╔╝    ██║   ██║██████╔╝     ██║█████╗  ██║        ██║   ███████╗
; ██╔══╝  ██║▄▄ ██║██║   ██║██║██╔═══╝     ██║   ██║██╔══██╗██   ██║██╔══╝  ██║        ██║   ╚════██║
; ███████╗╚██████╔╝╚██████╔╝██║██║         ╚██████╔╝██████╔╝╚█████╔╝███████╗╚██████╗   ██║   ███████║
; ╚══════╝ ╚══▀▀═╝  ╚═════╝ ╚═╝╚═╝          ╚═════╝ ╚═════╝  ╚════╝ ╚══════╝ ╚═════╝   ╚═╝   ╚══════╝

; Equip objects refer to OStims version of anim objects. Some are equipped automatically by OStim.
; They can also be equipped with the below functions or by annotations.
; You can define your own by adding .json files to "SKSE/plugins/OStim/equip objects"
; Equip objects will be automatically removed at scene end if not removed manually.

;/* EquipObject
* * equips an object on an actor
* *
* * @param: Act, the actor to equip the object on
* * @param: Type, the type of the object to equip
* *
* * @return: true if the object was equipped, false if the object type is unknown
*/;
bool Function EquipObject(Actor Act, string Type) Global Native

;/* UnequipObject
* * unequips an object on from actor
* *
* * @param: Act, the actor to unequip the item from
* * @param: Type, the type of the object to unequip
*/;
Function UnequipObject(Actor Act, string Type) Global Native

;/* IsObjectEquipped
* * checks if an object type is currently equipped on an actor
* *
* * @param: Act, the actor to check on
* * @param: Type, the type of the object to check for
* *
* * @return: true if the type is currently equipped
*/;
bool Function IsObjectEquipped(Actor Act, string Type) Global Native

;/* SetObjectVariant
* * sets the variant of an object on an actor
* *
* * @param: Act, the actor to set the variant on
* * @param: Type, the type of the object to set the variant for
* * @param: Variant, the variant to set
* * @param: Duration, the duration the variant stays on, if 0.0 it will stay until removed
* *
* * @return: true if the variant was set, false if the object type is unknown or the object does not have the variant
*/;
bool Function SetObjectVariant(Actor Act, string Type, string Variant, float Duration = 0.0) Global Native

;/* UnsetObjectVariant
* * sets the objects variant back to the default one
* *
* * @param: Act, the actor to unset the variant on
* * @param: Type, the type of the object to unset the variant for
*/;
Function UnsetObjectVariant(Actor Act, string Type) Global Native


; ██╗   ██╗████████╗██╗██╗     ██╗████████╗██╗   ██╗
; ██║   ██║╚══██╔══╝██║██║     ██║╚══██╔══╝╚██╗ ██╔╝
; ██║   ██║   ██║   ██║██║     ██║   ██║    ╚████╔╝
; ██║   ██║   ██║   ██║██║     ██║   ██║     ╚██╔╝
; ╚██████╔╝   ██║   ██║███████╗██║   ██║      ██║
;  ╚═════╝    ╚═╝   ╚═╝╚══════╝╚═╝   ╚═╝      ╚═╝

;/* HasSchlong
* * checks if the actor has a schlong
* * if SoS full is not installed or use SoS gender was disabled in the MCM this will simply check for the actors sex
* * if SoS full is installed this will check for the SOS_SchlongifiedFaction
* * additionally it will check for SOS - No Futanari Schlong and SOS Female Pubic Hair
* * (to not consider those schlongified even though they are in the faction)
* *	this function even works on actors that are not in a scene
* * 
* * @param: Act, the actor to check
* *
* * @return: true if the actor has a schlong, otherwise false
*/;
bool Function HasSchlong(Actor Act) Global Native

;/* SortActors
* * sorts all actors with schlongs to the front of the array and all actors without schlongs to the end
* * other than this the order is not altered (i.e. the sorting algorithm is stable)
* * this function even works on actors that are not in a scene
* *
* * @param: Actors, the array of actors to sort
* * @param: PlayerIndex, if given the player will be sorted to this index, independent of them having a schlong or not
* *
* * @return: the sorted array 
*/;
Actor[] Function SortActors(Actor[] Actors, int PlayerIndex = -1) Global Native


; ██████╗ ███████╗██████╗ ██████╗ ███████╗ ██████╗ █████╗ ████████╗███████╗██████╗ 
; ██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗╚══██╔══╝██╔════╝██╔══██╗
; ██║  ██║█████╗  ██████╔╝██████╔╝█████╗  ██║     ███████║   ██║   █████╗  ██║  ██║
; ██║  ██║██╔══╝  ██╔═══╝ ██╔══██╗██╔══╝  ██║     ██╔══██║   ██║   ██╔══╝  ██║  ██║
; ██████╔╝███████╗██║     ██║  ██║███████╗╚██████╗██║  ██║   ██║   ███████╗██████╔╝
; ╚═════╝ ╚══════╝╚═╝     ╚═╝  ╚═╝╚══════╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═════╝ 

; all of these are only here to not break old addons, don't use them in new addons, use whatever they're calling instead

Function UpdateExpression(Actor Act) Global
	ClearExpression(Act)
EndFunction