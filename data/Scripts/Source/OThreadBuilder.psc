;/* OThreadBuilder
* * helper script to build threads with more complex starting parameter
* * basically a factory but limited by Papyrus, so not really a factory
* *
* * required API Version: 29
* *
* * note: the BuilderID is most likely not going to be the same as the thread id
*/;
ScriptName OThreadBuilder

;/* Create
* * creates a a new thread builder
* *
* * @param: Actors, the actors to be involved in the thread
* *
* * @return: the id of the thread builder, returns -1 if at least one of the actors is invalid
*/;
int Function Create(Actor[] Actors) Global Native

;/* SetDominantActors
* * sets the actors to be dominant in the scene
* * if a scene contains at least one dominant actor all non dominants are considered submissive
* *
* * @param: BuilderID, the id of the thread builder
* * @param: Actors, the dominant actors
*/;
Function SetDominantActors(int BuilderID, Actor[] Actors) Global Native

;/* SetFurniture
* * sets the furniture to use in the thread
* *
* * @param: BuilderID, the id of the thread builder
* * @param: FurnitureRef, the furniture to use
*/;
Function SetFurniture(int BuilderID, ObjectReference FurnitureRef) Global Native

;/* SetDuration
* * sets the duration of the thread (in seconds), when this duration is over the thread ends
* * the thread can still end sooner than that due to player input or stop conditions (like end on climax)
* *
* * required API version: 30
* *
* * @param: BuilderID, the id of the thread builder
* * @param: Duration, the duration for the thread
*/;
Function SetDuration(int BuilderID, float Duration) Global Native

;/* SetStartingAnimation
* * sets the starting animation of the scene
* * if a starting sequence is set the starting animation is ignored
* *
* * @param: BuilderID, the id of the thread builder
* * @param: Animation, the id of the animation
*/;
Function SetStartingAnimation(int BuilderID, string Animation) Global Native

;/* SetStartingSequence
* * sets the starting sequence of the scene
* *
* * required API version: 30
* *
* * @param: BuilderID, the id of the thread builder
* * @param: Sequence, the id of the sequence
*/;
Function SetStartingSequence(int BuilderID, string Sequence) Global Native

;/* EndAfterSequence
* * sets the thread to end when the starting sequence has played through
* * if no startin sequence is set this doesn't do anything
* *
* * without this the thread will get to the usual navigation after the sequence is done
* *
* * @param: BuilderID, the id of the thread builder
*/;
Function EndAfterSequence(int BuidlerID) Global Native

;/* UndressActors
* * sets the thread to strip all actors on start
* * if this is called actors will always be fully stripped, no matter what's set in the MCM
* *
* * without this stripping will be done according to the MCM settings
* *
* * @param: BuilderID, the id of the thread builder
*/;
Function UndressActors(int BuilderID) Global Native

;/* NoAutoMode
* * disables auto mode for the scene
* * if this is called the scene will not run in auto mode, no matter what's set in the MCM
* * also prevents NPCxNPC threads from running auto mode, meaning you have to fully manually navigate them
* *
* * without this the player thread runs in auto mode depending on the MCM settings
* * and NPCxNPC threads always run in auto mode
* *
* * @param: BuilderID, the id of the thread builder
*/;
Function NoAutoMode(int BuilderID) Global Native

;/* NoPlayerControl
* * disables player control for the scene, does nothing on NPCxNPC scenes
* *
* * required API Version: 30
* *
* * @param: BuilderID, the id of the thread builder
*/;
Function NoPlayerControl(int BuilderID) Global Native

;/* NoUnressing
* * disables all undressing during the scene, no matter the MCM settings
* * this also overrules UndressActors
* *
* * required API Version: 30
* *
* * @param: BuilderID, the id of the thread builder
*/;
Function NoUndressing(int BuilderID) Global Native

;/* NoFurniture
* * disables furniture for the scene
* * if this is called the scene will not offer to use or automatically select furniture
* * if furniture was set manually with SetFurniture this function is pointless
* *
* * without this the scene will offer or choose furniture based on the MCM settings
* *
* * @param: BuilderID, the id of the thread builder
*/;
Function NoFurniture(int BuilderID) Global Native

;/* SetMetadata
* * sets the metadata of the thread
* *
* * @param: BuilderID, the id of the thread builder
* * @param: Metadata, an array of metadata
*/;
Function SetMetadata(int BuilderID, string[] Metadata) Global Native

;/* SetMetadataCSV
* * sets the metadata of the thread
* *
* * @param: BuilderID, the id of the thread builder
* * @param: Metadata, a csv-string of metadata
*/;
Function SetMetadataCSV(int BuilderID, string Metadata) Global Native


;/* Start
* * starts the thread
* *
* * @param: BuilderID, the id of the thread builder
* *
* * @return: the id of the thread
*/;
int Function Start(int BuilderID) Global Native

;/* Cancel
* * disposes of the thread builder, freeing up the id again
* *
* * @param: BuilderID, the id of the thread builder
*/;
Function Cancel(int BuilderID) Global Native