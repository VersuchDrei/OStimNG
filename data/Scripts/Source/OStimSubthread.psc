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
 
OSexIntegrationMain OStim

actor PlayerRef
actor AggressiveActor

bool Aggressive

string[] CurrentScene

; Call this function to start a new OStim scene in a subthread (for NPC on NPC scenes)
;/* StartSubthreadScene
* * Starts an OStim scene in a subthread (for NPC on NPC scenes), don't ever use for scenes where the player is involved.
* *
* * @param: Dom, the first actor, index 0, usually male
* * @param: Sub, the second actor, index 1, usually female
* * @param: zThirdActor, the third actor, index 2
* * @param: startingAnimation, the animation to start with. If empty string, a random animation will be chosen
* * @param: furniture, the furniture to start the animation on, can be None
* * @param: withAI, if false auto mode will be disabled
* * @param: isAggressive, if the scene is Aggressive
* * @param: AggressingActor, the aggressor in an Aggressive scene
*/;
bool Function StartSubthreadScene(actor dom, actor sub = none, actor zThirdActor = none, string startingAnimation = "", ObjectReference furnitureObj = none, bool withAI = true, bool isAggressive = false, actor aggressingActor = none)
	if OThread.IsRunning(id + 1)
		Console("Subthread is already in use")
		return false
	endif

	Console("Starting subthread with ID: " + id)

	If !dom.Is3DLoaded() || (sub && !sub.Is3DLoaded()) || (zThirdActor && !zThirdActor.Is3DLoaded())
		Console("One of the actors was not loaded. Closing subthread.")
		return False
	EndIf

	Actor[] Actors

	If zThirdActor
		Actors = new Actor[3]
		Actors[0] = dom
		Actors[1] = sub
		Actors[2] = zThirdActor
	ElseIf sub
		Actors = new Actor[2]
		Actors[0] = dom
		Actors[1] = sub
	Else
		Actors = new Actor[1]
		Actors[0] = dom
	EndIf

	Actors = OActor.SortActors(Actors)

	Aggressive = isAggressive
	AggressiveActor = aggressingActor

	int FurnitureType = OFurniture.GetFurnitureType(furnitureObj)

	If (startingAnimation == "")
		If FurnitureType == OStim.FURNITURE_TYPE_NONE
			startingAnimation = OLibrary.GetRandomSceneWithAnySceneTagAndAnyMultiActorTagForAllCSV(Actors, "idle", OCSV.CreateCSVMatrix(Actors.Length, "standing"))
		ElseIf FurnitureType == OStim.FURNITURE_TYPE_BED
			startingAnimation = OLibrary.GetRandomSceneWithAnySceneTagAndAnyMultiActorTagForAllCSV(Actors, "idle", OCSV.CreateCSVMatrix(Actors.Length, "allfours,kneeling,lyingback,lyingside,sitting"))
		Else
			startingAnimation = OLibrary.GetRandomFurnitureSceneWithSceneTag(Actors, OStim.FURNITURE_TYPE_STRINGS[FurnitureType], "idle")
		EndIf
	EndIf

	If (startingAnimation == "")
		Return false
	EndIf

	OSANative.StartScene(id + 1, furnitureObj, Actors)
	OSANative.ChangeAnimation(id + 1, startingAnimation)

	If !withAI
		OThread.StopAutoMode(id + 1)
	EndIf

	return true
EndFunction

Event OnInit()
	id = GetID()
	ostim = GetOStim()
	PlayerRef = Game.GetPlayer()
EndEvent

Function StartAI()
	OThread.StartAutoMode(id + 1)
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
	return OThread.IsRunning(id + 1)
endfunction

int Function GetScenePassword()
	return id + 1
endfunction

ObjectReference Function GetFurniture()
	Return OThread.GetFurniture(id + 1)
EndFunction

Bool Function AnimationRunning()
	OThread.IsRunning(id + 1)
EndFunction

Actor Function GetActor(int Index)
	Return OThread.GetActor(id + 1, Index)
EndFunction

Actor[] Function GetActors()
	Return OThread.GetActors(id + 1)
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
	Actor[] Actors = OThread.GetActors(id + 1)

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
	Actor[] Actors = OThread.GetActors(id + 1)
	int i = Actors.Length
	While i
		i -= 1
		If Actors[i].IsDead()
			Return true
		EndIf
	EndWhile
	return false
EndFunction

Bool Function IsAnyActorInCombat()
	Actor[] Actors = OThread.GetActors(id + 1)
	int i = Actors.Length
	While i
		i -= 1
		If Actors[i].GetCombatState() != 0
			Return true
		EndIf
	EndWhile
	return false
EndFunction

Function runOsexCommand(string cmd)
EndFunction

Function AutoIncreaseSpeed()
	; this is done in C++ now
EndFunction

Function Orgasm(Actor Act)
	OActor.Climax(Act)
EndFunction

Function EndAnimation()
	OThread.Stop(id + 1)
EndFunction

Function WarpToAnimation(String Animation) 
	OThread.WarpTo(id + 1, Animation, false)
EndFunction