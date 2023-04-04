Scriptname _oActro extends activemagiceffect  

import _oGlobal
;OSA Global Functions

actor Property PlayerRef Auto

int glyph

_oOmni Property OSO hidden
    _oOmni function get()
        return Quest.GetQuest("0SA") as _oOmni 
    endFunction
endProperty
;Add the _oOmni persistent script based in quest 0SA


actor[] Actra
string[] ActraIndex
string StageID
string Password
int totalActra

objectReference posObj

spell Property FXSP auto
magiceffect Property FXME auto

bool align = false

Event Oneffectstart (actor tarAct, actor Spot)
stageID = tarAct.GetFactionRank(OSO.OFaction[1])
glyph = OSO.glyph
actra = new Actor[10]
actra[0] = tarAct 
actraIndex = new String[10]
actraIndex[0] = 0
posObj = OSO.GlobalPosition[StageID as int]
password = StageID

registerEvents()

actroReady(stageID)
endEvent

function registerEvents()
RegisterForModEvent("0SA"+"_GameLoaded", "OnGameLoaded")
RegisterForModEvent("0SAO"+Password+"_OEnd", "OnOEnd")
RegisterForModEvent("0SAO"+Password+"_ActraSync", "OnActraSync")
RegisterForModEvent("0SAO"+Password+"_AlignTo", "OnAlignTo")
RegisterForModEvent("0SAO"+Password+"_ActraReveal", "OnActraReveal")
RegisterForModEvent("0SAO"+Password+"_ActraJoin", "OnActraJoin")
RegisterForModEvent("0SAO"+Password+"_ActraRemove", "OnActraRemove")
RegisterForModEvent("0SAO"+Password+"_OSpell", "onOSpell")
RegisterForModEvent("0SAO"+Password+"_OCinemaAudio", "onOCinemaAudio")
RegisterForModEvent("0SAO"+Password+"_Rumble", "onRumble")
RegisterForModEvent("0SAO"+Password+"_Report", "onReport")
endFunction


event onAlignTo(string eventName, string actorLocHub, float alignStyle, Form sender)
int style = alignStyle as int
	if style == 0
		align = true

		; Event name will be something like 0SAO55_AlignTo
		; So, split the name on the '_' character to get the Password reliably
		string[] eventNameSplit = StringUtil.Split(eventName, "_")
		int scenePassword = StringUtil.Substring(eventNameSplit[0], 4) as int

		ObjectReference FurnitureRef = OSO.GetSceneFurniture(scenePassword, Actra[0])

		If FurnitureRef != none
    		float X = FurnitureRef.GetPositionX()
    		float Y = FurnitureRef.GetPositionY()
    		float Z = FurnitureRef.GetPositionZ()
    		float Angle = FurnitureRef.GetAngleZ()
    		float Sin = Math.Sin(Angle)
    		float Cos = Math.Cos(Angle)
    		float[] offset = OFurniture.GetOffset(FurnitureRef)

    		posObj.SetPosition(X + Cos * offset[0] + Sin * offset[1], Y - Sin * offset[0] + Cos * offset[1], Z + offset[2])
    		posObj.SetAngle(0, 0, Angle + Math.RadiansToDegrees(offset[3]))
    	Else
    		OSexintegrationMain.Console("no furniture ref")
        	posObj.moveTo(actra[actorLocHub as int])
    	EndIf
		int count = 0
		while count < totalActra
		;delay To see if it fixes crash
		utility.wait(0.1)
		alignActraStage(actraIndex[count])
		count+=1
		endWhile
	elseIf style == 2
		align = true
		int count = 0
		while count < totalActra
		alignActraStage(actraIndex[count])
		count+=1
		endWhile
	endif
endEvent

event OnActraSync(string eventName, string ComboString, float numArg, Form sender)

String[] stringValues = StringUtil.split(ComboString, ",")
totalActra = stringValues[0] as int
Actor[] Temp = new Actor[10]
String[] TempIndex = new string[10]
int ix = 0

    while ix < totalActra
    int Index = ActraIndex.Find(StringValues[ix+1])
    Temp[ix] = Actra[index]
    TempIndex[ix] = StringValues[ix+1]
    ix+=1
    endWhile

Actra = Temp
ActraIndex = TempIndex

UI.InvokeString("HUD Menu", "_root.WidgetContainer."+glyph+".widget.com.skySyncComplete", StageID)
endEvent


event OnGameLoaded(string c, string a, float t, Form z)
Self.Dispel()
endEvent

event OnEffectFinish(Actor akTarget, Actor akCaster)
if OSO.GlobalPosition[stageID as int]
OSO.GlobalPosition[stageID as int].Delete()
OSO.GlobalPosition[stageID as int] = none
endif
endEvent

event OnOEnd(string eventName, string actorLocHub, float numArg, Form sender)
Self.Dispel()
endEvent


event onRumble(string eventName, string amdur, float range, Form sender)
int Finder = stringutil.find(amdur, ",", 0)
Actra[0].RampRumble(stringUtil.substring(amdur, 0, finder) as float, 1.0, range)
endEvent

event OnOSpell(string eventName, string spellData, float IDK, Form sender)
String[] stringValues = StringUtil.split(spellData, ",")
objectreference CastPoint = Actra[stringValues[1] as int].placeAtMe(OSO.OBlankStatic)
castPoint.MoveToNode(actra[stringValues[1] as int], stringValues[2])
objectreference TargetPoint = Actra[stringValues[1] as int].placeAtMe(OSO.OBlankStatic)
TargetPoint.MoveToNode(actra[stringValues[3] as int], stringValues[4])
FXME.SetProjectile(OSO.OProjectile.GetAt(stringvalues[0] as int) as Projectile)
FXSP.cast(CastPoint, targetPoint)
CastPoint.Delete()
CastPoint = none
TargetPoint.Delete()
TargetPoint = none
endEvent


int actraRevealed = -1
bool stageRunning = false

event OnActraReveal(form zAct, string ActorID)
if !stageRunning
actraRevealed +=1
actor zActra = zAct as Actor
Actra[actraRevealed] = zActra
ActraIndex[actraRevealed] = ActorID
endif
UI.InvokeString("HUD Menu", "_root.WidgetContainer."+glyph+".widget.com.skyActroReadyCheck", StageID)
endEvent



event OnActraRemove(string eventName, string actraIX, float arg, Form actraInc)
int ix = actraIX as int
Actra[ix] = none
ActraIndex[ix] = none
UI.Invoke("HUD Menu", "_root.WidgetContainer."+glyph+".widget.beacon.cbActraRemoveAO")
endEvent


event OnActraJoin(string eventName, string actorID, float arg, Form actraInc)
stageRunning = true
actor newActra = actraInc as actor
	oso.processActraAll(newActra, actorID)
    newActra.SetFactionRank(oso.OFaction[0], 1) 
    newActra.SetFactionRank(oso.OFaction[1], stageID as int) 
int L = totalActra
	actra[L] = newActra
	actraIndex[L] = actorID
    oso.OSpell[0].cast(newActra, newActra)
endEvent


event OnReport(string eventName, string a, float b, Form c)

string[] report = new string[32]

report[0] = "ReportArr"
report[1] = StageID
report[2] = glyph
report[3] = Password
report[9] = totalActra

int i = 0

while i < 10
report[i+10] = actra[i].getActorBase().GetName()+" | "+GetFormID_s(Actra[i].GetActorBase())
report[i+20] = ActraIndex[i]
i+=1
endWhile

UI.InvokeStringA("HUD Menu", "_root.WidgetContainer."+glyph+".widget.beacon.cbStringA", report)
endEvent
