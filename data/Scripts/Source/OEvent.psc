ScriptName OEvent

Function SendOStimEvent(int ThreadId, string Type, Actor eventActor, Actor eventTarget, Actor eventPerformer) Global
	int eventId = ModEvent.Create("ostimng_event")
	ModEvent.PushInt(eventId, ThreadId)
	ModEvent.PushString(eventId, Type)
	ModEvent.PushForm(eventId, eventActor)
	ModEvent.PushForm(eventId, eventTarget)
	ModEvent.PushForm(eventId, eventPerformer)
	ModEvent.Send(eventId)
EndFunction