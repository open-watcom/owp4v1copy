How to use tests for OW assembler.
----------------------------------

There are two test types.
First one is for inline assembler in C and C++ compilers and
second one is for standalone WASM assembler.
Test results details are writen to testcomp.log or testasm.log.

1. Inline assembler tests
--------------------------
They are in inline subdirectory.
Test must be started in this subdirectory.
By example, for test 16-bit C compiler inline assembler run

wmake testcomp=wcc (first C compiler on the path, if no testcomp 
is defined then default wcc386 is used)

or by

wmake testcomp=..\..\..\cc\nt386.i86\wcci86e.exe
( exact C or C++ compiler )


2. Standallone (WASM) assembler tests
--------------------------------------
There are tree subdirectories, test1, test2 and test3.
Test must be started in appropriate subdirectory.

For test standalone assembler run

wmake (first WASM assembler on the path)

or by

wmake testasm=..\..\nt386\wasm.exe
( exact WASM assembler )

Note:
Tests require standard building environment and they are using 
following OW tools - wmake, wdis, diff and dmpobj.

