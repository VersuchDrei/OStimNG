A scene defines what animations are played and what navigation options the player has from there.
They're also full of metadata and other information.
The sceneID which is referred to in other parts of the documentation is the scenes filename without the .json extension.
The scene parser also parses subfolders, but folder names are not part of the sceneID, only the filename.
If two files have the same filename one of them will overwrite the other, the order of which is overwriting which is not predictable.
So it is recommended to start all your scene names with your personal signature to avoid incompatibilities.

A lot of the fields in scenes have default values and don't necessarily have to be listed in the .json file.

WARNING: Whatever you do, do NOT start folder or filenames with the "OStim" prefix, that namespace is reserved for OStim alone.
Failure to comply will cause the integrity check to fail!

field summary:

scene fields:
"name" ($string): the display name of the scene in game
	$ the content of this field will be checked against translation files in data/Interface/translations
"modpack" (string): the display name of the modpack in game
"length" (float): the duration of the animation in seconds
"destination" (string): (only for transition) the sceneID of the transition destination
	adding this property turns this node into a transition, that means it will be played once and then automatically moves to the destination scene
	if this property is filled the "navigations" property will be ignored
"origin" (string): (only for transition) the sceneID of the transition origin
	if the transition is already in the navigations of the origin scene this field doesn't have to (and shouldn't) be filled
	if this field is filled you can also add the "priority", "description", "icon", "border" and "noWarnings" fields directly to the scene and use them like you would for a regular navigation
"navigations" (list<object>): a list of navigation options from or to this scene (see navigation fields)
"speeds" (list<object>): a list of available speeds for the scene (see speed fields)
"defaultSpeed" (int): the index of the default speed of the scene (default 0)
"noRandomSelection" (bool): if true this scene will never be chosen by random selections (default: false)
"furniture" (string): the furniture type of this scene (default: none) (see furniture types README)
"offset" (3d offset): an offset for the animation (see 3d offset fields)
	use with caution, unlike animations this does not have a smooth transition
	this means a change in offset will cause the actors to teleport away and then quickly slide pack into position
	therefore a heavy use of offsets is not recommended
"tags" (list<string>): a list of tags for this scene (see list of commonly used scene tags)
"autoTransitions" (map<string, string>): a map of auto transition ids and destination sceneIDs for this scene
"actors" (list<object>): a list of actors (see actor fields)
"actions" (list<object>): a list of actions (see action fields)

navigation fields:
"destination" (string): the destination of the navigation
	if you use this a navigation option is added to this scene that leads to the destination scene
	this is the preferred way, you should always use this over "origin" when navigating within an animation pack
"origin" (string): the origin of the navigation option
	if you use this a navigation option is added to the origin scene that leads to this scene
	this can be used to create navigations from scenes of other animation packs to your scene without overwriting them
	you should never use "origin" and "destination" at the same time
"priority" (int): a priority for the order of the navigation options in the menu (default: 0)
	the navigations are sorted ascending, so lower priorities will show first
	conventional priorities in idles are:
		   0 other idles
		1000 romantic scenes
		2000 undressing scenes
		3000 sexual scenes
	conventional priorities in other scenes are:
   	   -1000 return to idle
    	   0 detail changes (e.g. put hands on hips)
    	1000 positional changes (e.g. kneel down during HJ)
    	2000 action changes (e.g. go from HJ to BJ)
    	3000 climax
    it is recommended to stick to the convention so that navigation options always have the same logical order
    numbers can be slightly adjusted for fine tuning (e.g. 1999 instead of 2000 if you want it to be the first in its class)
"description" ($string): the display text of the navigation in game
	$ the content of this field will be checked against translation files in data/Interface/translations
"icon" (string): the path to the .dds file to be used as the icon
	this path will be appended to "../Data/Interface/OStim/icons", the file ending ".dds" will be added automatically
"border" (string): the hexadecimal RGB code of the icon border (default: ffffff)
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
"noStrip" (bool): if true the actor does not get undressed, no matter the actions involved (default: false)
"scale" (float): the scale of the actor (default: 1.0)
"scaleHeight" (float): the height against which the heel offset should be scaled (default: 120.748)
	this can be used to keep the most important part (for example the schlong in a penetrative scene) always at the same height, no matter the heel offset
	the default value is the total height of the vanilla skeleton
"animationIndex" (int): the index of the animation to play (see speed fields: animation) (defaults to the actors index)
	this can be used to invert the roles of a scene without having to register the animations twice
"underlyingExpression" (string): an expression set to be used as the underlying expression, if left out the underlying expression will be determined based on the actions instead (see facial expressions README)
"expressionAction" (int): the index of the action that takes priority for the actors facial expression (see facial expressions README)
	this does nothing if the "underlyingExpression" property is used
"expressionOverride" (string): an expression set to override the actors expression in this scene (see facial expressions README)
"lookUp" (int): the mfg value for the eyes to look up (default: 0)
	possible values range from -100 to 100, with negative ones causing a look down
"lookDown" (int): alternative to lookUp with inverted values
	if lookUp is defined this field is ignored
"lookLeft" (int) the mfg value for the eyes to look left (default: 0)
	possible values range from -100 to 100, with negative ones causing a look right
"lookRight" (int): alternative to lookLeft with inverted values
	if lookLeft is defined this field is ignored
"offset" (3d offset): an offset for the actor (see 3d offset fields)
	use with caution, unlike animations this does not have a smooth transition
	this means a change in offset will cause the actor to teleport away and then quickly slide pack into position
	therefore a heavy use of offsets is not recommended
"tags" (list<string>): a list of tags for this actor (see list of commonly used actor tags)
"feetOnGround" (bool): if true heel scaling is in effect, if false the heel offset will be removed
	this value defaults to true if the actor has the "standing" or "squatting" tag and false otherwise
"autoTransitions" (map<string, string>): a map of auto transition ids and destination sceneIDs for this actor

action fields:
"type" (string): the type of the action (see actions README)
"actor" (int): the index of the action actor
"target" (int): the index of the action target (default: action actor index)
"performer" (int): the index of the action performer (default: action actor index)
"muted" (bool): when true this action doesn't play sounds (default: false)

3d position fields:
"x" (float): the x offset
"y" (float): the y offset
"z" (float): the z offset
"r" (float): the rotational offset