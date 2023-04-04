;/* OActor
* * collection of methods to modify scene actors
* * all of these only affect actors that are currently in a scene
* * if you pass them an actor that is not in a scene nothing will happen
*/;
ScriptName OActor

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