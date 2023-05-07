ScriptName OThread

;/* GetScene
* * returns the scene id of the scene that is currently running in the thread
* *
* * @param: ThreadID, the id of the thread
* *
* * @return: the scene id, returns "" if the thread is still in startup or ended
*/;
string Function GetScene(int ThreadID) Global Native

;/* GetActorPosition
* * returns the index of the actor in the thread
* *
* * @param: ThreadID, the id of the thread
* * @param: Act, the actor to get the position for
* *
* * @return: the actors index, returns -1 the the thread doesn't contain the actor or ended
*/;
int Function GetActorPosition(int ThreadID, Actor Act) Global Native

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