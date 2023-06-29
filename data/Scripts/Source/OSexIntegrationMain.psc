ScriptName OSexIntegrationMain Extends Quest


;		What is this? Am I in the right place? How do I use this???

; This script is the core of OStim. If you want to start OStim scenes and/or manipulate them, you are in the right place

;	Structure of this script
; At the very top here, are the Properties. They are the settings you see in the MCM. You can toggle these at will on this script and it
; will update the MCM and everything. Below that are the OStim local variables, you can safely ignore those. Below those variables,
; you will find OStim's main loop and the StartScene() function. OStim's core logic runs in there, I recommend giving it a read.
; Below that is the UTILITIES area. These functions are going to be very useful to you and will let you access data in OStim as
; well as manipulate the currently running scene. Below the utilities area are some more specific groups of functions.

; Some parts of code, including undressing, on-screen bar, and animation data lookups, are in other scripts to make this script easier to
; read. You can call functions in the below utilities area to return those script objects.

; Want a list of all Events you can register with? CTRL + F this script for "SendModEvent" and you can see them all as well as the exact point they fire
; With the exception of the sound event, OStim events do not include data with them. They only let you know when something has happened. You can access
; OStim and get all of the data you need through the normal API here

; PROTIP: ctrl + F is your best friend when it comes to seeing how and where a function/variable/property/etc is used elsewhere

;			 ██████╗ ███████╗████████╗██╗███╗   ███╗
;			██╔═══██╗██╔════╝╚══██╔══╝██║████╗ ████║
;			██║   ██║███████╗   ██║   ██║██╔████╔██║
;			██║   ██║╚════██║   ██║   ██║██║╚██╔╝██║
;			╚██████╔╝███████║   ██║   ██║██║ ╚═╝ ██║
;			 ╚═════╝ ╚══════╝   ╚═╝   ╚═╝╚═╝     ╚═╝

; -------------------------------------------------------------------------------------------------
; CONSTANTS  --------------------------------------------------------------------------------------
int Property FURNITURE_TYPE_NONE = 0 AutoReadOnly
int Property FURNITURE_TYPE_BED = 1 AutoReadOnly
int Property FURNITURE_TYPE_BENCH = 2 AutoReadOnly
int Property FURNITURE_TYPE_CHAIR = 3 AutoReadOnly
int Property FURNITURE_TYPE_TABLE = 4 AutoReadOnly
int Property FURNITURE_TYPE_SHELF = 5 AutoReadOnly
int Property FURNITURE_TYPE_WALL = 6 AutoReadOnly
int Property FURNITURE_TYPE_COOKING_POT = 7 AutoReadOnly
int Property FURNITURE_TYPE_CANCEL = 8 AutoReadOnly

string[] Property FURNITURE_TYPE_STRINGS Auto

string[] Property POSITION_TAGS Auto

; -------------------------------------------------------------------------------------------------
; PROPERTIES  -------------------------------------------------------------------------------------


Faction Property OStimNoFacialExpressionsFaction Auto
Faction Property OStimExcitementFaction Auto

GlobalVariable Property OStimImprovedCamSupport Auto
bool Property EnableImprovedCamSupport
	bool Function Get()
		Return OStimImprovedCamSupport.value != 0
	EndFunction
EndProperty


; -------------------------------------------------------------------------------------------------
; SETTINGS  ---------------------------------------------------------------------------------------
string[] scenemetadata
string[] oldscenemetadata

int Property InstalledVersion Auto

; -------------------------------------------------------------------------------------------------
; GENERAL SETTINGS  -------------------------------------------------------------------------------

GlobalVariable Property OStimResetPosition Auto
Bool Property ResetPosAfterSceneEnd
	bool Function Get()
		Return OStimResetPosition.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimResetPosition.value = 1
		Else
			OStimResetPosition.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimCustomTimeScale Auto
Int Property CustomTimescale
	int Function Get()
		Return OStimCustomTimeScale.value As int
	EndFunction
	Function Set(int Value)
		OStimCustomTimeScale.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimUseFades Auto
bool Property UseFades
	bool Function Get()
		Return OStimUseFades.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseFades.value = 1
		Else
			OStimUseFades.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUseIntroScenes Auto
bool Property UseIntroScenes
	bool Function Get()
		Return OStimUseIntroScenes.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseIntroScenes.value = 1
		Else
			OStimUseIntroScenes.value = 0
		EndIf
	EndFunction
EndProperty


GlobalVariable Property OStimMaleLightMode Auto
int Property DomLightPos
	int Function Get()
		Return OStimMaleLightMode.value As int
	EndFunction
	Function Set(int Value)
		OStimMaleLightMode.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimFemaleLightMode Auto
int Property SubLightPos
	int Function Get()
		Return OStimFemaleLightMode.value As int
	EndFunction
	Function Set(int Value)
		OStimFemaleLightMode.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimOnlyLightInDark Auto
bool Property LowLightLevelLightsOnly
	bool Function Get()
		Return OStimOnlyLightInDark.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimOnlyLightInDark.value = 1
		Else
			OStimOnlyLightInDark.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimMaleLightBrightness Auto
int Property DomLightBrightness
	int Function Get()
		Return OStimMaleLightBrightness.value As int
	EndFunction
	Function Set(int Value)
		OStimMaleLightBrightness.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimFemaleLightBrightness Auto
int Property SubLightBrightness
	int Function Get()
		Return OStimFemaleLightBrightness.value As int
	EndFunction
	Function Set(int Value)
		OStimFemaleLightBrightness.value = Value
	EndFunction
EndProperty


; -------------------------------------------------------------------------------------------------
; CONTROLS SETTINGS  ------------------------------------------------------------------------------

GlobalVariable Property OStimKeyUp Auto
int Property KeyUp
	int Function Get()
		Return OStimKeyUp.value As int
	EndFunction
	Function Set(int Value)
		OstimKeyUp.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyDown Auto
int Property KeyDown
	int Function Get()
		Return OStimKeyDown.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyDown.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyLeft Auto
int Property KeyLeft
	int Function Get()
		Return OStimKeyLeft.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyLeft.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyRight Auto
int Property KeyRight
	int Function Get()
		Return OStimKeyRight.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyRight.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyYes Auto
int Property KeyYes
	int Function Get()
		Return OStimKeyYes.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyYes.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyEnd Auto
int Property KeyEnd
	int Function Get()
		Return OStimKeyEnd.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyEnd.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyToggle Auto
int Property KeyToggle
	int Function Get()
		Return OStimKeyToggle.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyToggle.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeySceneStart Auto
int Property KeyMap
	int Function Get()
		Return OStimKeySceneStart.value As int
	EndFunction
	Function Set(int Value)
		UnregisterForKey(OStimKeySceneStart.value As int)
		OStimKeySceneStart.value = Value
		If Value != 1
			RegisterForKey(Value)
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimKeySpeedUp Auto
Int Property SpeedUpKey
	int Function Get()
		Return OStimKeySpeedUp.value As int
	EndFunction
	Function Set(int Value)
		OStimKeySpeedUp.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeySpeedDown Auto
Int Property SpeedDownKey
	int Function Get()
		Return OStimKeySpeedDown.value As int
	EndFunction
	Function Set(int Value)
		OStimKeySpeedDown.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyPullOut Auto
Int Property PullOutKey
	int Function Get()
		Return OStimKeyPullOut.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyPullOut.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyAutoMode Auto
Int Property ControlToggleKey
	int Function Get()
		Return OStimKeyAutoMode.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyAutoMode.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyFreeCam Auto
int property FreecamKey
	int Function Get()
		Return OStimKeyFreeCam.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyFreeCam.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeySearch Auto
int Property SearchKey
	int Function Get()
		Return OStimKeySearch.value As int
	EndFunction
	Function Set(int Value)
		OStimKeySearch.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimKeyAlignment Auto
int Property AlignmentKey
	int Function Get()
		Return OStimKeyAlignment.value As int
	EndFunction
	Function Set(int Value)
		OStimKeyAlignment.value = Value
	EndFunction
EndProperty


GlobalVariable Property OStimUseRumble Auto
Bool Property UseRumble
	bool Function Get()
		Return OStimUseRumble.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseRumble.value = 1
		Else
			OStimUseRumble.value = 0
		EndIf
	EndFunction
EndProperty


; -------------------------------------------------------------------------------------------------
; AUTO CONTROL SETTINGS  --------------------------------------------------------------------------

GlobalVariable Property OStimAutoSpeedControl Auto
Bool Property EnableActorSpeedControl
	bool Function Get()
		Return OStimAutoSpeedControl.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimAutoSpeedControl.value = 1
		Else
			OStimAutoSpeedControl.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoSpeedControlIntervalMin Auto
int Property AutoSpeedControlIntervalMin
	int Function Get()
		Return OStimAutoSpeedControlIntervalMin.value As int
	EndFunction
	Function Set(int Value)
		If AutoSpeedControlIntervalMax < Value
			OStimAutoSpeedControlIntervalMin.value = OStimAutoSpeedControlIntervalMax.value
			OStimAutoSpeedControlIntervalMax.value = Value
		Else
			OStimAutoSpeedControlIntervalMin.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoSpeedControlIntervalMax Auto
int Property AutoSpeedControlIntervalMax
	int Function Get()
		Return OStimAutoSpeedControlIntervalMax.value As int
	EndFunction
	Function Set(int Value)
		If AutoSpeedControlIntervalMin > Value
			OStimAutoSpeedControlIntervalMax.value = OStimAutoSpeedControlIntervalMin.value
			OStimAutoSpeedControlIntervalMin.value = Value
		Else
			OStimAutoSpeedControlIntervalMax.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoSpeedControlExcitementMin Auto
int Property AutoSpeedControlExcitementMin
	int Function Get()
		Return OStimAutoSpeedControlExcitementMin.value As int
	EndFunction
	Function Set(int Value)
		If AutoSpeedControlExcitementMax < Value
			OStimAutoSpeedControlExcitementMin.value = OStimAutoSpeedControlExcitementMax.value
			OStimAutoSpeedControlExcitementMax.value = Value
		Else
			OStimAutoSpeedControlExcitementMin.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoSpeedControlExcitementMax Auto
int Property AutoSpeedControlExcitementMax
	int Function Get()
		Return OStimAutoSpeedControlExcitementMax.value As int
	EndFunction
	Function Set(int Value)
		If AutoSpeedControlExcitementMin > Value
			OStimAutoSpeedControlExcitementMax.value = OStimAutoSpeedControlExcitementMin.value
			OStimAutoSpeedControlExcitementMin.value = Value
		Else
			OStimAutoSpeedControlExcitementMax.value = Value
		EndIf
	EndFunction
EndProperty


GlobalVariable Property OStimNPCSceneDuration Auto
int Property NPCSceneDuration
	int Function Get()
		Return OStimNPCSceneDuration.value As int
	EndFunction
	Function Set(int Value)
		OStimNPCSceneDuration.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimEndNPCSceneOnOrgasm Auto
Bool Property EndNPCSceneOnOrgasm
	bool Function Get()
		Return OStimEndNPCSceneOnOrgasm.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimEndNPCSceneOnOrgasm.value = 1
		Else
			OStimEndNPCSceneOnOrgasm.value = 0
		EndIf
	EndFunction
EndProperty


GlobalVariable Property OStimUseAutoModeAlways Auto
Bool Property UseAIControl
	bool Function Get()
		Return OStimUseAutoModeAlways.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseAutoModeAlways.value = 1
		Else
			OStimUseAutoModeAlways.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUseAutoModeSolo Auto
Bool Property UseAIMasturbation
	bool Function Get()
		Return OStimUseAutoModeSolo.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseAutoModeSolo.value = 1
		Else
			OStimUseAutoModeSolo.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUseAutoModeDominant Auto
Bool Property UseAIPlayerAggressor
	bool Function Get()
		Return OStimUseAutoModeDominant.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseAutoModeDominant.value = 1
		Else
			OStimUseAutoModeDominant.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUseAutoModeSubmissive Auto
Bool Property UseAIPlayerAggressed
	bool Function Get()
		Return OStimUseAutoModeSubmissive.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseAutoModeSubmissive.value = 1
		Else
			OStimUseAutoModeSubmissive.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUseAutoModeVanilla Auto
Bool Property UseAINonAggressive
	bool Function Get()
		Return OStimUseAutoModeVanilla.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseAutoModeVanilla.value = 1
		Else
			OStimUseAutoModeVanilla.value = 0
		EndIf
	EndFunction
EndProperty


GlobalVariable Property OStimUseAutoModeFades Auto
Bool Property UseAutoFades
	bool Function Get()
		Return OStimUseAutoModeFades.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseAutoModeFades.value = 1
		Else
			OStimUseAutoModeFades.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoModeAnimDurationMin Auto
int Property AutoModeAnimDurationMin
	int Function Get()
		Return OStimAutoModeAnimDurationMin.value As int
	EndFunction
	Function Set(int Value)
		If AutoModeAnimDurationMax < Value
			OStimAutoModeAnimDurationMin.value = OStimAutoModeAnimDurationMax.value
			OStimAutoModeAnimDurationMax.value = Value
		Else
			OStimAutoModeAnimDurationMin.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoModeAnimDurationMax Auto
int Property AutoModeAnimDurationMax
	int Function Get()
		Return OStimAutoModeAnimDurationMax.value As int
	EndFunction
	Function Set(int Value)
		If AutoModeAnimDurationMin > Value
			OStimAutoModeAnimDurationMax.value = OStimAutoModeAnimDurationMin.value
			OStimAutoModeAnimDurationMin.value = Value
		Else
			OStimAutoModeAnimDurationMax.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoModeForeplayChance Auto
int Property AutoModeForeplayChance
	int Function Get()
		Return OStimAutoModeForeplayChance.value As int
	EndFunction
	Function Set(int Value)
		OStimAutoModeForeplayChance.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimAutoModeForeplayThresholdMin Auto
int Property AutoModeForeplayThresholdMin
	int Function Get()
		Return OStimAutoModeForeplayThresholdMin.value As int
	EndFunction
	Function Set(int Value)
		If AutoModeForeplayThresholdMax < Value
			OStimAutoModeForeplayThresholdMin.value = OStimAutoModeForeplayThresholdMax.value
			OStimAutoModeForeplayThresholdMax.value = Value
		Else
			OStimAutoModeForeplayThresholdMin.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoModeForeplayThresholdMax Auto
int Property AutoModeForeplayThresholdMax
	int Function Get()
		Return OStimAutoModeForeplayThresholdMax.value As int
	EndFunction
	Function Set(int Value)
		If AutoModeForeplayThresholdMin > Value
			OStimAutoModeForeplayThresholdMax.value = OStimAutoModeForeplayThresholdMin.value
			OStimAutoModeForeplayThresholdMin.value = Value
		Else
			OStimAutoModeForeplayThresholdMax.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoModePulloutChance Auto
int Property AutoModePulloutChance
	int Function Get()
		Return OStimAutoModePulloutChance.value As int
	EndFunction
	Function Set(int Value)
		OStimAutoModePulloutChance.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimAutoModePulloutThresholdMin Auto
int Property AutoModePulloutThresholdMin
	int Function Get()
		Return OStimAutoModePulloutThresholdMin.value As int
	EndFunction
	Function Set(int Value)
		If AutoModePulloutThresholdMax < Value
			OStimAutoModePulloutThresholdMin.value = OStimAutoModePulloutThresholdMax.value
			OStimAutoModePulloutThresholdMax.value = Value
		Else
			OStimAutoModePulloutThresholdMin.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoModePulloutThresholdMax Auto
int Property AutoModePulloutThresholdMax
	int Function Get()
		Return OStimAutoModePulloutThresholdMax.value As int
	EndFunction
	Function Set(int Value)
		If AutoModePulloutThresholdMin > Value
			OStimAutoModePulloutThresholdMax.value = OStimAutoModePulloutThresholdMin.value
			OStimAutoModePulloutThresholdMin.value = Value
		Else
			OStimAutoModePulloutThresholdMax.value = Value
		EndIf
	EndFunction
EndProperty


; -------------------------------------------------------------------------------------------------
; CAMERA SETTINGS  --------------------------------------------------------------------------------

GlobalVariable Property OStimUseFreeCam Auto
bool Property UseFreeCam
	bool Function Get()
		Return OStimUseFreeCam.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseFreeCam.value = 1
		Else
			OStimUseFreeCam.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimFreeCamFOV Auto
int Property FreecamFOV
	int Function Get()
		Return OStimFreeCamFOV.value As int
	EndFunction
	Function Set(int Value)
		OStimFreeCamFOV.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimFreeCamSpeed Auto
float Property FreecamSpeed
	float Function Get()
		Return OStimFreeCamSpeed.value
	EndFunction
	Function Set(float Value)
		OStimFreeCamSpeed.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimForceFirstPersonOnEnd Auto
Bool Property ForceFirstPersonAfter
	bool Function Get()
		Return OStimForceFirstPersonOnEnd.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimForceFirstPersonOnEnd.value = 1
		Else
			OStimForceFirstPersonOnEnd.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUseScreenShake Auto
Bool Property UseScreenShake
	bool Function Get()
		Return OStimUseScreenShake.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseScreenShake.value = 1
		Else
			OStimUseScreenShake.value = 0
		EndIf
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; EXCITEMENT SETTINGS  ----------------------------------------------------------------------------

GlobalVariable Property OStimMaleSexExcitementMult Auto
float Property MaleSexExcitementMult
	float Function Get()
		Return OStimMaleSexExcitementMult.value
	EndFunction
	Function Set(float value)
		OStimMaleSexExcitementMult.value = value
	EndFunction
EndProperty

GlobalVariable Property OStimFemaleSexExcitementMult Auto
float Property FemaleSexExcitementMult
	float Function Get()
		Return OStimFemaleSexExcitementMult.value
	EndFunction
	Function Set(float value)
		OStimFemaleSexExcitementMult.value = value
	EndFunction
EndProperty

GlobalVariable Property OStimExcitementDecayRate Auto
float Property ExcitementDecayRate
	float Function Get()
		Return OStimExcitementDecayRate.value
	EndFunction
	Function Set(float Value)
		OStimExcitementDecayRate.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimExcitementDecayGracePeriod Auto
int Property ExcitementDecayGracePeriod
	int Function Get()
		Return OStimExcitementDecayGracePeriod.value as int
	EndFunction
	Function Set(int Value)
		OStimExcitementDecayGracePeriod.value = Value
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; EXCITEMENT BAR SETTINGS  ------------------------------------------------------------------------

GlobalVariable Property OStimEnablePlayerBar Auto
bool Property EnablePlayerBar
	bool Function Get()
		Return OStimEnablePlayerBar.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimEnablePlayerBar.value = 1
		Else
			OStimEnablePlayerBar.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimEnableNpcBar Auto
bool Property EnableNpcBar
	bool Function Get()
		Return OStimEnableNpcBar.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimEnableNpcBar.value = 1
		Else
			OStimEnableNpcBar.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoHideBars Auto
Bool Property AutoHideBars
	bool Function Get()
		Return OStimAutoHideBars.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimAutoHideBars.value = 1
		Else
			OStimAutoHideBars.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimMatchBarColorToGender Auto
Bool Property MatchBarColorToGender
	bool Function Get()
		Return OStimMatchBarColorToGender.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimMatchBarColorToGender.value = 1
		Else
			OStimMatchBarColorToGender.value = 0
		EndIf
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; ORGASM SETTINGS  --------------------------------------------------------------------------------

GlobalVariable Property OStimEndOnPlayerOrgasm Auto
Bool Property EndOnPlayerOrgasm
	bool Function Get()
		Return OStimEndOnPlayerOrgasm.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimEndOnPlayerOrgasm.value = 1
		Else
			OStimEndOnPlayerOrgasm.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimEndOnMaleOrgasm Auto
Bool Property EndOnMaleOrgasm
	bool Function Get()
		Return OStimEndOnMaleOrgasm.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimEndOnMaleOrgasm.value = 1
		Else
			OStimEndOnMaleOrgasm.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimEndOnFemaleOrgasm Auto
Bool Property EndOnFemaleOrgasm
	bool Function Get()
		Return OStimEndOnFemaleOrgasm.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimEndOnFemaleOrgasm.value = 1
		Else
			OStimEndOnFemaleOrgasm.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimEndOnAllOrgasm Auto
Bool Property EndOnAllOrgasm
	bool Function Get()
		Return OStimEndOnAllOrgasm.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimEndOnAllOrgasm.value = 1
		Else
			OStimEndOnAllOrgasm.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimSlowMotionOnOrgasm Auto
Bool Property SlowMoOnOrgasm
	bool Function Get()
		Return OStimSlowMotionOnOrgasm.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimSlowMotionOnOrgasm.value = 1
		Else
			OStimSlowMotionOnOrgasm.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimBlurOnOrgasm Auto
Bool Property BlurOnOrgasm
	bool Function Get()
		Return OStimBlurOnOrgasm.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimBlurOnOrgasm.value = 1
		Else
			OStimBlurOnOrgasm.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAutoClimaxAnimations Auto
bool Property AutoClimaxAnimations
	bool Function Get()
		Return OStimAutoClimaxAnimations.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimAutoClimaxAnimations.value = 1
		Else
			OStimAutoClimaxAnimations.value = 0
		EndIf
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; UNDRESSING SETTINGS  ----------------------------------------------------------------------------

GlobalVariable Property OStimUndressAtStart Auto
Bool Property AlwaysUndressAtAnimStart
	bool Function Get()
		Return OStimUndressAtStart.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUndressAtStart.value = 1
		Else
			OStimUndressAtStart.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimRemoveWeaponsAtStart Auto
Bool Property RemoveWeaponsAtStart
	bool Function Get()
		Return OStimRemoveWeaponsAtStart.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimRemoveWeaponsAtStart.value = 1
		Else
			OStimRemoveWeaponsAtStart.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUndressMidScene Auto
Bool Property AutoUndressIfNeeded
	bool Function Get()
		Return OStimUndressMidScene.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUndressMidScene.value = 1
		Else
			OStimUndressMidScene.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimPartialUndressing Auto
Bool Property PartialUndressing
	bool Function Get()
		Return OStimPartialUndressing.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimPartialUndressing.value = 1
		Else
			OStimPartialUndressing.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimRemoveWeaponsWithSlot Auto
int Property RemoveWeaponsWithSlot
	int Function Get()
		Return OStimRemoveWeaponsWithSlot.value as int
	EndFunction
	Function Set(int Value)
		OStimRemoveWeaponsWithSlot.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimAnimateRedress Auto
Bool Property FullyAnimateRedress
	bool Function Get()
		Return OStimAnimateRedress.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimAnimateRedress.value = 1
		Else
			OStimAnimateRedress.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUndressWigs Auto
Bool Property UndressWigs
	bool Function Get()
		Return OStimUndressWigs.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUndressWigs.value = 1
		Else
			OStimUndressWigs.value = 0
		EndIf
	EndFunction
EndProperty

; changing the value of this global does not change the undressing behavior
; to change the undressing behavior you need to change the return value of OUndress.UsePapyrusUndressing()
; this global has a purely informative purpose, so consider it to be read only
GlobalVariable Property OStimUsePapyrusUndressing Auto
Bool Property UsePapyrusUndressing
	bool Function Get()
		Return OStimUsePapyrusUndressing.value != 0
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; GENDER ROLE SETTINGS  ---------------------------------------------------------------------------

GlobalVariable Property OStimIntendedSexOnly Auto
Bool Property IntendedSexOnly
	bool Function Get()
		Return OStimIntendedSexOnly.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimIntendedSexOnly.value = 1
		Else
			OStimIntendedSexOnly.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimPlayerAlwaysDomStraight Auto
Bool Property PlayerAlwaysDomStraight
	bool Function Get()
		Return OStimPlayerAlwaysDomStraight.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimPlayerAlwaysDomStraight.value = 1
		Else
			OStimPlayerAlwaysDomStraight.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimPlayerAlwaysSubStraight Auto
Bool Property PlayerAlwaysSubStraight
	bool Function Get()
		Return OStimPlayerAlwaysSubStraight.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimPlayerAlwaysSubStraight.value = 1
		Else
			OStimPlayerAlwaysSubStraight.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimPlayerAlwaysDomGay Auto
Bool Property PlayerAlwaysDomGay
	bool Function Get()
		Return OStimPlayerAlwaysDomGay.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimPlayerAlwaysDomGay.value = 1
		Else
			OStimPlayerAlwaysDomGay.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimPlayerAlwaysSubGay Auto
Bool Property PlayerAlwaysSubGay
	bool Function Get()
		Return OStimPlayerAlwaysSubGay.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimPlayerAlwaysSubGay.value = 1
		Else
			OStimPlayerAlwaysSubGay.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimPlayerSelectRoleStraight Auto
Bool Property PlayerSelectRoleStraight
	bool Function Get()
		Return OStimPlayerSelectRoleStraight.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimPlayerSelectRoleStraight.value = 1
		Else
			OStimPlayerSelectRoleStraight.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimPlayerSelectRoleGay Auto
Bool Property PlayerSelectRoleGay
	bool Function Get()
		Return OStimPlayerSelectRoleGay.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimPlayerSelectRoleGay.value = 1
		Else
			OStimPlayerSelectRoleGay.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimPlayerSelectRoleThreesome Auto
Bool Property PlayerSelectRoleThreesome
	bool Function Get()
		Return OStimPlayerSelectRoleThreesome.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimPlayerSelectRoleThreesome.value = 1
		Else
			OStimPlayerSelectRoleThreesome.value = 0
		EndIf
	EndFunction
EndProperty

Message Property OStimRoleSelectionMessage Auto
GlobalVariable Property OStimRoleSelectionCount Auto

; -------------------------------------------------------------------------------------------------
; STRAP-ON SETTINGS  ------------------------------------------------------------------------------

GlobalVariable Property OStimEquipStrapOnIfNeeded Auto
bool Property EquipStrapOnIfNeeded
	bool Function Get()
		Return OStimEquipStrapOnIfNeeded.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimEquipStrapOnIfNeeded.value = 1
		Else
			OStimEquipStrapOnIfNeeded.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUnequipStrapOnIfNotNeeded Auto
bool Property UnequipStrapOnIfNotNeeded
	bool Function Get()
		Return OStimUnequipStrapOnIfNotNeeded.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUnequipStrapOnIfNotNeeded.value = 1
		Else
			OStimUnequipStrapOnIfNotNeeded.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimUnequipStrapOnIfInWay Auto
bool Property UnequipStrapOnIfInWay
	bool Function Get()
		Return OStimUnequipStrapOnIfInWay.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUnequipStrapOnIfInWay.value = 1
		Else
			OStimUnequipStrapOnIfInWay.value = 0
		EndIf
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; FUTANARI SETTINGS  ------------------------------------------------------------------------------

GlobalVariable Property OStimUseSoSSex Auto
bool Property UseSoSSex
	bool Function Get()
		Return OStimUseSoSSex.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseSoSSex.value = 1
		Else
			OStimUseSoSSex.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimFutaUseMaleExcitement Auto
bool Property FutaUseMaleExcitement
	bool Function Get()
		Return OStimFutaUseMaleExcitement.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimFutaUseMaleExcitement.value = 1
		Else
			OStimFutaUseMaleExcitement.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimFutaUseMaleClimax Auto
bool Property FutaUseMaleClimax
	bool Function Get()
		Return OStimFutaUseMaleClimax.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimFutaUseMaleClimax.value = 1
		Else
			OStimFutaUseMaleClimax.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimFutaUseMaleLight Auto
bool Property FutaUseMaleLight
	bool Function Get()
		Return OStimFutaUseMaleLight.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimFutaUseMaleLight.value = 1
		Else
			OStimFutaUseMaleLight.value = 0
		EndIf
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; ALIGNMENT SETTINGS  -----------------------------------------------------------------------------

GlobalVariable Property OStimDisableScaling Auto
bool Property DisableScaling
	bool Function Get()
		Return OStimDisableScaling.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimDisableScaling.value = 1
		Else
			OStimDisableScaling.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimDisableSchlongBending Auto
bool Property DisableSchlongBending
	bool Function Get()
		Return OStimDisableSchlongBending.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimDisableSchlongBending.value = 1
		Else
			OStimDisableSchlongBending.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAlignmentGroupBySex Auto
bool Property AlignmentGroupBySex
	bool Function Get()
		Return OStimAlignmentGroupBySex.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimAlignmentGroupBySex.value = 1
		Else
			OStimAlignmentGroupBySex.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAlignmentGroupByHeight Auto
bool Property AlignmentGroupByHeight
	bool Function Get()
		Return OStimAlignmentGroupByHeight.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimAlignmentGroupByHeight.value = 1
		Else
			OStimAlignmentGroupByHeight.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimAlignmentGroupByHeels Auto
bool Property AlignmentGroupByHeels
	bool Function Get()
		Return OStimAlignmentGroupByHeels.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimAlignmentGroupByHeels.value = 1
		Else
			OStimAlignmentGroupByHeels.value = 0
		EndIf
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; FURNITURE SETTINGS  -----------------------------------------------------------------------------

GlobalVariable Property OStimUseFurniture Auto
bool Property UseFurniture
	bool Function Get()
		Return OStimUseFurniture.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimUseFurniture.value = 1
		Else
			OStimUseFurniture.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimSelectFurniture Auto
bool Property SelectFurniture
	bool Function Get()
		Return OStimSelectFurniture.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimSelectFurniture.value = 1
		Else
			OStimSelectFurniture.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimFurnitureSearchDistance Auto
int Property FurnitureSearchDistance
	int Function Get()
		Return OStimFurnitureSearchDistance.value As int
	EndFunction
	Function Set(int Value)
		OStimFurnitureSearchDistance.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimResetClutter Auto
bool Property ResetClutter
	bool Function Get()
		Return OStimResetClutter.value != 0
	EndFunction
	Function Set(bool Value)
		If Value
			OStimResetClutter.value = 1
		Else
			OStimResetClutter.value = 0
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimResetClutterRadius Auto
int Property ResetClutterRadius
	int Function Get()
		Return OStimResetClutterRadius.value As int
	EndFunction
	Function Set(int Value)
		OStimResetClutterRadius.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimBedRealignment Auto
float Property BedRealignment
	float Function Get()
		Return OStimBedRealignment.value
	EndFunction
	Function Set(float Value)
		OStimBedRealignment.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimBedOffset Auto
float Property BedOffset
	float Function Get()
		Return OStimBedOffset.value
	EndFunction
	Function Set(float Value)
		OStimBedOffset.value = Value
	EndFunction
EndProperty

Message Property OStimBedConfirmationMessage Auto
Message Property OStimFurnitureSelectionMessage Auto
GlobalVariable[] Property OStimFurnitureSelectionButtons Auto

; -------------------------------------------------------------------------------------------------
; EXPRESSION SETTINGS  ----------------------------------------------------------------------------

GlobalVariable Property OStimExpressionDurationMin Auto
int Property ExpressionDurationMin
	int Function Get()
		Return OStimExpressionDurationMin.value As int
	EndFunction
	Function Set(int Value)
		If ExpressionDurationMax < Value
			OStimExpressionDurationMin.value = OStimExpressionDurationMax.value
			OStimExpressionDurationMax.value = Value
		Else
			OStimExpressionDurationMin.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimExpressionDurationMax Auto
int Property ExpressionDurationMax
	int Function Get()
		Return OStimExpressionDurationMax.value As int
	EndFunction
	Function Set(int Value)
		If ExpressionDurationMin > Value
			OStimExpressionDurationMax.value = OStimExpressionDurationMin.value
			OStimExpressionDurationMin.value = Value
		Else
			OStimExpressionDurationMax.value = Value
		EndIf
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; SOUND SETTINGS  ---------------------------------------------------------------------------------

GlobalVariable Property OStimMoanIntervalMin Auto
int Property MoanIntervalMin
	int Function Get()
		Return OStimMoanIntervalMin.value As int
	EndFunction
	Function Set(int Value)
		If ExpressionDurationMax < Value
			OStimMoanIntervalMin.value = OStimMoanIntervalMax.value
			OStimMoanIntervalMax.value = Value
		Else
			OStimMoanIntervalMin.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimMoanIntervalMax Auto
int Property MoanIntervalMax
	int Function Get()
		Return OStimMoanIntervalMax.value As int
	EndFunction
	Function Set(int Value)
		If ExpressionDurationMin > Value
			OStimMoanIntervalMax.value = OStimMoanIntervalMin.value
			OStimMoanIntervalMin.value = Value
		Else
			OStimMoanIntervalMax.value = Value
		EndIf
	EndFunction
EndProperty

GlobalVariable Property OStimMoanVolume Auto
float Property MoanVolume
	float Function Get()
		Return OStimMoanVolume.value
	EndFunction
	Function Set(float Value)
		OStimMoanVolume.value = Value
	EndFunction
EndProperty

GlobalVariable Property OStimSoundVolume Auto
float Property SoundVolume
	float Function Get()
		Return OStimSoundVolume.value
	EndFunction
	Function Set(float Value)
		OStimSoundVolume.value = Value
	EndFunction
EndProperty

; -------------------------------------------------------------------------------------------------
; SCRIPTWIDE VARIABLES ----------------------------------------------------------------------------


Actor DomActor
Actor SubActor
Actor ThirdActor

Actor[] Actors

Bool SceneRunning
String[] CurrScene

Actor Property PlayerRef Auto

Bool Property UndressDom Auto
Bool Property UndressSub Auto
String StartingAnimation



Bool StallOrgasm

int FurnitureType
ObjectReference CurrentFurniture

Float StartTime

Float MostRecentOSexInteractionTime

Bool AggressiveThemedSexScene
Actor AggressiveActor

OBarsScript Property OBars Auto
OStimUpdaterScript OUpdater

Float DomStimMult
Float SubStimMult
Float ThirdStimMult

;--------- ID shortcuts

quest property subthreadquest auto 


; -------------------------------------------------------------------------------------------------
; BBLS/Migal mods stuff so we don't apply FaceLight over his actors who already have it -----------

Faction BBLS_FaceLightFaction
ActorBase Vayne
ActorBase Coralyn


; -------------------------------------------------------------------------------------------------
; -------------------------------------------------------------------------------------------------


Event OnInit()
	Console("OStim initializing")
	Startup() ; OStim install script
EndEvent


; Call this function to start a new OStim scene
;/* StartScene
* * starts an OStim scene, duh
* *
* * @param: Dom, the first actor, index 0, usually male
* * @param: Sub, the second actor, index 1, usually female
* * @param: zUndressDom, if True the first actor will get undressed no matter the MCM settings
* * @param: zUndressSub, if True the second actor will get undressed no matter the MCM settings
* * @param: zAnimateUndress, no longer in use
* * @param: zStartingAnimation, the animation to start with
* * @param: zThirdActor, the third actor, index 2
* * @param: Bed, the furniture to start the animation on, can be None
* * @param: Aggressive, if the scene is aggressive
* * @param: AggressingActor, the aggressor in an aggressive scene
*/;
Bool Function StartScene(Actor Dom, Actor Sub, Bool zUndressDom = False, Bool zUndressSub = False, Bool zAnimateUndress = False, String zStartingAnimation = "", Actor zThirdActor = None, ObjectReference Bed = None, Bool Aggressive = False, Actor AggressingActor = None)
	; If Player isn't involved, it's an NPC scene, so start it on a subthread instead
	If PlayerRef != Dom && PlayerRef != Sub && PlayerRef != zThirdActor
		if !GetUnusedSubthread().StartSubthreadScene(Dom, Sub, zThirdActor = zThirdActor, startingAnimation = zStartingAnimation, furnitureObj = Bed, withAI = true, isAggressive = Aggressive, aggressingActor = AggressingActor)
			Debug.Notification("OStim: Thread overload, please report this on Discord.")
			return false
		endif 
		return true
	EndIf

	If SceneRunning
		Debug.Notification("OStim scene already running")
		Return false
	EndIf

	If OActor.IsInOStim(Dom) || Sub && OActor.IsInOStim(Sub) || ThirdActor && OActor.IsInOStim(ThirdActor)
		Debug.Notification("At least one of the actors is already in an OStim scene.")
		Return false
	EndIf
	If !dom.Is3DLoaded()
		console("Dom actor is not loaded.")
		return false
	EndIf


	UndressDom = zUndressDom
	UndressSub = zUndressSub
	StartingAnimation = zStartingAnimation

	; set actor properties
	If zThirdActor
		Actors = new Actor[3]
		Actors[0] = Dom
		Actors[1] = Sub
		Actors[2] = zThirdActor
	ElseIf Sub
		Actors = new Actor[2]
		Actors[0] = Dom
		Actors[1] = Sub
	Else
		Actors = new Actor[1]
		Actors[0] = Dom
	EndIf

	; this actor order is a shitshow, MFM for threesomes wtf?
	; once we get our own UI and the xml scenes will become legacy we'll change this to MMF!
	; there has to be a better way to do this than lining up a ton of else ifs
	; maybe just remove all the player is this and that options since there is now a selection message box?
	; TODO: use OActor.SortActors once we went from MFM to MMF
	OStimRoleSelectionCount.value = Actors.Length
	If Actors.Length == 3
		If PlayerSelectRoleThreesome
			int PlayerIndex = OStimRoleSelectionMessage.Show()
			If PlayerIndex == 0
				If Actors[1] == PlayerRef
					Actors[1] = Actors[0]
					Actors[0] = PlayerRef
				ElseIf Actors[2] == PlayerRef
					Actors[2] = Actors[0]
					Actors[0] = PlayerRef
				EndIf
				If AppearsFemale(Actors[2]) && !AppearsFemale(Actors[1])
					Actor Temp = Actors[1]
					Actors[1] = Actors[2]
					Actors[2] = Temp
				EndIf
			ElseIf PlayerIndex == 1
				If Actors[0] == PlayerRef
					Actors[0] = Actors[1]
					Actors[1] = PlayerRef
				ElseIf Actors[2] == PlayerRef
					Actors[2] = Actors[1]
					Actors[1] = PlayerRef
				EndIf
				If AppearsFemale(Actors[0]) && !AppearsFemale(Actors[2])
					Actor Temp = Actors[0]
					Actors[0] = Actors[2]
					Actors[2] = Temp
				EndIf
			Else
				If Actors[0] == PlayerRef
					Actors[0] = Actors[2]
					Actors[2] = PlayerRef
				ElseIf Actors[1] == PlayerRef
					Actors[1] = Actors[2]
					Actors[2] = PlayerRef
				EndIf
				If AppearsFemale(Actors[0]) && !AppearsFemale(Actors[1])
					Actor Temp = Actors[0]
					Actors[0] = Actors[1]
					Actors[1] = Temp
				EndIf
			EndIf
		Else
			If AppearsFemale(Actors[0]) && !AppearsFemale(Actors[1])
				Actor Temp = Actors[0]
				Actors[0] = Actors[1]
				Actors[1] = Temp
			EndIf
			If AppearsFemale(Actors[0]) && !AppearsFemale(Actors[2])
				Actor Temp = Actors[0]
				Actors[0] = Actors[2]
				Actors[2] = Temp
			EndIf
			If AppearsFemale(Actors[2]) && !AppearsFemale(Actors[1])
				Actor Temp = Actors[1]
				Actors[1] = Actors[2]
				Actors[2] = Temp
			EndIf
		EndIf
	ElseIf Actors.Length == 2
		If AppearsFemale(Actors[0]) == AppearsFemale(Actors[1]) ; gay
			If PlayerSelectRoleGay
				If OStimRoleSelectionMessage.Show() == 0
					If Actors[1] == PlayerRef
						Actors[1] = Actors[0]
						Actors[0] = PlayerRef
					EndIf
				Else
					If Actors[0] == PlayerRef
						Actors[0] = Actors[1]
						Actors[1] = PlayerRef
					EndIf
				EndIf
			ElseIf PlayerAlwaysDomGay
				If Actors[1] == PlayerRef
					Actors[1] = Actors[0]
					Actors[0] = PlayerRef
				EndIf
			ElseIf PlayerAlwaysSubGay
				If Actors[0] == PlayerRef
					Actors[0] = Actors[1]
					Actors[1] = PlayerRef
				EndIf
			EndIf
		Else
			If PlayerSelectRoleStraight
				If OStimRoleSelectionMessage.Show() == 0
					If Actors[1] == PlayerRef
						Actors[1] = Actors[0]
						Actors[0] = PlayerRef
					EndIf
				Else
					If Actors[0] == PlayerRef
						Actors[0] = Actors[1]
						Actors[1] = PlayerRef
					EndIf
				EndIf
			ElseIf PlayerAlwaysDomStraight
				If Actors[1] == PlayerRef
					Actors[1] = Actors[0]
					Actors[0] = PlayerRef
				EndIf
			ElseIf PlayerAlwaysSubStraight
				If Actors[0] == PlayerRef
					Actors[0] = Actors[1]
					Actors[1] = PlayerRef
				EndIf
			Else
				If AppearsFemale(Actors[0]) && !AppearsFemale(Actors[1])
					Actor Temp = Actors[0]
					Actors[0] = Actors[1]
					Actors[1] = Temp
				EndIf
			EndIf
		EndIf
	EndIf

	DomActor = Actors[0]
	If Actors.Length >= 2
		SubActor = Actors[1]
	Else
		SubActor = None
	EndIf
	If Actors.Length == 3
		ThirdActor = Actors[2]
	Else
		ThirdActor = None
	EndIf

	If !OActor.VerifyActors(Actors)
		Debug.Notification("At least one of the actors is invalid.")
		Return false
	EndIf

	If (Aggressive)
		If (AggressingActor)
			If ((AggressingActor != SubActor) && (AggressingActor != DomActor))
				debug.MessageBox("Programmer:  The aggressing Actor you entered is not part of the scene!")
				Return False
			Else
				AggressiveActor = AggressingActor
				AggressiveThemedSexScene = True
			EndIf
		Else
			Debug.MessageBox("Programmer: Please enter the aggressor in this aggressive animation")
			Return False
		EndIf
	Else
		AggressiveThemedSexScene = False
		AggressingActor = None
	EndIf

	If (Bed)
		FurnitureType = OFurniture.GetFurnitureType(Bed)
		CurrentFurniture = Bed
	Else
		FurnitureType = FURNITURE_TYPE_NONE
		CurrentFurniture = None
	EndIf

	Console("Requesting scene start")
	RegisterForSingleUpdate(0.01) ; start main loop
	SceneRunning = True

	Return True
EndFunction

Event OnUpdate() ;OStim main logic loop
	Console("Starting scene asynchronously")

	OSANative.EndPlayerDialogue()

	bool InDialogue = false
	int i = Actors.Length
	While i
		i -= 1
		InDialogue = InDialogue || Actors[i].IsInDialogueWithPlayer()
	EndWhile

	While InDialogue
		InDialogue = false
		Utility.Wait(0.3)
		i = Actors.Length
		While i
			i -= 1
			InDialogue = InDialogue || Actors[i].IsInDialogueWithPlayer()
		EndWhile
	EndWhile

	If (UseFades)
		FadeToBlack()
	EndIf

	If FurnitureType == FURNITURE_TYPE_NONE && UseFurniture
		If StartingAnimation == ""
			SelectFurniture()

			If FurnitureType == FURNITURE_TYPE_CANCEL
				If (UseFades)
					FadeFromBlack()
				EndIf
				SceneRunning = False
				Return
			EndIf
		Else
			CurrentFurniture = FindBed(Actors[0])
			If CurrentFurniture && (!SelectFurniture || OStimBedConfirmationMessage.Show() == 0)
				FurnitureType == FURNITURE_TYPE_BED
			Else
				CurrentFurniture = None
			EndIf
		EndIf
	EndIf

	string SceneTag = "idle"
	If UseIntroScenes
		SceneTag = "intro"
	EndIf

	If (StartingAnimation == "")
		If FurnitureType == FURNITURE_TYPE_NONE
			StartingAnimation = OLibrary.GetRandomSceneWithAnySceneTagAndAnyMultiActorTagForAllCSV(Actors, SceneTag, OCSV.CreateCSVMatrix(Actors.Length, "standing"))
		ElseIf FurnitureType == FURNITURE_TYPE_BED
			StartingAnimation = OLibrary.GetRandomSceneWithAnySceneTagAndAnyMultiActorTagForAllCSV(Actors, SceneTag, OCSV.CreateCSVMatrix(Actors.Length, "allfours,kneeling,lyingback,lyingside,sitting"))
		Else
			StartingAnimation = OLibrary.GetRandomFurnitureSceneWithSceneTag(Actors, FURNITURE_TYPE_STRINGS[FurnitureType], SceneTag)
		EndIf
	EndIf

	If StartingAnimation == ""
		If (UseFades)
			FadeFromBlack()
		EndIf
		Debug.Notification("No valid starting animation found.")
		SceneRunning = False
		Return
	EndIf


	If SubActor
		If SubActor.GetParentCell() != DomActor.GetParentCell()
			If (SubActor == playerref)
				Domactor.moveto(SubActor)
			Else
				SubActor.MoveTo(DomActor)
			EndIf
		EndIf
	EndIf

	;ToggleActorAI(false)

	If (EnableImprovedCamSupport)
		Game.DisablePlayerControls(abCamswitch = True, abMenu = False, abFighting = False, abActivate = False, abMovement = False, aiDisablePOVType = 0)
	EndIf

 
	StallOrgasm = false
	DomStimMult = 1.0
	SubStimMult = 1.0
	ThirdStimMult = 1.0
	StallOrgasm = False
	MostRecentOrgasmedActor = None
	MostRecentOSexInteractionTime = Utility.GetCurrentRealTime()

	RegisterForModEvent("ostim_orgasm", "OstimOrgasm")

	If FurnitureType != FURNITURE_TYPE_NONE
		CurrentFurniture.BlockActivation(true)
	EndIf

	If (LowLightLevelLightsOnly && DomActor.GetLightLevel() < 20) || (!LowLightLevelLightsOnly)
		If (DomLightPos > 0)
			LightActor(DomActor, DomLightPos, DomLightBrightness)
		EndIf
		If (SubActor && SubLightPos > 0)
			LightActor(SubActor, SubLightPos, SubLightBrightness)
		EndIf
	EndIf

	OSANative.StartScene(0, CurrentFurniture, Actors)
	OSANative.ChangeAnimation(0, StartingAnimation)

	StartTime = Utility.GetCurrentRealTime()

	;ToggleActorAI(True)

	Float LoopStartTime
	
	If (UseFades)
		FadeFromBlack()
	EndIf

	If CurrentFurniture && ResetClutter
		OFurniture.ResetClutter(CurrentFurniture, ResetClutterRadius * 100)
	EndIf

	While OThread.IsRunning(0)
		Utility.Wait(1.0)
	EndWhile

	If (EnableImprovedCamSupport)
		Game.EnablePlayerControls(abCamSwitch = True)
	EndIf

	If CurrentFurniture && ResetClutter
		OFurniture.ResetClutter(CurrentFurniture, ResetClutterRadius * 100)
	EndIf

	If UseFades
		FadeFromBlack(2)
	EndIf

	Console(GetTimeSinceStart() + " seconds passed")

	oldscenemetadata = scenemetadata
	scenemetadata = PapyrusUtil.StringArray(0)

	SceneRunning = False

	If (FurnitureType != FURNITURE_TYPE_NONE)
		CurrentFurniture.BlockActivation(false)
		FurnitureType = FURNITURE_TYPE_NONE
		CurrentFurniture = None
	EndIf

	FurnitureType = 0
	CurrentFurniture = none

	OSANative.EndPlayerDialogue()
EndEvent

Function Masturbate(Actor Masturbator, Bool zUndress = False, Bool zAnimUndress = False, ObjectReference MBed = None)
	If !SoloAnimsInstalled()
		Debug.Notification("No solo animations installed")
		Return
	EndIf

	StartScene(Masturbator, None, zUndressDom = zUndress, zAnimateUndress = zAnimUndress, Bed = MBed)
EndFunction


;
;			██╗   ██╗████████╗██╗██╗     ██╗████████╗██╗███████╗███████╗
;			██║   ██║╚══██╔══╝██║██║     ██║╚══██╔══╝██║██╔════╝██╔════╝
;			██║   ██║   ██║   ██║██║     ██║   ██║   ██║█████╗  ███████╗
;			██║   ██║   ██║   ██║██║     ██║   ██║   ██║██╔══╝  ╚════██║
;			╚██████╔╝   ██║   ██║███████╗██║   ██║   ██║███████╗███████║
;			 ╚═════╝    ╚═╝   ╚═╝╚══════╝╚═╝   ╚═╝   ╚═╝╚══════╝╚══════╝
;
; 				The main API functions

; Most of what you want to do in OStim is available here, i advise reading through this entire Utilities section


OBarsScript Function GetBarScript()
	return obars
EndFunction

;/* GetAPIVersion
* * returns the current API version
* * 26 = old OStim
* * 27 = OStim NG 6.7 or earlier
* * 28 = OStim NG 6.8
* *
* * @return: the version of the current API
*/;
Int Function GetAPIVersion()
	Return 28
EndFunction

Bool Function ActorHasFacelight(Actor Act)
	{Checks if Actor already has FaceLight. Currently we only check BBLS NPCs and Vayne}
	If (BBLS_FaceLightFaction && Act.GetFactionRank(BBLS_FaceLightFaction) >= 0)
		return true
	EndIf

	If (Vayne && Act.GetActorBase() == Vayne)
		; Vayne's facelight can be turned on or off in her MCM menu
		; The below GlobalVariable tells us if Vayne's facelight is currently on or off
		return (Game.GetFormFromFile(0x0004B26B, "CS_Vayne.esp") as GlobalVariable).GetValueInt() == 1
	EndIf

	If (Coralyn && Act.GetActorBase() == Coralyn)
		; Coralyn's facelight can be turned on or off in her MCM menu
		; The below GlobalVariable tells us if Coralyn's facelight is currently on or off
		return (Game.GetFormFromFile(0x0004D91E, "CS_Coralyn.esp") as GlobalVariable).GetValueInt() == 1
	EndIf

	return false
EndFunction

Function LightActor(Actor Act, Int Pos, Int Brightness) ; pos 1 - ass, pos 2 - face | brightness - 0 = dim
	If (Pos == 0)
		Return
	EndIf

	String Which
	If (Pos == 1) ; ass
		If (Brightness == 0)
			Which = "AssDim"
		Else
			Which = "AssBright"
		EndIf
	ElseIf (Pos == 2) ;face
		If (!ActorHasFacelight(Act))
			If (Brightness == 0)
				Which = "FaceDim"
			Else
				Which = "FaceBright"
			EndIf
		Else
			Console(Act.GetActorBase().GetName() + " already has facelight, not applying it")
			return
		EndIf
	EndIf
EndFunction

bool Function AutoTransitionForActor(Actor Act, string Type)
	Return AutoTransitionForPosition(Actors.Find(Act), Type)
EndFunction

Function ToggleActorAI(bool enable)
	int i = Actors.Length
	While i
		i -= 1
		Actors[i].EnableAI(enable)
	EndWhile
EndFunction

Function EndAnimation(Bool SmoothEnding = True)
	If (AnimationRunning() && UseFades && SmoothEnding)
		FadeToBlack(1.5)
	EndIf
	Console("Trying to end scene")

	OThread.Stop(0)
EndFunction

Bool Function IsSceneAggressiveThemed() ; if the entire situation should be themed aggressively
	Return AggressiveThemedSexScene
EndFunction

Actor Function GetAggressiveActor()
	Return AggressiveActor
EndFunction

bool Function IsVictim(actor act)
	return AggressiveThemedSexScene && (act != AggressiveActor)
endfunction 

Actor Function GetSexPartner(Actor Char)
	If (Char == SubActor)
		Return DomActor
	EndIf
	Return SubActor
EndFunction

Actor Function GetActor(int Index)
	If Index >= 0 && Index < Actors.Length
		Return Actors[Index]
	EndIf

	Return None
EndFunction

; do not modify this array or OStim will break!
Actor[] Function GetActors()
	Return Actors
EndFunction

;/
Function SwapActorOrder() ; Swaps dom position in animation for sub. Only effects the animation scene. 2p scenes only
    if Actors.Length == 2
        UI.Invoke("HUD Menu", diasa + ".arrangeActra")
    endif
EndFunction
/;

Function AddSceneMetadata(string MetaTag)
	scenemetadata = PapyrusUtil.PushString(scenemetadata, MetaTag)
EndFunction

bool Function HasSceneMetadata(string MetaTag)
	string[] metadata
	if SceneRunning
		metadata = scenemetadata
	else 
		metadata = oldscenemetadata
	endif 

	return metadata.Find(metatag) >= 0
EndFunction

string[] Function GetAllSceneMetadata()
	return scenemetadata
EndFunction

; Warps to all of the scene IDs in the array.
; Does not do any waiting on it's own. To do that, you can add in numbers into the list, 
; and the function will wait that amount of time
; i.e. [sceneID, sceneID, "3", sceneID]
Function PlayAnimationSequence(String[] list)
	; TODO
EndFunction

function FadeFromBlack(float time = 4.0)
	Game.FadeOutGame(False, True, 0.0, time) ; welcome back
EndFunction

function FadeToBlack(float time = 1.25)
		Game.FadeOutGame(True, True, 0.0, Time)
		Utility.Wait(Time * 0.70)
		Game.FadeOutGame(False, True, 25.0, 25.0) ; total blackout
EndFunction

Float Function GetTimeSinceLastPlayerInteraction()
	Return Utility.GetCurrentRealTime() - MostRecentOSexInteractionTime
EndFunction

Bool Function UsingBed()
	Return FurnitureType == FURNITURE_TYPE_BED
EndFunction

Bool Function UsingFurniture()
	Return FurnitureType != FURNITURE_TYPE_NONE
EndFunction

string Function GetFurnitureType()
	Return FURNITURE_TYPE_STRINGS[FurnitureType]
EndFunction

ObjectReference Function GetFurniture()
	Return CurrentFurniture
EndFunction

Bool Function IsFemale(Actor Act)
	{genitalia based / has a vagina and not a penis}
	If !Act
		Return False
	EndIf

	Return !OActor.HasSchlong(Act)
EndFunction

Bool Function AppearsFemale(Actor Act) 
	{gender based / looks like a woman but can have a penis}
	Return OSANative.GetSex(OSANative.GetLeveledActorBase(act)) == 1
EndFunction

String[] Function GetScene()
	{this is not the sceneID, this is an internal osex thing}
	Return CurrScene
EndFunction

Function HideAllSkyUIWidgets() ; DEPRECIATED
	outils.SetSkyUIWidgetsVisible(false)
EndFunction

Function ShowAllSkyUIWidgets()
	outils.SetSkyUIWidgetsVisible(true)
EndFunction


float Function GetStimMult(Actor Act)
	If (Act == DomActor)
		Return DomStimMult
	Elseif (Act == SubActor)
		Return SubStimMult
	Elseif (Act == ThirdActor)
		Return ThirdStimMult
	EndIf
EndFunction

Function SetStimMult(Actor Act, Float Value)
	If (Act == DomActor)
		DomStimMult = Value
	Elseif (Act == SubActor)
		SubStimMult = Value
	Elseif (Act == ThirdActor)
		ThirdStimMult = Value
	EndIf
EndFunction

Function ModifyStimMult(actor act, float by)
	{thread-safe stimulation modification. Highly recomended you use this over Set.}
	OUtils.lock("mtx_stimmult")
	SetStimMult(act, GetStimMult(act) + by)
	osanative.unlock("mtx_stimmult")
endfunction

bool Function AutoTransitionForPosition(int Position, string Type)
	string SceneId = OMetadata.GetAutoTransitionForActor(OThread.GetScene(0), Position, Type)
	If SceneId == ""
		Return false
	EndIf

	WarpToAnimation(SceneId)
	Return true
EndFunction

bool Function IsBeingStimulated(Actor act)
	return (GetCurrentStimulation(act) * GetStimMult(act)) > 0.01
EndFunction

OStimSubthread Function GetUnusedSubthread()
	int i = 0
	int max = subthreadquest.GetNumAliases()
	while i < max 
		OStimSubthread thread = subthreadquest.GetNthAlias(i) as OStimSubthread

		if !thread.IsInUse()
			return thread 
		endif 

		i += 1
	endwhile
EndFunction

OStimSubthread Function GetSubthread(int id)
	OStimSubthread ret = subthreadquest.GetNthAlias(id) as OStimSubthread
	if !ret 
		Console("Subthread not found")
	endif 
	return ret
EndFunction

float Function GetTimeSinceStart()
	return Utility.GetCurrentRealTime() - StartTime
EndFunction

;
;			██████╗ ███████╗██████╗ ███████╗
;			██╔══██╗██╔════╝██╔══██╗██╔════╝
;			██████╔╝█████╗  ██║  ██║███████╗
;			██╔══██╗██╔══╝  ██║  ██║╚════██║
;			██████╔╝███████╗██████╔╝███████║
;			╚═════╝ ╚══════╝╚═════╝ ╚══════╝
;
;				Code related to beds

Function SelectFurniture()
	ObjectReference[] Furnitures = OFurniture.FindFurniture(Actors.Length, Actors[0], (FurnitureSearchDistance + 1) * 100.0, 96)
	If !SelectFurniture
		int i = 0
		While i < Furnitures.Length
			If Furnitures[i]
				CurrentFurniture = Furnitures[i]
				FurnitureType = i + 1
				Return
			EndIf
			i += 1
		EndWhile
	Else
		int i = 0
		bool hasValid = False
		While i < Furnitures.Length
			If Furnitures[i]
				OStimFurnitureSelectionButtons[i].Value = 1
				hasValid = True
			Else
				OStimFurnitureSelectionButtons[i].Value = 0
			EndIf
			i += 1
		EndWhile

		If !hasValid
			Return
		EndIf

		FurnitureType = OStimFurnitureSelectionMessage.Show()
		If FurnitureType == 0
			CurrentFurniture = None
		Else
			CurrentFurniture = Furnitures[FurnitureType - 1]
		EndIf
	EndIf
EndFunction

ObjectReference Function FindBed(ObjectReference CenterRef, Float Radius = 0.0)
	If !(Radius > 0.0)
		; we are searching from the center of the bed
		; center to edge of the bed is about 1 meter / 100 units
		Radius = (FurnitureSearchDistance + 1) * 100.0
	EndIf

	ObjectReference[] Beds = OSANative.FindBed(CenterRef, Radius, 96.0)

	ObjectReference NearRef = None

	Int i = 0
	Int L = Beds.Length

	While (i < L)
		ObjectReference Bed = Beds[i]
		If (!Bed.IsFurnitureInUse())
			NearRef = Bed
			i = L
		Else
			i += 1
		EndIf
	EndWhile

	If (NearRef)
		Return NearRef
	EndIf

	Return None
EndFunction

Bool Function SameFloor(ObjectReference BedRef, Float Z, Float Tolerance = 128.0)
	Return (Math.Abs(Z - BedRef.GetPositionZ())) <= Tolerance
EndFunction

Bool Function CheckBed(ObjectReference BedRef, Bool IgnoreUsed = True)
	Return BedRef && BedRef.IsEnabled() && BedRef.Is3DLoaded()
EndFunction


;
;			 ██████╗ ███████╗███████╗██╗  ██╗     ██████╗ ███████╗██╗      █████╗ ████████╗███████╗██████╗     ███████╗██╗   ██╗███████╗███╗   ██╗████████╗███████╗
;			██╔═══██╗██╔════╝██╔════╝╚██╗██╔╝     ██╔══██╗██╔════╝██║     ██╔══██╗╚══██╔══╝██╔════╝██╔══██╗    ██╔════╝██║   ██║██╔════╝████╗  ██║╚══██╔══╝██╔════╝
;			██║   ██║███████╗█████╗   ╚███╔╝█████╗██████╔╝█████╗  ██║     ███████║   ██║   █████╗  ██║  ██║    █████╗  ██║   ██║█████╗  ██╔██╗ ██║   ██║   ███████╗
;			██║   ██║╚════██║██╔══╝   ██╔██╗╚════╝██╔══██╗██╔══╝  ██║     ██╔══██║   ██║   ██╔══╝  ██║  ██║    ██╔══╝  ╚██╗ ██╔╝██╔══╝  ██║╚██╗██║   ██║   ╚════██║
;			╚██████╔╝███████║███████╗██╔╝ ██╗     ██║  ██║███████╗███████╗██║  ██║   ██║   ███████╗██████╔╝    ███████╗ ╚████╔╝ ███████╗██║ ╚████║   ██║   ███████║
;			 ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═════╝     ╚══════╝  ╚═══╝  ╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝
;
;				Event hooks that receive data from OSA

;
;			███████╗████████╗██╗███╗   ███╗██╗   ██╗██╗      █████╗ ████████╗██╗ ██████╗ ███╗   ██╗    ███████╗██╗   ██╗███████╗████████╗███████╗███╗   ███╗
;			██╔════╝╚══██╔══╝██║████╗ ████║██║   ██║██║     ██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║    ██╔════╝╚██╗ ██╔╝██╔════╝╚══██╔══╝██╔════╝████╗ ████║
;			███████╗   ██║   ██║██╔████╔██║██║   ██║██║     ███████║   ██║   ██║██║   ██║██╔██╗ ██║    ███████╗ ╚████╔╝ ███████╗   ██║   █████╗  ██╔████╔██║
;			╚════██║   ██║   ██║██║╚██╔╝██║██║   ██║██║     ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║    ╚════██║  ╚██╔╝  ╚════██║   ██║   ██╔══╝  ██║╚██╔╝██║
;			███████║   ██║   ██║██║ ╚═╝ ██║╚██████╔╝███████╗██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║    ███████║   ██║   ███████║   ██║   ███████╗██║ ╚═╝ ██║
;			╚══════╝   ╚═╝   ╚═╝╚═╝     ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝    ╚══════╝   ╚═╝   ╚══════╝   ╚═╝   ╚══════╝╚═╝     ╚═╝
;
;				All code related to the stimulation simulation


Float Function GetCurrentStimulation(Actor Act) ; how much an Actor is being stimulated in the current animation
	;TODO: Return this from c++?
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

Event OstimOrgasm(String EventName, String sceneId, Float index, Form Sender)
	Actor Act = Sender As Actor
	MostRecentOrgasmedActor = Act

	; Fertility Mode compatibility
	int actionIndex = OMetadata.FindActionForActor(sceneId, index as int, "vaginalsex")
	If  actionIndex != -1
		Actor impregnated = GetActor(OMetadata.GetActionTarget(sceneId, actionIndex))
		If impregnated
			int handle = ModEvent.Create("FertilityModeAddSperm")
			If handle
				ModEvent.PushForm(handle, impregnated)
				ModEvent.PushString(handle, Act.GetDisplayName())
				ModEvent.PushForm(handle, Act)
				ModEvent.Send(handle)
			EndIf
		EndIf
	EndIf
EndEvent

Function SetOrgasmStall(Bool Set)
	StallOrgasm = Set
EndFunction

Bool Function GetOrgasmStall()
	Return StallOrgasm
EndFunction

; Faces

Function MuteFaceData(Actor Act)
	Act.AddToFaction(OstimNoFacialExpressionsFaction)
EndFunction

Function UnMuteFaceData(Actor Act)
	Act.RemoveFromFaction(OstimNoFacialExpressionsFaction)

	int i = Actors.Find(Act)
	If i >= 0
		OActor.ClearExpression(Act)
	EndIf
EndFunction

Bool Function FaceDataIsMuted(Actor Act)
	Return Act.IsInFaction(OstimNoFacialExpressionsFaction)
EndFunction


;			███████╗ ██████╗ ██╗   ██╗███╗   ██╗██████╗
;			██╔════╝██╔═══██╗██║   ██║████╗  ██║██╔══██╗
;			███████╗██║   ██║██║   ██║██╔██╗ ██║██║  ██║
;			╚════██║██║   ██║██║   ██║██║╚██╗██║██║  ██║
;			███████║╚██████╔╝╚██████╔╝██║ ╚████║██████╔╝
;			╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝╚═════╝
;
;				Code related to Sound

;/* SendExpressionEvent
* * plays the event expression and if it is valid resets the expression when it's over
* * contains a Utility::Wait call, so best only call this from event listeners
*/;
Function SendExpressionEvent(Actor Act, string EventName)
	int Position = Actors.find(Act)
	If Position < 0
		Return
	EndIf

	float Duration = OActor.PlayExpression(Act, EventName)
	If Duration != -1
		Utility.Wait(Duration)
		OActor.ClearExpression(Act)
	EndIf
EndFunction




;			 ██████╗ ████████╗██╗  ██╗███████╗██████╗
;			██╔═══██╗╚══██╔══╝██║  ██║██╔════╝██╔══██╗
;			██║   ██║   ██║   ███████║█████╗  ██████╔╝
;			██║   ██║   ██║   ██╔══██║██╔══╝  ██╔══██╗
;			╚██████╔╝   ██║   ██║  ██║███████╗██║  ██║
;			 ╚═════╝    ╚═╝   ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝
;
;				Misc stuff
;				https://textkool.com/en/ascii-art-generator?hl=default&vl=default&font=ANSI%20Shadow&text=Other


Function Console(String In) Global
	MiscUtil.PrintConsole("OStim: " + In)
EndFunction

Bool Function ChanceRoll(Int Chance) ; input 60: 60% of returning true ;DEPRECIATED - moving to outils in future ver


	return OUtils.ChanceRoll(chance)

EndFunction

Function ShakeController(Float Power, Float Duration = 0.1)
	If UseRumble
		Game.ShakeController(Power, Power, Duration)
	EndIf
EndFunction

Function DisplayToastAsync(string txt, float lengthOftime)

	RegisterForModEvent("ostim_toast", "DisplayToastEvent")

	int handle = ModEvent.Create("ostim_toast")
	ModEvent.PushString(handle, txt)
	ModEvent.Pushfloat(handle, lengthOftime)

	ModEvent.send(handle)
endfunction

Event DisplayToastEvent(string txt, float time)
	outils.DisplayToastText(txt, time)
EndEvent

Bool Function GetGameIsVR()
	Return (PapyrusUtil.GetScriptVersion() == 36) ;obviously this no guarantee but it's the best we've got for now
EndFunction

Function ResetState()
	Console("Resetting thread state")
	SceneRunning = False

	int i = 0 
	Actor[] a = GetActors()
	while i < a.Length
		a[i].dispelallspells()
		i += 1
	endwhile
EndFunction

Float ProfileTime 
Function Profile(String Name = "")
	{Call Profile() to start. Call Profile("any string") to pring out the time since profiler started in console. Most accurate at 60fps}
	If (Name == "")
		ProfileTime = Game.GetRealHoursPassed() * 60 * 60
	Else
		float seconds = ((Game.GetRealHoursPassed() * 60 * 60) - ProfileTime - 0.016)
		float ms = seconds * 1000
		If seconds < 0.0
			Console(Name + ": Too fast to measure")
			Debug.Trace("Ostim: "+Name+" : Too fast to measure")
		else 
			Console(Name + ": " + seconds + " seconds (" + ms + " milliseconds)")
			Debug.Trace("Ostim: "+Name+": " + seconds + " seconds (" + ms + " milliseconds)")
		endif 
	EndIf
EndFunction

Bool Property SoSInstalled Auto
Faction SoSFaction

int rnd_s1
int rnd_s2
int rnd_s3


int Function RandomInt(int min = 0, int max = 100) ;DEPRECIATED - moving to osanative in future ver
	return OSANative.RandomInt(min, max)
EndFunction 

; Set initial seed values for the RNG. 
Function ResetRandom()
	return
EndFunction

Function Startup()
	LoadRegistrations = PapyrusUtil.FormArray(0, none)

	InstalledVersion = GetAPIVersion()

	SceneRunning = False

	OUpdater = Game.GetFormFromFile(0x000D67, "Ostim.esp") as OStimUpdaterScript

	scenemetadata = PapyrusUtil.StringArray(0)

	OnLoadGame()

	OUtils.DisplayTextBanner("OStim installed.")
EndFunction

Form[] LoadRegistrations 

Function RegisterForGameLoadEvent(form f)
	{Make a "Event OnGameLoad()" in the scripts attatched to the form you send and the event is called on game load}
	; Note the database is reset when ostim is updated so you should only use this if you also use OUpdater in your mod so you reregister

	OUtils.lock("mtx_os_registerload")

	LoadRegistrations = PapyrusUtil.PushForm(LoadRegistrations, f)
	Console("Registered for load event: " + f.getname())

	OSANative.unlock("mtx_os_registerload")
EndFunction 

Function SendLoadGameEvent()
	int l = LoadRegistrations.Length

	if l > 0
		int i = 0 

		while i < l 
			LoadRegistrations[i].RegisterForModEvent("ostim_gameload", "OnGameLoad")
			ModEvent.Send(ModEvent.Create("ostim_gameload"))
			LoadRegistrations[i].UnregisterForModEvent("ostim_gameload")



			Utility.Wait(0.5)
			i += 1
		endWhile

	endif

EndFunction

Function OnLoadGame()
	If (Game.GetModByName("Schlongs of Skyrim.esp") != 255)
		SoSFaction = (Game.GetFormFromFile(0x0000AFF8, "Schlongs of Skyrim.esp")) as Faction
	Else
		SoSFaction = none
	EndIf
	SoSInstalled = SoSFaction

	If SKSE.GetPluginVersion("ImprovedCameraSE") != -1
		OStimImprovedCamSupport.value = 1
	Else
		OStimImprovedCamSupport.value = 0
	EndIf
		
	OBars.OnGameLoad()

	SendLoadGameEvent()

	BBLS_FaceLightFaction = Game.GetFormFromFile(0x00755331, "BBLS_SKSE64_Patch.esp") as Faction
	Vayne = Game.GetFormFromFile(0x0000083D, "CS_Vayne.esp") as ActorBase
	Coralyn = Game.GetFormFromFile(0x0000080A, "CS_Coralyn.esp") as ActorBase

	FURNITURE_TYPE_STRINGS = new string[8]
	FURNITURE_TYPE_STRINGS[0] = ""
	FURNITURE_TYPE_STRINGS[1] = "bed"
	FURNITURE_TYPE_STRINGS[2] = "bench"
	FURNITURE_TYPE_STRINGS[3] = "chair"
	FURNITURE_TYPE_STRINGS[4] = "table"
	FURNITURE_TYPE_STRINGS[5] = "shelf"
	FURNITURE_TYPE_STRINGS[6] = "wall"
	FURNITURE_TYPE_STRINGS[7] = "cookingpot"

	POSITION_TAGS = new string[16]
	POSITION_TAGS[0]  = "allfours"
	POSITION_TAGS[1]  = "bendover"
	POSITION_TAGS[2]  = "facingaway"
	POSITION_TAGS[3]  = "handstanding"
	POSITION_TAGS[4]  = "kneeling"
	POSITION_TAGS[5]  = "lyingback"
	POSITION_TAGS[6]  = "facingaway"
	POSITION_TAGS[7]  = "lyingfront"
	POSITION_TAGS[8]  = "lyingside"
	POSITION_TAGS[9]  = "onbottom"
	POSITION_TAGS[10] = "ontop"
	POSITION_TAGS[11] = "sitting"
	POSITION_TAGS[12] = "spreadlegs"
	POSITION_TAGS[13] = "squatting"
	POSITION_TAGS[14] = "standing"
	POSITION_TAGS[15] = "suspended"

	if GetAPIVersion() != InstalledVersion
		OUtils.ForceOUpdate()
	endif
EndFunction


; ██████╗ ███████╗██████╗ ██████╗ ███████╗ ██████╗ █████╗ ████████╗███████╗██████╗ 
; ██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗╚══██╔══╝██╔════╝██╔══██╗
; ██║  ██║█████╗  ██████╔╝██████╔╝█████╗  ██║     ███████║   ██║   █████╗  ██║  ██║
; ██║  ██║██╔══╝  ██╔═══╝ ██╔══██╗██╔══╝  ██║     ██╔══██║   ██║   ██╔══╝  ██║  ██║
; ██████╔╝███████╗██║     ██║  ██║███████╗╚██████╗██║  ██║   ██║   ███████╗██████╔╝
; ╚═════╝ ╚══════╝╚═╝     ╚═╝  ╚═╝╚══════╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═════╝ 

; all of these are only here to not break old addons, don't use them in new addons, use whatever they're calling instead

Faction Property NVCustomOrgasmFaction Auto
int[] property SoundFormNumberWhitelist auto

bool Property UseAINPConNPC
	bool Function Get()
		Return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

bool Property ShowTutorials
	bool Function Get()
		Return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

int Property DefaultFOV
	int Function Get()
		Return 85
	EndFunction
	Function Set(int Value)
	EndFunction
EndProperty

bool Property HideBarsInNPCScenes
	bool Function Get()
		return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

bool Property UseStrongerUnequipMethod
	bool Function Get()
		Return false
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

bool Property TossClothesOntoGround
	bool Function Get()
		Return false
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

bool Property OrgasmIncreasesRelationship
	bool Function Get()
		Return false
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

bool Property UseNativeFunctions
	bool Function Get()
		Return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

float Property SexExcitementMult
	float Function Get()
		Return MaleSexExcitementMult
	EndFunction
	Function Set(float Value)
		MaleSexExcitementMult = Value
	EndFunction
EndProperty

bool Property UseBed
	bool Function Get()
		Return UseFurniture
	EndFunction
	Function Set(bool Value)
		UseFurniture = Value
	EndFunction
EndProperty

bool Property AllowUnlimitedSpanking
	bool Function Get()
		Return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

bool Property OnlyGayAnimsInGayScenes
	bool Function Get()
		Return IntendedSexOnly
	EndFunction
	Function Set(bool Value)
		IntendedSexOnly = Value
	EndFunction
EndProperty

bool Property EndOnDomOrgasm
	bool Function Get()
		Return EndOnMaleOrgasm
	EndFunction
	Function Set(bool Value)
		EndOnMaleOrgasm = Value
	EndFunction
EndProperty

bool Property EndOnSubOrgasm
	bool Function Get()
		Return EndOnFemaleOrgasm
	EndFunction
	Function Set(bool Value)
		EndOnFemaleOrgasm = Value
	EndFunction
EndProperty

bool Property RequireBothOrgasmsToFinish
	bool Function Get()
		Return EndOnAllOrgasm
	EndFunction
	Function Set(bool Value)
		EndOnAllOrgasm = Value
	EndFunction
EndProperty

Bool Property EnableDomBar
	bool Function Get()
		Return EnablePlayerBar
	EndFunction
	Function Set(bool Value)
		EnablePlayerBar = Value
	EndFunction
EndProperty

Bool Property EnableSubBar
	bool Function Get()
		Return EnableNpcBar
	EndFunction
	Function Set(bool Value)
		EnableNpcBar = Value
	EndFunction
EndProperty

Bool Property EnableThirdBar
	bool Function Get()
		Return EnableNpcBar
	EndFunction
	Function Set(bool Value)
		EnableNpcBar = Value
	EndFunction
EndProperty

Bool Property UseBrokenCosaveWorkaround
	bool Function Get()
		Return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

Bool Property EndAfterActorHit
	bool Function Get()
		Return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

int Property AiSwitchChance
	int Function Get()
		Return 0
	EndFunction
	Function Set(int Value)
	EndFunction
EndProperty

Bool property ForceCloseOStimThread
	bool Function Get()
		Return false
	EndFunction
	Function Set(bool Value)
		If Value
			EndAnimation(false)
		EndIf
	EndFunction
EndProperty

bool Property SkipEndingFadein
	bool Function Get()
		Return false
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

bool Property Installed
	bool Function Get()
		Return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

Bool Property BlockVRInstalls
	bool Function Get()
		Return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

Bool Property DisableStimulationCalculation
	bool Function Get()
		Return false
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

Bool Property PauseAI
	bool Function Get()
		Return OThread.IsInAutoMode(0)
	EndFunction
	Function Set(bool Value)
		If Value
			OThread.StartAutoMode(0)
		Else
			OThread.StopAutoMode(0)
		EndIf
	EndFunction
EndProperty

bool Property GetInBedAfterBedScene
	bool Function Get()
		Return false
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

Bool property EndedProper
	bool Function Get()
		Return true
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

Bool Property MuteOSA
	bool Function Get()
		Return false
	EndFunction
	Function Set(bool Value)
		If !AnimationRunning()
			Return
		EndIf

		; NV used this together with a whitelist to only mute the female moans
		; so we will do exactly that here
		If Value
			int i = Actors.Length
			While i
				i -= 1
				If AppearsFemale(Actors[i])
					OActor.Mute(Actors[i])
				EndIf
			EndWhile
		EndIf
	EndFunction
EndProperty

bool Property DisableOSAControls
	bool Function Get()
		return false
	EndFunction
	Function Set(bool Value)
	EndFunction
EndProperty

Actor Function GetDomActor()
	Return GetActor(0)
EndFunction

Actor Function GetSubActor()
	Return GetActor(1)
EndFunction

Actor Function GetThirdActor()
	Return GetActor(2)
EndFunction

ObjectReference Function GetBed()
	Return GetFurniture()
EndFunction

bool Function SoloAnimsInstalled()
	Actor[] _Actors = new Actor[1]
	_Actors[0] = None
	return OLibrary.GetRandomScene(_Actors) != ""
EndFunction

bool Function ThreesomeAnimsInstalled()
	Actor[] _Actors = new Actor[3]
	_Actors[0] = None
	_Actors[1] = None
	_Actors[2] = None
	return OLibrary.GetRandomScene(_Actors) != ""
EndFunction

Bool Function IsVaginal()
	Return OMetadata.FindAction(OThread.GetScene(0), "vaginalsex") != -1
EndFunction

Bool Function IsOral()
	; this method did not check for animation class VJ, so to keep it working as it was we don't check for cunnilingus
	Return OMetadata.FindAction(OThread.GetScene(0), "blowjob") != -1
EndFunction

Actor Function GetCurrentLeadingActor()
	int actorIndex = 0
	If OMetadata.HasActions(OThread.GetScene(0))
		actorIndex = OMetadata.GetActionPerformer(OThread.GetScene(0), 0)
	EndIf
	Return GetActor(actorIndex)
EndFunction

Bool Function GetCurrentAnimIsAggressive()
	int i = Actors.Length
	While i
		i -= 1
		If OMetadata.HasActorTag(OThread.GetScene(0), i, "aggressor")
			Return true
		EndIf
	EndWhile

	Return false
EndFunction

String Function GetCurrentAnimationClass()
	; don't use anim classes, use actions from OMetadata
	Return ""
EndFunction

Actor MostRecentOrgasmedActor
Actor Function GetMostRecentOrgasmedActor()
	; use (sender As Actor) in the ostim_orgasm event instead
	Return MostRecentOrgasmedActor
EndFunction

Bool Function IsNaked(Actor NPC)
	; now that there's partial stripping there isn't really a dedicated being naked condition
	Return (!(NPC.GetWornForm(0x00000004) as Bool))
EndFunction

Function PrintBedInfo(ObjectReference Bed)
	Console("--------------------------------------------")
	Console("BED - Name: " + Bed.GetDisplayName())
	Console("BED - Enabled: " + Bed.IsEnabled())
	Console("BED - 3D loaded: " + Bed.Is3DLoaded())
	Console("BED - Bed roll: " + IsBedRoll(Bed))
	Console("--------------------------------------------")
EndFunction

Bool Function IsPlayerInvolved()
	; NPC scenes no longer run on main thread ever. They will always run in subthreads
	; Some addons might still use this function, so we'll keep it here for now
	return True
EndFunction

Bool Function IsNPCScene()
	; NPC scenes no longer run on main thread ever. They will always run in subthreads
	; Some addons might still use this function, so we'll keep it here for now
	return False
EndFunction

Function Rescale()
	; C++ handles scaling
EndFunction

Int Function GetMaxSpanksAllowed()  
	Return 0
EndFunction

Function SetSpankMax(Int Max) 
EndFunction

Function Realign()
EndFunction

Function AlternateRealign()
EndFunction

Function ToggleFreeCam(Bool On = True)
EndFunction

Function RemapStartKey(Int zKey)
	KeyMap = zKey
EndFunction

Function RemapFreecamKey(Int zKey)
	FreecamKey = zKey
EndFunction

Function RemapControlToggleKey(Int zKey)
	ControlToggleKey = zKey
EndFunction

Function RemapSpeedUpKey(Int zKey)
	speedUpKey = zKey
EndFunction

Function RemapSpeedDownKey(Int zKey)
	speedDownKey = zKey
EndFunction

Function RemapPullOutKey(Int zKey)
	PullOutKey = zKey
EndFunction

Bool Function IntArrayContainsValue(Int[] Arr, Int Val)
	return outils.IntArrayContainsValue(arr, val)
EndFunction

Bool Function StringArrayContainsValue(String[] Arr, String Val)
	return outils.StringArrayContainsValue(arr, val)
EndFunction

bool Function StringContains(string str, string contains)
	return outils.StringContains(str, contains)
EndFunction

bool Function IsModLoaded(string ESPFile)
	return outils.IsModLoaded(ESPFile)
Endfunction

bool Function IsChild(actor act)
	return OUtils.IsChild(Act)
EndFunction

Int Function GetSpankCount() ; 
	Return 0
EndFunction

Function SetSpankCount(Int Count)
EndFunction

Function SetGameSpeed(String In)
	; the body was left in in case some addons call this
	; but we will not list ConsoleUtil as a requirement
	ConsoleUtil.ExecuteCommand("sgtm " + In)
EndFunction

Int Function GetTimesOrgasm(Actor Act)
	Return OActor.GetTimesClimaxed(Act)
EndFunction

Int Function GetTimeScale()
	Return (Game.GetFormFromFile(0x00003A, "Skyrim.esm") as GlobalVariable).GetValue() as Int
EndFunction

Function SetTimeScale(Int Time)
	(Game.GetFormFromFile(0x00003A, "Skyrim.esm") as GlobalVariable).SetValue(Time as Float)
EndFunction

Function HideNavMenu()
EndFunction

Function ShowNavMenu()
EndFunction

Function RegisterOSexControlKey(Int zKey)
EndFunction

Function LoadOSexControlKeys()
EndFunction

; I will remove these again in the future, don't call them!
Function ShowBars()
	MostRecentOSexInteractionTime = Utility.GetCurrentRealTime()
	If (AutoHideBars)
		If (!OBars.IsBarVisible(OBars.DomBar))
			OBars.SetBarVisible(OBars.DomBar, True)
		EndIf
		If (!OBars.IsBarVisible(OBars.SubBar))
			OBars.SetBarVisible(OBars.SubBar, True)
		EndIf
		If (!OBars.IsBarVisible(OBars.ThirdBar))
			OBars.SetBarVisible(OBars.ThirdBar, True)
		EndIf
	EndIf
EndFunction

Float Function GetActorExcitement(Actor Act)
	Return OActor.GetExcitement(Act)
EndFunction

Function SetActorExcitement(Actor Act, Float Value)
	OActor.SetExcitement(Act, Value)
EndFunction

Function AddActorExcitement(Actor Act, Float Value)
	OActor.ModifyExcitement(Act, Value)
EndFunction

bool function IsInFreeCam()
	Return false
endfunction

Function ShakeCamera(Float Power, Float Duration = 0.1)
EndFunction

Function AutoIncreaseSpeed()
	If (GetTimeSinceLastPlayerInteraction() < 5.0)
		Return
	EndIf

	String CClass = GetCurrentAnimationClass()
	Float MainExcitement = GetActorExcitement(DomActor)
	If (CClass == "VJ") || (CClass == "Cr") || (CClass == "Pf1") || (CClass == "Pf2")
		MainExcitement = GetActorExcitement(SubActor)
	EndIf

	Int MaxSpeed = GetCurrentAnimationMaxSpeed()
	Int NumSpeeds = MaxSpeed

	Int AggressionBonusChance = 0
	If (IsSceneAggressiveThemed())
		AggressionBonusChance = 80
		MainExcitement += 20
	EndIf

	Int Speed = GetCurrentAnimationSpeed()
	If (Speed == 0)
		Return
	EndIf

	If ((MainExcitement >= 85.0) && (Speed < NumSpeeds))
		If (ChanceRoll(80))
			IncreaseAnimationSpeed()
		EndIf
	ElseIf (MainExcitement >= 69.0) && (Speed <= (NumSpeeds - 1))
		If (ChanceRoll(50))
			IncreaseAnimationSpeed()
		EndIf
	ElseIf (MainExcitement >= 25.0) && (Speed <= (NumSpeeds - 2))
		If (ChanceRoll(20 + AggressionBonusChance))
			IncreaseAnimationSpeed()
		EndIf
	ElseIf (MainExcitement >= 05.0) && (Speed <= (NumSpeeds - 3))
		If (ChanceRoll(20 + AggressionBonusChance))
			IncreaseAnimationSpeed()
		EndIf
	EndIf
EndFunction

Function PlayDing()
EndFunction

Function PlayTickSmall()
EndFunction

Function PlayTickBig()
EndFunction

int Function GetScenePassword()
	return 0
endfunction

string Function GetNPCDiasa(actor act)
	return ""
EndFunction

Bool Function IsActorActive(Actor Act)
	Return OActor.IsInOStim(Act)
EndFunction

Function RestoreScales()
	int i = Actors.Length
	While i
		i -= 1
		Actors[i].SetScale(1.0)
	EndWhile
EndFunction

Bool Function AnimationRunning()
	Return OThread.IsRunning(0)
EndFunction

Bool Function IsSoloScene()
	Return OThread.GetActors(0).Length == 1
EndFunction 

Bool Function IsThreesome()
	; did only check for existence of third actor before
	Return OThread.GetActors(0).Length >= 3
EndFunction

Int Function GetCurrentAnimationSpeed()
	Return OThread.GetSpeed(0)
EndFunction

Bool Function AnimationIsAtMaxSpeed()
	Return OThread.GetSpeed(0) == OMetadata.GetMaxSpeed(OThread.GetScene(0))
EndFunction

Int Function GetCurrentAnimationMaxSpeed()
	Return OMetadata.GetMaxSpeed(OThread.GetScene(0))
EndFunction

Function IncreaseAnimationSpeed()
	OThread.SetSpeed(0, OThread.GetSpeed(0) + 1)
EndFunction

Function DecreaseAnimationSpeed()
	OThread.SetSpeed(0, OThread.GetSpeed(0) - 1)
EndFunction

String Function GetCurrentAnimation()
	Return ""
EndFunction

string function GetCurrentAnimationSceneID() 
	Return OThread.GetScene(0)
endfunction

Function TravelToAnimationIfPossible(String Animation)
	OThread.NavigateTo(0, Animation)
EndFunction

Function TravelToAnimation(String Animation)
	OThread.NavigateTo(0, Animation)
EndFunction

Function WarpToAnimation(String Animation) 
	OThread.WarpTo(0, Animation)
EndFunction

Bool Function IsActorInvolved(actor act)
	if act == none 
		return false 
	endif

	Return OThread.GetActors(0).Find(act) >= 0
EndFunction

Function ForceStop()
	OThread.Stop(0)
EndFunction

OUndressScript function GetUndressScript()
	return None
EndFunction

Bool Function IsBed(ObjectReference Bed)
	If (OSANative.GetDisplayName(bed) == "Bed") || (Bed.Haskeyword(Keyword.GetKeyword("FurnitureBedRoll"))) || (OSANative.GetDisplayName(bed) == "Bed (Owned)")
		Return True
	EndIf
	Return False
EndFunction

Bool Function IsBedRoll(objectReference Bed)
	Return (Bed.Haskeyword(Keyword.GetKeyword("FurnitureBedRoll")))
EndFunction

ObjectReference Function GetOSAStage()
	Return None
EndFunction

Function AdjustAnimationSpeed(float amount)
	OThread.SetSpeed(0, OThread.GetSpeed(0) + (amount As int))
EndFunction

Function SetCurrentAnimationSpeed(Int InSpeed)
	OThread.SetSpeed(0, InSpeed)
EndFunction

Function SetDefaultSettings()
	OData.ResetSettings()
EndFunction

Function Climax(Actor Act)
	OActor.Climax(Act, false)
EndFunction

Function Orgasm(Actor Act)
	OActor.Climax(Act, false)
EndFunction
