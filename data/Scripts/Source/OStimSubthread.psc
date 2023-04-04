ScriptName OStimSubthread extends ReferenceAlias

import OUtils
import OAIUtils

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

int Password
string[] CurrentScene
string CurrentAnimation
float CurrentSpeed
bool SceneRunning

bool Lesbian
bool Gay

Float DomExcitement
Float SubExcitement
Float ThirdExcitement

int DomTimesOrgasm
int SubTimesOrgasm
int ThirdTimesOrgasm

bool AIRunning = False
bool UseAI = False

bool ActorWasHit


; Call this function to start a new OStim scene in a subthread (for NPC on NPC scenes)
;/* StartSubthreadScene
* * Starts an OStim scene in a subthread (for NPC on NPC scenes), don't ever use for scenes where the player is involved.
* *
* * @param: Dom, the first actor, index 0, usually male
* * @param: Sub, the second actor, index 1, usually female
* * @param: zThirdActor, the third actor, index 2
* * @param: startingAnimation, the animation to start with. If empty string, a random animation will be chosen
* * @param: furniture, the furniture to start the animation on, can be None
* * @param: withAI, whether AI will run on the subthread or not. True by default, so that different animations play during the scene. If False, then the same starting animation will always be played until the end
* * @param: isAggressive, if the scene is Aggressive
* * @param: AggressingActor, the aggressor in an Aggressive scene
*/;
bool Function StartSubthreadScene(actor dom, actor sub = none, actor zThirdActor = none, string startingAnimation = "", ObjectReference furnitureObj = none, bool withAI = true, bool isAggressive = false, actor aggressingActor = none)
	if InUse
		Console("Subthread is already in use")
		return false
	endif

	InUse = true

	if withAI
		RegisterForModEvent("ostim_start_subthread_ai", "AI_Thread")
		UseAI = withAI
	endif

	Console("Starting subthread with ID: " + id)

	DomActor = dom 
	SubActor = sub 
	ThirdActor = zThirdActor

	If !DomActor.Is3DLoaded() || (SubActor && !SubActor.Is3DLoaded()) || (ThirdActor && !ThirdActor.Is3DLoaded())
		Console("One of the actors was not loaded. Closing subthread.")

		UnregisterForAllModEvents()
		UseAI = False
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

	If (OStim.IsFemale(DomActor) && OStim.IsFemale(SubActor) && (!ThirdActor ||  OStim.IsFemale(ThirdActor)))
		Lesbian = True
	ElseIf (!OStim.IsFemale(DomActor) && !OStim.IsFemale(SubActor) && (!ThirdActor || !OStim.IsFemale(ThirdActor)))
		Gay = True
	EndIf

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

	CurrentSpeed = 1

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
	SendModEvent("ostim_start_subthread_ai", numArg = id)
EndFunction

Event OnUpdate()
	CurrentScene = OSA.MakeStage()
	Password = CurrentScene[0] as int

	OSA.SetActorsStim(CurrentScene, Actors)
	OSA.SetModule(CurrentScene, "0Sex", CurrentAnimation, "")
	OSA.StimStart(CurrentScene)

	OSANative.StartScene(Password, CurrentFurniture, Actors)
	OSANative.ChangeAnimation(Password, CurrentAnimation)
	OSANative.UpdateSpeed(Password, OMetadata.GetDefaultSpeed(CurrentAnimation))

	If CurrentFurniture
		if OStim.ResetClutter
			OFurniture.ResetClutter(CurrentFurniture, OStim.ResetClutterRadius * 100)
		endif

		CurrentFurniture.BlockActivation(true)
	EndIf

	if !AIRunning && UseAI
		AIRunning = True
		StartAI()
	Endif

	RegisterForModEvent("ostim_actorhit", "OnActorHit")

	SendModEvent("ostim_subthread_start", numArg = id)

	; safety net in case the scene is taking too long so it doesn't run forever
	int numberLoops = 0

	While (!DidAnyActorDie() && !ActorWasHit && !IsAnyActorInCombat() && numberLoops < 300 && OStim.IsActorActive(DomActor) && DomActor.GetParentCell() == PlayerRef.GetParentCell() && inUse)
		numberLoops += 1

		AutoIncreaseSpeed()

		If (GetActorExcitement(SubActor) >= 100.0)
			SubTimesOrgasm += 1
			Orgasm(SubActor)
			If (!OStim.RequireBothOrgasmsToFinish) || (((DomTimesOrgasm > 0) && (SubTimesOrgasm > 0)))
				SetCurrentAnimationSpeed(0)
				Utility.Wait(4)
				EndAnimation()
			EndIf
		EndIf

		If (GetActorExcitement(ThirdActor) >= 100.0)
			ThirdTimesOrgasm += 1
			Orgasm(ThirdActor)
		EndIf

		If (GetActorExcitement(DomActor) >= 100.0)
			DomTimesOrgasm += 1
			Orgasm(DomActor)
			If (!OStim.RequireBothOrgasmsToFinish) || (((DomTimesOrgasm > 0) && (SubTimesOrgasm > 0)))
				SetCurrentAnimationSpeed(0)
				Utility.Wait(4)
				EndAnimation()
			EndIf
		EndIf

		Utility.Wait(1.5)
	EndWhile

	EndAnimation()
EndEvent

Function Orgasm(Actor Act)
	SetActorExcitement(Act, -3.0)

	Act.SendModEvent("ostim_subthread_orgasm", CurrentAnimation, id)

	If (Act == DomActor)
		SetCurrentAnimationSpeed(1)
	EndIf

	int actorIndex = Actors.find(Act)
	If actorIndex != -1
		int actionIndex = OMetadata.FindActionForTarget(CurrentAnimation, actorIndex, "vaginalsex")
		If actionIndex != -1
			Actor partner = GetActor(OMetadata.GetActionActor(CurrentAnimation, actionIndex))
			AddActorExcitement(partner, 5)
		EndIf
	EndIf

	Act.DamageActorValue("stamina", 250.0)
EndFunction

Function EndAnimation()
	SceneRunning = False
	AIRunning = False

	UseAI = False

	If DomActor.GetParentCell() != PlayerRef.GetParentCell()
		; Attempting to end the scene when the actors are not loaded will fail
		SendModEvent("0SA_GameLoaded")
	else
		runOsexCommand("$endscene")
	endif

	UnregisterForUpdate()
	UnregisterForAllModEvents()

	bool actorDied = DidAnyActorDie()

	Spell actra
	Spell actro

	; when actor dies midscene, we need to dispell the Actra and Actro spells from each actor so it's ended properly
	; if you know a better solution, let me know :)
	if actorDied
		actra = Game.GetFormFromFile(0x000D64, "OSA.esm") as Spell
		actro = Game.GetFormFromFile(0x000D67, "OSA.esm") as Spell
	endif

	int i = Actors.Length
	While i
		i -= 1

		if actorDied
			Actors[i].DispelSpell(actra)
			Actors[i].DispelSpell(actro)
		endif

		Actors[i].RemoveFromFaction(OStim.OStimExcitementFaction)
		Actors[i].SetScale(1.0)
	EndWhile

	OSANative.EndScene(Password)

	If CurrentFurniture
		if OStim.ResetClutter
			OFurniture.ResetClutter(CurrentFurniture, OStim.ResetClutterRadius * 100)
		endif

		CurrentFurniture.BlockActivation(false)
	EndIf

	CurrentFurniture = none
	FurnitureType = 0

	Password = 0

	ActorWasHit = False

	SendModEvent("ostim_subthread_end", numArg = id)

	InUse = false
EndFunction

; When we rewrite auto mode, we should update it here as well!
; For now, this is a copy of the main thread auto mode :)
Event AI_Thread(String EventName, String strArg, Float NumArg, Form Sender)
	if (numArg != id)
		return
	endif

	Console("Starting OAI Subthread")

	actor Agent = DomActor

	int Aggressor = Actors.Find(AggressiveActor)
	bool isAggressorFemale = false

	Int PulloutChance
	If (AggressiveActor)
		isAggressorFemale = OStim.isFemale(AggressiveActor)
		PulloutChance = 0
	Else
		Int Rel = Agent.GetRelationshipRank(subActor)
		If (Rel == 0)
			PulloutChance = 75
		Else
			PulloutChance = 10
		EndIf
	EndIf

	Int ForeplayChangeChance = OStim.AiSwitchChance * 2
	Float ForeplayEndThreshold

	If (AggressiveActor)
		ForeplayEndThreshold = Utility.RandomFloat(15.0, 105.0)
	Else
		ForeplayEndThreshold = Utility.RandomFloat(10.0, 45.0)
	EndIf

	Int SexChangeChance = OStim.AiSwitchChance
	Int ForeplayChance = 100

	If (AggressiveActor)
		ForeplayChance = 20
	EndIf

	Int Stage

	If (Actors.length == 1)
		Stage = 3 ; masturbation
	ElseIf (OStim.ChanceRoll(ForeplayChance))
		Stage = 1 ; foreplay
	Else
		Stage = 2 ; main
	EndIf

	Bool ChangeAnimation = True

	String Animation = ""

	While (DomActor && AIRunning)
		If (Actors.length > 1 && GetActorExcitement(DomActor) > 95)
			If (OStim.ChanceRoll(PulloutChance))
				Animation = GetPulledOutVersion(Actors, OStim.FURNITURE_TYPE_STRINGS[FurnitureType], CurrentAnimation, OStim.POSITION_TAGS)
				If (Animation != "")
					WarpToAnimation(Animation)
				EndIf
			EndIf
			Stage = 4
		EndIf

		If (Stage == 1)
			If (!ChangeAnimation)
				ChangeAnimation = OStim.ChanceRoll(ForeplayChangeChance)
			EndIf

			If (ChangeAnimation)
				ChangeAnimation = False
				Animation = GetRandomForeplayAnimation(Actors, OStim.FURNITURE_TYPE_STRINGS[FurnitureType], Aggressor, isAggressorFemale, Lesbian, Gay)
				If (Animation != "")
					WarpToAnimation(Animation)
				EndIf
			EndIf

			If (GetHighestExcitement() > ForeplayEndThreshold)
				ChangeAnimation = True
				Stage = 2
			EndIf
		ElseIf (Stage == 2)
			If (!ChangeAnimation)
				ChangeAnimation = OStim.ChanceRoll(SexChangeChance)
			EndIf

			If (ChangeAnimation)
				ChangeAnimation = False
				Animation = GetRandomSexAnimation(Actors, OStim.FURNITURE_TYPE_STRINGS[FurnitureType], Aggressor, Lesbian, Gay)
				If (Animation != "")
					WarpToAnimation(animation)
				EndIf
			EndIf
		ElseIf (Stage == 3)
			If (!ChangeAnimation)
				ChangeAnimation = OStim.ChanceRoll(SexChangeChance)
			EndIf

			If (ChangeAnimation)
				ChangeAnimation = False
				string type = "malemasturbation"
				If OStim.IsFemale(DomActor)
					type = "femalemasturbation"
				EndIf
				Animation = OLibrary.GetRandomFurnitureSceneWithAction(Actors, OStim.FURNITURE_TYPE_STRINGS[FurnitureType], type)
				If (Animation != "")
					WarpToAnimation(Animation)
				EndIf
			EndIf
		EndIf

		; Failsafes
		; If Animation is empty, then the scene will be stuck on starting animation
		If (!Animation)
			; If Lesbian or Gay, it means there are no lesbian/gay anims installed
			; Mark the animation as not lesbian/gay so it can generate new anims
			If (Lesbian || Gay)
				Lesbian = False
				Gay = False
			; If stage is foreplay, then there are no foreplay animations for current scenario, dvance to sex stage
			ElseIf (stage == 1)
				stage = 2
			; If stage is sex, then there are no sex animations for current scenario, revert to foreplay stage
			; In theory, this will not generate a loop where stage constantly changes between foreplay and sex,
			; as there will always be either foreplay or sex animations in any given scenario
			ElseIf (stage == 2)
				stage = 1
			EndIf

			ChangeAnimation = True
		EndIf

		Utility.Wait(6)
	EndWhile

	Console("Closed AI subthread")
EndEvent

Event OnActorHit(String EventName, String zAnimation, Float NumArg, Form Sender)
	int i = Actors.Length
	While i
		i -= 1
		; GetCombatState() returns the following values
		; 0: Not in combat
		; 1: In combat
		; 2: Searching
		; So we should consider as being in combat if actor is in either actual combat or searching for combat target
		If Actors[i].GetCombatState() != 0
			ActorWasHit = True
			Return
		EndIf
	EndWhile
EndEvent

Function WarpToAnimation(String Animation) 
	; this is a bit hacky but other ways to change animation either break or introduce other bugs
	CurrentAnimation = Animation
	OSA.SetModule(CurrentScene, "0Sex", CurrentAnimation, "")
	CurrentSpeed = OMetadata.GetDefaultSpeed(Animation)
	runOsexCommand("$Speed,0," + CurrentSpeed)
	OSA.StimStart(CurrentScene)

	OSANative.ChangeAnimation(Password, CurrentAnimation)
	OSANative.UpdateSpeed(Password, CurrentSpeed as int)
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
	return Password
endfunction

ObjectReference Function GetFurniture()
	Return CurrentFurniture
EndFunction

Bool Function AnimationRunning()
	Return SceneRunning
EndFunction

Bool Function DidAnyActorDie()
	return DomActor.IsDead() || (SubActor && SubActor.IsDead()) || (ThirdActor && ThirdActor.IsDead())
EndFunction

Bool Function IsAnyActorInCombat()
	return DomActor.GetCombatState() != 0 || (SubActor && SubActor.GetCombatState() != 0) || (ThirdActor && ThirdActor.GetCombatState() != 0)
EndFunction

Function runOsexCommand(string cmd)
	string[] Plan = new string[2]
	Plan[1] = cmd

	OSA.setPlan(CurrentScene, Plan) 
	OSA.stimstart(CurrentScene)
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

Float Function GetActorExcitement(Actor Act) ; at 100, Actor orgasms
	if(Act)
		return OSANative.GetActorExcitement(Password, Act)
	endif
	return 0
EndFunction

float Function GetHighestExcitement()
	float Highest = 0

	int i = Actors.Length
	While i
		i -= 1
		float Excitement = GetActorExcitement(Actors[i])
		If Excitement > Highest
			Highest = Excitement
		EndIf
	EndWhile

	return Highest
EndFunction

Function SetActorExcitement(Actor Act, Float Value)
	if (Act)
		OSANative.SetActorExcitement(Password, Act, Value)
		Act.SetFactionRank(OStim.OStimExcitementFaction, Value as int)
	endIf
EndFunction

Function AddActorExcitement(Actor Act, Float Value)
	SetActorExcitement(Act, GetActorExcitement(Act) + Value)
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
	{Increase or decrease the animation speed by the amount}
	If amount < 0
		int times = math.abs(amount * 2.0) as int
		While times > 0
			CurrentSpeed -= 0.5
			runOsexCommand("$Speed,0," + CurrentSpeed)
			times -= 1
		EndWhile
	Else
		CurrentSpeed += amount
		runOsexCommand("$Speed,0," + CurrentSpeed)
	EndIf
	OSANative.UpdateSpeed(password, CurrentSpeed as int)
EndFunction

Function IncreaseAnimationSpeed()
	If (CurrentSpeed >= OMetadata.GetMaxSpeed(CurrentAnimation))
		Return
	EndIf
	AdjustAnimationSpeed(1)
EndFunction

Function DecreaseAnimationSpeed()
	If (CurrentSpeed < 1)
		Return
	EndIf
	AdjustAnimationSpeed(-1)
EndFunction

Function SetCurrentAnimationSpeed(Int InSpeed)
	AdjustAnimationSpeed(inspeed - CurrentSpeed)
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
