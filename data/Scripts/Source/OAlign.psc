ScriptName OAlign

function ToggleMenu() global native


Function SetWorldPosition(actor act, float[] zoffsets, float[] stageCoords)
	;Console(act.GetDisplayName())
	;Console(zoffsets)

	float theta = OUtils.TrigAngleZ(act.GetAngleZ())
	;Console("Theta: " + theta)

	float x = (math.cos(Theta) * zoffsets[0]) - (math.sin(Theta) * zoffsets[1])
	float y = (math.sin(Theta) * zoffsets[0]) + (math.cos(Theta) * zoffsets[1])

	;Console("X:" + x)
	;Console("Y:" + y)    
	if act == Game.GetPlayer() 
		OSANative.SetPositionEx(act, stageCoords[0] + x, stageCoords[1] + y, stageCoords[2] + zoffsets[2])
	else 
		act.setposition(stageCoords[0] + x, stageCoords[1] + y, stageCoords[2] + zoffsets[2])
	endif 

EndFunction