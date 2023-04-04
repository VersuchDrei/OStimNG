ScriptName OsexIntegrationMCM Extends SKI_ConfigBase

; sex settings
Int SetActorSpeedControl
Int SetClipinglessFirstPerson
Int SetEndAfterActorHit
Int SetUseRumble
Int SetUseScreenShake
int SetUseIntroScenes
int SetResetPosition

; clothes settings
Int SetAlwaysUndressAtStart
int SetRemoveWeaponsAtStart
Int SetUndressIfNeed
int SetPartialUndressing
int SetRemoveWeaponsWithSlot
int SetAnimateRedress
int[] SlotSets
int UndressingSlotMask

; actor role settings

; bar settings
Int SetSubBar
Int SetDomBar
Int SetThirdBar
Int SetAutoHideBar
Int SetMatchColorToGender
int SetHideNPCOnNPCBars

; orgasm settings
Int SetSlowMoOrgasms
Int SetBlurOrgasms
Int SetEndOnOrgasm
Int SetEndOnSubOrgasm
Int SetEndOnBothOrgasm
int SetAutoClimaxAnims

; light settings
Int SetDomLightMode
Int SetSubLightMode
Int SetSubLightBrightness
Int SetDomLightBrightness
Int SetOnlyLightInDark

Int SetResetState

Int SetThanks

; light settings
String[] DomLightModeList
String[] SubLightModeList

String[] SubLightBrightList
String[] DomLightBrightList

; bed settings
Int SetEnableFurniture
Int SetSelectFurniture
Int SetFurnitureSearchDistance
int SetResetClutter
int SetResetClutterRadius
Int SetBedRealignment

; ai control settings
Int SetAIControl
Int SetControlToggle
Int SetAIChangeChance

Int SetForceAIIfAttacking
Int SetForceAIIfAttacked
Int SetForceAIInConsensualScenes
Int SetForceAIForMasturbation

; misc settings afaik
Int SetCustomTimescale
int SetTutorialMessages

Int SetUseFades
Int SetUseAutoFades

; camera settings
Int SetUseFreeCam
Int SetFreeCamFOV
Int SetCameraSpeed
Int SetForceFirstPerson

Int SetUseCosaveWorkaround

; mcm save/load settings
Int ExportSettings
Int ImportSettings
Int ImportDefaultSettings

int SetUpdate

OsexIntegrationMain Main


string currPage

actor playerref

int SetUndressingAbout

Int osaMainMenuKeyDefault = 156 ; numpad ENTER
Int osaUpKeyDefault = 72 ; numpad 8
Int osaDownKeyDefault = 76 ; numpad 5
Int osaLeftKeyDefault = 75 ; numpad 4
Int osaRightKeyDefault = 77 ; numpad 6
Int osaTogKeyDefault = 73 ; numpad 9
Int osaYesKeyDefault = 71 ; numpad 7
Int osaEndKeyDefault = 83 ; numpad .

_oControl OSAControl

;ORomance 
int SetORDifficulty
int SetORSexuality
int SetORKey
int SetORColorblind
int SetORStationary
int SetORLeft
int SetORRight
int SetORNakadashi

string ORomance = "ORomance.esp"
int GVORDifficulty = 0x0063A4
int GVORSexuality = 0x0063A5
int GVORKey = 0x006E6A
int GVORLeft = 0x73D2
int GVORRight = 0x73D3
int GVORColorblind = 0x73D0
int GVORStationaryMode = 0x73D1
int GVORNakadashi = 0x73D4

string OCrime = "ocrime.esp"
int SetOCBounty
string SUOCBounty = "ocrime.bounty"

string OAroused = "OAroused.esp"
int SetOAKey 
string SUOAKey = "oaroused.key"
int SetOARequireLowArousalBeforeEnd
string SUOALowArousalReq = "oaroused.emptybeforeend"
int SetOAStatBuffs
string SUOAStatBuffs = "oaroused.modifystats"
int SetOANudityBroadcast
string SUOANudityBroadcast = "oaroused.EnableNudityBroadcast"

string OSearch = "OSearch.esp"
int SetOSKey
string SUOSKey = "osearch.key"
int SetOSAllowHub
string SUOSAllowHub = "osearch.allowhub"
int SetOSAllowTransitory
string SUOSAllowTransitory = "osearch.allowTransitory"
int SetOSAllowSex
string SUOSAllowSex = "osearch.allowSex"

string OProstitution = "OProstitution.esp"
int SetOPFreq
string SUOPFreq = "oprostitution.freqmod"

Event OnInit()
	Init()
EndEvent

Function Init()
	Parent.OnGameReload()
	Main = (Self as Quest) as OsexIntegrationMain
	OSAControl = Quest.GetQuest("0SAControl") as _oControl

	DomLightModeList = new String[3]
	DomLightModeList[0] = "$ostim_light_mode_none"
	DomLightModeList[1] = "$ostim_light_mode_rear"
	DomLightModeList[2] = "$ostim_light_mode_face"

	SubLightModeList = new String[3]
	SubLightModeList[0] = "$ostim_light_mode_none"
	SubLightModeList[1] = "$ostim_light_mode_rear"
	SubLightModeList[2] = "$ostim_light_mode_face"

	SubLightBrightList = new String[2]
	SubLightBrightList[0] = "$ostim_light_type_dim"
	SubLightBrightList[1] = "Bright"

	DomLightBrightList = new String[2]
	DomLightBrightList[0] = "$ostim_light_type_dim"
	DomLightBrightList[1] = "$ostim_light_type_bright"

	playerref = game.getplayer()

	SetupPages()
EndFunction

int Function GetVersion()
	Return 4
EndFunction

Event OnVersionUpdate(int version)
	SetupPages()
EndEvent

Function SetupPages()
	Pages = new string[9]
	Pages[0] = "$ostim_page_configuration"
	Pages[1] = "$ostim_page_controls"
	Pages[2] = "$ostim_page_excitement"
	Pages[3] = "$ostim_page_gender_roles"
	Pages[4] = "$ostim_page_undress"
	Pages[5] = "$ostim_page_expression"
	Pages[6] = "$ostim_page_alignment"
	Pages[7] = "$ostim_page_addons"
	Pages[8] = "$ostim_page_about"
EndFunction

Event OnConfigRegister()
	ImportSettings()
endEvent

Event onConfigOpen()
	; For some reason, SkyUI's OnGameReload() isn't firing properly when loading an existing game
	; so we have to instantiate OSAControl when the menu is opened
	; for key rebinding to work in existing saves with a previous version of OStim installed
	OSAControl = Quest.GetQuest("0SAControl") as _oControl
EndEvent

Event OnConfigClose()
	; set the OSA keys array in the _oControl script
	OSAControl.oPlayerControls()
EndEvent

Event OnPageReset(String Page)
	{Called when a new page is selected, including the initial empty page}
	currPage = page

	If (Page == "")
		LoadCustomContent("Ostim/logo.dds", 184, 31)
		Return
	Else
		UnloadCustomContent()
		SetInfoText(" ")
	EndIf

	If (Page == "$ostim_page_configuration")
		If (!Main)
			Init()
			If (!Main.EndOnDomOrgasm)
				Main.Startup()
			EndIf
			Debug.MessageBox("$ostim_message_main_not_initialized")
		EndIf

		SetInfoText(" ")
		Main.playTickBig()
		SetCursorFillMode(TOP_TO_BOTTOM)
		SetThanks = AddTextOption("$ostim_configuration", "")
		SetCursorPosition(1)
		AddTextOption("$ostim_config_text", "")
		SetCursorPosition(2)

		;=============================================================================================
		AddColoredHeader("$ostim_header_sex_scenes")
		SetActorSpeedControl = AddToggleOption("$ostim_speed_control", Main.EnableActorSpeedControl)
		SetClipinglessFirstPerson = AddToggleOption("$ostim_clippingless", Main.EnableImprovedCamSupport)
		SetCustomTimescale = AddSliderOption("$ostim_timescale", Main.CustomTimescale, "{0}")
		SetUseFades = AddToggleOption("$ostim_use_fades", Main.UseFades)
		SetEndAfterActorHit = AddToggleOption("$ostim_end_on_hit", Main.EndAfterActorHit)
		SetUseRumble = AddToggleOption("$ostim_use_rumble", Main.UseRumble)
		SetUseScreenShake = AddToggleOption("$ostim_screenshake", Main.UseScreenShake)
		SetForceFirstPerson = AddToggleOption("$ostim_force_first", Main.ForceFirstPersonAfter)
		SetUseIntroScenes = AddToggleOption("$ostim_use_intro_scenes", Main.UseIntroScenes)
		SetResetPosition = AddToggleOption("$ostim_reset_position", Main.ResetPosAfterSceneEnd) 		
		AddEmptyOption()

		AddColoredHeader("$ostim_header_orgasms")
		SetEndOnOrgasm = AddToggleOption("$ostim_orgasm_end_dom", Main.EndOnDomOrgasm)
		SetEndOnSubOrgasm = AddToggleOption("$ostim_orgasm_end_sub", Main.EndOnSubOrgasm)
		SetEndOnBothOrgasm = AddToggleOption("$ostim_orgasm_end_both", Main.RequireBothOrgasmsToFinish)
		SetSlowMoOrgasms = AddToggleOption("$ostim_slowmo_orgasm", Main.SlowMoOnOrgasm)
		SetBlurOrgasms = AddToggleOption("$ostim_blur_orgasm", Main.BlurOnOrgasm)
		SetAutoClimaxAnims = AddToggleOption("$ostim_auto_climax_anims", Main.AutoClimaxAnimations)
		AddEmptyOption()

		AddColoredHeader("$ostim_header_furniture")
		SetEnableFurniture = AddToggleOption("$ostim_use_furniture", Main.UseFurniture)
		SetSelectFurniture = AddToggleOption("$ostim_select_furniture", Main.SelectFurniture)
		SetFurnitureSearchDistance = AddSliderOption("$ostim_furniture_search_rad", Main.FurnitureSearchDistance, "{0} meters")
		SetResetClutter = AddToggleOption("$ostim_reset_clutter", Main.ResetClutter)
		SetResetClutterRadius = AddSliderOption("$ostim_reset_clutter_radius", Main.ResetClutterRadius, "{0} meters")
		SetBedRealignment = AddSliderOption("$ostim_bed_realignment", Main.BedRealignment, "{0} units")
		AddEmptyOption()

		AddColoredHeader("$ostim_header_excitement_bars")
		SetDomBar = AddToggleOption("$ostim_dom_bar", Main.EnableDomBar)
		SetSubBar = AddToggleOption("$ostim_sub_bar", Main.EnableSubBar)
		SetThirdBar = AddToggleOption("$ostim_third_bar", Main.EnableThirdBar)
		SetAutoHideBar = AddToggleOption("$ostim_auto_hide_bar", Main.AutoHideBars)
		SetMatchColorToGender = AddToggleOption("$ostim_match_color_gender", Main.MatchBarColorToGender)
		SetHideNPCOnNPCBars = AddToggleOption("$ostim_hide_npc_bars", Main.HideBarsInNPCScenes)
		AddEmptyOption()

		AddColoredHeader("$ostim_header_system")
		SetResetState = AddTextOption("$ostim_reset_state", "")
		SetUpdate = AddTextOption("$ostim_update", "")
		SetTutorialMessages = AddToggleOption("$ostim_tutorial", Main.ShowTutorials)
		;SetUseCosaveWorkaround = AddToggleOption("$ostim_cosave", Main.useBrokenCosaveWorkaround)
		AddEmptyOption()

		;=============================================================================================

		SetCursorPosition(3)
		AddColoredHeader("$ostim_header_ai_control")
		SetAIControl = AddToggleOption("$ostim_full_auto", Main.UseAIControl)
		SetForceAIIfAttacking = AddToggleOption("$ostim_force_auto_attacking", Main.UseAIPlayerAggressor)
		SetForceAIIfAttacked = AddToggleOption("$ostim_force_auto_attacked", Main.UseAIPlayerAggressed)
		SetForceAIInConsensualScenes = AddToggleOption("$ostim_force_auto_consentual", Main.UseAINonAggressive)
		SetForceAIForMasturbation = AddToggleOption("$ostim_force_auto_masturbation", Main.UseAIMasturbation)
		SetUseAutoFades = AddToggleOption("$ostim_auto_fades", Main.UseAutoFades)
		SetAIChangeChance = AddSliderOption("$ostim_ai_change_chance", Main.AiSwitchChance, "{0}")
		AddEmptyOption()

		AddColoredHeader("$ostim_header_freecam")
		SetUseFreeCam = AddToggleOption("$ostim_freecam", Main.UseFreeCam)
		SetFreeCamFOV = AddSliderOption("$ostim_freecam_fov", Main.FreecamFOV, "{0}")
		SetCameraSpeed = AddSliderOption("$ostim_freecam_speed", Main.FreecamSpeed, "{1}")
		AddEmptyOption()

		AddColoredHeader("$ostim_header_lights")
		SetDomLightMode = AddMenuOption("$ostim_dom_light_mode", DomLightModeList[Main.DomLightPos])
		SetSubLightMode = AddMenuOption("$ostim_sub_light_mode", SubLightModeList[Main.SubLightPos])
		SetDomLightBrightness = AddMenuOption("$ostim_dom_light_brightness", DomLightBrightList[Main.DomLightBrightness])
		SetSubLightBrightness = AddMenuOption("$ostim_sub_light_brightness", SubLightBrightList[Main.SubLightBrightness])
		SetOnlyLightInDark = AddToggleOption("$ostim_dark_light", Main.LowLightLevelLightsOnly)
		AddEmptyOption()

		AddColoredHeader("$ostim_header_save_load")
		ExportSettings = AddTextOption("$ostim_export", "$ostim_done")
		ImportSettings = AddTextOption("$ostim_import", "$ostim_done")
		ImportDefaultSettings = AddTextOption("$ostim_import_default", "$ostim_done")
		AddEmptyOption()
	ElseIf (Page == "$ostim_page_addons")
		SetInfoText(" ")
		Main.playTickBig()
		SetCursorFillMode(TOP_TO_BOTTOM)
		UnloadCustomContent()
		SetThanks = AddTextOption("", "")
		SetCursorPosition(0)
		AddTextOption("$ostim_addon_settings_text", "")
		SetCursorPosition(2)

		if main.IsModLoaded(ORomance)
			AddColoredHeader("ORomance")
			SetORSexuality = AddToggleOption("$ostim_addon_or_npc_sexualities", GetExternalBool(ORomance, GVORSexuality))
			SetORDifficulty = AddSliderOption("$ostim_addon_or_difficulty", GetExternalInt(ORomance, GVORDifficulty), "{0}")
			SetORKey = AddKeyMapOption("$ostim_addon_or_mainkey", GetExternalInt(oromance, gvorkey))
			SetORColorblind = AddToggleOption("$ostim_addon_or_colorblind", GetExternalBool(ORomance, GVORColorblind))
			;SetORStationary = AddToggleOption("$ostim_addon_or_stationary", GetExternalBool(ORomance, GVORStationaryMode))
			SetORLeft = AddKeyMapOption("$ostim_addon_or_left_key", GetExternalInt(oromance, GVORLeft))
			SetORRight = AddKeyMapOption("$ostim_addon_or_right_key", GetExternalInt(oromance, GVORRight))
			SetORNakadashi = AddToggleOption("$ostim_addon_or_nakadashi", GetExternalBool(ORomance, GVORNakadashi))
		endif

		if main.IsModLoaded(OSearch)
			AddColoredHeader("OSearch")
			SetOSKey = AddKeyMapOption("$ostim_addon_os_key", StorageUtil.GetIntValue(none, SUOSKey))
			SetOSAllowSex = AddToggleOption("$ostim_addon_os_sex", StorageUtil.GetIntValue(none, SUOSAllowSex))
			SetOSAllowHub = AddToggleOption("$ostim_addon_os_hub", StorageUtil.GetIntValue(none, SUOSAllowHub))
			SetOSAllowTransitory = AddToggleOption("$ostim_addon_os_transitory", StorageUtil.GetIntValue(none, SUOSAllowTransitory))
		endif 

		if main.IsModLoaded(OProstitution)
			AddColoredHeader("OProstitution")

			SetOPFreq = AddSliderOption("$ostim_addon_op_freq", StorageUtil.GetIntValue(none, SUOPFreq), "{0}")
		endif 

		;===================================

		SetCursorPosition(3)

		if main.IsModLoaded(OCrime)
			AddColoredHeader("OCrime")
			SetOCBounty = AddSliderOption("$ostim_addon_oc_bounty", StorageUtil.GetIntValue(none, suocbounty), "{0} Gold")
		endif 

		if main.IsModLoaded(OAroused)
			AddColoredHeader("OAroused")
			SetOAKey = AddKeyMapOption("$ostim_addon_oa_key", StorageUtil.GetIntValue(none, SUOAKey))
			SetOARequireLowArousalBeforeEnd = AddToggleOption("$ostim_addon_oa_low_arousal_end", StorageUtil.GetIntValue(none, SUOALowArousalReq))
			SetOAStatBuffs = AddToggleOption("$ostim_addon_oa_stat_buffs", StorageUtil.GetIntValue(none, SUOAStatBuffs))
			SetOANudityBroadcast = AddToggleOption("$ostim_addon_oa_nudity_bc", StorageUtil.GetIntValue(none, SUOANudityBroadcast))

		endif
	ElseIf Page == "$ostim_page_controls"
		DrawControlsPage()
	ElseIf Page == "$ostim_page_excitement"
		DrawExcitementPage()
	ElseIf Page == "$ostim_page_gender_roles"
		DrawGenderRolesPage()
	ElseIf (Page == "$ostim_page_undress")
		LoadCustomContent("Ostim/logo.dds", 184, 31)
		Main.PlayTickBig()
		UnloadCustomContent()
		SetInfoText(" ")
		Main.playTickBig()

		SetCursorFillMode(LEFT_TO_RIGHT)
		SetAlwaysUndressAtStart = AddToggleOption("$ostim_undress_start", Main.AlwaysUndressAtAnimStart)
		SetRemoveWeaponsAtStart = AddToggleOption("$ostim_remove_weapons_start", Main.RemoveWeaponsAtStart)
		SetUndressIfneed = AddToggleOption("$ostim_undress_need", Main.AutoUndressIfNeeded)
		AddEmptyOption()
		SetPartialUndressing = AddToggleOption("$ostim_partial_undressing", Main.PartialUndressing)
		SetRemoveWeaponsWithSlot = AddSliderOption("$ostim_remove_weapons_slot", Main.RemoveWeaponsWithSlot, "{0}")
		SetAnimateRedress= AddToggleOption("$ostim_animate_redress", Main.FullyAnimateRedress)

		SetCursorPosition(10)
		SetUndressingAbout = AddTextOption("$ostim_undress_about", "")
		SetCursorPosition(11)
		AddTextOption("$ostim_undress_text{OStim}", "")
		SetCursorPosition(12)
		AddColoredHeader("$ostim_undress_slots_header")
		AddColoredHeader("")

		DrawSlotPage()

	ElseIf Page == "$ostim_page_expression"
		DrawExpressionPage()
	ElseIf Page == "$ostim_page_alignment"
		DrawAlignmentPage()
	ElseIf (Page == "$ostim_page_about")
		UnloadCustomContent()
		SetInfoText(" ")
		SetCursorFillMode(TOP_TO_BOTTOM)
		SetCursorPosition(0)

		AddTextOption("OStimNG ", "-")
		
		AddTextOption("", "")
		AddColoredHeader("$ostim_authors")
		AddTextOption("OStimNG ", "$ostim_by{Aietos, Kannonfodder}")
		AddTextOption("- ", "$ostim_and{VersuchDrei}")
		AddTextOption("OStim ", "$ostim_by{Sairion}")
		AddTextOption("OSA ", "$ostim_by{CE0}")
		AddTextOption("OpenSex ", "$ostim_by{Ace Animations}")

		SetCursorPosition(1)
		AddTextOption("Powered By Sswaye's Reshade", "")

		AddTextOption("", "")
		AddColoredHeader("$ostim_links")
		AddTextOption("discord.gg/ostim", "")
		AddTextOption("https://github.com/VersuchDrei/OStim", "")

		Main.PlayDing()
	EndIf
EndEvent

bool Function GetExternalBool(string modesp, int id)
	;osexintegrationmain.console((game.GetFormFromFile(id, modesp) as GlobalVariable).GetValueInt())

	return (game.GetFormFromFile(id, modesp) as GlobalVariable).GetValueInt() == 1

endfunction

Function SetExternalBool(string modesp, int id, bool val)
	int set = 0
	if val
		set = 1
	endif 

	(game.GetFormFromFile(id, modesp) as GlobalVariable).SetValueInt(set)
endfunction

Function SetExternalInt(string modesp, int id, int val)
	(game.GetFormFromFile(id, modesp) as GlobalVariable).SetValueInt(val)
endfunction
int Function GetExternalInt(string modesp, int id)
	return (game.GetFormFromFile(id, modesp) as GlobalVariable).GetValueInt() 
endfunction

Function SetExternalfloat(string modesp, int id, float val)
	(game.GetFormFromFile(id, modesp) as GlobalVariable).SetValue(val)
endfunction
float Function GetExternalfloat(string modesp, int id)
	return (game.GetFormFromFile(id, modesp) as GlobalVariable).GetValue() 
endfunction

Event OnOptionSelect(Int Option)
	Main.PlayTickBig()
	if currPage == "$ostim_page_undress"
		If Option == SetAlwaysUndressAtStart
			Main.AlwaysUndressAtAnimStart = !Main.AlwaysUndressAtAnimStart
			SetToggleOptionValue(Option, Main.AlwaysUndressAtAnimStart)
		ElseIf Option == SetRemoveWeaponsAtStart
			Main.RemoveWeaponsAtStart = !Main.RemoveWeaponsAtStart
			SetToggleOptionValue(Option, Main.RemoveWeaponsAtStart)
		ElseIf Option == SetUndressIfneed
			Main.AutoUndressIfNeeded = !Main.AutoUndressIfNeeded
			SetToggleOptionValue(Option, Main.AutoUndressIfNeeded)
		ElseIf Option == SetPartialUndressing
			Main.PartialUndressing = !Main.PartialUndressing
			SetToggleOptionValue(Option, Main.PartialUndressing)
		ElseIf Option == SetAnimateRedress
			Main.FullyAnimateRedress = !Main.FullyAnimateRedress
			SetToggleOptionValue(Option, Main.FullyAnimateRedress)
		Else
			OnSlotSelect(option)
		EndIf
	elseif currPage == "$ostim_page_addons"
		if option == SetORSexuality
			SetExternalBool(oromance, GVORSexuality, !GetExternalBool(oromance, GVORSexuality))
			SetToggleOptionValue(SetORSexuality, GetExternalBool(oromance, GVORSexuality))
		elseif option == SetORColorblind
			SetExternalBool(oromance, GVORColorblind, !GetExternalBool(oromance, GVORColorblind))
			SetToggleOptionValue(SetORColorblind, GetExternalBool(oromance, GVORColorblind))
		elseif option == SetORNakadashi
			SetExternalBool(oromance, GVORNakadashi, !GetExternalBool(oromance, GVORNakadashi))
			SetToggleOptionValue(SetORNakadashi, GetExternalBool(oromance, GVORNakadashi))
		elseif option == SetORStationary
			SetExternalBool(oromance, GVORStationaryMode, !GetExternalBool(oromance, GVORStationaryMode))
			SetToggleOptionValue(SetORStationary, GetExternalBool(oromance, GVORStationaryMode))
		elseif option == SetOSAllowSex
			StorageUtil.SetIntValue(none, SUOSAllowSex, (!(StorageUtil.GetIntValue(none, SUOSAllowSex) as bool)) as int)
			SetToggleOptionValue(SetOSAllowSex, StorageUtil.GetIntValue(none, SUOSAllowSex))
		elseif option == SetOSAllowTransitory
			StorageUtil.SetIntValue(none, SUOSAllowTransitory, (!(StorageUtil.GetIntValue(none, SUOSAllowTransitory) as bool)) as int)
			SetToggleOptionValue(SetOSAllowTransitory, StorageUtil.GetIntValue(none, SUOSAllowTransitory))
		elseif option == SetOARequireLowArousalBeforeEnd
			StorageUtil.SetIntValue(none, SUOALowArousalReq, (!(StorageUtil.GetIntValue(none, SUOALowArousalReq) as bool)) as int)
			SetToggleOptionValue(SetOARequireLowArousalBeforeEnd, StorageUtil.GetIntValue(none, SUOALowArousalReq))
		elseif option == SetOSAllowHub
			StorageUtil.SetIntValue(none, SUOSAllowHub, (!(StorageUtil.GetIntValue(none, SUOSAllowHub) as bool)) as int)
			SetToggleOptionValue(SetOSAllowHub, StorageUtil.GetIntValue(none, SUOSAllowHub))
		elseif option == SetOAStatBuffs
			StorageUtil.SetIntValue(none, SUOAStatBuffs, (!(StorageUtil.GetIntValue(none, SUOAStatBuffs) as bool)) as int)
			SetToggleOptionValue(SetOAStatBuffs, StorageUtil.GetIntValue(none, SUOAStatBuffs))
		elseif option == SetOANudityBroadcast
			StorageUtil.SetIntValue(none, SUOANudityBroadcast, (!(StorageUtil.GetIntValue(none, SUOANudityBroadcast) as bool)) as int)
			SetToggleOptionValue(SetOANudityBroadcast, StorageUtil.GetIntValue(none, SUOANudityBroadcast))
		endif
		return
	EndIf

	If (Option == SetEndOnOrgasm)
		Main.EndOnDomOrgasm = !Main.EndOnDomOrgasm
		SetToggleOptionValue(SetEndOnOrgasm, Main.EndOnDomOrgasm)
	ElseIf (Option == SetEndOnSubOrgasm)
		Main.EndOnSubOrgasm = !Main.EndOnSubOrgasm
		SetToggleOptionValue(SetEndOnSubOrgasm, Main.EndOnSubOrgasm)
	ElseIf (Option == SetEndOnBothOrgasm)
		Main.RequireBothOrgasmsToFinish = !Main.RequireBothOrgasmsToFinish
		SetToggleOptionValue(SetEndOnBothOrgasm, Main.RequireBothOrgasmsToFinish)
	ElseIf (Option == SetResetState)
		Main.ResetState()
		ShowMessage("$ostim_message_reset_state", false)
	ElseIf (Option == SetUpdate)
		ShowMessage("$ostim_message_update_close_menus", false)
		OUtils.ForceOUpdate()
	ElseIf (Option == SetActorSpeedControl)
		Main.EnableActorSpeedControl = !Main.EnableActorSpeedControl
		SetToggleOptionValue(Option, Main.EnableActorSpeedControl)
	ElseIf (Option == SetResetPosition)
		Main.ResetPosAfterSceneEnd = !Main.ResetPosAfterSceneEnd
		SetToggleOptionValue(Option, Main.ResetPosAfterSceneEnd)
	ElseIf (Option == SetEnableFurniture)
		Main.UseFurniture = !Main.UseFurniture
		SetToggleOptionValue(Option, Main.UseFurniture)
	ElseIf (Option == SetSelectFurniture)
		Main.SelectFurniture = !Main.SelectFurniture
		SetToggleOptionValue(Option, Main.SelectFurniture)
	ElseIf Option == SetResetClutter
		Main.ResetClutter = !Main.ResetClutter
		SetToggleOptionValue(Option, Main.ResetClutter)	
	ElseIf (Option == SetTutorialMessages)
		Main.ShowTutorials = !Main.ShowTutorials
		SetToggleOptionValue(Option, Main.ShowTutorials)
	ElseIf (Option == SetUseIntroScenes)
		Main.UseIntroScenes = !Main.UseIntroScenes
		SetToggleOptionValue(Option, Main.UseIntroScenes)
	ElseIf (Option == SetUseRumble)
		Main.UseRumble = !Main.UseRumble
		SetToggleOptionValue(Option, Main.UseRumble)
	ElseIf (Option == SetUseScreenShake)
		Main.UseScreenShake = !Main.UseScreenShake
		SetToggleOptionValue(Option, Main.UseScreenShake)
	ElseIf (Option == SetForceAIInConsensualScenes)
		Main.UseAINonAggressive = !Main.UseAINonAggressive
		SetToggleOptionValue(Option, Main.UseAINonAggressive)
	ElseIf (Option == SetForceAIForMasturbation)
		Main.UseAIMasturbation = !Main.UseAIMasturbation
		SetToggleOptionValue(Option, Main.UseAIMasturbation)
	ElseIf (Option == SetForceAIIfAttacked)
		Main.UseAIPlayerAggressed = !Main.UseAIPlayerAggressed
		SetToggleOptionValue(Option, Main.UseAIPlayerAggressed)
	ElseIf (Option == SetForceFirstPerson)
		Main.ForceFirstPersonAfter = !Main.ForceFirstPersonAfter
		SetToggleOptionValue(Option, Main.ForceFirstPersonAfter)
	ElseIf (Option == SetUseAutoFades)
		Main.UseAutoFades = !Main.UseAutoFades
		SetToggleOptionValue(Option, Main.UseAutoFades)
	ElseIf (Option == SetMatchColorToGender)
		Main.MatchBarColorToGender = !Main.MatchBarColorToGender
		SetToggleOptionValue(Option, Main.MatchBarColorToGender)
	ElseIf (Option == SetHideNPCOnNPCBars)
		Main.HideBarsInNPCScenes = !Main.HideBarsInNPCScenes
		SetToggleOptionValue(Option, Main.HideBarsInNPCScenes)
	ElseIf (Option == SetEndAfterActorHit)
		Main.EndAfterActorHit = !Main.EndAfterActorHit
		SetToggleOptionValue(Option, Main.EndAfterActorHit)
	ElseIf (Option == SetUseFreeCam)
		Main.UseFreeCam = !Main.UseFreeCam
		SetToggleOptionValue(Option, Main.UseFreeCam)
	ElseIf (Option == SetUseCosaveWorkaround)
		Main.UseBrokenCosaveWorkaround = !Main.UseBrokenCosaveWorkaround
		SetToggleOptionValue(Option, Main.UseBrokenCosaveWorkaround)
	ElseIf (Option == SetForceAIIfAttacking)
		Main.UseAIPlayerAggressor = !Main.UseAIPlayerAggressor
		SetToggleOptionValue(Option, Main.UseAIPlayerAggressor)
	ElseIf (Option == SetClipinglessFirstPerson)
		Main.EnableImprovedCamSupport = !Main.EnableImprovedCamSupport
		SetToggleOptionValue(Option, Main.EnableImprovedCamSupport)
	ElseIf (Option == SetAIControl)
		Main.UseAIControl = !Main.UseAIControl
		SetToggleOptionValue(Option, Main.UseAIControl)
	ElseIf (Option == SetDomBar)
		Main.EnableDomBar = !Main.EnableDomBar
		SetToggleOptionValue(Option, Main.EnableDomBar)
	ElseIf (Option == SetThirdBar)
		Main.EnableThirdBar = !Main.EnableThirdBar
		SetToggleOptionValue(Option, Main.EnableThirdBar)
	ElseIf (Option == SetSubBar)
		Main.EnableSubBar = !Main.EnableSubBar
		SetToggleOptionValue(Option, Main.EnableSubBar)
	ElseIf (Option == SetAutoHideBar)
		Main.AutoHideBars = !Main.AutoHideBars
		SetToggleOptionValue(Option, Main.AutoHideBars)
	ElseIf (Option == SetSlowMoOrgasms)
		Main.SlowMoOnOrgasm = !Main.SlowMoOnOrgasm
		SetToggleOptionValue(Option, Main.SlowMoOnOrgasm)
	ElseIf (Option == SetBlurOrgasms)
		Main.BlurOnOrgasm = !Main.BlurOnOrgasm
		SetToggleOptionValue(Option, Main.BlurOnOrgasm)
	ElseIf (Option == SetAutoClimaxAnims)
		Main.AutoClimaxAnimations = !Main.AutoClimaxAnimations
		SetToggleOptionValue(Option, Main.AutoClimaxAnimations)
	ElseIf (Option == SetUseFades)
		Main.UseFades = !Main.UseFades
		SetToggleOptionValue(Option, Main.UseFades)
	ElseIf (Option == SetOnlyLightInDark)
		Main.LowLightLevelLightsOnly = !Main.LowLightLevelLightsOnly
		SetToggleOptionValue(Option, Main.LowLightLevelLightsOnly)
	ElseIf (Option == ExportSettings)
		If ShowMessage("$ostim_message_export_confirm", true)
			ExportSettings()
		EndIf
	ElseIf (Option == ImportSettings)
		If ShowMessage("$ostim_message_import_confirm")
			ImportSettings()
		EndIf
	ElseIf (Option == ImportDefaultSettings)
		If ShowMessage("$ostim_message_import_confirm")
			ImportSettings(true)
		EndIf
	EndIf
EndEvent

Event OnOptionHighlight(Int Option)
	;Main.playTickSmall()
	if currPage == "$ostim_page_undress"
		If Option == SetAlwaysUndressAtStart
			SetInfoText("$ostim_tooltip_always_undress")
		ElseIf Option == SetRemoveWeaponsAtStart
			SetInfoText("$ostim_tooltip_remove_weapons_start")
		ElseIf Option == SetUndressIfNeed
			SetInfoText("$ostim_tooltip_undress_if_need")
		ElseIf Option == SetPartialUndressing
			SetInfoText("$ostim_tooltip_partial_undressing")
		ElseIf Option == SetRemoveWeaponsWithSlot
			SetInfoText("$ostim_tooltip_remove_weapons_slot")
		ElseIf Option == SetAnimateRedress
			SetInfoText("$ostim_tooltip_animate_redress")
		ElseIf Option == SetUndressingAbout
			SetInfoText("$ostim_tooltip_undressing_about")
		Else
			OnSlotMouseOver(option)
		EndIf
		Return
	elseif currPage == "$ostim_page_addons"
		If (Option == SetORKey)
			SetInfoText("$ostim_tooltip_or_mainkey")
		elseif (option == SetORDifficulty)
			SetInfoText("$ostim_tooltip_or_difficulty")
		elseif (option == SetOAKey)
			SetInfoText("$ostim_tooltip_oa_key")
		elseif (option == SetOSKey)
			SetInfoText("$ostim_tooltip_os_key")
		elseif (option == SetORSexuality)
			SetInfoText("$ostim_tooltip_or_sexuality")
		elseif (option == SetORColorblind)
			SetInfoText("$ostim_tooltip_or_colorblind")
		elseif (option == SetORLeft)
			SetInfoText("$ostim_tooltip_or_left_key")
		elseif (option == SetORRight)
			SetInfoText("$ostim_tooltip_or_right_key")
		elseif (option == SetORNakadashi)
			SetInfoText("$ostim_tooltip_or_nakadashi")
		ElseIf (Option == SetOCBounty)
			SetInfoText("$ostim_tooltip_oc_bounty")
		ElseIf (Option == SetOPFreq)
			SetInfoText("$ostim_tooltip_op_freq")
		Elseif (Option == SetOARequireLowArousalBeforeEnd)
			SetInfoText("$ostim_tooltip_oa_low_arousal_end")
		Elseif (Option == SetOSAllowHub)
			SetInfoText("$ostim_tooltip_os_hub")
		Elseif (Option == SetOSAllowTransitory)
			SetInfoText("$ostim_tooltip_os_transitory")
		Elseif (Option == SetOSAllowSex)
			SetInfoText("$ostim_tooltip_os_transitory")
		Elseif (Option == SetOANudityBroadcast)
			SetInfoText("$ostim_tooltip_oa_nudity_bc")
		Elseif (Option == SetOAStatBuffs)
			SetInfoText("$ostim_tooltip_oa_stat_buffs")
		endif 

		return
	EndIf
	If (Option == SetEndOnOrgasm)
		SetInfoText("$ostim_tooltip_orgasm_end_dom")
	ElseIf (Option == SetEndOnSubOrgasm)
		SetInfoText("$ostim_tooltip_orgasm_end_sub")
	ElseIf (Option == SetEndOnBothOrgasm)
		SetInfoText("$ostim_tooltip_orgasm_end_both")
	ElseIf (Option == SetResetState)
		SetInfoText("$ostim_tooltip_reset")
	ElseIf (Option == SetForceAIIfAttacked)
		SetInfoText("$ostim_tooltip_ai_attacked")
	ElseIf (Option == SetForceAIIfAttacking)
		SetInfoText("$ostim_tooltip_ai_attacking")
	ElseIf (Option == SetForceAIInConsensualScenes)
		SetInfoText("$ostim_tooltip_ai_consent")
	ElseIf (Option == SetForceAIForMasturbation)
		SetInfoText("$ostim_tooltip_ai_masturbation")
	ElseIf (Option == SetUseFades)
		SetInfoText("$ostim_tooltip_fades")
	ElseIf (Option == SetUseIntroScenes)
		SetInfoText("$ostim_tooltip_use_intro_scenes")
	ElseIf (Option == SetUseCosaveWorkaround)
		SetInfoText("$ostim_tooltip_cosave")
	ElseIf (Option == SetFreeCamFOV)
		SetInfoText("$ostim_tooltip_fov")
	ElseIf (Option == SetUseRumble)
		SetInfoText("$ostim_tooltip_rumble")
	ElseIf (Option == SetMatchColorToGender)
		SetInfoText("$ostim_tooltip_gendered_colors")
	ElseIf (Option == SetHideNPCOnNPCBars)
		SetInfoText("$ostim_tooltip_npc_bars")
	ElseIf (Option == SetEndAfterActorHit)
		SetInfoText("$ostim_tooltip_end_on_hit")
	ElseIf (Option == SetForceFirstPerson)
		SetInfoText("$ostim_tooltip_force_first")
	ElseIf (Option == SetCustomTimescale)
		SetInfoText("$ostim_tooltip_custom_timescale")
	ElseIf (Option == SetClipinglessFirstPerson)
		 SetInfoText("$ostim_tooltip_clippingless")
	ElseIf (Option == SetActorSpeedControl)
		SetInfoText("$ostim_tooltip_speed_control")
	ElseIf (Option == SetResetPosition)
		SetInfoText("$ostim_tooltip_reset_position")
	ElseIf (Option == SetUseAutoFades)
		SetInfoText("$ostim_tooltip_auto_fades")
	ElseIf (Option == SetAIChangeChance)
		SetInfoText("$ostim_tooltip_ai_change_chance")
	ElseIf (Option == SetDomBar)
		SetInfoText("$ostim_tooltip_dom_bar")
	ElseIf (Option == SetthirdBar)
		SetInfoText("$ostim_tooltip_third_bar")
	ElseIf (Option == SetSubBar)
		SetInfoText("$ostim_tooltip_sub_bar")
	ElseIf (Option == SetEnableFurniture)
		SetInfoText("$ostim_tooltip_enable_furniture")
	ElseIf (Option == SetSelectFurniture)
		SetInfoText("$ostim_tooltip_select_furniture")
	ElseIf (Option == SetFurnitureSearchDistance)
		SetInfoText("$ostim_tooltip_furniture_search_dist")
	ElseIf Option == SetResetClutter
		SetInfoText("$ostim_tooltip_reset_clutter")
	Elseif Option == SetResetClutterRadius
		SetInfoText("$ostim_tooltip_reset_clutter_radius")
	ElseIf (Option == SetTutorialMessages)
		SetInfoText("$ostim_tooltip_enable_tutorial")
	ElseIf (Option == setupdate)
		SetInfoText("$ostim_tooltip_update")
	ElseIf (Option == SetAIControl)
		SetInfoText("$ostim_tooltip_enable_ai")
	ElseIf (Option == SetAutoHideBar)
		SetInfoText("$ostim_tooltip_auto_hide_bar")
	ElseIf (Option == SetSlowMoOrgasms)
		SetInfoText("$ostim_tooltip_slowmo_orgasms")
	ElseIf (Option == SetBlurOrgasms)
		SetInfoText("$ostim_tooltip_blur_orgasms")
	ElseIf (Option == SetAutoClimaxAnims)
		SetInfoText("$ostim_tooltip_auto_climax_anims")
	ElseIf (Option == SetDomLightMode)
		SetInfoText("$ostim_tooltip_dom_light")
	ElseIf (Option == SetSubLightMode)
		SetInfoText("$ostim_tooltip_sub_light")
	ElseIf (Option == SetCameraSpeed)
		SetInfoText("$ostim_tooltip_fc_speed")
	ElseIf (Option == SetUseFreeCam)
		SetInfoText("$ostim_tooltip_auto_fc")
	ElseIf (Option == SetDomLightBrightness)
		SetInfoText("$ostim_tooltip_dom_brightness")
	ElseIf (Option == SetSubLightBrightness)
		SetInfoText("$ostim_tooltip_sub_brightness")
	ElseIf (Option == SetOnlyLightInDark)
		SetInfoText("$ostim_tooltip_dark_light")
	ElseIf (Option == SetUseScreenShake)
		SetInfoText("$ostim_tooltip_screen_shake")
	ElseIf (Option == SetBedRealignment)
		SetInfoText("$ostim_tooltip_bed_realignment")
	ElseIf (Option == SetThanks)
		SetInfoText("$ostim_tooltip_thanks")
	ElseIf (Option == ExportSettings)
		SetInfoText("$ostim_tooltip_export")
	ElseIf (Option == ImportSettings)
		SetInfoText("$ostim_tooltip_import")
	ElseIf (Option == ImportDefaultSettings)
		SetInfoText("$ostim_tooltip_import_default")
	EndIf
EndEvent

Event OnOptionMenuOpen(Int Option)
	Main.PlayTickBig()
	If (Option == SetDomLightmode)
		SetMenuDialogOptions(DomLightModeList)
		;SetMenuDialogStartIndex(DifficultyIndex)
		;SetMenuDialogDefaultIndex(1)
	ElseIf (Option == SetSubLightMode)
		SetMenuDialogOptions(SubLightModeList)
	ElseIf (Option == SetDomLightBrightness)
		SetMenuDialogOptions(DomLightBrightList)
	ElseIf (Option == SetSubLightBrightness)
		SetMenuDialogOptions(SubLightBrightList)
	EndIf
EndEvent

Event OnOptionMenuAccept(Int Option, Int Index)
	Main.PlayTickBig()
	If (Option == SetDomLightMode)
		Main.DomLightPos = Index
		SetMenuOptionValue(SetDomLightMode, DomLightModeList[Index])
	ElseIf (Option == SetSubLightMode)
		Main.SubLightPos = Index
		SetMenuOptionValue(Option, SubLightModeList[Index])
	ElseIf (Option == SetDomLightBrightness)
		Main.DomLightBrightness = Index
		SetMenuOptionValue(Option, DomLightBrightList[Index])
	ElseIf (Option == SetSubLightBrightness)
		Main.SubLightBrightness = Index
		SetMenuOptionValue(Option, SubLightBrightList[Index])
	EndIf
EndEvent

Event OnOptionSliderOpen(Int Option)
	Main.PlayTickBig()

	If currPage == "$ostim_page_undress"
		If Option == SetRemoveWeaponsWithSlot
			SetSliderDialogStartValue(Main.RemoveWeaponsWithSlot)
			SetSliderDialogDefaultValue(32)
			SetSliderDialogRange(30, 60)
			SetSliderDialogInterval(1)
		EndIf

		Return
	EndIf

	If (Option == SetFurnitureSearchDistance)
		SetSliderDialogStartValue(Main.FurnitureSearchDistance)
		SetSliderDialogDefaultValue(15.0)
		SetSliderDialogRange(1, 30)
		SetSliderDialogInterval(1)
	ElseIf Option == SetResetClutterRadius
		SetSliderDialogStartValue(Main.ResetClutterRadius)
		SetSliderDialogDefaultValue(5.0)
		SetSliderDialogRange(1, 30)
		SetSliderDialogInterval(1)
	ElseIf (Option == SetCustomTimescale)
		SetSliderDialogStartValue(Main.CustomTimescale)
		SetSliderDialogDefaultValue(0.0)
		SetSliderDialogRange(0, 40)
		SetSliderDialogInterval(1)
	ElseIf (Option == SetFreeCamFOV)
		SetSliderDialogStartValue(Main.FreecamFOV)
		SetSliderDialogDefaultValue(45.0)
		SetSliderDialogRange(1, 120)
		SetSliderDialogInterval(1)
	ElseIf (Option == SetCameraSpeed)
		SetSliderDialogStartValue(Main.FreecamSpeed)
		SetSliderDialogDefaultValue(3.0)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(0.5)
	ElseIf (Option == SetBedRealignment)
		SetSliderDialogStartValue(Main.BedRealignment)
		SetSliderDialogDefaultValue(0.0)
		SetSliderDialogRange(-250, 250)
		SetSliderDialogInterval(1)
	ElseIf (Option == SetAIChangeChance)
		SetSliderDialogStartValue(Main.AiSwitchChance)
		SetSliderDialogDefaultValue(6.0)
		SetSliderDialogRange(0, 100)
		SetSliderDialogInterval(1)
	elseif (option == SetORDifficulty)
		SetSliderDialogStartValue(GetExternalInt(oromance, GVORDifficulty))
		SetSliderDialogDefaultValue(0.0)
		SetSliderDialogRange(-100, 150)
		SetSliderDialogInterval(1)
	elseif (option == SetOCBounty)
		SetSliderDialogStartValue(StorageUtil.GetIntValue(none, SUOCBounty))
		SetSliderDialogDefaultValue(200)
		SetSliderDialogRange(1, 2000)
		SetSliderDialogInterval(1)
	elseif (option == SetOPFreq)
		SetSliderDialogStartValue(StorageUtil.GetIntValue(none, SUOPFreq))
		SetSliderDialogDefaultValue(0)
		SetSliderDialogRange(-80, 80)
		SetSliderDialogInterval(1)
	EndIf
EndEvent

Event OnOptionSliderAccept(Int Option, Float Value)
	Main.PlayTickBig()

	If currPage == "$ostim_page_undress"
		If Option == SetRemoveWeaponsWithSlot
			Main.RemoveWeaponsWithSlot = Value as int
			SetSliderOptionValue(SetRemoveWeaponsWithSlot, Value, "{0}")
		EndIf

		Return
	EndIf

	if (option == SetORDifficulty)
		SetExternalInt(oromance, GVORDifficulty, value as int)
		SetSliderOptionValue(SetORDifficulty, Value as int, "{0}")
	Elseif (option == SetOCBounty)
		StorageUtil.SetIntValue(none, SUOCBounty, value as int)
		SetSliderOptionValue(SetOCBounty, Value, "{0} Gold")
	Elseif (option == SetOPFreq)
		StorageUtil.SetIntValue(none, SUOPFreq, value as int)
		SetSliderOptionValue(SetOPFreq, Value, "{0}")
	ElseIf (Option == SetFurnitureSearchDistance)
		Main.FurnitureSearchDistance = (Value as Int)
		SetSliderOptionValue(Option, Value, "{0} Meters")
	ElseIf (Option == SetCustomTimescale)
		Main.CustomTimescale = (Value as Int)
		SetSliderOptionValue(Option, Value, "{0}")
	ElseIf (Option == SetFreeCamFOV)
		Main.FreecamFOV = (Value as Int)
		SetSliderOptionValue(Option, Value, "{0}")
	ElseIf (Option == SetCameraSpeed)
		Main.FreecamSpeed = Value
		SetSliderOptionValue(Option, Value, "{1}")
	ElseIf (Option == SetBedRealignment)
		Main.BedRealignment = (Value as Int)
		SetSliderOptionValue(Option, Value, "{0} Units")
	ElseIf (Option == SetAIChangeChance)
		Main.AiSwitchChance = (Value as Int)
		SetSliderOptionValue(Option, Value, "{0}")
	EndIf
EndEvent

Event OnOptionKeyMapChange(Int Option, Int KeyCode, String ConflictControl, String ConflictName)
	Main.PlayTickBig()
	If (Option == SetORKey)
		SetExternalInt(oromance, gvorkey, KeyCode)
		SetKeyMapOptionValue(Option, KeyCode)
	Elseif (Option == SetORLeft)
		SetExternalInt(oromance, GVORLeft, KeyCode)
		SetKeyMapOptionValue(Option, KeyCode)
	Elseif (Option == SetOSKey)
		StorageUtil.SetIntValue(none, "osearch.key", keycode)
		SetKeyMapOptionValue(Option, KeyCode)
	Elseif (Option == SetOAKey)
		StorageUtil.SetIntValue(none, "oaroused.key", keycode)
		SetKeyMapOptionValue(Option, KeyCode)
	Elseif (Option == SetORRight)
		SetExternalInt(oromance, GVORRight, KeyCode)
		SetKeyMapOptionValue(Option, KeyCode)
	Endif
EndEvent

function DrawSlotPage()
	UndressingSlotMask = OData.GetUndressingSlotMask()
	SlotSets = new int[31]

	int i = 0
	int slot = 1

	While i < 31
		; TODO update to slotmask
		SlotSets[i] = AddToggleOption("$ostim_slot_" + (30 + i), Math.LogicalAnd(UndressingSlotMask, slot))
		i += 1
		slot *= 2
	EndWhile

endfunction

Function OnSlotSelect(int option)
	int i = SlotSets.Length
	While i
		i -= 1
		If SlotSets[i] == option
			int Mask = Math.Pow(2, i) as int
			OSexIntegrationMain.Console(Mask)
			UndressingSlotMask = Math.LogicalXor(UndressingSlotMask, Mask)
			OData.SetUndressingSlotMask(UndressingSlotMask)
			SetToggleOptionValue(option, Math.LogicalAnd(UndressingSlotMask, Mask))

			Return
		EndIf
	EndWhile
	debug.messagebox("$ostim_message_slot_error")
EndFunction

Function OnSlotMouseOver(int option)
	int i = SlotSets.Length
	While i
		i -= 1
		If SlotSets[i] == option
			int Slot = i + 30
			armor equipped = playerref.getEquippedArmorInSlot(slot) ; se exclusive

			if equipped
				SetInfoText("$ostim_slot_contains{" + equipped.getname() + "}{" + equipped.GetSlotMask() + "}")
			else
				SetInfoText("$ostim_slots_empty")
			endif

			Return
		EndIf
	EndWhile
endfunction

Event OnGameReload()
	Parent.OnGameReload()
EndEvent

Bool Color1
Function AddColoredHeader(String In)
	String Blue = "#6699ff"
	String Pink = "#ff3389"
	String Color
	If Color1
		Color = Pink
		Color1 = False
	Else
		Color = Blue
		Color1 = True
	EndIf

	AddHeaderOption("<font color='" + Color +"'>" + In)
EndFunction

Function ExportSettings()
	; Export to file.
	int OstimSettingsFile = JMap.object()
	
	ShowMessage("$ostim_message_export", false)
	
	osexintegrationmain.Console("Saving Ostim settings.")

	; Sex settings export.
	JMap.SetInt(OstimSettingsFile, "SetEndOnOrgasm", Main.EndOnDomOrgasm as Int)
	JMap.SetInt(OstimSettingsFile, "SetEndOnSubOrgasm", Main.EndOnSubOrgasm as Int)
	JMap.SetInt(OstimSettingsFile, "SetEndOnBothOrgasm", Main.RequireBothOrgasmsToFinish as Int)
	JMap.SetInt(OstimSettingsFile, "SetActorSpeedControl", Main.EnableActorSpeedControl as Int)
	JMap.SetInt(OstimSettingsFile, "SetResetPosition", Main.ResetPosAfterSceneEnd as Int)
	JMap.SetFlt(OstimSettingsFile, "SetsexExcitementMult", Main.MaleSexExcitementMult as Float)
	JMap.SetFlt(OstimSettingsFile, "SetFemaleSexExcitementMult", Main.FemaleSexExcitementMult as Float)
	JMap.SetInt(OstimSettingsFile, "SetClipinglessFirstPerson", Main.EnableImprovedCamSupport as Int)
	JMap.SetInt(OstimSettingsFile, "SetEndAfterActorHit", Main.EndAfterActorHit as Int)
	JMap.SetInt(OstimSettingsFile, "SetUseRumble", Main.UseRumble as Int)
	JMap.SetInt(OstimSettingsFile, "SetUseScreenShake", Main.UseScreenShake as Int)
	JMap.SetInt(OstimSettingsFile, "SetScaling", Main.DisableScaling as Int)
	JMap.SetInt(OstimSettingsFile, "SetSchlongBending", Main.DisableSchlongBending As Int)
	JMap.SetInt(OStimSettingsFile, "SetUseIntroScenes", Main.UseIntroScenes As Int)
	JMap.SetInt(OstimSettingsFile, "SetOnlyGayAnimsInGayScenes", Main.OnlyGayAnimsInGayScenes as Int)

	; Player roles settings.
	JMap.SetInt(OstimSettingsFile, "PlayerAlwaysSubStraight", main.PlayerAlwaysSubStraight as Int)
	JMap.SetInt(OstimSettingsFile, "PlayerAlwaysSubGay", main.PlayerAlwaysSubGay as Int)
	JMap.SetInt(OstimSettingsFile, "PlayerAlwaysDomStraight", main.PlayerAlwaysDomStraight as Int)
	JMap.SetInt(OstimSettingsFile, "PlayerAlwaysDomGay", main.PlayerAlwaysDomGay as Int)
	
	; Clothes settings export.
	JMap.SetInt(OstimSettingsFile, "SetAlwaysUndressAtStart", Main.AlwaysUndressAtAnimStart as Int)
	JMap.SetInt(OstimSettingsFile, "SetRemoveWeaponsAtStart", Main.RemoveWeaponsAtStart as Int)
	JMap.SetInt(OstimSettingsFile, "SetUndressIfNeed", Main.AutoUndressIfNeeded as Int)
	JMap.SetInt(OstimSettingsFile, "SetPartialUndressing", Main.PartialUndressing as Int)
	JMap.SetInt(OstimSettingsFile, "SetRemoveWeaponsWithSlot", Main.RemoveWeaponsWithSlot as Int)
	JMap.SetInt(OstimSettingsFile, "SetAnimateRedress", Main.FullyAnimateRedress as Int)
	JMap.SetInt(OstimSettingsFile, "SetUndressingSlotMask", OData.GetUndressingSlotMask())

	; Bar settings export.
	JMap.SetInt(OstimSettingsFile, "SetSubBar", Main.EnableSubBar as Int)
	JMap.SetInt(OstimSettingsFile, "SetDomBar", Main.EnableDomBar as Int)
	JMap.SetInt(OstimSettingsFile, "SetThirdBar", Main.EnableThirdBar as Int)
	JMap.SetInt(OstimSettingsFile, "SetAutoHideBar", Main.AutoHideBars as Int)
	JMap.SetInt(OstimSettingsFile, "SetMatchColorToGender", Main.MatchBarColorToGender as Int)
	JMap.SetInt(OstimSettingsFile, "SetHideNPCOnNPCBars", Main.HideBarsInNPCScenes as Int)

	; Orgasm settings export.
	JMap.SetInt(OstimSettingsFile, "SetSlowMoOrgasms", Main.SlowMoOnOrgasm as Int)
	JMap.SetInt(OstimSettingsFile, "SetBlurOrgasms", Main.BlurOnOrgasm as Int)
	JMap.SetInt(OstimSettingsFile, "SetAutoClimaxAnims", Main.AutoClimaxAnimations as Int)

	; Light settings export.
	Jmap.SetInt(OstimSettingsFile, "SetDomLightMode", Main.DomLightPos as Int)
	Jmap.SetInt(OstimSettingsFile, "SetSubLightMode", Main.SubLightPos as Int)
	Jmap.SetInt(OstimSettingsFile, "SetSubLightBrightness", Main.SubLightBrightness as Int)
	Jmap.SetInt(OstimSettingsFile, "SetDomLightBrightness", Main.DomLightBrightness as Int)
	Jmap.SetInt(OstimSettingsFile, "SetOnlyLightInDark", Main.LowLightLevelLightsOnly as Int)
	
	; Keys settings export.
	JMap.SetInt(OstimSettingsFile, "SetKeymap", Main.KeyMap as Int)
	JMap.SetInt(OstimSettingsFile, "SetKeyUp", Main.SpeedUpKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetKeyDown", Main.SpeedDownKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetPullOut", Main.PullOutKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetControlToggle", Main.ControlToggleKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetFreecamToggleKey", Main.FreecamKey as Int)

	; Bed settings export.
	JMap.SetInt(OstimSettingsFile, "SetEnableFurniture", Main.UseFurniture as Int)
	JMap.SetInt(OstimSettingsFile, "SetSelectFurniture", Main.SelectFurniture as Int)
	JMap.SetInt(OstimSettingsFile, "SetFurnitureSearchDistance", Main.FurnitureSearchDistance as Int)
	JMap.SetInt(OstimSettingsFile, "SetResetClutter", Main.ResetClutter as Int)
	JMap.SetInt(OstimSettingsFile, "SetResetClutterRadius", Main.ResetClutterRadius)
	JMap.SetInt(OstimSettingsFile, "SetBedRealignment", Main.BedRealignment as Int)

	; Expressions
	JMap.SetInt(OstimSettingsFile, "SetExpressionDurationMin", Main.ExpressionDurationMin)
	JMap.SetInt(OstimSettingsFile, "SetExpressionDurationMax", Main.ExpressionDurationMax)

	; Ai/Control settings export.
	JMap.SetInt(OstimSettingsFile, "SetAIControl", Main.UseAIControl as Int)
	JMap.SetInt(OstimSettingsFile, "SetForceAIIfAttacking", Main.UseAIPlayerAggressor as Int)
	JMap.SetInt(OstimSettingsFile, "SetForceAIIfAttacked", Main.UseAIPlayerAggressed as Int)
	JMap.SetInt(OstimSettingsFile, "SetForceAIInConsensualScenes", Main.UseAINonAggressive as Int)
	JMap.SetInt(OstimSettingsFile, "SetForceAIForMasturbation", Main.UseAIMasturbation as Int)
	JMap.SetInt(OstimSettingsFile, "SetAIChangeChance", Main.AiSwitchChance as Int)

	; Camera settings export.
	JMap.SetInt(OstimSettingsFile, "SetUseFreeCam", Main.UseFreeCam as Int)
	JMap.SetInt(OstimSettingsFile, "SetFreeCamFOV", Main.FreecamFOV as Int)
	JMap.SetInt(OstimSettingsFile, "SetCameraSpeed", Main.FreecamSpeed as Int)
	JMap.SetInt(OstimSettingsFile, "SetForceFirstPerson", Main.ForceFirstPersonAfter as Int)

	; OSA keys settings export.
	JMap.SetInt(OstimSettingsFile, "SetOsaMainMenuKey", OSAControl.osaMainMenuKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetOsaUpKey", OSAControl.osaUpKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetOsaDownKey", OSAControl.osaDownKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetOsaLeftKey", OSAControl.osaLeftKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetOsaRightKey", OSAControl.osaRightKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetOsaTogKey", OSAControl.osaTogKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetOsaYesKey", OSAControl.osaYesKey as Int)
	JMap.SetInt(OstimSettingsFile, "SetOsaEndKey", OSAControl.osaEndKey as Int)

	; Misc settings export.
	JMap.SetInt(OstimSettingsFile, "SetCustomTimescale", Main.CustomTimescale as Int)

	JMap.SetInt(OstimSettingsFile, "SetTutorialMessages", Main.ShowTutorials as Int)

	JMap.SetInt(OstimSettingsFile, "SetUseFades", Main.UseFades as Int)
	JMap.SetInt(OstimSettingsFile, "SetUseAutoFades", Main.UseAutoFades as Int)

	; addon stuff
	if main.IsModLoaded(ORomance)
		osexintegrationmain.Console("Saving ORomance settings.")
		JMap.setInt(OstimSettingsFile, "savedORomance", 1)
		JMap.setInt(OstimSettingsFile, "SetORSexuality", (GetExternalBool(ORomance, GVORSexuality)) as int)
		JMap.setInt(OstimSettingsFile, "SetORDifficulty", (GetExternalInt(ORomance, GVORDifficulty)) as int)
		JMap.setInt(OstimSettingsFile, "SetORKey", GetExternalInt(oromance, gvorkey))
		JMap.setInt(OstimSettingsFile, "SetORColorblind", GetExternalBool(ORomance, GVORColorblind) as int)
		JMap.setInt(OstimSettingsFile, "SetORLeft",  GetExternalInt(oromance, GVORLeft))
		JMap.setInt(OstimSettingsFile, "SetORRight", GetExternalInt(oromance, GVORRight))
		JMap.setInt(OstimSettingsFile, "SetORNakadashi", GetExternalBool(ORomance, GVORNakadashi) as int)
	Else
		JMap.setInt(OstimSettingsFile, "savedORomance", 0)
	endif

	if main.IsModLoaded(OSearch)
		osexintegrationmain.Console("Saving OSearch settings.")
		JMap.setInt(OstimSettingsFile, "savedOSearch", 1)
		JMap.setInt(OstimSettingsFile, "SetOSKey", StorageUtil.GetIntValue(none, SUOSKey))
		JMap.setInt(OstimSettingsFile, "SetOSAllowSex", StorageUtil.GetIntValue(none, SUOSAllowSex))
		JMap.setInt(OstimSettingsFile, "SetOSAllowHub", StorageUtil.GetIntValue(none, SUOSAllowHub))
		JMap.setInt(OstimSettingsFile, "SetOSAllowTransitory", StorageUtil.GetIntValue(none, SUOSAllowTransitory))
	else
		JMap.setInt(OstimSettingsFile, "savedOSearch", 0)
	endif

	if main.IsModLoaded(OCrime)
		osexintegrationmain.Console("Saving OCrime settings.")
		JMap.setInt(OstimSettingsFile, "savedOCrime", 1)
		JMap.setInt(OstimSettingsFile, "SetOCBounty", StorageUtil.GetIntValue(none, suocbounty))
	Else
		JMap.setInt(OstimSettingsFile, "savedOCrime", 0)
	endif

	if main.IsModLoaded(OAroused)
		osexintegrationmain.Console("Saving OAroused settings.")
		JMap.setInt(OstimSettingsFile, "savedOAroused", 1)
		JMap.setInt(OstimSettingsFile, "SetOAKey", StorageUtil.GetIntValue(none, SUOAKey))
		JMap.setInt(OstimSettingsFile, "SetOARequireLowArousalBeforeEnd", StorageUtil.GetIntValue(none, SUOALowArousalReq))
		JMap.setInt(OstimSettingsFile, "SetOAStatBuffs", StorageUtil.GetIntValue(none, SUOAStatBuffs))
		JMap.setInt(OstimSettingsFile, "SetOANudityBroadcast", StorageUtil.GetIntValue(none, SUOANudityBroadcast))
	Else
		JMap.setInt(OstimSettingsFile, "savedOAroused", 0)
	endif

	; Save to file.
	JMap.SetInt(OstimSettingsFile, "OStimAPIVersion", outils.getostim().getapiversion())
	osexintegrationmain.Console("Saved Ostim settings.")
	Jvalue.WriteToFile(OstimSettingsFile, JContainers.UserDirectory() + "OstimMCMSettings.json")
	
	OData.ExportSettings()

	; Force page reset to show updated changes.
	ForcePageReset()
EndFunction

Function ImportSettings(bool default = false)
	osexintegrationmain.Console("Loading Ostim settings.")

	; Import from file.
	int OstimSettingsFile
	if !default
		int OstimSettingsFileAlt

		if (JContainers.FileExistsAtPath(JContainers.UserDirectory() + "OstimMCMSettings.json"))
			OstimSettingsFile = JValue.readFromFile(JContainers.UserDirectory() + "OstimMCMSettings.json")
		endif

		if (JContainers.FileExistsAtPath(".\\Data\\OstimMCMSettings.json"))
			; Tries to import from Data folder as well, this is to allow Modlist creators to package configuration files as mods for mo2 or vortex.
			OstimSettingsFileAlt = JValue.readFromFile(".\\Data\\OstimMCMSettings.json")
		endif

		if (OstimSettingsFile == False && OstimSettingsFileAlt == False)
			osexintegrationmain.Console(osanative.translate("$ostim_import_no_file"))
			return
		ElseIf (OstimSettingsFile == False && OstimSettingsFileAlt == True)
			OstimSettingsFile = OstimSettingsFileAlt
			osexintegrationmain.Console(osanative.translate("$ostim_message_import_ml_settings"))
		Else
			osexintegrationmain.Console(osanative.translate("$ostim_message_import"))
		EndIf

		if (outils.getostim().getapiversion() != JMap.GetInt(OstimSettingsFile, "OStimAPIVersion") && !OstimSettingsFileAlt) ;if api version is different, and didn't load modlist setting file from data folder.
			osexintegrationmain.Console(osanative.translate("$ostim_message_import_old_api"))
			outils.getostim().DisplayToastAsync(osanative.translate("$ostim_message_import_old_api"), 10)
		endif
	Else
		if (JContainers.FileExistsAtPath(".\\Data\\Interface\\Ostim\\DefaultOstimMCMSettings.json"))
			OstimSettingsFile = JValue.readFromFile(".\\Data\\Interface\\Ostim\\DefaultOstimMCMSettings.json")
		Else
			ShowMessage("$ostim_default_missing_error", false)
			return
		endif
	endif
	
	; Sex settings import.
	Main.EndOnDomOrgasm = Jmap.GetInt(OstimSettingsFile, "SetEndOnOrgasm")
	Main.EndOnSubOrgasm = JMap.GetInt(OstimSettingsFile, "SetEndOnSubOrgasm")
	Main.RequireBothOrgasmsToFinish = JMap.GetInt(OstimSettingsFile, "SetEndOnBothOrgasm")
	Main.EnableActorSpeedControl = JMap.GetInt(OstimSettingsFile, "SetActorSpeedControl")
	Main.ResetPosAfterSceneEnd = JMap.GetInt(OstimSettingsFile, "SetResetPosition", 1)
	Main.MaleSexExcitementMult = JMap.GetFlt(OstimSettingsFile, "SetsexExcitementMult", 1.0)
	Main.FemaleSexExcitementMult = JMap.GetFlt(OstimSettingsFile, "SetFemaleSexExcitementMult", 1.0)
	Main.EnableImprovedCamSupport = JMap.GetInt(OstimSettingsFile, "SetClipinglessFirstPerson")
	Main.EndAfterActorHit = JMap.GetInt(OstimSettingsFile, "SetEndAfterActorHit")
	Main.UseRumble = JMap.GetInt(OstimSettingsFile, "SetUseRumble")
	Main.UseScreenShake = JMap.GetInt(OstimSettingsFile, "SetUseScreenShake")
	Main.DisableScaling = JMap.GetInt(OstimSettingsFile, "SetScaling")
	Main.DisableSchlongBending = JMap.GetInt(OstimSettingsFile, "SetSchlongBending")
	Main.UseIntroScenes = JMap.GetInt(OstimSettingsFile, "SetUseIntroScenes", 1)
	Main.OnlyGayAnimsInGayScenes = JMap.GetInt(OstimSettingsFile, "SetOnlyGayAnimsInGayScenes")

	;Player Roles settings
	Main.PlayerAlwaysSubStraight = Jmap.GetInt(OstimSettingsFile, "PlayerAlwaysSubStraight")
	Main.PlayerAlwaysSubGay = Jmap.GetInt(OstimSettingsFile, "PlayerAlwaysSubGay")
	Main.PlayerAlwaysDomStraight = Jmap.GetInt(OstimSettingsFile, "PlayerAlwaysDomStraight")
	Main.PlayerAlwaysDomGay = Jmap.GetInt(OstimSettingsFile, "PlayerAlwaysDomGay")
	
	; Clothes settings import.
	Main.AlwaysUndressAtAnimStart = JMap.GetInt(OstimSettingsFile, "SetAlwaysUndressAtStart")
	Main.RemoveWeaponsAtStart = JMap.GetInt(OstimSettingsFile, "SetRemoveWeaponsAtStart", 1)
	Main.AutoUndressIfNeeded = JMap.GetInt(OstimSettingsFile, "SetUndressIfNeed", 1)
	Main.PartialUndressing = JMap.GetInt(OstimSettingsFile, "SetPartialUndressing", 1)
	Main.RemoveWeaponsWithSlot = JMap.GetInt(OstimSettingsFile, "SetRemoveWeaponsWithSlot", 32)
	Main.FullyAnimateRedress = JMap.GetInt(OstimSettingsFile, "SetAnimateRedress")
	OData.SetUndressingSlotMask(JMap.GetInt(OstimSettingsFile, "SetUndressingSlotMask", 0x3D8BC39D))
	
	; Bar settings import.
	Main.EnableSubBar = JMap.GetInt(OstimSettingsFile, "SetSubBar")
	Main.EnableDomBar = JMap.GetInt(OstimSettingsFile, "SetDomBar")
	Main.EnableThirdBar = JMap.GetInt(OstimSettingsFile, "SetThirdBar")
	Main.AutoHideBars = JMap.GetInt(OstimSettingsFile, "SetAutoHideBar")
	Main.MatchBarColorToGender = JMap.GetInt(OstimSettingsFile, "SetMatchColorToGender")
	Main.HideBarsInNPCScenes = JMap.GetInt(OstimSettingsFile, "SetHideNPCOnNPCBars")

	; Light settings export.
	Main.DomLightPos = Jmap.GetInt(OstimSettingsFile, "SetDomLightMode")
	Main.SubLightPos = Jmap.GetInt(OstimSettingsFile, "SetSubLightMode")
	Main.SubLightBrightness = Jmap.GetInt(OstimSettingsFile, "SetSubLightBrightness")
	Main.DomLightBrightness = Jmap.GetInt(OstimSettingsFile, "SetDomLightBrightness")
	Main.LowLightLevelLightsOnly = Jmap.GetInt(OstimSettingsFile, "SetOnlyLightInDark")
	
	; Keys settings import.
	Main.KeyMap = JMap.GetInt(OstimSettingsFile, "SetKeymap")
	Main.RemapStartKey(Main.KeyMap)
	Main.SpeedUpKey = JMap.GetInt(OstimSettingsFile, "SetKeyUp")
	Main.RemapSpeedUpKey(Main.SpeedUpKey)
	Main.SpeedDownKey = JMap.GetInt(OstimSettingsFile, "SetKeyDown")
	Main.RemapSpeedDownKey(Main.SpeedDownKey)
	Main.PullOutKey = JMap.GetInt(OstimSettingsFile, "SetPullOut")
	Main.RemapPullOutKey(Main.PullOutKey)
	Main.ControlToggleKey = JMap.GetInt(OstimSettingsFile, "SetControlToggle")
	Main.RemapControlToggleKey(Main.ControlToggleKey)
	Main.FreecamKey = JMap.GetInt(OstimSettingsFile, "SetFreecamToggleKey")
	Main.RemapFreecamKey(main.FreecamKey)

	; OSA keys settings import.
	OSAControl.osaMainMenuKey =  JMap.GetInt(OstimSettingsFile, "SetOsaMainMenuKey", 156)
	OSAControl.osaUpKey = JMap.GetInt(OstimSettingsFile, "SetOsaUpKey", 72)
	OSAControl.osaDownKey = JMap.GetInt(OstimSettingsFile, "SetOsaDownKey", 76)
	OSAControl.osaLeftKey = JMap.GetInt(OstimSettingsFile, "SetOsaLeftKey", 75)
	OSAControl.osaRightKey = JMap.GetInt(OstimSettingsFile, "SetOsaRightKey", 77)
	OSAControl.osaTogKey = JMap.GetInt(OstimSettingsFile, "SetOsaTogKey", 73)
	OSAControl.osaYesKey = JMap.GetInt(OstimSettingsFile, "SetOsaYesKey", 71)
	OSAControl.osaEndKey = JMap.GetInt(OstimSettingsFile, "SetOsaEndKey", 83)

	; Furniture settings export.
	Main.UseFurniture = JMap.GetInt(OstimSettingsFile, "SetEnableFurniture",1)
	Main.SelectFurniture = JMap.GetInt(OstimSettingsFile, "SetSelectFurniture", 1)
	Main.FurnitureSearchDistance = JMap.GetInt(OstimSettingsFile, "SetFurnitureSearchDistance", 15)
	Main.ResetClutter = JMap.GetInt(OstimSettingsFile, "SetResetClutter", 1)
	Main.ResetClutterRadius = JMap.GetInt(OstimSettingsFile, "SetResetClutterRadius", 5)
	Main.BedRealignment = JMap.GetInt(OstimSettingsFile, "SetBedRealignment")
	Main.AiSwitchChance = JMap.GetInt(OstimSettingsFile, "SetAIChangeChance")

	; Expressions
	Main.ExpressionDurationMin = JMap.GetInt(OstimSettingsFile, "SetExpressionDurationMin", 1000)
	Main.ExpressionDurationMin = JMap.GetInt(OstimSettingsFile, "SetExpressionDurationMin", 3000)
	
	;Orgasm settings
	Main.SlowMoOnOrgasm = JMap.GetInt(OstimSettingsFile, "SetSlowMoOrgasms", 1)
	Main.BlurOnOrgasm = JMap.GetInt(OstimSettingsFile, "SetBlurOrgasms", 1)
	Main.AutoClimaxAnimations = JMap.GetInt(OstimSettingsFile, "SetAutoClimaxAnims")
	
	; Ai/Control settings export.
	Main.UseAIControl = JMap.GetInt(OstimSettingsFile, "SetAIControl")
	Main.UseAIPlayerAggressor = JMap.GetInt(OstimSettingsFile, "SetForceAIIfAttacking")
	Main.UseAIPlayerAggressed = JMap.GetInt(OstimSettingsFile, "SetForceAIIfAttacked")
	Main.UseAINonAggressive = JMap.GetInt(OstimSettingsFile, "SetForceAIInConsensualScenes")
	Main.UseAIMasturbation = JMap.GetInt(OstimSettingsFile, "SetForceAIForMasturbation")
	
	; Camera settings export.
	Main.UseFreeCam = JMap.GetInt(OstimSettingsFile, "SetUseFreeCam")
	Main.FreecamFOV = JMap.GetInt(OstimSettingsFile, "SetFreeCamFOV")
	Main.FreecamSpeed = JMap.GetInt(OstimSettingsFile, "SetCameraSpeed")
	Main.ForceFirstPersonAfter = JMap.GetInt(OstimSettingsFile, "SetForceFirstPerson")

	; Misc settings export.
	Main.CustomTimescale = JMap.GetInt(OstimSettingsFile, "SetCustomTimescale")
	
	Main.UseFades = JMap.GetInt(OstimSettingsFile, "SetUseFades")
	Main.UseAutoFades = JMap.GetInt(OstimSettingsFile, "SetUseAutoFades")

	Main.ShowTutorials = JMap.GetInt(OstimSettingsFile, "SetTutorialMessages")

	if !default ; don't load addon settings for reset to default button
	; addon stuff
		if main.IsModLoaded(ORomance) && JMap.getInt(OstimSettingsFile, "savedORomance") == 1
			osexintegrationmain.Console("Loading ORomance settings.")
			SetExternalBool(ORomance, GVORSexuality, JMap.getInt(OstimSettingsFile, "SetORSexuality") as bool)
			SetExternalInt(ORomance, GVORDifficulty, JMap.getInt(OstimSettingsFile, "SetORDifficulty"))
			SetExternalInt(ORomance, gvorkey, JMap.getInt(OstimSettingsFile, "SetORKey"))
			SetExternalBool(ORomance, GVORColorblind, JMap.getInt(OstimSettingsFile, "SetORColorblind") as bool)
			SetExternalInt(ORomance, GVORLeft, JMap.getInt(OstimSettingsFile, "SetORLeft"))
			SetExternalInt(ORomance, GVORRight, JMap.getInt(OstimSettingsFile, "SetORRight"))
			SetExternalBool(ORomance, GVORNakadashi, JMap.getInt(OstimSettingsFile, "SetORNakadashi") as bool)
		endif

		if main.IsModLoaded(OSearch) && JMap.getInt(OstimSettingsFile, "savedOSearch") == 1
			osexintegrationmain.Console("Loading OSearch settings.")
			 StorageUtil.SetIntValue(none, SUOSKey, JMap.getInt(OstimSettingsFile, "SetOSKey"))
			 StorageUtil.SetIntValue(none, SUOSAllowSex, JMap.getInt(OstimSettingsFile, "SetOSAllowSex"))
			 StorageUtil.SetIntValue(none, SUOSAllowHub, JMap.getInt(OstimSettingsFile, "SetOSAllowHub"))
			 StorageUtil.SetIntValue(none, SUOSAllowTransitory, JMap.getInt(OstimSettingsFile, "SetOSAllowTransitory"))
		endif

		if main.IsModLoaded(OCrime) && JMap.getInt(OstimSettingsFile, "savedOCrime") == 1
			osexintegrationmain.Console("Loading OCrime settings.")
			StorageUtil.SetIntValue(none, suocbounty, JMap.getInt(OstimSettingsFile, "SetOCBounty"))
		endif

		if main.IsModLoaded(OAroused) && JMap.getInt(OstimSettingsFile, "savedOAroused") == 1
			osexintegrationmain.Console("Loading OAroused settings.")
			StorageUtil.SetIntValue(none, SUOAKey, JMap.getInt(OstimSettingsFile, "SetOAKey"))
			StorageUtil.SetIntValue(none, SUOALowArousalReq, JMap.getInt(OstimSettingsFile, "SetOARequireLowArousalBeforeEnd"))
			StorageUtil.SetIntValue(none, SUOAStatBuffs, JMap.getInt(OstimSettingsFile, "SetOAStatBuffs"))
			StorageUtil.SetIntValue(none, SUOANudityBroadcast, JMap.getInt(OstimSettingsFile, "SetOANudityBroadcast"))
		endif
	endif

	OData.ImportSettings()

	osexintegrationmain.Console("Loaded Ostim settings.")
	; Force page reset to show updated changes.
	ForcePageReset()
EndFunction


;  ██████╗ ██████╗ ███╗   ██╗████████╗██████╗  ██████╗ ██╗     ███████╗
; ██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔══██╗██╔═══██╗██║     ██╔════╝
; ██║     ██║   ██║██╔██╗ ██║   ██║   ██████╔╝██║   ██║██║     ███████╗
; ██║     ██║   ██║██║╚██╗██║   ██║   ██╔══██╗██║   ██║██║     ╚════██║
; ╚██████╗╚██████╔╝██║ ╚████║   ██║   ██║  ██║╚██████╔╝███████╗███████║
;  ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝

Function DrawControlsPage()
	SetCursorFillMode(LEFT_TO_RIGHT)
	SetCursorPosition(0)
	AddColoredHeader("$ostim_header_ostim_keys")
	SetCursorPosition(2)
	AddKeyMapOptionST("OID_KeyMain", "$ostim_main_key", Main.KeyMap)
	SetCursorPosition(4)
	AddKeyMapOptionST("OID_KeySpeedUp", "$ostim_speed_up_key", Main.SpeedUpKey)
	SetCursorPosition(6)
	AddKeyMapOptionST("OID_KeySpeedDown", "$ostim_speed_down_key", Main.SpeedDownKey)
	SetCursorPosition(8)
	AddKeyMapOptionST("OID_KeyPullOut", "$ostim_pullout_key", Main.PullOutKey)
	SetCursorPosition(10)
	AddKeyMapOptionST("OID_KeyControlToggle", "$ostim_control_toggle_key", Main.ControlToggleKey)
	SetCursorPosition(12)
	AddKeyMapOptionST("OID_KeyFreeCamToggle", "$ostim_tfc_key", Main.FreecamKey)
	SetCursorPosition(14)
	AddKeyMapOptionST("OID_KeyAlignmentMenu", "$ostim_key_alignment_menu", Main.AlignmentKey)

	SetCursorPosition(1)
	AddColoredHeader("$ostim_header_osa_keys")
	SetCursorPosition(3)
	AddKeyMapOptionST("OID_OSA_KeyMainMenu", "$ostim_osaKeys_mainMenu", OSAControl.osaMainMenuKey)
	SetCursorPosition(5)
	AddKeyMapOptionST("OID_OSA_KeyUp", "$ostim_osaKeys_up", OSAControl.osaUpKey)
	SetCursorPosition(7)
	AddKeyMapOptionST("OID_OSA_KeyDown", "$ostim_osaKeys_down", OSAControl.osaDownKey)
	SetCursorPosition(9)
	AddKeyMapOptionST("OID_OSA_KeyLeft", "$ostim_osaKeys_left", OSAControl.osaLeftKey)
	SetCursorPosition(11)
	AddKeyMapOptionST("OID_OSA_KeyRight", "$ostim_osaKeys_right", OSAControl.osaRightKey)
	SetCursorPosition(13)
	AddKeyMapOptionST("OID_OSA_KeyTog", "$ostim_osaKeys_tog", OSAControl.osaTogKey)
	SetCursorPosition(15)
	AddKeyMapOptionST("OID_OSA_KeyYes", "$ostim_osaKeys_yes", OSAControl.osaYesKey)
	SetCursorPosition(17)
	AddKeyMapOptionST("OID_OSA_KeyEnd", "$ostim_osaKeys_end", OSAControl.osaEndKey)
	SetCursorPosition(19)
	AddToggleOptionST("OID_OSA_ResetKeys", "$ostim_osaKeys_reset", false)
EndFunction


State OID_KeyMain
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_main_key")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		Main.KeyMap = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_KeySpeedUp
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_speed_up_key")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		Main.SpeedUpKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_KeySpeedDown
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_speed_down_key")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		Main.SpeedDownKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_KeyPullOut
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_pullout_key")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		Main.PullOutKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_KeyControlToggle
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_control_toggle_key")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		Main.ControlToggleKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_KeyFreeCamToggle
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_tfc_key")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		Main.FreecamKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_KeyAlignmentMenu
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_key_alignment_menu")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		Main.AlignmentKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState


State OID_OSA_KeyMainMenu
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_osa_main_menu")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		OSAControl.osaMainMenuKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_OSA_KeyUp
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_osa_up")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		OSAControl.osaUpKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_OSA_KeyDown
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_osa_down")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		OSAControl.osaDownKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_OSA_KeyLeft
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_osa_left")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		OSAControl.osaLeftKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_OSA_KeyRight
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_osa_right")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		OSAControl.osaRightKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_OSA_KeyTog
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_osa_tog")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		OSAControl.osaTogKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_OSA_KeyYes
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_osa_yes")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		OSAControl.osaYesKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_OSA_KeyEnd
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_osa_end")
	EndEvent

	Event OnKeyMapChangeST(int KeyCode, string ConflictControl, string ConflictName)
		OSAControl.osaEndKey = KeyCode
		SetKeyMapOptionValueST(KeyCode)
	EndEvent
EndState

State OID_OSA_ResetKeys
	Event OnHighlightST()
		SetInfoText("")
	EndEvent

	Event OnSelectST()
		OSAControl.osaMainMenuKey = osaMainMenuKeyDefault
		SetKeyMapOptionValueST(osaMainMenuKeyDefault, false, "OID_OSA_KeyMainMenu")

		OSAControl.osaUpKey = osaUpKeyDefault
		SetKeyMapOptionValueST(osaUpKeyDefault, false, "OID_OSA_KeyUp")

		OSAControl.osaDownKey = osaDownKeyDefault
		SetKeyMapOptionValueST(osaDownKeyDefault, false, "OID_OSA_KeyDown")

		OSAControl.osaLeftKey = osaLeftKeyDefault
		SetKeyMapOptionValueST(osaLeftKeyDefault, false, "OID_OSA_KeyLeft")

		OSAControl.osaRightKey = osaRightKeyDefault
		SetKeyMapOptionValueST(osaRightKeyDefault, false, "OID_OSA_KeyRight")

		OSAControl.osaTogKey = osaTogKeyDefault
		SetKeyMapOptionValueST(osaTogKeyDefault, false, "OID_OSA_KeyTog")

		OSAControl.osaYesKey = osaYesKeyDefault
		SetKeyMapOptionValueST(osaYesKeyDefault, false, "OID_OSA_KeyYes")

		OSAControl.osaEndKey = osaEndKeyDefault
		SetKeyMapOptionValueST(osaEndKeyDefault, false, "OID_OSA_KeyEnd")

		ShowMessage("$ostim_message_reset_osa_keys", false)
	EndEvent
EndState


; ███████╗██╗  ██╗ ██████╗██╗████████╗███████╗███╗   ███╗███████╗███╗   ██╗████████╗
; ██╔════╝╚██╗██╔╝██╔════╝██║╚══██╔══╝██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝
; █████╗   ╚███╔╝ ██║     ██║   ██║   █████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   
; ██╔══╝   ██╔██╗ ██║     ██║   ██║   ██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   
; ███████╗██╔╝ ██╗╚██████╗██║   ██║   ███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   
; ╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝   ╚═╝   ╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   

Function DrawExcitementPage()
	SetCursorFillMode(TOP_TO_BOTTOM)
	SetCursorPosition(0)
	AddSliderOptionST("OID_MaleExcitementMult", "$ostim_excitement_mult_male", Main.MaleSexExcitementMult, "{1} x")
	SetCursorPosition(2)
	AddSliderOptionST("OID_FemaleExcitementMult", "$ostim_excitement_mult_female", Main.FemaleSexExcitementMult, "{1} x")

	SetCursorPosition(1)
	AddSliderOptionST("OID_ExcitementDecayRate", "$ostim_excitement_decay_rate", Main.ExcitementDecayRate, "{2} /s")
	SetCursorPosition(3)
	AddSliderOptionST("OID_ExcitementDecayGracePeriod", "$ostim_excitement_decay_grace_period", Main.ExcitementDecayGracePeriod / 1000, "{1} s")
EndFunction

State OID_MaleExcitementMult
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_excitement_mult_male")
	EndEvent

	Event OnSliderOpenST()
		SetSliderDialogStartValue(Main.MaleSexExcitementMult)
		SetSliderDialogDefaultValue(1)
		SetSliderDialogRange(0.1, 3.0)
		SetSliderDialogInterval(0.1)
	EndEvent

	Event OnSliderAcceptST(float Value)
		Main.MaleSexExcitementMult = Value
		SetSliderOptionValueST(Value, "{1} x")
	EndEvent
EndState

State OID_FemaleExcitementMult
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_excitement_mult_female")
	EndEvent

	Event OnSliderOpenST()
		SetSliderDialogStartValue(Main.FemaleSexExcitementMult)
		SetSliderDialogDefaultValue(1)
		SetSliderDialogRange(0.1, 3.0)
		SetSliderDialogInterval(0.1)
	EndEvent

	Event OnSliderAcceptST(float Value)
		Main.FemaleSexExcitementMult = Value
		SetSliderOptionValueST(Value, "{1} x")
	EndEvent
EndState

State OID_ExcitementDecayRate
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_excitement_decay_rate")
	EndEvent

	Event OnSliderOpenST()
		SetSliderDialogStartValue(Main.ExcitementDecayRate)
		SetSliderDialogDefaultValue(0.5)
		SetSliderDialogRange(0.1, 5.0)
		SetSliderDialogInterval(0.05)
	EndEvent

	Event OnSliderAcceptST(float Value)
		Main.ExcitementDecayRate = Value
		SetSliderOptionValueST(Value, "{2} /s")
	EndEvent
EndState

State OID_ExcitementDecayGracePeriod
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_excitement_decay_grace_period")
	EndEvent

	Event OnSliderOpenST()
		SetSliderDialogStartValue(Main.ExcitementDecayGracePeriod / 1000)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(0.0, 20.0)
		SetSliderDialogInterval(0.5)
	EndEvent

	Event OnSliderAcceptST(float Value)
		Main.ExcitementDecayGracePeriod = (Value * 1000) as int
		SetSliderOptionValueST(Value, "{1} s")
	EndEvent
EndState


;  ██████╗ ███████╗███╗   ██╗██████╗ ███████╗██████╗     ██████╗  ██████╗ ██╗     ███████╗███████╗
; ██╔════╝ ██╔════╝████╗  ██║██╔══██╗██╔════╝██╔══██╗    ██╔══██╗██╔═══██╗██║     ██╔════╝██╔════╝
; ██║  ███╗█████╗  ██╔██╗ ██║██║  ██║█████╗  ██████╔╝    ██████╔╝██║   ██║██║     █████╗  ███████╗
; ██║   ██║██╔══╝  ██║╚██╗██║██║  ██║██╔══╝  ██╔══██╗    ██╔══██╗██║   ██║██║     ██╔══╝  ╚════██║
; ╚██████╔╝███████╗██║ ╚████║██████╔╝███████╗██║  ██║    ██║  ██║╚██████╔╝███████╗███████╗███████║
;  ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚═╝  ╚═╝    ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚══════╝

Function DrawGenderRolesPage()
	SetCursorFillMode(TOP_TO_BOTTOM)
	SetCursorPosition(0)
	AddColoredHeader("$ostim_header_animation_settings")
	SetCursorPosition(2)
	AddToggleOptionST("OID_ForceGayAnims", "$ostim_force_gay_anims", Main.OnlyGayAnimsInGayScenes)

	SetCursorPosition(6)
	AddColoredHeader("$ostim_header_player_roles")
	SetCursorPosition(8)
	AddToggleOptionST("OID_PlayerAlwaysDomStraight", "$ostim_always_dom_straight", Main.PlayerAlwaysDomStraight)
	SetCursorPosition(10)
	AddToggleOptionST("OID_PlayerAlwaysSubStraight", "$ostim_always_sub_straight", Main.PlayerAlwaysSubStraight)
	SetCursorPosition(12)
	AddToggleOptionST("OID_PlayerAlwaysDomGay", "$ostim_always_dom_gay", Main.PlayerAlwaysDomGay)
	SetCursorPosition(14)
	AddToggleOptionST("OID_PlayerAlwaysSubGay", "$ostim_always_sub_gay", Main.PlayerAlwaysSubGay)


	SetCursorPosition(1)
	AddColoredHeader("$ostim_header_strap_ons")
	SetCursorPosition(3)
	AddToggleOptionST("OID_EquipStrapOnIfNeeded", "$ostim_equip_strap_on_if_needed", Main.EquipStrapOnIfNeeded)
	SetCursorPosition(5)
	AddToggleOptionST("OID_UnequipStrapOnIfNotNeeded", "$ostim_unequip_strap_on_if_not_needed", Main.UnequipStrapOnIfNotNeeded)
	SetCursorPosition(7)
	int UnequipStrapOnIfInWayFlags = OPTION_FLAG_NONE
	If Main.UnequipStrapOnIfNotNeeded
		UnequipStrapOnIfInWayFlags = OPTION_FLAG_DISABLED
	EndIf
	AddToggleOptionST("OID_UnequipStrapOnIfInWay", "$ostim_unequip_strap_on_if_in_way", Main.UnequipStrapOnIfInWay, UnequipStrapOnIfInWayFlags)

	SetCursorPosition(11)
	AddMenuOptionST("OID_DefaultStrapOn", "$ostim_default_strap_on", OData.GetEquipObjectName(0x1, "strapon"))
	SetCursorPosition(13)
	AddMenuOptionST("OID_PlayerStrapOn", "$ostim_player_strap_on", OData.GetEquipObjectName(0x7, "strapon"))
EndFunction

State OID_ForceGayAnims
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_force_gay_anims")
	EndEvent

	Event OnSelectST()
		Main.OnlyGayAnimsInGayScenes = !Main.OnlyGayAnimsInGayScenes
		SetToggleOptionValueST(Main.OnlyGayAnimsInGayScenes)
	EndEvent
EndState

State OID_PlayerAlwaysDomStraight
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_always_dom_straight")
	EndEvent

	Event OnSelectST()
		Main.PlayerAlwaysDomStraight = !Main.PlayerAlwaysDomStraight
		SetToggleOptionValueST(Main.PlayerAlwaysDomStraight)
	EndEvent
EndState

State OID_PlayerAlwaysSubStraight
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_always_sub_straight")
	EndEvent

	Event OnSelectST()
		Main.PlayerAlwaysSubStraight = !Main.PlayerAlwaysSubStraight
		SetToggleOptionValueST(Main.PlayerAlwaysSubStraight)
	EndEvent
EndState

State OID_PlayerAlwaysDomGay
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_always_dom_gay")
	EndEvent

	Event OnSelectST()
		Main.PlayerAlwaysDomGay = !Main.PlayerAlwaysDomGay
		SetToggleOptionValueST(Main.PlayerAlwaysDomGay)
	EndEvent
EndState

State OID_PlayerAlwaysSubGay
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_always_sub_gay")
	EndEvent

	Event OnSelectST()
		Main.PlayerAlwaysSubGay = !Main.PlayerAlwaysSubGay
		SetToggleOptionValueST(Main.PlayerAlwaysSubGay)
	EndEvent
EndState

State OID_EquipStrapOnIfNeeded
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_equip_strap_on_if_needed")
	EndEvent

	Event OnSelectST()
		Main.EquipStrapOnIfNeeded = !Main.EquipStrapOnIfNeeded
		SetToggleOptionValueST(Main.EquipStrapOnIfNeeded)
	EndEvent
EndState

State OID_UnequipStrapOnIfNotNeeded
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_unequip_strap_on_if_not_needed")
	EndEvent

	Event OnSelectST()
		Main.UnequipStrapOnIfNotNeeded = !Main.UnequipStrapOnIfNotNeeded
		SetToggleOptionValueST(Main.UnequipStrapOnIfNotNeeded)

		int UnequipStrapOnIfInWayFlags = OPTION_FLAG_NONE
		If Main.UnequipStrapOnIfNotNeeded
			UnequipStrapOnIfInWayFlags = OPTION_FLAG_DISABLED
		EndIf
		SetOptionFlagsST(UnequipStrapOnIfInWayFlags, false, "OID_UnequipStrapOnIfInWay")
	EndEvent
EndState

State OID_UnequipStrapOnIfInWay
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_unequip_strap_on_if_in_way")
	EndEvent

	Event OnSelectST()
		Main.UnequipStrapOnIfInWay = !Main.UnequipStrapOnIfInWay
		SetToggleOptionValueST(Main.UnequipStrapOnIfInWay)
	EndEvent
EndState

State OID_DefaultStrapOn
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_default_strap_on")
	EndEvent

	Event OnMenuOpenST()
		OpenEquipObjectMenu(0x1, "strapon")
	EndEvent

	Event OnMenuAcceptST(int Index)
		SetEquipObjectID(0x1, "strapon", Index)
	EndEvent

	Event OnDefaultST()
		SetEquipObjectIDToDefault(0x1, "strapon")
	EndEvent
EndState

State OID_PlayerStrapOn
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_player_strap_on")
	EndEvent

	Event OnMenuOpenST()
		OpenEquipObjectMenu(0x7, "strapon")
	EndEvent

	Event OnMenuAcceptST(int Index)
		SetEquipObjectID(0x7, "strapon", Index)
	EndEvent

	Event OnDefaultST()
		SetEquipObjectIDToDefault(0x7, "strapon")
	EndEvent
EndState


; ██╗   ██╗███╗   ██╗██████╗ ██████╗ ███████╗███████╗███████╗██╗███╗   ██╗ ██████╗ 
; ██║   ██║████╗  ██║██╔══██╗██╔══██╗██╔════╝██╔════╝██╔════╝██║████╗  ██║██╔════╝ 
; ██║   ██║██╔██╗ ██║██║  ██║██████╔╝█████╗  ███████╗███████╗██║██╔██╗ ██║██║  ███╗
; ██║   ██║██║╚██╗██║██║  ██║██╔══██╗██╔══╝  ╚════██║╚════██║██║██║╚██╗██║██║   ██║
; ╚██████╔╝██║ ╚████║██████╔╝██║  ██║███████╗███████║███████║██║██║ ╚████║╚██████╔╝
;  ╚═════╝ ╚═╝  ╚═══╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚═╝╚═╝  ╚═══╝ ╚═════╝ 



; ███████╗██╗  ██╗██████╗ ██████╗ ███████╗███████╗███████╗██╗ ██████╗ ███╗   ██╗███████╗
; ██╔════╝╚██╗██╔╝██╔══██╗██╔══██╗██╔════╝██╔════╝██╔════╝██║██╔═══██╗████╗  ██║██╔════╝
; █████╗   ╚███╔╝ ██████╔╝██████╔╝█████╗  ███████╗███████╗██║██║   ██║██╔██╗ ██║███████╗
; ██╔══╝   ██╔██╗ ██╔═══╝ ██╔══██╗██╔══╝  ╚════██║╚════██║██║██║   ██║██║╚██╗██║╚════██║
; ███████╗██╔╝ ██╗██║     ██║  ██║███████╗███████║███████║██║╚██████╔╝██║ ╚████║███████║
; ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

Function DrawExpressionPage()
	SetCursorFillMode(TOP_TO_BOTTOM)
	SetCursorPosition(0)
	AddSliderOptionST("OID_ExpressionDurationMin", "$ostim_expression_duration_min", Main.ExpressionDurationMin / 1000.0, "{2} s")
	SetCursorPosition(2)
	AddSliderOptionST("OID_ExpressionDurationMax", "$ostim_expression_duration_max", Main.ExpressionDurationMax / 1000.0, "{2} s")


	SetCursorPosition(1)
	AddMenuOptionST("OID_DefaultTongueMale", "$ostim_default_tongue_male", OData.GetEquipObjectName(0x0, "tongue"))
	SetCursorPosition(3)
	AddMenuOptionST("OID_DefaultTongueFemale", "$ostim_default_tongue_female", OData.GetEquipObjectName(0x1, "tongue"))
	SetCursorPosition(5)
	AddMenuOptionST("OID_PlayerTongue", "$ostim_player_tongue", OData.GetEquipObjectName(0x7, "tongue"))
EndFunction

State OID_ExpressionDurationMin
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_expression_duration_min")
	EndEvent

	Event OnSliderOpenST()
		SetSliderDialogStartValue(Main.ExpressionDurationMin / 1000.0)
		SetSliderDialogDefaultValue(1)
		SetSliderDialogRange(0.1, 10)
		SetSliderDialogInterval(0.05)
	EndEvent

	Event OnSliderAcceptST(float Value)
		Main.ExpressionDurationMin = (Value * 1000) as int
		SetSliderOptionValueST(Main.ExpressionDurationMin / 1000.0, "{2} s")
		SetSliderOptionValueST(Main.ExpressionDurationMax / 1000.0, "{2} s", false, "OID_ExpressionDurationMax")
	EndEvent
EndState

State OID_ExpressionDurationMax
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_expression_duration_max")
	EndEvent

	Event OnSliderOpenST()
		SetSliderDialogStartValue(Main.ExpressionDurationMax / 1000.0)
		SetSliderDialogDefaultValue(3)
		SetSliderDialogRange(0.1, 10)
		SetSliderDialogInterval(0.05)
	EndEvent

	Event OnSliderAcceptST(float Value)
		Main.ExpressionDurationMax = (Value * 1000) as int
		SetSliderOptionValueST(Main.ExpressionDurationMin / 1000.0, "{2} s", false, "OID_ExpressionDurationMin")
		SetSliderOptionValueST(Main.ExpressionDurationMax / 1000.0, "{2} s")
	EndEvent
EndState

State OID_DefaultTongueMale
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_default_tongue_male")
	EndEvent

	Event OnMenuOpenST()
		OpenEquipObjectMenu(0x0, "tongue")
	EndEvent

	Event OnMenuAcceptST(int Index)
		SetEquipObjectID(0x0, "tongue", Index)
	EndEvent

	Event OnDefaultST()
		SetEquipObjectIDToDefault(0x0, "tongue")
	EndEvent
EndState

State OID_DefaultTongueFemale
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_default_tongue_female")
	EndEvent

	Event OnMenuOpenST()
		OpenEquipObjectMenu(0x1, "tongue")
	EndEvent

	Event OnMenuAcceptST(int Index)
		SetEquipObjectID(0x1, "tongue", Index)
	EndEvent

	Event OnDefaultST()
		SetEquipObjectIDToDefault(0x1, "tongue")
	EndEvent
EndState

State OID_PlayerTongue
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_player_tongue")
	EndEvent

	Event OnMenuOpenST()
		OpenEquipObjectMenu(0x7, "tongue")
	EndEvent

	Event OnMenuAcceptST(int Index)
		SetEquipObjectID(0x7, "tongue", Index)
	EndEvent

	Event OnDefaultST()
		SetEquipObjectIDToDefault(0x7, "tongue")
	EndEvent
EndState


;  █████╗ ██╗     ██╗ ██████╗ ███╗   ██╗███╗   ███╗███████╗███╗   ██╗████████╗
; ██╔══██╗██║     ██║██╔════╝ ████╗  ██║████╗ ████║██╔════╝████╗  ██║╚══██╔══╝
; ███████║██║     ██║██║  ███╗██╔██╗ ██║██╔████╔██║█████╗  ██╔██╗ ██║   ██║ 
; ██╔══██║██║     ██║██║   ██║██║╚██╗██║██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║ 
; ██║  ██║███████╗██║╚██████╔╝██║ ╚████║██║ ╚═╝ ██║███████╗██║ ╚████║   ██║ 
; ╚═╝  ╚═╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝ 

Function DrawAlignmentPage()
	SetCursorFillMode(TOP_TO_BOTTOM)
	SetCursorPosition(0)
	AddToggleOptionST("OID_DisableScaling", "$ostim_scaling", Main.DisableScaling)
	SetCursorPosition(2)
	AddToggleOptionST("OID_DisableSchlongBending", "$ostim_schlong_bending", Main.DisableSchlongBending)

	SetCursorPosition(1)
	AddHeaderOption("$ostim_header_alignment_menu")
	SetCursorPosition(3)
	AddToggleOptionST("OID_AlignmentGroupBySex", "$ostim_alignment_group_by_sex", Main.AlignmentGroupBySex)
	SetCursorPosition(5)
	int AlignmentGroupByHeightFlags = OPTION_FLAG_NONE
	If !Main.DisableScaling
		AlignmentGroupByHeightFlags = OPTION_FLAG_DISABLED
	EndIf
	AddToggleOptionST("OID_AlignmentGroupByHeight", "$ostim_alignment_group_by_height", Main.AlignmentGroupByHeight, AlignmentGroupByHeightFlags)
	SetCursorPosition(7)
	AddToggleOptionST("OID_AlignmentGroupByHeels", "$ostim_alignment_group_by_heels", Main.AlignmentGroupByHeels)
EndFunction

State OID_DisableScaling
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_scaling")
	EndEvent

	Event OnSelectST()
		Main.DisableScaling = !Main.DisableScaling
		SetToggleOptionValueST(Main.DisableScaling)

		int AlignmentGroupByHeightFlags = OPTION_FLAG_NONE
		If !Main.DisableScaling
			AlignmentGroupByHeightFlags = OPTION_FLAG_DISABLED
		EndIf
		SetOptionFlagsST(AlignmentGroupByHeightFlags, false, "OID_AlignmentGroupByHeight")
	EndEvent
EndState

State OID_DisableSchlongBending
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_schlong_bending")
	EndEvent
	
	Event OnSelectST()
		Main.DisableSchlongBending = !Main.DisableSchlongBending
		SetToggleOptionValueST(Main.DisableSchlongBending)
	EndEvent
EndState

State OID_AlignmentGroupBySex
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_alignment_group_by_sex")
	EndEvent

	Event OnSelectST()
		Main.AlignmentGroupBySex = !Main.AlignmentGroupBySex
		SetToggleOptionValueST(Main.AlignmentGroupBySex)
	EndEvent
EndState

State OID_AlignmentGroupByHeight
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_alignment_group_by_height")
	EndEvent

	Event OnSelectST()
		Main.AlignmentGroupByHeight = !Main.AlignmentGroupByHeight
		SetToggleOptionValueST(Main.AlignmentGroupByHeight)
	EndEvent
EndState

State OID_AlignmentGroupByHeels
	Event OnHighlightST()
		SetInfoText("$ostim_tooltip_alignment_group_by_heels")
	EndEvent

	Event OnSelectST()
		Main.AlignmentGroupByHeels = !Main.AlignmentGroupByHeels
		SetToggleOptionValueST(Main.AlignmentGroupByHeels)
	EndEvent
EndState


; ██╗   ██╗████████╗██╗██╗     ██╗████████╗██╗   ██╗
; ██║   ██║╚══██╔══╝██║██║     ██║╚══██╔══╝╚██╗ ██╔╝
; ██║   ██║   ██║   ██║██║     ██║   ██║    ╚████╔╝ 
; ██║   ██║   ██║   ██║██║     ██║   ██║     ╚██╔╝
; ╚██████╔╝   ██║   ██║███████╗██║   ██║      ██║
;  ╚═════╝    ╚═╝   ╚═╝╚══════╝╚═╝   ╚═╝      ╚═╝

string[] EquipObjectPairs

Function OpenEquipObjectMenu(int FormID, string Type)
	EquipObjectPairs = OData.GetEquipObjectPairs(FormID, Type)
	SetMenuDialogOptions(OData.PairsToNames(EquipObjectPairs))
	SetMenuDialogStartIndex(0)
	SetMenuDialogDefaultIndex(0)
EndFunction

Function SetEquipObjectID(int FormID, string Type, int Index)
	OData.SetEquipObjectID(FormID, Type, EquipObjectPairs[Index * 2])
	SetMenuOptionValueST(EquipObjectPairs[Index * 2 + 1])
EndFunction

Function SetEquipObjectIDToDefault(int FormID, string Type)
	string ID = "default"
	If FormID < 2
		ID = "random"
	EndIf
	OData.SetEquipObjectID(FormID, Type, ID)
	SetMenuOptionValueST(ID)
EndFunction