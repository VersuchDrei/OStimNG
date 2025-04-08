;/* OUtility
* * collection of general utility functions
* *
* * requred API version: 7.3.5
*/;
ScriptName OUtility

;  ██████╗ ██╗   ██╗███████╗███████╗████████╗███████╗
; ██╔═══██╗██║   ██║██╔════╝██╔════╝╚══██╔══╝██╔════╝
; ██║   ██║██║   ██║█████╗  ███████╗   ██║   ███████╗
; ██║▄▄ ██║██║   ██║██╔══╝  ╚════██║   ██║   ╚════██║
; ╚██████╔╝╚██████╔╝███████╗███████║   ██║   ███████║
;  ╚══▀▀═╝  ╚═════╝ ╚══════╝╚══════╝   ╚═╝   ╚══════╝

;/* GetQuestsWithGlobal
* * returns a list of all quests that have the given global in their text display globals
* * this allows you to "flag" quests with a global for quick access
* *
* * @param: Tag, the global to look for on the quests
* *
* * @return: a list of all quests with the given global
*/;
Quest[] Function GetQuestsWithGlobal(GlobalVariable Tag) Global Native