ScriptName OThread

;/* IsRunning
* * checks if the thread is still running
* *
* * @param: ThreadID, the id of the thread
* *
* * @return: true if the thread is still running, otherwise false
*/;
bool Function IsRunning(int ThreadID) Global Native

;/* Stop
* * ends the thread
* *
* * @param: ThreadID, the id of the thread
*/;
Function Stop(int ThreadID) Global Native


;/* GetScene
* * returns the scene id of the scene that is currently running in the thread
* *
* * @param: ThreadID, the id of the thread
* *
* * @return: the scene id, returns "" if the thread is still in startup or ended
*/;
string Function GetScene(int ThreadID) Global Native

;/* NavigateTo
* * tries to naviate the thread to a new scene, if it's not possible warps it there
* *
* * @param: ThreadID, the id of the thread
* * @param: SceneID, the id of the scene to naviate to
*/;
Function NavigateTo(int ThreadID, string SceneID) Global Native

;/* WarpTo
* * warps the thread to a new scene
* *
* * @param: ThreadID, the id of the thread
* * @param: SceneID, the id of the scene to warp to
* * @param: UseFades, if set to true the game will fade out before the scene change and back in afterwards
*/;
Function WarpTo(int ThreadID, string SceneID, bool UseFades = False) Global Native

;/* GetSpeed
* * returns the speed index at which the thread is currently running
* *
* * @param: ThreadID, the id of the thread
* *
* * @return: the speed of the thread, returns -1 if the thread is still in startup or ended
*/;
int Function GetSpeed(int ThreadID) Global Native

;/* SetSpeed
* * sets the speed index at which the thread will run
* * values out of range will be clamped to the available speeds
* *
* * @param: ThreadID, the id of the thread
* * @param: Speed, the speed index to use
*/;
Function SetSpeed(int ThreadID, int Speed) Global Native


;/* GetActors
* * returns the actors of the thread
* *
* * @param: ThreadID, the id of the thread
* *
* * @return: an array of actors
*/;
Actor[] Function GetActors(int ThreadID) Global Native

;/* GetActor
* * returns the actor at the given index
* *
* * @param: ThreadID, the id of the thread
* * @index: the index of the actor
* *
* * @returns: the actor
*/;
Actor Function GetActor(int ThreadID, int Index) Global Native

;/* GetActorPosition
* * returns the index of the actor in the thread
* *
* * @param: ThreadID, the id of the thread
* * @param: Act, the actor to get the position for
* *
* * @return: the actors index, returns -1 the the thread doesn't contain the actor or ended
*/;
int Function GetActorPosition(int ThreadID, Actor Act) Global Native


;/* IsInAutoMode
* * checks if the thread is currently running in automatic mode
* *
* * @param: ThreadID, the id of the thread
* *
* * @return: true if the thread is in auto mode, otherwise false
*/;
bool Function IsInAutoMode(int ThreadID) Global Native

;/* StartAutoMode
* * sets the thread to automatic mode
* *
* * @param: ThreadID, the id of the thread
*/;
Function StartAutoMode(int ThreadID) Global Native

;/* StopAutoMode
* * sets the thread to manual mode
* * for the player thread that means the player is now again in control of the navigation
* * for NPC threads this means they will need to be controlled from the outside
* *
* * @param: ThreadID, the id of the thread
*/;
Function StopAutoMode(int ThreadID) Global Native


;/* CallEvent
* * calls the event for the thread, events and their properties can be defined in data/SKSE/plugins/OStim/events
* * the corresponding mod events will be thrown even if no event was defined for the event name
* *
* * @param: ThreadID, the id of the thread
* * @param: EventName, the name of the event
* * @param: Actor, the index of the event actor
* * @param: Target, the index of the event target, if none is given the actor index is used
* * @param: Performer, the index of the event performer, if none is given the actor index is used
*/;
Function CallEvent(int ThreadID, string EventName, int Actor, int Target = -1, int Performer = -1) Global Native