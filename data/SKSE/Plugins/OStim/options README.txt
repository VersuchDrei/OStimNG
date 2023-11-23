Options files can be used to define additional 'Options' available to users in the Options menu.

These options are constructed in a similar way to scenes but will execute the named static papyrus function when selected by the user.

Pages are defined in each file, if a definition already exists it will be skipped

page fields:
"name" (string): the name of the page
"description" (string): the display text of the navigation in game
"icon" (string): the path to the icon for the page
"parent" (string): the parent page to attach this page to
"border" (string): the hexadecimal RGB code of the icon border (default: ffffff)
"repeat" (string): how many pages to create based on scene paramenters
  "actor": repeat per actor in scene

node fields:
"page" (string): the name of the page this node attaches to
"toggle" (boolean): whether to toggle between an on and off state when function is fired. 
  If true requires an on and off state object for icons and descriptions
"states" (stategroup): states to show in the UI. See stategroup fields
"repeat" (string): how many options to create based on scene paramenters
  "actor": repeat per actor in scene
  "equipObject": repeat for every type of equipObject in equipType
"equipType" (string): if repeat is equipObject, which equipObject type to repeat for


stategroup fields:
  equipObject:
   "object" (state): UI to display for the equip object variants
   "toggle" (state): UI to display for the button to toggle the equipObject on or off
  <name>: UI to display when in named state

state fields:
"icon" (string): the path to the .dds file to be used as the icon
  this path will be appended to "../Data/Interface/Ostim/icons", the file ending ".dds" will be added automatically
"description" (string) : the display text of the navigation in game
"border" (string): the hexadecimal RGB code of the icon border (default: ffffff)
"function" (string): Global papyrus Function to call when the option is toggled. <Script/FunctionName> arguments are determined by state and any repeats
  equipObject & actor repeat: (stateName, actorForm, equipObjectVariant)
  actor repeat: (stateName, actorForm)
  no repeat: (stateName)