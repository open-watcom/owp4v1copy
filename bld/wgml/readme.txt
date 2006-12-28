
            The WGML/Script Project
            =======================

This project has a page on the Wiki, which should be consulted for further 
information. This file will contain notes on integration with the build 
system.

Note that only the DOS386/NT386/OS2386 build system is discussed here. 
Those implementing OW on Linux will have to integrate wgml (and whpcvt) into
the Linux build system when they are ready to start producing documentation
under Linux.

When wgml is ready to go, the file bld\lang.ctl will need to be opened and 
the line [ INCLUDE <DEVDIR>/wgml/prereq.ctl ] uncommented.
