ScriptName OActorUtil

;/* HasSchlong
* * checks if the actor has a schlong
* * if SoS full is not installed or use SoS gender was disabled in the MCM this will simply check for the actors sex
* * if SoS full is installed this will check for the SOS_SchlongifiedFaction
* * additionally it will check for SOS - No Futanari Schlong and SOS Female Pubic Hair
* * (to not consider those schlongified even though they are in the faction)
* * 
* * @param: Act, the actor to check
* *
* * @return: true if the actor has a schlong, otherwise false
*/;
bool Function HasSchlong(Actor Act) Global Native

;/* EmptyArray
* * returns a size zero array of type Actor
* *
* * @return: a size zero array
*/;
Actor[] Function EmptyArray() Global Native

;/* ToArray
* * creates an array out of the given actors, sorts out none entires
* *
* * @param: X, the actors for the array
* *
* * @return: an array of actors
*/;
Actor[] Function ToArray(Actor One = None, Actor Two = None, Actor Three = None, Actor Four = None, Actor Five = None, Actor Six = None, Actor Seven = None, Actor Eight = None, Actor Nine = None, Actor Ten = None) Global Native

;/* SortActors
* * sorts all dominant actors to the front of the array and all non dominant actors to the back
* * within those groups sorts all actors with schlongs to the front and all actors without to the back
* * obeys the MCM settings for two actor scenes if no player index is passed
* * other than this the order is not altered (i.e. the sorting algorithm is stable)
* *
* * @param: Actors, the array of actors to sort
* * @param: DominantActors, an array of dominant actors
* * @param: PlayerIndex, if given the player will be sorted to this index, independent of them having a schlong or not
* *
* * @return: the sorted array 
*/;
Actor[] Function Sort(Actor[] Actors, Actor[] DominantActors, int PlayerIndex = -1) Global Native

;/* SelectIndexAndSortActors
* * pops up the index selection for the player depending on the MCM settings and then sorts the actors according to the selected value
* *
* * @param: Actors, the array of actors to sort
* * @param: DominantActors, a list of dominant actors
* *
* * @return: the sorted array
*/;
Actor[] Function SelectIndexAndSort(Actor[] Actors, Actor[] DominantActors) Global
	OSexIntegrationMain OStim = OUtils.GetOStim()

	bool SelectRole = false
	If Actors.Length >= 3
		SelectRole = OStim.PlayerSelectRoleThreesome
	ElseIf Actors.Length == 2
		If OStim.AppearsFemale(Actors[0]) == OStim.AppearsFemale(Actors[1])
			SelectRole = OStim.PlayerSelectRoleGay
		Else
			SelectRole = OStim.PlayerSelectRoleStraight
		EndIf
	EndIf
	If SelectRole
		OStim.OStimRoleSelectionCount.value = Actors.Length
		int PlayerIndex = OStim.OStimRoleSelectionMessage.Show()
		Return OActor.SortActors(Actors, PlayerIndex)
	Else
		Return Sort(Actors, DominantActors)
	EndIf
EndFunction