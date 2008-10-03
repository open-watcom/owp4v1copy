Open Watcom W32API Headers and Libraries - README
-------------------------------------------------

This release of Open Watcom includes a new version of the Win32 headers and
libraries.  These headers and libraries were written specifically for Open
Watcom and have no basis in the MinGW headers that were shipped with Open
Watcom 1.7a and earlier.  The goal is to provide headers with more thorough
coverage of the Win32 API, consistency with OW style guidelines, and fewer bugs.
Every effort has been made to preserve compatibility with the MinGW headers.

The headers support compiling to target any version of Windows up through
Windows Vista.  However, to preserve compatibility with existing code, the
default target is Windows 95 or Windows NT 4.0 with Internet Explorer 4.0.
If you wish to use features specific to a more recent version of Windows, you
must define the macros WINVER, _WIN32_WINNT, _WIN32_IE, and NTDDI_VERSION
appropriately, either in your source code or using compiler switches.

Documentation for the Win32 API is available over the Internet from Microsoft
at http://msdn.microsoft.com/en-us/library/default.aspx.

