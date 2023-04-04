ScriptName OThread

;/* GetScene
* * returns the scene id of the scene that is currently running in the thread
* *
* * @param: ThreadId, the id of the thread
* *
* * @return: the scene id, returns "" if the thread is still in startup or ended
*/;
string Function GetScene(int ThreadId) Global Native

;/* GetActorPosition
* * returns the index of the actor in the thread
* *
* * @param: ThreadId, the id of the thread
* * @param: Act, the actor to get the position for
* *
* * @return: the actors index, returns -1 the the thread doesn't contain the actor or ended
*/;
int Function GetActorPosition(int ThreadId, Actor Act) Global Native