OpenWatcom DirectX Sample Code
------------------------------

These folders contain some DirectX sample code.
They require at least DirectX 9.0 to compile.

The latest DirectX SDK can be downloaded from
http://www.microsoft.com/directx

At the time of writing DirectX9.0b is the latest version.

You should add the path to where you installed the SDK
to the front of the LIB and INCLUDE environment variables,
before the OpenWatcom paths
eg.
set LIB=c:\dxsdk9\lib;c:\ow\lib386\nt;c:\ow\lib386
set INCLUDE=c:\dxsdk9\include;c:\ow\h\nt;c:\ow\h

It might be a good idea to install the SDK into a 8.3
compatible folder name.

A few notes on the samples.

Usually, pressing Escape will exit the demos.

Direct3D
--------
You can use the keys
A                 - toggle animation.
R                 - reset cube to start position.
UP/DOWN
LEFT/RIGHT
PGUP/PGDN         - control the object's XYZ position.
NUMPAD7/4/8/5/9/6 - control the object's pitch/yaw/roll.
NUMPAD+/-         - control the object's size

Inside the code there are toggles for running fullscreen or
windowed, with or without a Z-buffer.

DirectSound
-----------
The sample will probably not work properly with exotic
files (like mp3) encapsulated inside WAV files.

DirectShow
----------
The sample will probably not work properly with exotic
files (like mpeg2) encapsulated inside AVI files.

Inside the code there is a toggle to allow DirectShow to
manage its own window instead of using the application one.

DirectInput
-----------
The sample uses the most conservative locking of resources
to be most Windows friendly.

DirectPlay
----------
There's currently no DirectPlay example.  I think this API
has gone mad!  Use BSD sockets instead.

DirectMusic
-----------
Up for grabs.


Jim Shaw 16/3/2004
