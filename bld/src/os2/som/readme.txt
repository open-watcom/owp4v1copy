 SOM (System Object Model) Programming Examples
 ----------------------------------------------

 The SOM programming examples require the IBM SOMobject Developer Toolkit.
This toolkit is not distributed with Open Watcom; it is part of the IBM OS/2
Developer's Toolkit. These examples are intended to be used in conjunction
with version 4.5 of the IBM OS/2 Developer's Toolkit.

 Makefiles are provided for the examples. However, the makefiles assume that
the environment is properly set up to enable use of the SOMobjects Toolkit.
In particular, the following environment variables must be set:

    PATH     - must contain the SOMobjects Compiler, 'sc'
    INCLUDE  - must contain the SOM include directory
    LIB      - must contain the SOM library directory

 The SOMobjects Toolkit may require additional environment variables
to be set.

 The aim of these examples is demonstrating how to use the Open Watcom
C and C++ compilers to develop SOM classes. For information on SOM programming
concepts and details of the SOMobjects compiler usage, please refer to
"System Object Model Programming Guide" and "System Object Model Programming
Reference", both distributed with the IBM OS/2 Developer's Toolkit.

 Note: Building the C sample programs may produce compiler warnings (W100 or
W104, inconsitent levels of indirection). The IBM VisualAge C++ compiler
produces analogous warnings.
