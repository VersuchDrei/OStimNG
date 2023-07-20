A scene defines what animations are played and what navigation options the player has from there.
They're also full of metadata and other information.
The sceneID which is referred to in other parts of the documentation is the scenes filename without the .json extension.

A lot of the fields in scenes have default values and don't necessarily have to be listed in the .json file.

field summary:

scene fields:
"name" (string): the display name of the scene in game
"modpack" (string): the display name of the modpack in game
"length" (float): the duration of the animation in seconds
"destination" (string): (only for transition) the sceneID of the transition destination
	adding this property turns this node into a transition, that means it will be played once and then automatically moves to the destination scene
	if this property is filled the "navigations" property will be ignored
"origin" (string): (only for transition) the sceneID of the transition origin
	if the transition is already in the navigations of the origin scene this field doesn't have to (and shouldn't) be filled
"navigations" (list<object>): a list of navigation options from or to this scene (see navigation fields)
"speeds" (list<object>): a list of available speeds for the scene (see speed fields)
"defaultSpeed" (int): the index of the default speed of the scene (default 0)
"noRandomSelection" (bool): if true this scene will never be chosen by random selections (default: false)
"furniture" (string): the furniture type of this scene (default: none)
"tags" (list<string>): a list of tags for this scene
"autoTransitions" (map<string, string>): a map of auto transition ids and destination sceneIDs for this scene
"actors" (list<object>): a list of actors (see actor fields)
"actions" (list<object>): a list of actions (see action fields)

navigation fields:
"destination" (string): the destination of the navigation
	if you use this the scene it is defined in will be used as origin
	this is the preferred way, you should always use this over "origin" when navigating within an animation pack
"origin" (string): the origin of the navigation option
	if you use this the scene it is defined in will be used as destination
	this can be used to create navigations from scenes of other animation packs to your scene without overwriting them
	you should never use "origin" and "destination" at the same time
"priority" (int): a priority for the order of the navigation options in the menu (default: 0)
	the navigations are sorted ascending, so lower priorities will show first
"description" (string): the display text of the navigation in game
"icon" (string): the path to the .dds file to be used as the icon
	this path will be appended to "../Data/Interface/OStim/icons", the file ending ".dds" will be added automatically
"border" (string): the hexadecimal RBG code of the icon border (default: ffffff)
"noWarnings" (bool): disables warnings about origin or destination not existing (default: false)
	this can be used to prevent spamming the log when connecting to other animation packs that a user might not have installed
	doesn't prevent warnings about mismatching furniture types or actor counts

speed fields:
"animation" (string): the animation event name of the animation to play for this speed
	this animation will be send as animation event to the actors with _X appended, X being the actors index
"playbackSpeed" (float): the speed at which to play the animation (default: 1.0)
	in order for this to work the animation has to be registered with a Nemesis patch that links it to the OStimSpeed graph variable
"displaySpeed" (float): the speed value to display in game

actor fields:
"type" (string): the type of the actor (see actor types README) (default: npc)
"intendedSex" (string): the intended sex of this actor (default: any)
	possible values are "male" and "female"
	this can be used to limit navigation to this scene by sex without any action in it requiring it
	for example because one of the actors makes a feminine pose that would look weird on a male
"sosBend" (int): the SoSBend value for the actor (default: 0)
	sos angles range from -9 to 9, additionally -10 will cause a flaccid schlong
	these cause the SoSBendX animation event to be send to the actor, with X being the sosBend value
"scale" (float): the scale of the actor (default: 1.0)
"scaleHeigth" (float): the height against which the heel offset should be scaled (default: 120.748)
	this can be used to keep the most important part (for example the schlong in a penetrative scene) always at the same height, no matter the heel offset
	the default value is the total height of the vanilla skeleton
"expressionAction" (int): the index of the action that takes priority for the actors facial expression (see actions README)
"lookUp" (int): the mfg value for the eyes to look up
	possible values range from -100 to 100, with negative ones causing a look down
"lookDown" (int): alternative to lookUp with inverted values
	if lookUp is defined this field is ignored
"lookLeft" (int) the mfg value for the eyes to look left
	possible values range from -100 to 100, with negative ones causing a look right
"lookRight" (int): alternative to lookLeft with inverted values
	if lookLeft is defined this field is ignored
"tags" (list<string>): a list of tags for this actor
"feetOnGround" (bool): if true heel scaling is in effect, if false the heel offset will be removed
	this value defaults to true if the actor has the "standing" or "squatting" tag and false otherwise
"autoTransitions" (map<string, string>): a map of auto transition ids and destination sceneIDs for this actor

action fields:
"type" (string): the type of the action (see actions README)
"actor" (int): the index of the action actor
"target" (int): the index of the action target (default: action actor index)
"performer" (int): the index of the action performer (default: action actor index)