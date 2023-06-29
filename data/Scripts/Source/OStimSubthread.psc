ScriptName OStimSubthread extends ReferenceAlias

import OUtils

; Subthreads
;
; These are OStim threads exclusively for NPC on NPC scenes. Player scenes should never go in a subthread.
; Subthreads have similar features to the main thread, including the sending of events, tracking of actor excitement
; and AI (they change animations dynamically as the scene progresses). They also have support for furniture animations.
;
;
; Please note that most utility functions from OSexIntegrationMain will give you information about the actors in the
; main thread, and NOT about actors in the subthreads! To get information from Subthreads, you must either work with the
; information sent in the events, or fetch the Subthread alias and work directly with the properties and functions
; provided in this script.
;
;
; The main thread has always ID -1. You access its info from the OSexIntegrationMain script.
;
; Subthreads have ID 0 thru 9. But there is only one subthread script...
; So how would you access info from a specific subthread?
; It's easy! For example, if you want to get the dom actor from the subthread with ID 2, you would do the following:
;
;		Quest subthreadQuest = Game.GetFormFromFile(0x000806, "OStim.esp") as Quest
;		OStimSubthread thread = subthreadQuest.GetNthAlias(2) as OStimSubthread
;		Actor subthreadDom
;		if thread.IsInUse() ; always check if thread is active first!
;			subthreadDom = thread.GetActor(0)
;		endif
;


int property id auto
bool InUse
 
OSexIntegrationMain OStim

actor PlayerRef

actor DomActor
actor SubActor
actor ThirdActor
actor[] Actors
actor AggressiveActor

ObjectReference CurrentFurniture
int FurnitureType

bool Aggressive

string[] CurrentScene
string CurrentAnimation
bool SceneRunning

; Call this function to start a new OStim scene in a subthread (for NPC on NPC scenes)
;/* StartSubthreadScene
* * Starts an OStim scene in a subthread (for NPC on NPC scenes), don't ever use for scenes where the player is involved.
* *
* * @param: Dom, the first actor, index 0, usually male
* * @param: Sub, the second actor, index 1, usually female
* * @param: zThirdActor, the third actor, index 2
* * @param: startingAnimation, the animation to start with. If empty string, a random animation will be chosen
* * @param: furniture, the furniture to start the animation on, can be None
* * @param: withAI, no longer in use, NPC threads always run in auto mode
* * @param: isAggressive, if the scene is Aggressive
* * @param: AggressingActor, the aggressor in an Aggressive scene
*/;
bool Function StartSubthreadScene(actor dom, actor sub = none, actor zThirdActor = none, string startingAnimation = "", ObjectReference furnitureObj = none, bool withAI = true, bool isAggressive = false, actor aggressingActor = none)
	if InUse
		Console("Subthread is already in use")
		return false
	endif

	InUse = true

	Console("Starting subthread with ID: " + id)

	DomActor = dom 
	SubActor = sub 
	ThirdActor = zThirdActor

	If !DomActor.Is3DLoaded() || (SubActor && !SubActor.Is3DLoaded()) || (ThirdActor && !ThirdActor.Is3DLoaded())
		Console("One of the actors was not loaded. Closing subthread.")

		UnregisterForAllModEvents()
		InUse = False

		return False
	EndIf

	If (SubActor && AppearsFemale(dom) && !AppearsFemale(sub))
		DomActor = sub
		SubActor = dom
	EndIf

	If ThirdActor
		If AppearsFemale(ThirdActor) && !AppearsFemale(SubActor)
			SubActor = zThirdActor
			ThirdActor = sub
		EndIf
	EndIf

	If ThirdActor
		Actors = new Actor[3]
		Actors[0] = DomActor
		Actors[1] = SubActor
		Actors[2] = ThirdActor
	ElseIf SubActor
		Actors = new Actor[2]
		Actors[0] = DomActor
		Actors[1] = SubActor
	Else
		Actors = new Actor[1]
		Actors[0] = DomActor
	EndIf

	Aggressive = isAggressive
	AggressiveActor = aggressingActor

	int i = Actors.Length

	While i
		i -= 1

		bool isFemale = OStim.AppearsFemale(Actors[i])

		Actors[i].AddToFaction(OStim.OStimExcitementFaction)
	EndWhile

	CurrentFurniture = furnitureObj
	FurnitureType = OFurniture.GetFurnitureType(CurrentFurniture)

	CurrentAnimation = startingAnimation

	If (CurrentAnimation == "")
		If FurnitureType == OStim.FURNITURE_TYPE_NONE
			CurrentAnimation = OLibrary.GetRandomSceneWithAnySceneTagAndAnyMultiActorTagForAllCSV(Actors, "idle", OCSV.CreateCSVMatrix(Actors.Length, "standing"))
		ElseIf FurnitureType == OStim.FURNITURE_TYPE_BED
			CurrentAnimation = OLibrary.GetRandomSceneWithAnySceneTagAndAnyMultiActorTagForAllCSV(Actors, "idle", OCSV.CreateCSVMatrix(Actors.Length, "allfours,kneeling,lyingback,lyingside,sitting"))
		Else
			CurrentAnimation = OLibrary.GetRandomFurnitureSceneWithSceneTag(Actors, OStim.FURNITURE_TYPE_STRINGS[FurnitureType], "idle")
		EndIf
	EndIf

	If (CurrentAnimation == "")
		CurrentAnimation = "AUTO"
	EndIf

	RegisterForSingleUpdate(0.01)
	SceneRunning = True

	return true
EndFunction

Event OnInit()
	id = GetID()
	ostim = GetOStim()
	PlayerRef = Game.GetPlayer()
	InUse = false
EndEvent

Function StartAI()
	OThread.StartAutoMode(id + 1)
EndFunction

Event OnUpdate()
	OSANative.StartScene(id + 1, CurrentFurniture, Actors)
	OSANative.ChangeAnimation(id + 1, CurrentAnimation)

	If CurrentFurniture
		if OStim.ResetClutter
			OFurniture.ResetClutter(CurrentFurniture, OStim.ResetClutterRadius * 100)
		endif

		CurrentFurniture.BlockActivation(true)
	EndIf

	SendModEvent("ostim_subthread_start", numArg = id)

	While OThread.IsRunning(id + 1)
		Utility.Wait(1.5)
	EndWhile

	EndAnimation()
EndEvent

Function EndAnimation()
	SceneRunning = False

	UnregisterForUpdate()
	UnregisterForAllModEvents()

	OThread.Stop(id + 1)

	If CurrentFurniture
		if OStim.ResetClutter
			OFurniture.ResetClutter(CurrentFurniture, OStim.ResetClutterRadius * 100)
		endif

		CurrentFurniture.BlockActivation(false)
	EndIf

	CurrentFurniture = none
	FurnitureType = 0

	SendModEvent("ostim_subthread_end", numArg = id)

	InUse = false
EndFunction

Function WarpToAnimation(String Animation) 
	OThread.WarpTo(id + 1, Animation, false)
EndFunction


;
;			██╗   ██╗████████╗██╗██╗     ██╗████████╗██╗███████╗███████╗
;			██║   ██║╚══██╔══╝██║██║     ██║╚══██╔══╝██║██╔════╝██╔════╝
;			██║   ██║   ██║   ██║██║     ██║   ██║   ██║█████╗  ███████╗
;			██║   ██║   ██║   ██║██║     ██║   ██║   ██║██╔══╝  ╚════██║
;			╚██████╔╝   ██║   ██║███████╗██║   ██║   ██║███████╗███████║
;			 ╚═════╝    ╚═╝   ╚═╝╚══════╝╚═╝   ╚═╝   ╚═╝╚══════╝╚══════╝
;
; 				Utility functions for Subthread

bool Function IsInUse()
	return InUse
endfunction

int Function GetScenePassword()
	return id + 1
endfunction

ObjectReference Function GetFurniture()
	Return CurrentFurniture
EndFunction

Bool Function AnimationRunning()
	OThread.IsRunning(id + 1)
EndFunction

Actor Function GetActor(int Index)
	If Index >= 0 && Index < Actors.Length
		Return Actors[Index]
	EndIf

	Return None
EndFunction

Actor[] Function GetActors()
	Return Actors
EndFunction


;
;			███████╗████████╗██╗███╗   ███╗██╗   ██╗██╗      █████╗ ████████╗██╗ ██████╗ ███╗   ██╗    ███████╗██╗   ██╗███████╗████████╗███████╗███╗   ███╗
;			██╔════╝╚══██╔══╝██║████╗ ████║██║   ██║██║     ██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║    ██╔════╝╚██╗ ██╔╝██╔════╝╚══██╔══╝██╔════╝████╗ ████║
;			███████╗   ██║   ██║██╔████╔██║██║   ██║██║     ███████║   ██║   ██║██║   ██║██╔██╗ ██║    ███████╗ ╚████╔╝ ███████╗   ██║   █████╗  ██╔████╔██║
;			╚════██║   ██║   ██║██║╚██╔╝██║██║   ██║██║     ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║    ╚════██║  ╚██╔╝  ╚════██║   ██║   ██╔══╝  ██║╚██╔╝██║
;			███████║   ██║   ██║██║ ╚═╝ ██║╚██████╔╝███████╗██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║    ███████║   ██║   ███████║   ██║   ███████╗██║ ╚═╝ ██║
;			╚══════╝   ╚═╝   ╚═╝╚═╝     ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝    ╚══════╝   ╚═╝   ╚══════╝   ╚═╝   ╚══════╝╚═╝     ╚═╝
;
;				All code related to the stimulation simulation

float Function GetHighestExcitement()
	float Highest = 0

	int i = Actors.Length
	While i
		i -= 1
		float Excitement = OActor.GetExcitement(Actors[i])
		If Excitement > Highest
			Highest = Excitement
		EndIf
	EndWhile

	return Highest
EndFunction


;
;			███████╗██████╗ ███████╗███████╗██████╗     ██╗   ██╗████████╗██╗██╗     ██╗████████╗██╗███████╗███████╗
;			██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗    ██║   ██║╚══██╔══╝██║██║     ██║╚══██╔══╝██║██╔════╝██╔════╝
;			███████╗██████╔╝█████╗  █████╗  ██║  ██║    ██║   ██║   ██║   ██║██║     ██║   ██║   ██║█████╗  ███████╗
;			╚════██║██╔═══╝ ██╔══╝  ██╔══╝  ██║  ██║    ██║   ██║   ██║   ██║██║     ██║   ██║   ██║██╔══╝  ╚════██║
;			███████║██║     ███████╗███████╗██████╔╝    ╚██████╔╝   ██║   ██║███████╗██║   ██║   ██║███████╗███████║
;			╚══════╝╚═╝     ╚══════╝╚══════╝╚═════╝      ╚═════╝    ╚═╝   ╚═╝╚══════╝╚═╝   ╚═╝   ╚═╝╚══════╝╚══════╝
;
;				Some code related to the speed system

Function AdjustAnimationSpeed(float amount)
	OThread.SetSpeed(id + 1, OThread.GetSpeed(id + 1) + (amount As int))
EndFunction

Function IncreaseAnimationSpeed()
	OThread.SetSpeed(id + 1, OThread.GetSpeed(id + 1) + 1)
EndFunction

Function DecreaseAnimationSpeed()
	OThread.SetSpeed(id + 1, OThread.GetSpeed(id + 1) - 1)
EndFunction

Function SetCurrentAnimationSpeed(Int InSpeed)
	OThread.SetSpeed(id + 1, InSpeed)
EndFunction


; ██████╗ ███████╗██████╗ ██████╗ ███████╗ ██████╗ █████╗ ████████╗███████╗██████╗ 
; ██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗╚══██╔══╝██╔════╝██╔══██╗
; ██║  ██║█████╗  ██████╔╝██████╔╝█████╗  ██║     ███████║   ██║   █████╗  ██║  ██║
; ██║  ██║██╔══╝  ██╔═══╝ ██╔══██╗██╔══╝  ██║     ██╔══██║   ██║   ██╔══╝  ██║  ██║
; ██████╔╝███████╗██║     ██║  ██║███████╗╚██████╗██║  ██║   ██║   ███████╗██████╔╝
; ╚═════╝ ╚══════╝╚═╝     ╚═╝  ╚═╝╚══════╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═════╝ 

; this is only here to not break old addons, don't use it in new addons, use whatever they're calling instead

bool Function StartScene(actor dom, actor sub = none, actor third = none, float time = 120.0, ObjectReference bed = none, bool isAggressive = false, actor aggressingActor = none, bool LinkToMain = false)
	return StartSubthreadScene(dom, sub = sub, zThirdActor = third, startingAnimation = "", furnitureObj = bed, withAI = true, isAggressive = isAggressive, aggressingActor = aggressingActor)
EndFunction

Float Function GetActorExcitement(Actor Act) ; at 100, Actor orgasms
	Return OActor.GetExcitement(Act)
EndFunction

Function SetActorExcitement(Actor Act, Float Value)
	OActor.SetExcitement(Act, Value)
EndFunction

Function AddActorExcitement(Actor Act, Float Value)
	OActor.ModifyExcitement(Act, Value)
EndFunction

Bool Function DidAnyActorDie()
	return DomActor.IsDead() || (SubActor && SubActor.IsDead()) || (ThirdActor && ThirdActor.IsDead())
EndFunction

Bool Function IsAnyActorInCombat()
	return DomActor.GetCombatState() != 0 || (SubActor && SubActor.GetCombatState() != 0) || (ThirdActor && ThirdActor.GetCombatState() != 0)
EndFunction

Function runOsexCommand(string cmd)
EndFunction

Function AutoIncreaseSpeed()
	Float MainExcitement = GetActorExcitement(DomActor)

	If (MainExcitement >= 85.0)
		If OStim.ChanceRoll(80)
			IncreaseAnimationSpeed()
		EndIf
	ElseIf (MainExcitement >= 69.0)
		If OStim.ChanceRoll(50)
			IncreaseAnimationSpeed()
		EndIf
	ElseIf (MainExcitement >= 25.0)
		If OStim.ChanceRoll(20)
			IncreaseAnimationSpeed()
		EndIf
	ElseIf (MainExcitement >= 5.0)
		If OStim.ChanceRoll(20)
			IncreaseAnimationSpeed()
		EndIf
	EndIf
EndFunction

Function Orgasm(Actor Act)
	OActor.Climax(Act)
EndFunction