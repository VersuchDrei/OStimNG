;/* OThread
* * collection of methods to modify threads
* * the thread containing the player will always have the ThreadID 0
* * NPC on NPC threads will always have positive ThreadIDs
*/;
ScriptName OThread

;  ██████╗ ██╗   ██╗██╗ ██████╗██╗  ██╗███████╗████████╗ █████╗ ██████╗ ████████╗
; ██╔═══██╗██║   ██║██║██╔════╝██║ ██╔╝██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝
; ██║   ██║██║   ██║██║██║     █████╔╝ ███████╗   ██║   ███████║██████╔╝   ██║
; ██║▄▄ ██║██║   ██║██║██║     ██╔═██╗ ╚════██║   ██║   ██╔══██║██╔══██╗   ██║
; ╚██████╔╝╚██████╔╝██║╚██████╗██║  ██╗███████║   ██║   ██║  ██║██║  ██║   ██║
;  ╚══▀▀═╝  ╚═════╝ ╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝

;/* QuickStart
* * starts a new thread without the need of a thread builder, but only limited parameters
* * if the actors contain the player the thread will be started asynchronously
* * if no starting animation is passed the actors will get sorted and one will be chosen automatically
* * if no furniture is passed one will be chosen automatically, if a starting animation is passed only beds will be chosen
* *
* * @param: Actors, the actors to be involved in the thread
* * @param: StartingAnimation, the animation to start the scene in
* * @param: FurnitureRef, the furniture to play the scene on
* *
* * @return: the ThreadID of the thread, or -1 if the thread could not be started
*/;
int Function QuickStart(Actor[] Actors, string StartingAnimation = "", ObjectReference FurnitureRef = None) Global Native


;  ██████╗ ███████╗███╗   ██╗███████╗██████╗  █████╗ ██╗     
; ██╔════╝ ██╔════╝████╗  ██║██╔════╝██╔══██╗██╔══██╗██║     
; ██║  ███╗█████╗  ██╔██╗ ██║█████╗  ██████╔╝███████║██║     
; ██║   ██║██╔══╝  ██║╚██╗██║██╔══╝  ██╔══██╗██╔══██║██║     
; ╚██████╔╝███████╗██║ ╚████║███████╗██║  ██║██║  ██║███████╗
;  ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝

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


; ███╗   ██╗ █████╗ ██╗   ██╗██╗ ██████╗  █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
; ████╗  ██║██╔══██╗██║   ██║██║██╔════╝ ██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
; ██╔██╗ ██║███████║██║   ██║██║██║  ███╗███████║   ██║   ██║██║   ██║██╔██╗ ██║
; ██║╚██╗██║██╔══██║╚██╗ ██╔╝██║██║   ██║██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
; ██║ ╚████║██║  ██║ ╚████╔╝ ██║╚██████╔╝██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
; ╚═╝  ╚═══╝╚═╝  ╚═╝  ╚═══╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝

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

;/* AutoTransition
* * plays the auto transition for the thread
* *
* * @param: ThreadID, the id of the thread
* * @param: Type, the type of the auto transition
* *
* * @param: true if the transition exists and was successfully played, otherwise false
*/;
bool Function AutoTransition(int ThreadID, string Type) Global Native

;/* AutoTransitionForActor
* * plays the auto transition for the actor
* *
* * @param: ThreadID, the id of the thread
* * @param: Index, the index of the actor to play the transition for
* * @param: Type, the type of auto transition
* *
* * @return: true if the transition exists and was successfully played, otherwise false
*/;
bool Function AutoTransitionForActor(int ThreadID, int Index, string Type) Global Native

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


;  █████╗  ██████╗████████╗ ██████╗ ██████╗ ███████╗
; ██╔══██╗██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗██╔════╝
; ███████║██║        ██║   ██║   ██║██████╔╝███████╗
; ██╔══██║██║        ██║   ██║   ██║██╔══██╗╚════██║
; ██║  ██║╚██████╗   ██║   ╚██████╔╝██║  ██║███████║
; ╚═╝  ╚═╝ ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝

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
* * @return: the actor
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


;  ██████╗██╗     ██╗███╗   ███╗ █████╗ ██╗  ██╗
; ██╔════╝██║     ██║████╗ ████║██╔══██╗╚██╗██╔╝
; ██║     ██║     ██║██╔████╔██║███████║ ╚███╔╝ 
; ██║     ██║     ██║██║╚██╔╝██║██╔══██║ ██╔██╗ 
; ╚██████╗███████╗██║██║ ╚═╝ ██║██║  ██║██╔╝ ██╗
;  ╚═════╝╚══════╝╚═╝╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝

;/* StallClimax
* * prevents all actors in the thread from climaxing, including the prevention of auto climax animations
* * does not prevent the climaxes of auto climax animations that already started
* *
* * @param: Thread, the id of the thread
*/;
Function StallClimax(int ThreadID) Global Native

;/* PermitClimax
* * permits the actors in the thread to climax again (as in it undoes StallClimax)
* *
* * @param: Thread, the id of the thread
* * @param: PermitActors, if true this also undoes the StallClimax calls for the individual actors
*/;
Function PermitClimax(int ThreadID, bool PermitActors = false) Global Native

;/* IsClimaxStalled
* * checks if this actor is currently prevented from climaxing
* *
* * @param: Thread, the id of the thread
* *
* * @return: true if the actor is currently prevented from climaxing
*/;
bool Function IsClimaxStalled(int ThreadID) Global Native


; ███████╗██╗   ██╗██████╗ ███╗   ██╗██╗████████╗██╗   ██╗██████╗ ███████╗
; ██╔════╝██║   ██║██╔══██╗████╗  ██║██║╚══██╔══╝██║   ██║██╔══██╗██╔════╝
; █████╗  ██║   ██║██████╔╝██╔██╗ ██║██║   ██║   ██║   ██║██████╔╝█████╗  
; ██╔══╝  ██║   ██║██╔══██╗██║╚██╗██║██║   ██║   ██║   ██║██╔══██╗██╔══╝  
; ██║     ╚██████╔╝██║  ██║██║ ╚████║██║   ██║   ╚██████╔╝██║  ██║███████╗
; ╚═╝      ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝

;/* GetFurniture
* * returns the furniture object used by the thread
* *
* * @param: ThreadID, the id of the thread
* *
* * @return: the furniture object, or None if the thread isn't using one
*/;
ObjectReference Function GetFurniture(int ThreadID) Global Native

;/* GetFurnitureType
* * returns the furniture type used in the thread
* *
* * @param: ThreadID, the id of the thread
* *
* * @return: the furniture type, returns "" if the thread is still in startup or ended
*/;
string Function GetFurnitureType(int ThreadID) Global Native


;  █████╗ ██╗   ██╗████████╗ ██████╗     ███╗   ███╗ ██████╗ ██████╗ ███████╗
; ██╔══██╗██║   ██║╚══██╔══╝██╔═══██╗    ████╗ ████║██╔═══██╗██╔══██╗██╔════╝
; ███████║██║   ██║   ██║   ██║   ██║    ██╔████╔██║██║   ██║██║  ██║█████╗  
; ██╔══██║██║   ██║   ██║   ██║   ██║    ██║╚██╔╝██║██║   ██║██║  ██║██╔══╝  
; ██║  ██║╚██████╔╝   ██║   ╚██████╔╝    ██║ ╚═╝ ██║╚██████╔╝██████╔╝███████╗
; ╚═╝  ╚═╝ ╚═════╝    ╚═╝    ╚═════╝     ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝

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


; ███╗   ███╗███████╗████████╗ █████╗ ██████╗  █████╗ ████████╗ █████╗ 
; ████╗ ████║██╔════╝╚══██╔══╝██╔══██╗██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗
; ██╔████╔██║█████╗     ██║   ███████║██║  ██║███████║   ██║   ███████║
; ██║╚██╔╝██║██╔══╝     ██║   ██╔══██║██║  ██║██╔══██║   ██║   ██╔══██║
; ██║ ╚═╝ ██║███████╗   ██║   ██║  ██║██████╔╝██║  ██║   ██║   ██║  ██║
; ╚═╝     ╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝

;/* AddMetadata
* * adds metadata to the thread
* *
* * @param: ThreadID, the id of the thread
* * @param: Metadata, the metadata to add
*/;
Function AddMetadata(int ThreadID, string Metadata) Global Native

;/* HasMetadata
* * checks if the thread has a specific metadata
* *
* * @param: ThreadID, the id of the thread
* * @param: Metadata, the metadata to check for
* *
* * @return: true if the thread has the metadata, otherwise false
*/;
bool Function HasMetadata(int ThreadID, string Metadata) Global Native

;/* GetMetadata
* * returns a list of all metadata of the thread
* *
* * @param: ThreadID, the id of the thread
* *
* * @return: the list of metadata
*/;
string[] Function GetMetadata(int ThreadID) Global Native


; ███████╗██╗   ██╗███████╗███╗   ██╗████████╗███████╗
; ██╔════╝██║   ██║██╔════╝████╗  ██║╚══██╔══╝██╔════╝
; █████╗  ██║   ██║█████╗  ██╔██╗ ██║   ██║   ███████╗
; ██╔══╝  ╚██╗ ██╔╝██╔══╝  ██║╚██╗██║   ██║   ╚════██║
; ███████╗ ╚████╔╝ ███████╗██║ ╚████║   ██║   ███████║
; ╚══════╝  ╚═══╝  ╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝

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