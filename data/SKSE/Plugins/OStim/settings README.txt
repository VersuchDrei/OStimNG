Settings are MCM settings related to your OStim add-on.
If you feel you don't have enough custom options to warrant for you own MCM (or simply don't want to bother setting one up) You can instead register them to the OStim Addons page.
Settings registered this way will automatically be part of the settings export/import.

All settings registered by a .json file will work via global variables.
If the user changes an MCM setting OStim will write it into the corresponding variable.
So you can check the value of this variable in your code to see what settings the end user chose.

field summary:
note: most string fields will be run against the translation files, so if your texts start with a $ they will be taken from your translation file

settings fields:
"name" (string): the name of your mod, this will be the header in the MCM
"settings" (list<object>): a list of settings (see setting fields)

setting fields:
"name" (string): the name of the setting
"tooltip" (string): the tooltip of the setting
"type" (string): the type of setting, depending on type additional fields may be required (see setting types)
"variable" (object): the global variable to store the setting in (see form fields)

form fields:
"mod" (string): the name of the mod this form is defined in, including the file extension
"formid" (string): the form id of the form in hexadecimal notation
	note: json doesn't support hexadecimal notation, which is why this is a string and not an int

setting types:

"toggle":
	the MCM will consider 0 as disabled and anything other than 0 as enabled
	when toggled the global variable will be set to either 0 or 1
"defaultValue" (bool): the default value of the setting (default: false)

"slider":
"defaultValue" (float): the default value of the setting (default: 0)
"valueStep": (float): the magnitude in which the slider increments/decrements
"minValue": (float): the minimum value of the slider (default: 0)
"maxvalue": (float): the maximum value of the slider (default: 0)

"dropdown":
	the equivalent of what is called a "Menu Setting" by SkyUI
	the global variable will be set to the index of the selected option
"defaultIndex" (int): the index of the default value
"options" (list<string>): the list of options

"keymap":
	keep in mind that you still have to register to the key in your script
	and you will not be notified about changes in this setting, so this only works for temporary hotkeys
"defaultKey" (int): the default value of the setting (default: 0)