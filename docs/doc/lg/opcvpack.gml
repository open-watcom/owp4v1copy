.*
.*
.option CVPACK
.*
.np
.ix 'Codeview'
This option is only meaningful when generating Microsoft Codeview
debugging information.
.ix 'CVPACK'
.ix 'Debugging Information Compactor'
.ix 'Compactor'
.ix 'CV4'
This option causes the linker to automatically run the &company
Codeview 4 Symbolic Debugging Information Compactor, CVPACK, on the
executable that it has created.
This is necessary to get the Codeview debugging information into a
state where the Microsoft Codeview debugger will accept it.
.np
The format of the "CVPACK" option (short form "CVP") is as follows.
.mbigbox
    OPTION CVPACK
.embigbox
.np
For more information on generating Codeview debugging information into
the executable, see the section entitled :HDREF refid='xdebug'.
