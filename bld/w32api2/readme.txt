Open Watcom W32API Headers Version 0.2
Original Code by David Golub

This project is a complete rewrite of the w32api headers from stratch to eliminate the
dependency of Open Watcom on MinGW.  The goal is to provide more thorough coverage of the
Win32 API, be consistent with OW style guidelines, and hopefully contain fewer bugs.  This
package is an alpha release.  It is provided to allow users the opportunity to test the SDK
and DDK headers with their code.  It differs from the 0.1 release in that it contains a
basic set of DDK headers, as well a numerous bug fixes.  To install these headers, copy the
files in the ZIP file into their respective directories in your Open Watcom installation.
If you find any errors, you may either fix them in the bld/w32api2 folder of the source
tree, post to the contributors newsgroup, or send email to david.golub@yale.edu.
