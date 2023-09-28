A sequence is a chain of scenes that can be played automatically.

Field summary:

sequence fields:
"scenes" (list<object>): a list of scenes to play (see sequence entry fields)
"tags" (list<string>): a list of tags for this sequence

sequence entry fields:
"id" (string): the id of the scene to play
"duration" (float): the amount of time in seconds that this scene should play before going to the next
	if no duration is given the duration defined in the scene file will be used (see scenes README)