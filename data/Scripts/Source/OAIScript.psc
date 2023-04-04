ScriptName OAiScript Extends Quest

import OAIUtils

; modules & other goodies
OsexIntegrationMain OStim
Actor PlayerRef

; actor stuff
Actor Agent

Bool Gay
Bool Lesbian

Int NumActors

Event OnInit()
	;Startup()
	OStim = OUtils.GetOStim()
	PlayerRef = Game.GetPlayer()

	OnGameLoad()
EndEvent

;Event OStimStart(String EventName, String strArg, Float NumArg, Form Sender)
;	If (!OStim.UseAIControl)
;		Return
;	EndIf
;	StartAI()
;EndEvent

Event AI_Thread(String EventName, String strArg, Float NumArg, Form Sender)
	Console("Starting OAI")

	Actor[] Actors = OStim.GetActors()
	Actor DomActor = OStim.GetDomActor()
	Actor SubActor = OStim.GetSubActor()

	string SceneID = OStim.GetCurrentAnimationSceneID()

	string FurnitureType = OStim.GetFurnitureType()
	
	Actor AggressiveActor = OStim.GetAggressiveActor()
	int Aggressor = Actors.Find(AggressiveActor)
	bool isAggressorFemale = false

	Lesbian = False
	Gay = False

	If (OStim.IsFemale(DomActor) && OStim.IsFemale(SubActor))
		Lesbian = True
	ElseIf (!OStim.IsFemale(DomActor) && !OStim.IsFemale(SubActor))
		Gay = True
	EndIf

	If (OStim.isplayerinvolved())
		Agent = OStim.GetSexPartner(PlayerRef)
	Else
		Console("Player is not active")
		Agent = DomActor
	EndIf

	NumActors = ostim.getactors().Length

	Int PulloutChance
	If (Aggressor != -1)
		isAggressorFemale = OStim.isFemale(AggressiveActor)
		PulloutChance = 0
	Else
		Int Rel = Agent.GetRelationshipRank(OStim.GetSexPartner(Agent))
		If (Rel == 0)
			PulloutChance = 75
		Else
			PulloutChance = 10
		EndIf
	EndIf

	Int ForeplayChangeChance = ostim.AiSwitchChance * 2
	Float ForeplayEndThreshold
	If (Aggressor != -1)
		ForeplayEndThreshold = Utility.RandomFloat(15.0, 105.0)
	Else
		Int Rel = Agent.GetRelationshipRank(OStim.GetSexPartner(Agent))
		If (Rel == 0)
			If (ChanceRoll(50))
				ForeplayEndThreshold = 125.0
			Else
				ForeplayEndThreshold = Utility.RandomFloat(10.0, 35.0)
			EndIf
		Else
			If (OStim.ChanceRoll(90))
				ForeplayEndThreshold = Utility.RandomFloat(10.0, 35.0)
			Else
				ForeplayEndThreshold = 125.0
			EndIf
		EndIf
	EndIf

	Int SexChangeChance = ostim.AiSwitchChance
	Int ForeplayChance
	If (Aggressor != -1)
		ForeplayChance = 20
	Else
		ForeplayChance = 80
	EndIf

	Console("Furniture type: " + FurnitureType)
	Console("Pullout chance: " + PulloutChance)
	Console("foreplay chance: " + ForeplayChance)
	Console("foreplay change chance: " + ForeplayChangeChance)
	Console("foreplay end thresh : " + ForeplayEndThreshold)
	Console("sex change anim chance: " + SexChangeChance)

	Int Stage
	If (NumActors == 1)
		Stage = 3 ; masturbation
	ElseIf (ChanceRoll(ForeplayChance))
		Stage = 1 ; foreplay
	Else
		Stage = 2 ; main
	EndIf


	Bool ChangeAnimation = True
	While (OStim.isactorinvolved(DomActor) && ostim.AnimationRunning())

		If (NumActors > 1 && OStim.GetActorExcitement(DomActor) > 95)
			If (ChanceRoll(PulloutChance))
				String Animation = GetPulledOutVersion(Actors, FurnitureType, SceneID, OStim.POSITION_TAGS)
				If (Animation != "")
					Warp(Animation)
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
				Console("Changing to other foreplay animation")
				String Animation = GetRandomForeplayAnimation(Actors, FurnitureType, Aggressor, isAggressorFemale, Lesbian, Gay)
				If (Animation != "")
					Warp(Animation)
				EndIf
			EndIf

			If (OStim.GetHighestExcitement() > ForeplayEndThreshold)
				ChangeAnimation = True
				Console("Changing to sex animation")
				Stage = 2
			EndIf
		ElseIf (Stage == 2)
			If (!ChangeAnimation)
				ChangeAnimation = OStim.ChanceRoll(SexChangeChance)
			EndIf

			If (ChangeAnimation)
				ChangeAnimation = False
				Console("Changing to other sex animation")
				String Animation = getRandomSexAnimation(Actors, FurnitureType, Aggressor, Lesbian, Gay)
				If (Animation != "")
					Warp(animation)
				EndIf
			EndIf
		ElseIf (Stage == 3)
			If (!ChangeAnimation)
				ChangeAnimation = OStim.ChanceRoll(SexChangeChance)
			EndIf

			If (ChangeAnimation)
				ChangeAnimation = False
				Console("Changing to another masturbation animation")
				string type = "malemasturbation"
				If OStim.IsFemale(Actors[0])
					type = "femalemasturbation"
				EndIf
				String Animation = OLibrary.GetRandomFurnitureSceneWithAction(Actors, FurnitureType, type)
				If (Animation != "")
					Warp(Animation)
				EndIf
			EndIf
		EndIf

		Utility.Wait(6)

		While (OStim.PauseAI && OStim.AnimationRunning())
			Utility.Wait(1)
		EndWhile
	EndWhile
	Console("Closed AI thread")
EndEvent

 
Function StartAI()
	Console("Firing off AI thread")
	SendModEvent("ostim_start_ai")
EndFunction

Function Warp(String aScene)
	If (OStim.UseAutoFades && OStim.IsActorActive(PlayerRef))
		ostim.FadeToBlack(1)
	EndIf

	OStim.WarpToAnimation(aScene)

	If (OStim.UseAutoFades && OStim.IsActorActive(PlayerRef))
		Utility.Wait(0.55)
		ostim.FadeFromBlack(1)
	EndIf
EndFunction

Function Console(String In)
	OsexIntegrationMain.Console(In)
EndFunction

Bool Function ChanceRoll(Int Chance)
	Return OStim.ChanceRoll(Chance)
EndFunction

Function OnGameLoad()
	;Console("Fixing AI thread")
	;RegisterForModEvent("ostim_start", "Ostimstart")
	RegisterForModEvent("ostim_start_ai", "AI_Thread")
EndFunction
