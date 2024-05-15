;/* OToys
* * bunch of native functions for sex toy settings
* * meant for internal use and not to be called by addons
* * might still document it at some point... maybe..
*/;
ScriptName OToys

string[] Function GetToySlots() Global Native
string[] Function GetToys() Global Native

int Function GetSynchronizationType() Global Native
Function SetSynchronizationType(int Type) Global Native

bool Function GetToyEnabled(string Toy) Global Native
Function SetToyEnabled(string Toy, bool Enabled) Global Native
int Function GetToySynchronizationType(string Toy) Global Native
Function SetToySynchronizationType(string Toy, int Type) Global Native
bool Function GetToyClimax(string Toy) Global Native
Function SetToyClimax(string Toy, bool Climax) Global Native
float Function GetToyClimaxMagnitude(string Toy) Global Native
Function SetToyClimaxMagnitude(string Toy, float Magnitude) Global Native

int Function GetSlotSynchronizationType(string Toy, string Slot) Global Native
Function SetSlotSynchronizationType(string Toy, string Slot, int Type) Global Native
float Function GetSlotMinScale(string Toy, string Slot) Global Native
Function SetSlotMinScale(string Toy, string Slot, float Scale) Global Native
float Function GetSlotMaxScale(string Toy, string Slot) Global Native
Function SetSlotMaxScale(string Toy, string Slot, float Scale) Global Native
float Function GetSlotMinMagnitude(string Toy, string Slot) Global Native
Function SetSlotMinMagnitude(string Toy, string Slot, float Magnitude) Global Native
float Function GetSlotMaxMagnitude(string Toy, string Slot) Global Native
Function SetSlotMaxMagnitude(string Toy, string Slot, float Magnitude) Global Native
bool Function GetSlotDoPeaks(string Toy, string Slot) Global Native
Function SetSlotDoPeaks(string Toy, string Slot, bool DoPeaks) Global Native
float Function GetSlotMinPeak(string Toy, string Slot) Global Native
Function SetSlotMinPeak(string Toy, string Slot, float Peak) Global Native
float Function GetSlotMaxPeak(string Toy, string Slot) Global Native
Function SetSlotMaxPeak(string Toy, string Slot, float Peak) Global Native