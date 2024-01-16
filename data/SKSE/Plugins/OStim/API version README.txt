API versions prior to OStim Standalone 7.3:
26 = old OStim
27 = OStim NG 6.0 to 6.7
28 = OStim NG 6.8
29 = OStim Standalone Public Beta and 7.0
30 = OStim Standalone 7.1
31 = OStim Standalone 7.1e
32 = OStim Standalone 7.2
33 = OStim Standalone 7.2d

Starting with OStim Standalone 7.3 all version numbers have been straightened up.

The API version is now equal to the .dll version, which is now equal to the file version on Nexus.
So OSexIntegrationMain.GetAPIVersion will now return the same value as SKSE.GetPluginVersion("OStim").
The version number is split up into four different parts:
major - minor - patch - hotfix
Every part has exactly two digits in the hexadecimal representation that is returned by SKSE.GetPluginVersion.
While the hotfix is shown as a letter on Nexus it will be an integer in this representation.
The patch has as of yet never been used in version numbers, but might be used in the future.
So the current version (as of writing this) 7.3 has the hexadecimal representation 0x07030000.
The (most likely upcoming) hotfix 7.3a will have the hexadecimal representation 0x07030001.
A version like 7.3.1b would have the hexadecimal representation 0x07030102.