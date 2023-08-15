Furniture types are used to determine what kind of animations can play on an object reference.

Furniture types follow a hierarchy in which some types are subtypes of others. For example bench is a subtype of chair. That means that scenes on benches will also play chair animations.
You can then create a navigation option from a chair animation to a bench animation and it will only show up if the scene is playing on a bench, not if it is playing on a chair.

by default without any offset the scene will be placed at the first furniture marker for furniture references and at the 0,0,0 spot for other object references

field summary:
"name" (string): the display name of the furniture type
"priority" (int): the priority of the furniture type, if an object reference qualifies for multiple types the one with the higher priority will be chosen (default: 0)
"listIndividually" (bool): if true this type will be listed in the selection individually from its supertype, if false it will be listed as its supertype (default: false)
"ignoreMarkerOffsetX" (bool): if true the offset of the X position of the furniture marker will be ignored and the scene will be scentered on the 0 coordinate (default: false)
"ignoreMarkerOffsetY" (bool): if true the offset of the Y position of the furniture marker will be ignored and the scene will be scentered on the 0 coordinate (default: false)
"ignoreMarkerOffsetZ" (bool): if true the offset of the Z position of the furniture marker will be ignored and the scene will be scentered on the 0 coordinate (default: false)
"offsetX" (float): modifies the X position of the scene by this much relative to the furniture (can be used with or without a disabled marker offset) (default: 0)
"offsetY" (float): modifies the Y position of the scene by this much relative to the furniture (can be used with or without a disabled marker offset) (default: 0)
"offsetZ" (float): modifies the Z position of the scene by this much relative to the furniture (can be used with or without a disabled marker offset) (default: 0)
"rotation" (float): rotates the scene (in radians) by this much relative to the furniture (default: 0)
"multiply scale" (float): the actors scale will be multiplied by this value when using this furniture (default: 1.0)
"offsetXGlobal" (object): a global variable which value will be added to offsetX if set (see form fields)
"offsetYGlobal" (object): a global variable which value will be added to offsetY if set (see form fields)
"offsetZGlobal" (object): a global variable which value will be added to offsetZ if set (see form fields)
"supertype" (string): the furniture super type, note: "none" doesn't mean no supertype, but "no furniture" (so default animations) as supertype (default: no supertype)
"conditions" (list<object>): a list of conditions for the object reference to qualify as this furniture (see condition fields)
	multiple conditions are connected with a logical and, so all conditions have to apply for the object reference to qualify as this furniture

form fields:
"mod" (string): the name of the mod this form is defined in, including the file extension
"formid" (string): the form id of the form in hexadecimal notation
	note: json doesn't support hexadecimal notation, which is why this is a string and not an int

condition fields:
"type" (string): the type of the condition, depending on type additional fields will be required (see condition types)

condition types:

"anykeyword":
	the condition applies if the object reference has at least one of a list of keywords
"keywords" (list<object>): the list of keywords (see form fields)

"keywordblacklist":
	the condition applies if the object reference has none of a list of keywords
"keywords" (list<object>): the list of keywords (see form fields)

"formlist":
	the condition applies if the object references base type is in a formlist
"formlist" (object): the formlist (see form fields)

"formblacklist":
	the condition applies if the object references base type is not in a formlist
"formlist" (object): the formlist (see form fields)

"markercount":
	the condition applies if the count of qualifying furniture markers matches the number condition
"markertype" (string): the type of markers to count
	possible values are "sleep" and "sit"
"minZ": (string) the minimum Z coordinate a marker needs to have to be counted
"maxZ": (string) the maximum Z coordinate a marker can have to be counted
"value" (int): the value to check the marker count against
"condition" (string): the number condition to check agains the value, possible values are:
	"equals": the marker count has to be equal to the value
	"greater": the marker count has to be greater than the value

"cellname":
	the condition applies if the cell name contains the given string (case insensitive)
"cellname": the string to search for in the cell name