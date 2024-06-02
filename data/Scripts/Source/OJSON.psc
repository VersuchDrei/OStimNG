;/* OJSON
* * utility script to get thread information out of json strings that get passed by some events
*/;
ScriptName OJSON

;/* GetActors
* * gets all the actors that were involved in the thread
* *
* * @param: Json, the json string
* *
* * @return: an array of actors 
*/;
Actor[] Function GetActors(string Json) Global Native

;/* GetScene
* * gets the scene that was played by the thread
* *
* * @param: Json, the json string
* *
* * @return: the scene id
*/;
string Function GetScene(string Json) Global Native