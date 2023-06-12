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
EndEvent

Event AI_Thread(String EventName, String strArg, Float NumArg, Form Sender)
EndEvent

 
Function StartAI()
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
EndFunction
