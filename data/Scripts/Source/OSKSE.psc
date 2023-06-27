;/* OFurniture
* * bunch of functions supposed to be called from the SKSE plugin
* * meant for internal use and not to be called by addons
* * some of these are still save to call, however others are not
* * the ones that aren't will say so in the documentation
*/;
ScriptName OSKSE

;/* GetRmScale
* * returns the value of the RaceMenu height slider added by XPMSSE
* *
* * @param: Act, the actor the check the slider for
* * @param: IsFemale, if the actor is female
* *
* * @return: the height slider value
*/; 
float Function GetRmScale(Actor Act, bool IsFemale) Global
	If nioverride.HasNodeTransformScale(Act, False, IsFemale, "NPC", "RSMPlugin")
			Return nioverride.GetNodeTransformScale(Act, False, IsFemale, "NPC", "RSMPlugin")
		Else
			Return 1
		EndIf
EndFunction

;/* UpdateHeelOffset
* * do NOT ever call this, the .dll caches if the offset is currently removed or not
* * if you bring that out of sync shit goes sideways
*/;
Function UpdateHeelOffset(Actor Act, float Offset, bool Add, bool Remove, bool IsFemale) Global
	If Add
		nioverride.RemoveNodeTransformPosition(Act, false, IsFemale, "NPC", "OStim")
	EndIf
	If Remove
		float[] Pos = new float[3]
		Pos[0] = 0
		Pos[1] = 0
		Pos[2] = -Offset
		nioverride.AddNodeTransformPosition(Act, false, IsFemale, "NPC", "OStim", Pos)
	EndIf
	nioverride.UpdateNodeTransform(Act, false, IsFemale, "NPC")
EndFunction

; TEMPORARY ONLY
; don't call any of these, we will remove them again in later versions

Function StartScene(Actor Dom, Actor Sub, Actor Third) Global
	OUtils.GetOstim().StartScene(Dom, Sub, zThirdActor = Third)
EndFunction

Function Masturbate(Actor Act) Global
	OUtils.GetOStim().Masturbate(Act)
EndFunction

Function ShowBars() Global
	OUtils.GetOStim().ShowBars()
EndFunction

Function EndAnimation() Global
	OUtils.GetOStim().EndAnimation(true)
EndFunction

Function IncreaseSpeed() Global
	OUtils.GetOStim().IncreaseAnimationSpeed()
EndFunction

Function DecreaseSpeed() Global
	OUtils.GetOStim().DecreaseAnimationSpeed()
EndFunction

Function PullOut() Global
	OUtils.GetOStim().PullOut()
EndFunction

Function ChangeNode(string Node) Global
	OUtils.GetOStim().WarpToAnimation(Node)
EndFunction