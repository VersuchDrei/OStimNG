;/* OData
* * bunch of native functions for data serialization
* * meant for internal use and not to be called by addons
* * might still document it at some point... maybe..
*/;
ScriptName OData

int Function GetUndressingSlotMask() Global Native

Function SetUndressingSlotMask(int Mask) Global Native


string[] Function PairsToNames(string[] Pairs) Global Native


string[] Function GetEquipObjectPairs(int FormID, string Type) Global Native

string Function GetEquipObjectName(int FormID, string Type) Global Native

Function SetEquipObjectID(int FormID, string Type, string ID) Global Native


string[] Function GetVoiceSetPairs() Global Native

string Function GetVoiceSetName(int FormID) Global Native

Function SetVoiceSet(int FormID, string Voice) Global Native 


Function ResetSettings() Global Native

Function ExportSettings() Global Native

Function ImportSettings() Global Native