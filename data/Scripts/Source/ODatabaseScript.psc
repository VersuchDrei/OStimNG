; ██╗██╗██╗ ██████╗ ███████╗██████╗ ██████╗ ███████╗ ██████╗ █████╗ ████████╗███████╗██████╗  ██╗██╗██╗
; ██║██║██║ ██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗╚══██╔══╝██╔════╝██╔══██╗ ██║██║██║
; ██║██║██║ ██║  ██║█████╗  ██████╔╝██████╔╝█████╗  ██║     ███████║   ██║   █████╗  ██║  ██║ ██║██║██║
; ╚═╝╚═╝╚═╝ ██║  ██║██╔══╝  ██╔═══╝ ██╔══██╗██╔══╝  ██║     ██╔══██║   ██║   ██╔══╝  ██║  ██║ ╚═╝╚═╝╚═╝
; ██╗██╗██╗ ██████╔╝███████╗██║     ██║  ██║███████╗╚██████╗██║  ██║   ██║   ███████╗██████╔╝ ██╗██╗██╗
; ╚═╝╚═╝╚═╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝  ╚═╝╚══════╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═════╝  ╚═╝╚═╝╚═╝

;/* ODatabaseScript
* * this entire script is deprecated
* * we will remove it at some point in the future so don't use it or your mod will get broken
* * if you don't know how to do whatever it is you want to do without it ask for help in the #ostim-dev channel in the OStim NG Discord (discord.gg/ostim)
*/;
ScriptName ODatabaseScript Extends Quest

OsexIntegrationMain OStim
String[] Modules
Int Database

;odthreadmanager threadmanager

String[] property OriginalModules auto
String SavePath
Bool Loaded = False

Function InitDatabase()
	OriginalModules = new String[1]
	OriginalModules[0] = "0MF"
	OriginalModules = PapyrusUtil.PushString(OriginalModules, "0M2F")
	OriginalModules = PapyrusUtil.PushString(OriginalModules, "BB")
	OriginalModules = PapyrusUtil.PushString(OriginalModules, "BG")
	OriginalModules = PapyrusUtil.PushString(OriginalModules, "EMF")
	OriginalModules = PapyrusUtil.PushString(OriginalModules, "WZH0")

	SavePath = JContainers.UserDirectory() + "ODatabase.json"
	Loaded = False

	OStim = (Self as Quest) as OsexIntegrationMain
EndFunction

Int Function GetDatabaseOArray(); never cache this for more than a few seconds, it can be cleared from memory at any time
	If (!Loaded)
		Load()
	EndIf
	Return Database
EndFunction

Function Unload()
	JDB.SetObj("OStim.ODatabase", 0)
	JValue.Release(GetDatabaseOArray())

	;threadmanager.Destroy()

	Console("Unloaded Database")
	Loaded = False
EndFunction

Function Load()
	If (Loaded)
		Return
	EndIf

	If (JContainers.FileExistsAtPath(SavePath))
		Database = JValue.ReadFromFile(SavePath)
		JDB.SetObj("OStim.ODatabase", Database)
	EndIf

	;threadmanager.Destroy()
	;threadmanager = ODThreadManager.newobject()

	Console("Loaded Database")
	Loaded = True
EndFunction

Bool Function IsBuilt()
	Return True
EndFunction

;--------------------------------------------
;  	Retrieve Data from animation OMaps
;--------------------------------------------

String Function GetFullName(Int Animation)
	Return GetStringOMap(Animation, "name")
EndFunction

String Function GetAnimationClass(Int Animation)
	Return GetStringOMap(Animation, "animclass")
EndFunction

Int Function GetAnimationIDOArray(Int Animation) ; returns an OArray of animation IDs
	Return GetObjOMap(Animation, "OAanimids")
EndFunction

Int Function GetNumActors(Int Animation)
	Return GetIntOMap(Animation, "NumActors")
EndFunction

String Function GetPositionData(Int Animation)
	Return GetStringOMap(Animation, "positiondata")
EndFunction

String Function GetSceneID(Int Animation)
	Return GetStringOMap(Animation, "sceneid")
EndFunction

String Function GetModule(Int Animation)
	Return GetStringOMap(Animation, "sourcemodule")
EndFunction

Bool Function IsAggressive(Int Animation)
	Return GetBoolOMap(Animation, "aggressive")
EndFunction

Bool Function IsHubAnimation(Int Animation)
	Return GetBoolOMap(Animation, "ishub")
EndFunction

Bool Function IsTransitoryAnimation(Int Animation)
	Return GetBoolOMap(Animation, "istransitory")
EndFunction

Bool Function IsSexAnimation(Int Animation)
	Return (!IsHubAnimation(Animation) && !IsTransitoryAnimation(Animation))
EndFunction

Int Function GetMainActor(Int Animation) ; the main actor is the one doing the most work. 0 - dom | 1 - sub
	If (!IsSexAnimation(Animation))
		Return -1
	EndIf
	Return GetIntOMap(Animation, "mainActor")
EndFunction

Int Function GetMaxSpeed(Int Animation)
	If (!IsSexAnimation(Animation))
		Return -1
	EndIf
	Return GetIntOMap(Animation, "maxspeed")
EndFunction

Int Function GetMinSpeed(Int Animation)
	If (!IsSexAnimation(Animation))
		Return -1
	EndIf
	Return GetIntOMap(Animation, "minspeed")
EndFunction

Bool Function HasIdleSpeed(Int Animation) ; if the lowest speed on a sex animation is called "idle"
	If (!IsSexAnimation(Animation))
		Return -1
	EndIf
	Return GetBoolOMap(Animation, "hasidlespeed")
EndFunction

; extras
String Function GetPositionDataForActorSlot(Int Animation, Int Slot)
	Return StringUtil.Split(GetPositionData(Animation), "!")[Slot]
EndFunction

Bool Function HasPositionDataInSlot(Int Animation, Int Slot, String Data) ; this supports partial data, ie "Kn" at slot 0 will tell you if a dom actor is kneeling on both knees
	String slotdata = GetPositionDataForActorSlot(Animation, Slot)
	Return !(StringUtil.Find(SlotData, Data) == -1)
EndFunction

Bool Function HasPositionData(Int Animation, String Data) ; this supports partial data
	String SlotData = GetPositionData(Animation)
	Return !(StringUtil.Find(SlotData, Data) == -1)
EndFunction

;----------------------------------------------
;  	Retrieve OArray of animations from Database
;-----------------------------------------------

int function DatabaseKeyAndParameterLookup(Int zDatabase, String zKey, Int IntParam = -100, String StringParam = "", Int BoolParam = -1, Bool AllowPartialStringResult = False, Bool NegativePartial = False)
	Int Ret = JArray.object()
	if (IntParam > -100)
		ret = JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'"+zKey+"', "+IntParam+")")
		Return Ret
    elseif (StringParam != "")
        if (!AllowPartialStringResult)
			;normal
			ret = JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'"+zKey+"', '"+StringParam+"')")
		else
			if (NegativePartial)
				;negative partial
				ret = JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'"+zKey+"', '"+StringParam+"', 1, 1)")
			else
				;normal partial
				ret = JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'"+zKey+"', '"+StringParam+"', 1, 0)")
			endif
        endif
		Return Ret
    elseif (BoolParam > -1)
		ret = JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'"+zKey+"', "+BoolParam+")") ;no need to cast boolParam as int because it already is one.
		Return Ret
    endif
	Return Ret
endfunction

Int Function getAnimationWithAnimID(Int zDatabase, String AnimID) ;returns OArray
	Int Base = zDatabase
	Int Ret = NewOArray()

	Int i = 0
	Int L = GetLengthOArray(Base)

	Int Animation ;optimization
	Int OAAnimIDs
	while (i < L) ;iterate through scene objects
		Animation = GetObjectOArray(Base, i)
		OAAnimIDs = GetAnimationIDOArray(Animation)

		If JArray.findStr(OAAnimids, AnimID) != -1
			AppendObjectOArray(Ret, Animation)
			Return Ret
		EndIf

		i += 1
	endwhile
	Return Ret
EndFunction

Int Function GetAnimationsWithSpecificPositionData(Int zDatabase, String Data, Int Slot = -1, Bool Has = True)  ; this supports partial data, ie "Kn" at slot 0 will tell you if a dom actor is kneeling on both knees
	; slot -1 = search all slots
	;Has = False will give you animations that do not contain the data
	Int Base = zDatabase
	Int Ret = NewOArray()

	Int i = 0
	Int L = GetLengthOArray(base)
	While (i < L)
		Int Animation = GetObjectOArray(Base, i)
		If (Slot != -1)
			If (HasPositionDataInSlot(Animation, Slot, Data))
				If (Has)
					AppendObjectOArray(Ret, Animation)
				EndIf
			Else
				If (!Has)
					AppendObjectOArray(Ret, Animation)
				EndIf
			EndIf
		Else
			If (HasPositionData(Animation, Data))
				If (Has)
					AppendObjectOArray(Ret, Animation)
				EndIf
			Else
				If (!Has)
					AppendObjectOArray(Ret, Animation)
				EndIf
			EndIf
		EndIf
		i += 1
	EndWhile
	Return Ret
EndFunction

;-----------

Int Function GetAnimationsWithActorCount(Int zDatabase, Int Num) ; returns OArray
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'NumActors', "+Num+")")
EndFunction

Int Function GetAnimationsWithName(Int zDatabase, String Name, Bool AllowPartialResult = False, Bool Negative = False) ; returns OArray
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject, 'name', '"+Name+"', "+AllowPartialResult as int+", "+Negative as int+")")
EndFunction

Int Function GetAnimationsWithAnimationClass(Int zDatabase, String zClass) ; returns OArray
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'animclass', '"+zClass+"')")
EndFunction

Int Function GetAnimationsWithPositionData(Int zDatabase, String Pos) ; returns OArray
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'positiondata', '"+Pos+"')")
EndFunction

Int Function GetAnimationsWithSceneID(Int zDatabase, String SceneID) ; returns OArray
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'sceneid', '"+SceneID+"')")
EndFunction

Int Function GetAnimationsFromModule(Int zDatabase, String Module) ; returns OArray
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'sourcemodule', '"+Module+"')")
EndFunction

Int Function GetAnimationsByAggression(Int zDatabase, Bool IsAggressive) ; returns OArray
	Int Send = 0
	If (IsAggressive)
		Send = 1
	EndIf
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'aggressive', "+Send+")")
EndFunction

Int Function GetHubAnimations(Int zDatabase, Bool IsHub) ; returns OArray
	Int Send = 0
	If (IsHub)
		Send = 1
	EndIf
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'ishub', "+Send+")")
EndFunction

Int Function GetTransitoryAnimations(Int zDatabase, Bool IsTransitory) ; returns OArray
	Int Send = 0
	If (IsTransitory)
		Send = 1
	EndIf
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'istransitory', "+Send+")")
EndFunction

Int Function GetSexAnimations(Int zDatabase, Bool IsSex) ; returns OArray
	; returns all sexual animations, for intercourse animations only, use the animation class "Sx" as a lookup instead
	int[] merge = new int[2]
	merge[0] = GetHubAnimations(zDatabase, !issex)
	merge[1] = GetTransitoryAnimations(zDatabase, !issex)
	Return MergeOArrays(merge)
EndFunction

Int Function GetAnimationsByMainActor(Int zDatabase, Int MainActor) ; returns OArray
	; 0 - dom, 1 - sub
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'mainActor', "+MainActor+")")
EndFunction

Int Function GetAnimationsByMaxSpeed(Int zDatabase, Int Speed) ; returns OArray
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'maxspeed', "+Speed+")")
EndFunction

Int Function GetAnimationsByMinSpeed(Int zDatabase, Int Speed) ; returns OArray
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'minspeed', "+Speed+")")
EndFunction

Int Function GetAnimationsWithIdleSpeed(Int zDatabase, Bool zIdle) ; returns OArray
	Return JValue.evalLuaObj(zDatabase, "return ostim.getAnimationsKeyValuePair(jobject,'hasidlespeed', "+zIdle as Int+")")
EndFunction

;--- auxiliary functions

String Function GetSceneIDByAnimID(String AnimID)
	Int OMAnim = GetObjectOArray(GetAnimationWithAnimID(GetDatabaseOArray(), AnimID), 0)
	Return GetSceneID(OMAnim)
EndFunction

;----------------------
;  OArray Manipulation
;----------------------

Int Function newOArray()
	Return JArray.object()
EndFunction

Function AppendIntOArray(Int OArray, Int Append)
	JArray.AddInt(OArray, Append)
EndFunction

Function AppendObjectOArray(Int OArray, Int Append)
	JArray.AddObj(OArray, Append)
EndFunction

Int Function GetIntOArray(Int OArray, Int Index)
	Return JArray.GetInt(OArray, Index, Default = -10001)
EndFunction

Int Function GetObjectOArray(Int OArray, Int Index)
	Return JArray.GetObj(OArray, Index)
EndFunction

String Function GetStringOArray(Int OArray, Int Index)
	Return JArray.GetStr(OArray, Index, Default = "")
EndFunction

Int Function GetLengthOArray(Int OArray) ;number of items total
	Return JArray.Count(OArray)
EndFunction

Int Function MergeOArrays(Int[] Arrays) ;may only work if arrays are full of omap or oarray objects
	Int Ret = JArray.object()

	Int i = 0
	Int max = Arrays.Length
	While (i < max)
		JArray.addFromArray(ret, Arrays[i])

		i += 1
	EndWhile

	Return Ret
EndFunction

; Fisher–Yates shuffle Algorithm
Int Function ShuffleOArray(Int OArray)
	int L = GetLengthOArray(OArray)

	int i = L

	int j ; an index
	While (i > 0)
		i -= 1

		j = OStim.RandomInt(0, i)

		JArray.swapItems(OArray, i, j)

	EndWhile

	return OArray
EndFunction

;----------------------
;  OMap Manipulation
;----------------------

String Function GetStringOMap(Int OMap, String zKey)
	Return JMap.GetStr(OMap, zKey, Default = "")
EndFunction

Int Function getIntOMap(Int OMap, String zKey)
	Return JMap.GetInt(OMap, zKey, Default = -10001)
EndFunction

Int Function getObjOMap(Int OMap, String zKey)
	Return JMap.GetObj(OMap, zKey)
EndFunction

Bool Function getBoolOMap(Int OMap, String zKey)
	Int a = JMap.getInt(OMap, zKey, Default = 0)
	If (a == 1)
		Return True
	EndIf
	Return False
EndFunction

Int Function getActorProperties(Int Animation, Int Position)
	int Actors = JMap.getObj(Animation, "actors")
	Return JArray.getObj(Actors, Position)
EndFunction

;----------------------------------
; 			Other
;----------------------------------

Function Console(String In)
	OsexIntegrationMain.Console(In)
EndFunction

Function DumpString(String In)
	MiscUtil.WriteToFile("data/ostimdump.txt", In, TimeStamp = False)
EndFunction

Function DumpOmap(Int OMap)
	JValue.WriteToFile(OMap, "data/ostimdump.txt")
	Console("Dumped")
EndFunction
