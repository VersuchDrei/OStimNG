;/* OData
* * bunch of native functions for data serialization
* * meant for internal use and not to be called by addons
* * might still document it at some point... maybe..
*/;
ScriptName OData

int Function GetUndressingSlotMask() Global Native

Function SetUndressingSlotMask(int Mask) Global Native


string[] Function PairsToNames(string[] Pairs) Global Native


string[] Function GetEquipObjectTypes() Global Native

string[] Function GetEquipObjectPairs(int FormID, string Type) Global Native

string Function GetEquipObjectName(int FormID, string Type) Global Native

Function SetEquipObjectID(int FormID, string Type, string ID) Global Native


string[] Function GetVoiceSetPairs() Global Native

string Function GetVoiceSetName(int FormID) Global Native

Function SetVoiceSet(int FormID, string Voice) Global Native


float Function GetActionActorStimulation(int FormID, string Actn) Global Native
Function SetActionActorStimulation(int FormID, string Actn, float Stimulation) Global Native
float Function GetActionActorMaxStimulation(int FormID, string Actn) Global Native
Function SetActionActorMaxStimulation(int FormID, string Actn, float Stimulation) Global Native
float Function GetActionTargetStimulation(int FormID, string Actn) Global Native
Function SetActionTargetStimulation(int FormID, string Actn, float Stimulation) Global Native
float Function GetActionTargetMaxStimulation(int FormID, string Actn) Global Native
Function SetActionTargetMaxStimulation(int FormID, string Actn, float Stimulation) Global Native
float Function GetActionPerformerStimulation(int FormID, string Actn) Global Native
Function SetActionPerformerStimulation(int FormID, string Actn, float Stimulation) Global Native
float Function GetActionPerformerMaxStimulation(int FormID, string Actn) Global Native
Function SetActionPerformerMaxStimulation(int FormID, string Actn, float Stimulation) Global Native

float Function GetEventActorStimulation(int FormID, string Evt) Global Native
Function SetEventActorStimulation(int FormID, string Evt, float Stimulation) Global Native
float Function GetEventActorMaxStimulation(int FormID, string Evt) Global Native
Function SetEventActorMaxStimulation(int FormID, string Evt, float Stimulation) Global Native
float Function GetEventTargetStimulation(int FormID, string Evt) Global Native
Function SetEventTargetStimulation(int FormID, string Evt, float Stimulation) Global Native
float Function GetEventTargetMaxStimulation(int FormID, string Evt) Global Native
Function SetEventTargetMaxStimulation(int FormID, string Evt, float Stimulation) Global Native
float Function GetEventPerformerStimulation(int FormID, string Evt) Global Native
Function SetEventPerformerStimulation(int FormID, string Evt, float Stimulation) Global Native
float Function GetEventPerformerMaxStimulation(int FormID, string Evt) Global Native
Function SetEventPerformerMaxStimulation(int FormID, string Evt, float Stimulation) Global Native


Function ResetSettings() Global Native

Function ExportSettings() Global Native

Function ImportSettings() Global Native