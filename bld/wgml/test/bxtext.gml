:CMT.This is from fl\frecord.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Unions"
.if &'lower(&syspdev) = ps .pa
.np
The organization of the record in memory is as follows:
.millust begin
.in 12
offset  +0          +4          +8
.* .bx on 7 17 27 37
.sr c0=&INDlvl+11
.sr c1=&INDlvl+21
.sr c2=&INDlvl+31
.sr c3=&INDlvl+41
.bx on &c0 &c1 &c2 &c3
          integer     logical     (slack)
.* .bx 17 27 37
.bx &c1 &c2 &c3
                      integer     (slack)
.bx
                      real        (slack)
.* .bx 17 37
.bx &c1 &c3
                      double precision
.bx off
.millust end
.*
:CMT.This is from cmn\pragma.gml and sets macros for sections in both the 
:CMT.C/C++ User's Guide and FORTRAN User's Guide
.dm @id begin
:HP2.&*.
.dm @id end
.gt id add @id cont
.*
.dm @eid begin
:eHP2.&*
.dm @eid end
.gt eid add @eid cont
.*
.dm @op begin
:HP2.[:eHP2.&*
.dm @op end
.gt op add @op cont
.*
.dm @eop begin
:HP2.]:eHP2.&*
.dm @eop end
.gt eop add @eop cont
.*
.dm @rp begin
:HP2.{:eHP2.&*
.dm @rp end
.gt rp add @rp cont
.*
.dm @erp begin
:HP2.}:eHP2.&*
.dm @erp end
.gt erp add @erp cont
.*
.dm @or begin
:HP2.|:eHP2.&*
.dm @or end
.gt or add @or cont
.*
:CMT.This is from cmn\pragma.gml and sets macros and symbols for sections in the 
:CMT.C/C++ User's Guide
.dm @prgbeg begin
#pragma&*.
.dm @prgbeg end
.gt prgbeg add @prgbeg cont
.*
.dm @prgend begin
:HP2.[:eHP2.;:HP2.]:eHP2.
.dm @prgend end
.gt prgend add @prgend cont texterror tagnext
.*
:set symbol="pragma" value="#pragma".
:set symbol="epragma" value="~;".
:set symbol="function" value="function".
:set symbol="functions" value="functions".
:set symbol="ufunction" value="Function".
:set symbol="ufunctions" value="Functions".
:set symbol="short_int" value="short int".
:set symbol="long_int" value="long int".
:set symbol="int" value="int".
:set symbol="char" value="char".
:set symbol="ushort_int" value="unsigned short int".
:set symbol="ulong_int" value="unsigned long int".
:set symbol="uint" value="unsigned int".
:set symbol="uchar" value="unsigned char".
:set symbol="double" value="double".
:set symbol="single" value="float".
:set symbol="alias_name" value="HIGH_C".
:set symbol="other_cmp" value="MetaWare High C".
:set symbol="winopt" value="zW".
:set symbol="pragcont" value="".
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The ALIAS Pragma (C Only)"
:CMT.      & "32-bit:  The ALIAS Pragma (C Only)"
.np
.ix 'pragmas' 'alias'
.ix 'alias pragma'
The "alias" pragma can be used to emit alias records in the object file,
causing the linker to substitute references to a specified symbol with
references to another symbol. Either identifiers or names (strings) may
be used. Strings are used verbatim, while names corresponding to identifiers
are derived as appropriate for the kind and calling convention of the symbol.
The following describes the form of the "alias" pragma.
.mbox begin
:prgbeg. alias ( :id.alias:eid., :id.subst:eid. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The ALLOC_TEXT Pragma (C Only)" 
:CMT.      & "32-bit:  The ALLOC_TEXT Pragma (C Only)"
.np
.ix 'pragmas' 'alloc_text'
.ix 'alloc_text pragma'
The "alloc_text" pragma can be used to specify the name of the text
segment into which the generated code for a function, or a list of
functions, is to be placed.
The following describes the form of the "alloc_text" pragma.
.mbox begin
:prgbeg. alloc_text ( :id.seg_name:eid., :id.fn:eid. :rp., :id.fn:eid.:erp. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The CODE_SEG Pragma" & "32-bit:  The CODE_SEG Pragma"
.np
.ix 'pragmas' 'code_seg'
.ix 'code_seg pragma'
The "code_seg" pragma can be used to specify the name of the text
segment into which the generated code for functions is to be placed.
The following describes the form of the "code_seg" pragma.
.mbox begin
:prgbeg. code_seg ( :id.seg_name:eid. :op., :id.class_name:eid.:eop. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The COMMENT Pragma" & "32-bit:  The COMMENT Pragma"
.np
.ix 'pragmas' 'comment'
.ix 'comment pragma'
The "comment" pragma can be used to place a comment record in an
object file or executable file.
The following describes the form of the "comment" pragma.
.mbox begin
:prgbeg. comment ( :id.comment_type:eid. :op., :id."comment_string":eid.:eop. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The DATA_SEG Pragma" & "32-bit:  The DATA_SEG Pragma"
.np
.ix 'pragmas' 'data_seg'
.ix 'data_seg pragma'
The "data_seg" pragma can be used to specify the name of the
segment into which data is to be placed.
The following describes the form of the "data_seg" pragma.
.mbox begin
:prgbeg. data_seg ( :id.seg_name:eid. :op., :id.class_name:eid.:eop. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The DISABLE_MESSAGE Pragma"
:CMT.      & "32-bit:  The DISABLE_MESSAGE Pragma"
.np
.ix 'pragmas' 'disable_message'
.ix 'disable_message pragma'
The "disable_message" pragma disables the issuance of specified
diagnostic messages.
The form of the "disable_message" pragma is as follows.
.mbox begin
:prgbeg. disable_message ( :id.msg_num:eid. :rp., :id.msg_num:eid.:erp. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The DUMP_OBJECT_MODEL Pragma (C++ Only)"
:CMT.      & "32-bit:  The DUMP_OBJECT_MODEL Pragma (C++ Only)"
.np
.ix 'object model'
.ix 'pragmas' 'dump_object_model'
.ix 'dump_object_model pragma'
.ix 'class information'
The "dump_object_model" pragma causes the C++ compiler to print
information about the object model for an indicated
class or an enumeration name to the diagnostics file.
.ix 'enumeration' 'information'
.ix 'enumeration' 'values'
For class names, this information includes the offsets and sizes of
fields within the class and within base classes.
For enumeration names, this information consists of a list of all the
enumeration constants with their values.
.np
The general form of the "dump_object_model" pragma is as follows.
.mbox begin
:prgbeg. dump_object_model :id.class:eid. :prgend.
:prgbeg. dump_object_model :id.enumeration:eid. :prgend.
:id.class ::= a defined C++ class free of errors:eid.
:id.enumeration ::= a defined C++ enumeration name:eid.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The ENABLE_MESSAGE Pragma"
:CMT.      & "32-bit:  The ENABLE_MESSAGE Pragma"
.np
.ix 'pragmas' 'enable_message'
.ix 'enable_message pragma'
The "enable_message" pragma re-enables the issuance of specified
diagnostic messages that have been previously disabled.
The form of the "enable_message" pragma is as follows.
.mbox begin
:prgbeg. enable_message ( :id.msg_num:eid. :rp., :id.msg_num:eid.:erp. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The ENUM Pragma" & "32-bit:  The ENUM Pragma"
.np
.ix 'pragmas' 'enum'
.ix 'enum pragma'
The "enum" pragma affects the underlying storage-definition for
subsequent
.us enum
declarations.
The forms of the "enum" pragma are as follows.
.mbox begin
:prgbeg. enum int :prgend.
:prgbeg. enum minimum :prgend.
:prgbeg. enum original :prgend.
:prgbeg. enum pop :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The ERROR Pragma" & "32-bit:  The ERROR Pragma"
.np
.ix 'pragmas' 'error'
.ix 'error pragma'
The "error" pragma can be used to issue an error message with the
specified text.
The following describes the form of the "error" pragma.
.mbox begin
:prgbeg. error :id."error text":eid. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The EXTREF Pragma" & "32-bit:  The EXTREF Pragma"
.np
.ix 'pragmas' 'extref'
.ix 'extref pragma'
.ix 'external references'
The "extref" pragma is used to generate a reference to an external
function or data item.
The form of the "extref" pragma is as follows.
.mbox begin
:prgbeg. extref :id.name:eid. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The FUNCTION Pragma" & "32-bit:  The FUNCTION Pragma"
.np
.ix 'pragmas' 'function'
.ix 'function pragma'
Certain functions, such as those listed in the description of the "oi"
and "om" options, have intrinsic forms.
These functions are special functions that are recognized by the
compiler and processed in a special way.
For example, the compiler may choose to generate in-line code for the
function.
The intrinsic attribute for these special functions is set by
specifying the "oi" or "om" option or using an "intrinsic" pragma.
The "function" pragma can be used to remove the intrinsic attribute
for a specified list of functions.
.np
The following describes the form of the "function" pragma.
.mbox begin
:prgbeg. function ( :id.fn:eid. :rp., :id.fn:eid.:erp. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The INCLUDE_ALIAS Pragma"
:CMT.      & "32-bit:  The INCLUDE_ALIAS Pragma"
.np
.ix 'pragmas' 'include_alias'
.ix 'include_alias pragma'
In certain situations, it can be advantageous to remap the names of include
files. Most commonly this occurs on systems that do not support long file
names when building source code that references header files with long names.
.np
The form of the "include_alias" pragma follows.
.mbox begin
:prgbeg. include_alias ( ":id.alias_name:eid.", ":id.real_name:eid." ) :prgend.
:prgbeg. include_alias ( <:id.alias_name:eid.>, <:id.real_name:eid.> ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Setting Priority of Static Data Initialization (C++ Only)"
:CMT.      & "32-bit:  Setting Priority of Static Data Initialization (C++ Only)"
.np
.ix 'pragmas' 'initialize'
.ix 'initialize pragma'
The "initialize" pragma sets the priority for initialization of
static data in the file.
This priority only applies to initialization of static data that requires
the execution of code.
For example, the initialization of a class that contains a constructor
requires the execution of the constructor.
Note that if the sequence in which initialization of static data in your
program takes place has no dependencies, the "initialize" pragma need not
be used.
.np
The general form of the "initialize" pragma is as follows.
.mbox begin
:prgbeg. initialize :op.before :or. after:eop. :id.priority:eid. :prgend.

:id.priority ::=:eid. :id.n:eid. :or. library :or. program
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The INLINE_DEPTH Pragma (C++ Only)"
:CMT.      & "32-bit:  The INLINE_DEPTH Pragma (C++ Only)"
.ix 'pragmas' 'inline_depth'
.ix 'inline_depth pragma'
.np
When an in-line function is called, the function call may be replaced
by the in-line expansion for that function.
This in-line expansion may include calls to other in-line functions
which can also be expanded.
The "inline_depth" pragma can be used to set the number of times this
expansion of in-line functions will occur for a call.
.np
The form of the "inline_depth" pragma is as follows.
.mbox begin
:prgbeg. inline_depth :op.(:eop. :id.n:eid. :op.):eop. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The INLINE_RECURSION Pragma (C++ Only)"
:CMT.      & "32-bit:  The INLINE_RECURSION Pragma (C++ Only)"
.np
.ix 'pragmas' 'inline_recursion'
.ix 'inline_recursion pragma'
The "inline_recursion" pragma controls the recursive expansion of
inline functions.
The form of the "inline_recursion" pragma is as follows.
.mbox begin
:prgbeg. inline_recursion :op.(:eop. on :or. off :op.):eop. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The INTRINSIC Pragma" & "32-bit:  The INTRINSIC Pragma"
.np
.ix 'pragmas' 'intrinsic'
.ix 'intrinsic pragma'
Certain functions, those listed in the description of the "oi" option,
have intrinsic forms.
These functions are special functions that are recognized by the compiler
and processed in a special way.
For example, the compiler may choose to generate in-line code for the
function.
The intrinsic attribute for these special functions is set by specifying
the "oi" option or using an "intrinsic" pragma.
.np
The following describes the form of the "intrinsic" pragma.
.mbox begin
:prgbeg. intrinsic ( :id.fn:eid. :rp., :id.fn:eid.:erp. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The MESSAGE Pragma" & "32-bit:  The MESSAGE Pragma"
.np
.ix 'pragmas' 'message'
.ix 'message pragma'
The "message" pragma can be used to issue a message with the specified
text to the standard output without terminating compilation.
The following describes the form of the "message" pragma.
.mbox begin
:prgbeg. message ( :id."message text":eid. ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The ONCE Pragma" & "32-bit:  The ONCE Pragma"
.np
.ix 'pragmas' 'once'
.ix 'once pragma'
.ix 'optimization'
The "once" pragma can be used to indicate that the file which contains
this pragma should only be opened and processed "once".
The following describes the form of the "once" pragma.
.mbox begin
:prgbeg. once :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The PACK Pragma" & "32-bit:  The PACK Pragma"
.np
.ix 'pragmas' 'pack'
.ix 'pack pragma'
The "pack" pragma can be used to control the way in which structures
are stored in memory.
There are 4 forms of the "pack" pragma.
.np
The following form of the "pack" pragma can be used to change the
alignment of structures and their fields in memory.
.mbox begin
:prgbeg. pack ( :id.n:eid. ) :prgend.
.mbox end
.*
.np
The following form of the "pack" pragma can be used to save the current
alignment amount on an internal stack.
.mbox begin
:prgbeg. pack ( push ) :prgend.
.mbox end
.*
.np
The following form of the "pack" pragma can be used to save the current
alignment amount on an internal stack and set the current alignment.
.mbox begin
:prgbeg. pack ( push, :id.number:eid. ) :prgend.
.mbox end
.np
The following form of the "pack" pragma can be used to restore the
previous alignment amount from an internal stack.
.mbox begin
:prgbeg. pack ( pop ) :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The READ_ONLY_FILE Pragma"
:CMT.      & "32-bit:  The READ_ONLY_FILE Pragma"
.np
.ix 'pragmas' 'read_only_file'
.ix 'read_only_file pragma'
.ix 'AUTODEPEND'
Explicit listing of dependencies in a makefile can often be tedious
in the development and maintenance phases of a project.
The &cmpname compiler will insert dependency information into the
object file as it processes source files so that a complete snapshot
of the files necessary to build the object file are recorded.
The "read_only_file" pragma can be used to prevent the name of the
source file that includes it from being included in the dependency
information that is written to the object file.
.np
This pragma is commonly used in system header files since they change
infrequently (and, when they do, there should be no impact on source
files that have included them).
.np
The form of the "read_only_file" pragma follows.
.mbox begin
:prgbeg. read_only_file :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The TEMPLATE_DEPTH Pragma (C++ Only)"
:CMT.      & "32-bit:  The TEMPLATE_DEPTH Pragma (C++ Only)"
.ix 'pragmas' 'template_depth'
.ix 'template_depth pragma'
.np
The "template_depth" pragma provides a hard limit for the amount of
nested template expansions allowed so that infinite expansion can be
detected.
.np
The form of the "template_depth" pragma is as follows.
.mbox begin
:prgbeg. template_depth :op.(:eop. :id.n:eid. :op.):eop. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  The WARNING Pragma (C++ Only)"
:CMT.      & "32-bit:  The WARNING Pragma (C++ Only)"
.np
.ix 'pragmas' 'warning'
.ix 'warning pragma'
The "warning" pragma sets the level of warning messages.
The form of the "warning" pragma is as follows.
.mbox begin
:prgbeg. warning :id.msg_num:eid. :id.level:eid. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Alias Names" & "32-bit:  Alias Names"
.np
.ix 'alias name (pragma)'
When a symbol referred to by an auxiliary pragma includes an alias name,
the attributes of the alias name are also assumed by the specified symbol.
.np
There are two methods of specifying alias information.
In the first method, the symbol assumes only the attributes of the
alias name; no additional attributes can be specified.
The second method is more general since it is possible to specify
an alias name as well as additional auxiliary information.
In this case, the symbol assumes the attributes of the alias name as well as
the attributes specified by the additional auxiliary information.
.np
The simple form of the auxiliary pragma used to specify an alias is as
follows.
.mbox begin
:prgbeg. aux ( :id.sym:eid., :op.far16:eop. :id.alias:eid. ) :prgend.
.mbox end
.mbox begin
:prgbeg. aux ( :id.sym:eid., :id.alias:eid. ) :prgend.
.mbox end
.*
.np
The general form of an auxiliary pragma that can be used to specify an
alias is as follows.
.ix 'pragmas' 'alias name'
.mbox begin
:prgbeg. aux ( :id.alias:eid. ) :id.sym:eid. :id.aux_attrs:eid. :prgend.
.mbox end
.*
.warn
The alias name
.id &alias_name
is just another symbol.
If
.id &alias_name
appeared in your source code, it would assume the attributes specified
in the pragma for
.id &alias_name..
.ewarn
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Alternate Names for Symbols"
:CMT.      & "32-bit:  Alternate Names for Symbols"
.np
The following form of the auxiliary pragma can be used to describe the
mapping of a symbol from its source form to its object form.
.ix 'pragmas' 'alternate name'
:cmt. IN 1 added because &sysin at this point is "-1", producing left position
:cmt. for .mbox of 0.
.in 1
.mbox begin
:prgbeg. aux :id.sym:eid. :id.obj_name:eid. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Describing Calling Information"
:CMT.      & "32-bit:  Describing Calling Information"
.np
.ix 'calling &functions' 'near'
.ix 'calling &functions' 'far'
The following form of the auxiliary pragma can be used to describe the
way a &function is to be called.
.ix 'pragmas' 'calling information'
.ix 'pragmas' 'far'
.ix 'pragmas' 'near'
.ix 'pragmas' '= const'
.ix 'pragmas' 'in-line assembly'
.ix 'calling information (pragma)'
.ix 'far (pragma)'
.ix 'near (pragma)'
.ix 'in-line assembly' 'in pragmas'
.* ---------------------------------------
.mbox begin
:prgbeg. aux :id.sym:eid. far :prgend.
    or
:prgbeg. aux :id.sym:eid. near :prgend.
    or
:prgbeg. aux :id.sym:eid. = :id.in_line:eid. :prgend.

:id.in_line ::= { const | (:eid.seg:id. id) | (:eid.offset:id. id) | (:eid.reloff:id. id)
                    | (:eid.float:id. fpinst) | :eid.":id.asm:eid." :id.}:eid.
.mbox end
.*
.mbox begin
:prgbeg. aux :id.sym:eid. far :prgend.
    or
:prgbeg. aux :id.sym:eid. near :prgend.
    or
:prgbeg. aux :id.sym:eid. = :id.in_line:eid. :prgend.

:id.in_line ::= { const | (:eid.seg:id. id) | (:eid.offset:id. id) | (:eid.reloff:id. id)
                    | :eid.":id.asm:eid." :id.}:eid.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Loading Data Segment Register"
:CMT.      & "32-bit:  Loading Data Segment Register"
.np
.ix 'loading DS before calling a &function'
An application may have been compiled so that the segment register DS
does not contain the segment address of the default data segment
(group "DGROUP").
This is usually the case if you are using a large data memory model.
Suppose you wish to call a &function that assumes that the segment
register DS contains the segment address of the default data segment.
It would be very cumbersome if you were forced to compile your
application so that the segment register DS contained the default data
segment (a small data memory model).
.np
The following form of the auxiliary pragma will cause the segment
register DS to be loaded with the segment address of the default data
segment before calling the specified &function..
.ix 'pragmas' 'loadds'
.ix 'loadds (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. parm loadds :prgend.
.mbox end
.np
.ix 'loading DS in prologue sequence of a &function'
Alternatively, the following form of the auxiliary pragma will cause
the segment register DS to be loaded with the segment address of the
default data segment as part of the prologue sequence for the
specified &function..
.mbox begin
:prgbeg. aux :id.sym:eid. loadds :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Defining Exported Symbols in Dynamic Link Libraries"
:CMT.      & "32-bit:  Defining Exported Symbols in Dynamic Link Libraries"
.np
.ix 'exporting symbols in dynamic link libraries'
An exported symbol in a dynamic link library is a symbol that can be
referenced by an application that is linked with that dynamic link
library.
Normally, symbols in dynamic link libraries are exported using the
&lnkname "EXPORT" directive.
An alternative method is to use the following form of the auxiliary
pragma.
.ix 'pragmas' 'export'
.ix 'export (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. export :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topic: "16-bit:  Defining Windows Callback Functions"
.np
.ix 'callback functions'
When compiling a Microsoft Windows application, you must use the
"&winopt" option so that special prologue/epilogue sequences are
generated.
Furthermore, callback functions require larger prologue/epilogue
sequences than those generated when the "&winopt" compiler option is
specified.
The following form of the auxiliary pragma will cause a callback
prologue/epilogue sequence to be generated for a callback function
when compiled using the "&winopt" option.
.ix 'pragmas' 'export'
.ix 'export (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. export :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topic: "16-bit:  Forcing a Stack Frame" & "32-bit:  Forcing a Stack Frame"
.np
.ix 'stack frame'
Normally, a function contains a stack frame if arguments are passed on
the stack or an automatic variable is allocated on the stack.
No stack frame will be generated if the above conditions are not satisfied.
The following form of the auxiliary pragma will force a stack frame to
be generated under any circumstance.
.ix 'pragmas' 'frame'
.ix 'frame (pragma)'
.ix 'stack frame (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. frame :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Describing Argument Information"
:CMT.      & "32-bit:  Describing Argument Information"
.np
.ix 'argument list (pragma)'
.ix 'pragmas' 'describing argument lists'
Using auxiliary pragmas, you can describe the calling convention that
&cmpname is to use for calling &functions..
This is particularly useful when interfacing to &functions that have
been compiled by other compilers or &functions written in other
programming languages.
.np
The general form of an auxiliary pragma that describes argument
passing is the following.
.mbox begin
:prgbeg. aux :id.sym:eid. parm :id.:rp. pop_info :or. :eid.reverse:id. :or. :rp.reg_set:erp. :erp.:eid. :prgend.

:id.pop_info ::=:eid. caller :or. routine
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Passing Arguments in Registers"
:CMT.      & "32-bit:  Passing Arguments in Registers"
.np
The following form of the auxiliary pragma can be used to specify the
registers that are to be used to pass arguments to a particular
&function..
.ix 'pragmas' 'parm'
.ix 'parm (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. parm :rp.:id.reg_set:eid.:erp. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Removing Arguments from the Stack"
:CMT.      & "32-bit:  Removing Arguments from the Stack"
.np
The following form of the auxiliary pragma specifies who removes
from the stack arguments that were pushed on the stack.
.ix 'pragmas' 'parm caller'
.ix 'pragmas' 'parm routine'
.ix 'parm caller (pragma)'
.ix 'parm routine (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. parm :id.(:eid.caller :or. routine:id.):eid. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Passing Arguments in Reverse Order"
:CMT.      & "32-bit:  Passing Arguments in Reverse Order"
.np
The following form of the auxiliary pragma specifies that arguments
are passed in the reverse order.
.ix 'pragmas' 'parm reverse'
.ix 'parm reverse (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. parm reverse :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Describing Function Return Information"
:CMT.      & "32-bit:  Describing Function Return Information"
.np
.ix 'return value (pragma)'
.ix 'pragmas' 'describing return value'
Using auxiliary pragmas, you can describe the way functions are to
return values.
This is particularly useful when interfacing to functions that have
been compiled by other compilers or functions written in other
programming languages.
.np
The general form of an auxiliary pragma that describes the way a
function returns its value is the following.
.ix 'pragmas' 'value'
.ix 'pragmas' 'no8087'
.ix 'pragmas' 'struct float'
.ix 'pragmas' 'struct routine'
.ix 'pragmas' 'struct caller'
.ix 'value (pragma)'
.ix 'no8087 (pragma)'
.ix 'struct float (pragma)'
.ix 'struct routine (pragma)'
.ix 'struct caller (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. value :rp.no8087 :or. :id.reg_set:eid. :or. :id.struct_info:eid.:erp. :prgend.
:cmt..millust break

:id.struct_info ::=:eid. struct :rp.float :or. struct :or. :id.(:eid.routine :or. caller:id.):eid. :or. :id.reg_set:eid.:erp.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Returning Function Values in Registers"
:CMT.      & "32-bit:  Returning Function Values in Registers"
.np
The following form of the auxiliary pragma can be used to specify the
registers that are to be used to return a function's value.
.ix 'pragmas' 'value'
.ix 'value (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. value :id.reg_set:eid. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Returning Structures" & "32-bit:  Returning Structures"
:CMT.and in the FORTRAN 77 User's Guide
:CMT.Topics: "16-bit:  Returning Structures and Complex Numbers"
:CMT.      & "32-bit:  Returning Structures and Complex Numbers"
.np
The following form of the auxiliary pragma can be used to specify the
register that is to be used to point to the return value.
.ix 'pragmas' 'value'
.ix 'pragmas' 'struct caller'
.ix 'pragmas' 'struct routine'
.ix 'value (pragma)'
.ix 'struct caller (pragma)'
.ix 'struct routine (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. value struct :id.(:eid.caller:or.routine:id.):eid. :id.reg_set:eid. :prgend.
.mbox end
.np
The following form of the auxiliary pragma can be used to specify that
structures whose size is 1, 2 or 4 bytes are not to be returned in
registers.
Instead, the caller will allocate space on the stack for the structure
return value and point register &siup to it.
.mbox begin
:prgbeg. aux :id.sym:eid. value struct struct :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Returning Floating-Point Data"
:CMT.      & "32-bit:  Returning Floating-Point Data"
.np
There are a few ways available for specifying how the value for a
function whose type is
.bd &single
or
.bd &double
is to be returned.
.np
The following form of the auxiliary pragma can be used to specify that
function return values whose type is
.bd &single
or
.bd &double
are not to be returned in registers.
Instead, the caller will allocate space on the stack for the
return value and point register &siup to it.
.ix 'pragmas' 'value'
.ix 'pragmas' 'struct float'
.ix 'value (pragma)'
.ix 'struct float (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. value struct float :prgend.
.mbox end
.np
The following form of the auxiliary pragma can be used to specify that
function return values whose type is
.bd &single
or
.bd &double
are not to be returned in 80x87 registers when compiling with the
"fpi" or "fpi87" option.
Instead, the value will be returned in 80x86 registers.
This is the default behaviour for the "fpc" option.
Function return values whose type is
.bd &single
will be returned in
register EAX.
Function return values whose type is
.bd &double
will be returned in registers
EDX:EAX.
This is the default method for the "fpc" option.
.ix 'pragmas' 'value no8087'
.ix 'value no8087 (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. value no8087 :prgend.
.mbox end
.if &'lower(&syspdev) = ps .pa
.np
The following form of the auxiliary pragma can be used to specify that
function return values whose type is
.bd &single
or
.bd &double
are to be returned in ST(0) when compiling with the
"fpi" or "fpi87" option.
This form of the auxiliary pragma is not legal for the "fpc" option.
.ix 'pragmas' 'value [8087]'
.ix 'value [8087] (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. value [8087] :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  A Function that Never Returns"
:CMT.      & "32-bit:  A Function that Never Returns"
.np
The following form of the auxiliary pragma can be used to describe a
&function that does not return to the caller.
.ix 'pragmas' 'aborts'
.ix 'aborts (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. aborts :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Describing How Functions Use Memory"
:CMT.      & "32-bit:  Describing How Functions Use Memory"
:CMT.and is in the FORTRAN 77 User's Guide
:CMT.Topics: "16-bit:  Describing How Functions Use Variables in Common"
:CMT.      & "32-bit:  Describing How Functions Use Variables in Common"
.np
.ix 'side effects of &functions'
The following form of the auxiliary pragma can be used to describe a
&function that does not modify any
memory (i.e., global or static variables) that is used directly or
indirectly by the caller.
.ix 'pragmas' 'modify nomemory'
.ix 'modify nomemory (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. modify nomemory :prgend.
.mbox end
.np
The following form of the auxiliary pragma can be used to describe a
&function that does not reference any
memory (i.e., global or static variables) that is used directly or
indirectly by the caller.
.ix 'pragmas' 'parm nomemory'
.ix 'pragmas' 'modify nomemory'
.ix 'parm nomemory (pragma)'
.ix 'modify nomemory (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. parm nomemory modify nomemory :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the C/C++ User's Guide
:CMT.Topics: "16-bit:  Describing the Registers Modified by a Function"
:CMT.      & "32-bit:  Describing the Registers Modified by a Function"
.np
The following form of the auxiliary pragma can be used to describe
the registers that a &function will use without saving.
.ix 'pragmas' 'modify exact'
.ix 'modify exact (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. modify :op.exact:eop. :id.reg_set:eid. :prgend.
.mbox end
.np
Registers that are used to pass arguments are assumed to be modified
and hence do not have to be saved and restored by the called
&function..
Also, since the &axup register is frequently used to return a value,
it is always assumed to be modified.
If necessary, the caller will contain code to save and restore the
contents of registers used to pass arguments.
Note that saving and restoring the contents of these registers may not
be necessary if the called &function does not modify them.
The following form of the auxiliary pragma can be used to describe
exactly those registers that will be modified by the called
&function..
.ix 'pragmas' 'modify exact'
.ix 'modify exact (pragma)'
.mbox begin
:prgbeg. aux :id.sym:eid. modify exact :id.reg_set:eid. :prgend.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and sets macros and symbols for pragmas in the 
:CMT.FORTRAN 77 User's Guide
.gt prgbeg delete
.gt prgend delete
.dm @prgbeg begin
*$pragma&*
.dm @prgbeg end
.gt prgbeg add @prgbeg cont
.*
.dm @prgend begin
.dm @prgend end
.gt prgend add @prgend cont texterror tagnext
:set symbol="pragma" value="*$pragma".
:set symbol="epragma" value="".
:set symbol="function" value="subprogram".
:set symbol="functions" value="subprograms".
:set symbol="ufunction" value="Subprogram".
:set symbol="ufunctions" value="Subprograms".
:set symbol="short_int" value="INTEGER*2".
:set symbol="long_int" value="INTEGER*4".
:set symbol="int" value="INTEGER".
:set symbol="char" value="INTEGER*1".
:set symbol="ushort_int" value="INTEGER*2".
:set symbol="ulong_int" value="INTEGER*4".
:set symbol="uint" value="INTEGER".
:set symbol="uchar" value="INTEGER*1".
:set symbol="double" value="DOUBLE PRECISION".
:set symbol="single" value="REAL".
:set symbol="alias_name" value="WC".
:set symbol="other_cmp" value="&company C".
:set symbol="winopt" value="windows".
:set symbol="pragcont" value="c".
.*
:CMT.This is from cmn\pragma.gml and is in the FORTRAN 77 User's Guide
:CMT.Topics: "16-bit:  Describing Calling Information"
:CMT.      & "32-bit:  Describing Calling Information"
.np
:cmt..ix 'calling &functions' 'near'
:cmt..ix 'calling &functions' 'far'
The following form of the auxiliary pragma can be used to describe the
way a &function is to be called.
:cmt..ix 'pragmas' 'calling information'
:cmt..ix 'pragmas' 'far'
:cmt..ix 'pragmas' 'far16'
:cmt..ix 'pragmas' 'near'
:cmt..ix 'pragmas' '= const'
:cmt..ix 'pragmas' 'in-line assembly'
:cmt..ix 'calling information (pragma)'
:cmt..ix 'far (pragma)'
:cmt..ix 'far16 (pragma)'
:cmt..ix 'near (pragma)'
:cmt..ix 'in-line assembly' 'in pragmas'
.* ---------------------------------------
.mbox begin
:prgbeg. aux :id.sym:eid. far
    or
:prgbeg. aux :id.sym:eid. near
    or
:prgbeg. aux :id.sym:eid. = :id.in_line:eid.

:id.in_line ::= { const | :eid.":id.asm:eid.":id. | (:eid.float:id. fpinst) }:eid.
.mbox end
.if &'lower(&syspdev) = ps .pa
.mbox begin
:prgbeg. aux :id.sym:eid. far
    or
:prgbeg. aux :id.sym:eid. far16
    or
:prgbeg. aux :id.sym:eid. near
    or
:prgbeg. aux :id.sym:eid. = :id.in_line:eid.

:id.in_line ::= { const | :eid.":id.asm:eid.":id. }:eid.
.mbox end
.*
:CMT.This is from cmn\pragma.gml and is in the FORTRAN 77 User's Guide
:CMT.Topics: "16-bit:  Describing Argument Information"
:CMT.      & "32-bit:  Describing Argument Information"
.if &'lower(&syspdev) NE tasa .do begin
.np
.ix 'argument list (pragma)'
.ix 'pragmas' 'describing argument lists'
Using auxiliary pragmas, you can describe the calling convention that
&cmpname is to use for calling &functions..
This is particularly useful when interfacing to &functions that have
been compiled by other compilers or &functions written in other
programming languages.
.np
The general form of an auxiliary pragma that describes argument
passing is the following.
.cp 19
.mbox begin
:prgbeg. aux :id.sym:eid. parm :id.:rp. arg_info :or. pop_info :or. :eid.reverse:id. :rp.reg_set:erp. :erp.:eid.

:id.arg_info ::=:eid. ( :id.arg_attr:eid. :rp., :id.arg_attr:eid.:erp. )

:id.arg_attr ::=:eid. value :op.:id.v_attr:eid.:eop.
                :or. reference :op.:id.r_attr:eid.:eop.
                :or. data_reference :op.:id.d_attr:eid.:eop.

:id.v_attr ::=:eid. far :or. near :or. *1 :or. *2 :or. *4 :or. *8

:id.r_attr ::=:eid. :op.far :or. near:eop. :op.descriptor :or. nodescriptor:eop.

:id.d_attr ::=:eid. :op.far :or. near:eop.

:id.pop_info ::=:eid. caller :or. routine
.mbox end
.do end
.*
:CMT.This is from cmn\pragma.gml and is in the FORTRAN 77 User's Guide
:CMT.Topics: "16-bit:  Passing Arguments to non-FORTRAN Subprograms"
:CMT.      & "32-bit:  Passing Arguments to non-FORTRAN Subprograms"
.if &'lower(&syspdev) = ps .pa
.np
When calling a subprogram written in a different language, it may be
necessary to provide the arguments in a form different than the
default methods used by &cmpname..
For example, C functions require scalar arguments to be passed by
value instead of by reference.
For information on the methods &cmpname uses to pass arguments, see
the chapter entitled "Assembly Language Considerations".
.np
The following form of the auxiliary pragma can be used to alter the
default calling mechanism used for passing arguments.
.cp 15
.mbox begin
:prgbeg. aux :id.sym:eid. parm ( :id.arg_attr:eid. :rp., :id.arg_attr:eid.:erp. )

:id.arg_attr ::=:eid. value :op.:id.v_attr:eid.:eop.
                :or. reference :op.:id.r_attr:eid.:eop.
                :or. data_reference :op.:id.d_attr:eid.:eop.

:id.v_attr ::=:eid. far :or. near :or. *1 :or. *2 :or. *4 :or. *8

:id.r_attr ::=:eid. :op.far :or. near:eop. :op.descriptor :or. nodescriptor:eop.

:id.d_attr ::=:eid. :op.far :or. near:eop.
.mbox end
.*
:CMT.This is from cmn\wdis.gml and is in the Tool User's Guide
:CMT.Topic: "An Example"
.np
.ix '&discmdup example'
.ix 'disassembly example'
Consider the following program contained in the file
.fi hello.&langsuff..
.mbox begin
#include <stdio.h>

void main()
{
    printf( "Hello world\n" );
}
.mbox end
.*
:CMT.This is from cmn\wdis.gml and is in the FORTRAN 77 Tool User's Guide
:CMT.Topic: "An Example"
.np
.ix '&discmdup example'
.ix 'disassembly example'
Consider the following program contained in the file
.fi hello.&langsuff..
.mbox begin
      program main
      print *, 'Hello world'
      end
.mbox end
.*
:CMT.This is from cmn\wdis.gml and is in the Tool User's Guide
:CMT.Topic: "The Object File Disassembler"
.np
The &disname command line syntax is the following.
.ix '&discmdup' 'command line format'
.ix 'command line format' '&discmdup'
.mbigbox
&discmdup [options] [d:][path]filename[.ext] [options]
.embigbox
.*
:CMT.This is from cw\ref.gml and is in the CauseWay User's Guide
:CMT.Topic: "API functions (alphabetical order)"
.np
.ix 'CauseWay API reference'
.mbox begin
:api.AliasSel:eapi. :dsc.Create a read/write data selector from source selector.:edsc.
.mbox end
.*
.mbox begin
:api.CleanUp:eapi. :dsc.Close all open file handles.:edsc.
.mbox end
.*
.mbox begin
:api.CodeSel:eapi. :dsc.Make a selector execute/read type.:edsc.
.mbox end
.*
.mbox begin
:api.cwcInfo:eapi. :dsc.Validate and get expanded length of a CWC'd file.:edsc.
.mbox end
.*
.mbox begin
:api.cwcLoad:eapi. :dsc.Load/Expand a CWC'ed data file into memory.:edsc.
.mbox end
.*
.mbox begin
:api.CWErrName:eapi. :dsc.Change error file name, with optional drive/pathspec.:edsc.
.mbox end
.*
.mbox begin
:api.cwLoad:eapi. :dsc.Load another CauseWay program as an overlay.:edsc.
.mbox end
.*
.mbox begin
:api.Exec:eapi. :dsc.Run another CauseWay program directly.:edsc.
.mbox end
.*
.mbox begin
:api.ExecDebug:eapi. :dsc.Load CauseWay program for debug.:edsc.
.mbox end
.*
.mbox begin
:api.FarCallReal:eapi. :dsc.Simulate real mode far call.:edsc.
.mbox end
.*
.mbox begin
:api.GetCallBack:eapi. :dsc.Allocate real mode call back address.:edsc.
.mbox end
.*
.mbox begin
:api.GetDOSTrans:eapi. :dsc.Get current address and size of the buffer used for DOS memory transfers.:edsc.
.mbox end
.*
.mbox begin
:api.GetEVect:eapi. :dsc.Get Protected mode exception handler address.:edsc.
.mbox end
.*
.mbox begin
:api.GetMCBSize:eapi. :dsc.Get current memory control block (MCB) memory allocation block size.:edsc.
.mbox end
.*
.mbox begin
:api.GetMem:eapi. :dsc.Allocate a block of memory.:edsc.
.mbox end
.*
.mbox begin
:api.GetMem32:eapi. :dsc.Allocate a block of memory.:edsc.
.mbox end
.*
.mbox begin
:api.GetMemDOS:eapi. :dsc.Allocate a region of DOS (conventional) memory.:edsc.
.mbox end
.*
.mbox begin
:api.GetMemLinear:eapi. :dsc.Allocate a block of memory without a selector.:edsc.
.mbox end
.*
.mbox begin
:api.GetMemLinear32:eapi. :dsc.Allocate a block of memory without a selector.:edsc.
.mbox end
.*
.mbox begin
:api.GetMemSO:eapi. :dsc.Allocate a block of memory with selector:offset.:edsc.
.mbox end
.*
.mbox begin
:api.GetPatch:eapi. :dsc.Get patch table address.:edsc.
.mbox end
.*
.mbox begin
:api.GetSel:eapi. :dsc.Allocate a new selector.:edsc.
.mbox end
.*
.mbox begin
:api.GetSelDet:eapi. :dsc.Get selector linear base and limit.:edsc.
.mbox end
.*
.mbox begin
:api.GetSelDet32:eapi. :dsc.Get selector linear base and limit.:edsc.
.mbox end
.*
.mbox begin
:api.GetSels:eapi. :dsc.Allocate multiple selectors.:edsc.
.mbox end
.*
.mbox begin
:api.GetVect:eapi. :dsc.Get Protected mode interrupt handler address.:edsc.
.mbox end
.*
.mbox begin
:api.ID:eapi. :dsc.Get CauseWay identifier, PageDIRLinear and Page1stLinear info.:edsc.
.mbox end
.*
.mbox begin
:api.Info:eapi. :dsc.Get system selectors/flags.:edsc.
.mbox end
.*
.mbox begin
:api.IntXX:eapi. :dsc.Simulate real mode interrupt.:edsc.
.mbox end
.*
.mbox begin
:api.LinearCheck:eapi. :dsc.Check linear address of memory.:edsc.
.mbox end
.*
.mbox begin
:api.LockMem:eapi. :dsc.Lock a region of memory.:edsc.
.mbox end
.*
.mbox begin
:api.LockMem32:eapi. :dsc.Lock a region of memory.:edsc.
.mbox end
.*
.mbox begin
:api.RelCallBack:eapi. :dsc.Release a real mode call back entry.:edsc.
.mbox end
.*
.mbox begin
:api.RelMem:eapi. :dsc.Release memory allocated by either GetMem or GetMem32.:edsc.
.mbox end
.*
.mbox begin
:api.RelMemDOS:eapi. :dsc.Release a block of DOS (conventional) memory allocated by GetMemDOS.:edsc.
.mbox end
.*
.mbox begin
:api.RelMemLinear:eapi. :dsc.Release previously allocated block of memory (linear address).:edsc.
.mbox end
.*
.mbox begin
:api.RelMemLinear32:eapi. :dsc.Release previously allocated block of memory (linear address).:edsc.
.mbox end
.*
.mbox begin
:api.RelMemSO:eapi. :dsc.Release a block of memory allocated via GetMemSO.:edsc.
.mbox end
.*
.mbox begin
:api.RelSel:eapi. :dsc.Release a selector.:edsc.
.mbox end
.*
.mbox begin
:api.ResMem:eapi. :dsc.Resize a previously allocated block of memory.:edsc.
.mbox end
.*
.mbox begin
:api.ResMemSO:eapi. :dsc.Resize a block of memory allocated via GetMemSO.:edsc.
.mbox end
.*
.mbox begin
:api.ResMem32:eapi. :dsc.Resize a previously allocated block of memory.:edsc.
.mbox end
.*
.mbox begin
:api.ResMemDOS:eapi. :dsc.Resize a block of DOS (conventional) memory allocated with GetMemDOS.:edsc.
.mbox end
.*
.mbox begin
:api.ResMemLinear:eapi. :dsc.Resize a previously allocated block of memory without a selector.:edsc.
.mbox end
.*
.mbox begin
:api.ResMemLinear32:eapi. :dsc.Resize a previously allocated block of memory without a selector.:edsc.
.mbox end
.*
.mbox begin
:api.SetDOSTrans:eapi. :dsc.Set new address and size of the buffer used for DOS memory transfers.:edsc.
.mbox end
.*
.mbox begin
:api.SetDump:eapi. :dsc.Disable/enable error display and CW.ERR creation.:edsc.
.mbox end
.*
.mbox begin
:api.SetEVect:eapi. :dsc.Set Protected mode exception handler address.:edsc.
.mbox end
.*
.mbox begin
:api.SetMCBMax:eapi. :dsc.Set new memory control block (MCB) memory allocation block size.:edsc.
.mbox end
.*
.mbox begin
:api.SetRVect:eapi. :dsc.Set real mode interrupt handler address.:edsc.
.mbox end
.*
.mbox begin
:api.SetSelDet:eapi. :dsc.Set selector linear base and limit.:edsc.
.mbox end
.*
.mbox begin
:api.SetSelDet32:eapi. :dsc.Set selector linear base and limit.:edsc.
.mbox end
.*
.mbox begin
:api.SetVect:eapi. :dsc.Set Protected mode interrupt handler address.:edsc.
.mbox end
.*
.mbox begin
:api.UnLockMem:eapi. :dsc.Unlock a region of memory.:edsc.
.mbox end
.*
.mbox begin
:api.UnLockMem32:eapi. :dsc.Unlock a region of memory.:edsc.
.mbox end
.*
.mbox begin
:api.UserDump:eapi. :dsc.Setup user-defined error buffer dump in CW.ERR.:edsc.
.mbox end
.*
.mbox begin
:api.UserErrTerm:eapi. :dsc.Call user error termination routine.:edsc.
.mbox end
.*
:CMT.This is from fg\fcall.gml and is in the FORTRAN 77 User's Guide
:CMT.Topics: "16-bit: Writing Assembly Language Subprograms"
:CMT.      & "32-bit: Writing Assembly Language Subprograms"
.np
.if &'lower(&syspdev) = ps .pa
Consider the following example.
.mbox begin
      INTEGER HRS, MINS, SECS, HSECS
      CALL GETTIM( HRS, MINS, SECS, HSECS )
      PRINT 100, HRS, MINS, SECS, HSECS
100   FORMAT( 1X,I2.2,':',I2.2,':',I2.2,'.',I2.2 )
      END
.mbox end
.*
:CMT.This is from fg\fcall.gml and is in the FORTRAN 77 User's Guide
:CMT.Topic: "32-bit: Using the Stack-Based Calling Convention"
.np
.ix 'stack-based calling convention' 'writing assembly language subprograms'
Consider the following example.
.mbox begin
      INTEGER HRS, MINS, SECS, HSECS
      CALL GETTIM( HRS, MINS, SECS, HSECS )
      PRINT 100, HRS, MINS, SECS, HSECS
100   FORMAT( 1X,I2.2,':',I2.2,':',I2.2,'.',I2.2 )
      END
.mbox end
.*
:CMT.This is from fg\fcall.gml and is in the FORTRAN 77 User's Guide
:CMT.Topic: "32-bit: Processing Function Return Values with no 80x87"
.remark
.ix 'stack-based calling convention'
The way in which a function returns its value does not change when the
stack-based calling convention is used.
.eremark
.*
:CMT.This is from fg\fcall.gml and is in the FORTRAN 77 User's Guide
:CMT.Topic: "32-bit: Processing Function Return Values Using an 80x87"
.remark
.ix 'stack-based calling convention'
When the stack-based calling convention is used, floating-point values
are not returned using the 80x87.
.bd REAL*4
values are returned in register EAX.
.bd REAL*8
values are returned in registers EDX:EAX.
.eremark
.*
:CMT.This is from fg\fcall.gml and is in the FORTRAN 77 User's Guide
:CMT.Topic: "32-bit: Processing Alternate Returns"
.remark
.ix 'stack-based calling convention'
The way in which a alternate returns are processed does not change
when the stack-based calling convention is used.
.eremark
.*
:CMT.This is from fg\fcall.gml and is in the FORTRAN 77 User's Guide
:CMT.Topic: "32-bit: Returning Values from Assembly Language Functions"
.remark
.ix 'stack-based calling convention'
The way in which a function returns its value does not change when the
stack-based calling convention is used.
.eremark
.*
:CMT.This is from fg\farrays.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Properties of Arrays"
.np
Arrays are defined by an array declarator.
The form of an
.us array declarator
.ix 'array declarator'
is:
:cmt.The IN 1 was added to allow .mbox to have a valid left start position.
.in 1
.mbox begin
      a( d [,d] ... )
.mbox end
.np
Each dimension has a range of values.
When referencing elements in that dimension, the dimension expression
must fall in that range.
The range of a dimension is defined in the dimension declarator.
A
.us dimension declarator
.ix 'dimension declarator'
has the following form:
.mbox begin
      [lo:] hi
.mbox end
.*
:CMT.This is from fg\farrays.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Array Elements"
.np
Each array is comprised of a sequence of array elements.
An array element is referenced by following the array name with a
.us subscript.
.ix 'subscript'
Different elements of the array are referenced by simply
changing the subscript.
An
.us array element
.ix 'array element'
has the following form:
.mbox begin
      a(s[,s]...)
.mbox end
.*
:CMT.This is from fl\fassmnt.gml  and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Arithmetic Assignment"
.np
.ix 'arithmetic assignment statement'
.ix 'assignment statement' 'arithmetic'
The form of an
.us arithmetic assignment statement
is
.mbox begin
      v = e
.mbox end
.*
:CMT.This is from fl\fassmnt.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Logical Assignment"
.np
.ix 'logical assignment statement'
.ix 'assignment statement' 'logical'
The form of a
.us logical assignment statement
is
.mbox begin
      v = e
.mbox end
.*
:CMT.This is from fl\fassmnt.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Statement Label Assignment"
.np
.ix 'statement label assignment'
.ix 'assignment statement' 'statement label'
The form of a
.us statement label assignment
is
.mbox begin
      ASSIGN s to i
.mbox end
.*
:CMT.This is from fl\fassmnt.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Character Assignment"
.np
.ix 'character assignment statement'
.ix 'assignment statement' 'character'
The form of a
.us character assignment statement
is
.mbox begin
      v = e
.mbox end
.*
:CMT.This is from fl\fassmnt.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended Assignment Statement"
.np
.ix 'extended assignment statement'
.ix 'assignment statement' 'extended'
&product supports an extension to the FORTRAN 77 assignment
statement, namely the 
.us extended assignment statement
shown here:
.mext begin
      v  =  v  =  v  = ... =  v  = e
      1    2    3          n
.mext end
.*
:CMT.This is from fl\fexpr.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Arithmetic Operators"
.np
The following table lists the arithmetic operators and the
operation they perform.
.* .mbox on 1 12 40
.sr c0=&INDlvl+1
.sr c1=&INDlvl+12
.sr c2=&INDlvl+40
.mbox on &c0 &c1 &c2
Operator    Arithmetic Operation
.mbox
**          Exponentiation
/           Division
*           Multiplication
-           Subtraction or Negation
+           Addition or Identity
.mbox off
.np
Some operators can be either binary or unary.
.ix 'binary operator'
.ix 'operator' 'binary'
A
.us binary operator
is one that requires two operands.
.ix 'unary operator'
.ix 'operator' 'unary'
A
.us unary operator
is one that requires one operand.
Each of the operators **, /, and * are binary operators.
The operators + and &minus. can either be binary or unary operators.
The following table describes how each operator is used with their
operands.
.sr c0=&INDlvl+1
.sr c1=&INDlvl+12
.sr c2=&INDlvl+40
.cp 12
.mbox on &c0 &c1 &c2
Operator    Arithmetic Operation
.mbox
x ** y      x is raised to the power y
x / y       x is divided by y
x * y       x is multiplied by y
x - y       y is subtracted from x
x + y       y is added to x
  - x       x is negated
  + x       identity
.mbox off
.*
:CMT.This is from fl\fexpr.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Relational Operators"
.np
.ix 'relational operator'
.ix 'operator' 'relational'
The following table lists the
.us relational operators
and the operation they perform.
.sr c0=&INDlvl+1
.sr c1=&INDlvl+12
.sr c2=&INDlvl+40
.mbox on &c0 &c1 &c2
Operator    Relational Operation
.mbox
 .LT.        Less than
 .LE.        Less than or equal
 .EQ.        Equal
 .NE.        Not equal
 .GT.        Greater than
 .GE.        Greater than or equal
.mbox off
.*
:CMT.This is from fl\fexpr.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Form of a Relational Expression"
.np
.ix 'relational expression'
.ix 'expression' 'relational'
The form of a
.us relational expression
is as follows.
.sr c0=&INDlvl+1
.sr c1=&INDlvl+26
.mbox on &c0 &c1
  e1 relop e2
.mbox off
.*
:CMT.This is from fl\fexpr.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Logical Operators"
.if &'lower(&syspdev) = ps .pa
.np
.ix 'logical operator'
The following table lists the
.us logical operators
and the operation they perform.
.sr c0=&INDlvl+1
.sr c1=&INDlvl+12
.sr c2=&INDlvl+40
.mbox on &c0 &c1 &c2
Operator    Logical Operation
.mbox
 .NOT.       Logical negation
 .AND.       Logical conjunction
 .OR.        Logical inclusive disjunction
 .EQV.       Logical equivalence
 .NEQV.      Logical non-equivalence
 .XOR.       Exclusive or
.mbox off
.np
The logical operator .NOT. is a unary operator; all other logical
operators are binary.
The following tables describe the result of each operator when it
is used with logical operands.
.* .mbox on 1 12 30
.sr c0=&INDlvl+1
.sr c1=&INDlvl+12
.sr c2=&INDlvl+30
.mbox on &c0 &c1 &c2
  x           .NOT. x
.mbox
true          false
false         true
.mbox off
.* .mbox on 1 9 17 30
.sr c0=&INDlvl+1
.sr c1=&INDlvl+9
.sr c2=&INDlvl+17
.sr c3=&INDlvl+30
.mbox on &c0 &c1 &c2 &c3
  x       y       x .AND. y
.mbox
true    true        true
true    false       false
false   true        false
false   false       false
.mbox off
.mbox on &c0 &c1 &c2 &c3
  x       y       x .OR. y
.mbox
true    true        true
true    false       true
false   true        true
false   false       false
.mbox off
.if &'lower(&syspdev) = ps .pa
.mbox on &c0 &c1 &c2 &c3
  x       y       x .EQV. y
.mbox
true    true        true
true    false       false
false   true        false
false   false       true
.mbox off
.mbox on &c0 &c1 &c2 &c3
x       y         x .NEQV. y
 ................ x .XOR. y
.mbox
true    true        false
true    false       true
false   true        true
false   false       false
.mbox off
.np
.* .mext on 1 12 30
.sr c0=&INDlvl+1
.sr c1=&INDlvl+12
.sr c2=&INDlvl+30
.mext on &c0 &c2 &c1
x           .NOT. x
.mext
0              1
1              0
.mext off
.* .mext on 1 8 16 30
.sr c0=&INDlvl+1
.sr c1=&INDlvl+8
.sr c2=&INDlvl+15
.sr c3=&INDlvl+30
.mext on &c0 &c3 &c1 &c2
x       y       x .AND. y
.mext
1       1           1
1       0           0
0       1           0
0       0           0
.mext off
.mext on &c0 &c3 &c1 &c2
x       y       x .OR. y
.mext
1       1           1
1       0           1
0       1           1
0       0           0
.mext off
.mext on &c0 &c3 &c1 &c2
x       y       x .EQV. y
.mext
1       1           1
1       0           0
0       1           0
0       0           1
.mext off
.mext on &c0 &c3 &c1 &c2
x       y       x .NEQV. y
 .............. x .XOR. y
.mext
1       1           0
1       0           1
0       1           1
0       0           0
.mext off
.*
:CMT.This is from fl\fformats.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "The FORMAT Statement"
.np
The form of a
.kw FORMAT
statement is
.mbox begin
      label    FORMAT fs
.mbox end
.*
:CMT.This is from fl\fformats.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Format Specification"
.np
.ix 'format' 'see also' 'edit descriptor'
A
.us format specification
.ix 'format specification'
has the following form.
.mbox begin
      ( [flist] )
.mbox end
.*
:CMT.This is from fl\fformats.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "E and D Editing"
.np
The form of the input field and processing of it is the same as that
for
.id F
editing.
The form of the output field is:
.mbox begin
      [+] [0] . x  x  ... x  exp
      [-]        1  2      d
.mbox end
.*
:CMT.This is from fl\fio.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Printing of Formatted Records"
.np
.ix 'ASA'
.ix 'carriage control'
The first character of the record controls the vertical spacing.
This feature is quite often called ASA (American Standards
Association) carriage control.
.sk 1 c
.* .mbox on 1 13 45
.sr c0=&INDlvl+1
.sr c1=&INDlvl+13
.sr c2=&INDlvl+45
.mbox on &c0 &c1 &c2
Character    Vertical Spacing Before Printing
.mbox
Blank           One Line
0               Two Lines
-               Three Lines
1               To First Line of Next Page
+               No Advance
.mbox off
.*
:CMT.This is from fl\fsubp.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Statement Functions"
.np
A statement function is a procedure defined by a single statement.
Its definition must follow all specification statements and precede
the first executable statement.
The statement defining a statement function is not an executable
statement.
.np
.ix 'statement function'
.ix 'function' 'statement'
A
.us statement function
has the following form.
.mbox begin
      sf ( [d [,d] ...] ) = e
.mbox end
.*
:CMT.This is from fl\fsubp.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "RETURN Statement in Function Subprograms"
.np
The form of a
.kw RETURN
statement in a function subprogram is:
.mbox begin
      RETURN
.mbox end
.*
:CMT.This is from fl\fsubp.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "RETURN Statement in Subroutine Subprograms"
.np
The form of a
.kw RETURN
statement in a subroutine subprogram is:
.mbox begin
      RETURN [ e ]
.mbox end
.*
:CMT.This is from fl\fsubp.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "RETURN Statement in Subroutine Subprograms"
.np
.mext begin
      RETURN
.mext end
.*
:CMT.This is from fl\fsubstr.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Substring Names"
.np
Substrings are formed by specifying a substring name.
The forms of a
.us substring name
.ix 'substring name'
are:
.mbox begin
      v( [ e1] : [ e2] )
      a(s [,s] ...)( [ e1] : [ e2] )
.mbox end
.*
:CMT.This is from fl\sassign.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Statement Label Assignment (ASSIGN) Statement"
.mbox begin
      ASSIGN s TO i
.mbox end
.*
:CMT.This is from fl\sbackspa.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "BACKSPACE Statement"
.mbox begin
      BACKSPACE u
      BACKSPACE (alist)
.mbox end
.*
:CMT.This is from fl\sblock.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "BLOCK DATA Statement"
.mbox begin
      BLOCK DATA [sub]
.mbox end
.*
:CMT.This is from fl\scall.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "CALL Statement"
.mbox begin
      CALL sub [( [a [, a] ... ] )]
.mbox end
.*
:CMT.This is from fl\sclose.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "CLOSE Statement"
.mbox begin
      CLOSE (cllist)
.mbox end
.*
:CMT.This is from fl\scommon.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "COMMON Statement"
.mbox begin
      COMMON [/[cb]/] nlist [[,]/[cb]/ nlist] ...
.mbox end
.*
:CMT.This is from fl\scontinu.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "CONTINUE Statement"
.mbox begin
      CONTINUE
.mbox end
.*
:CMT.This is from fl\sdata.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "DATA Statement"
.mbox begin
      DATA nlist/clist/ [[,]nlist/clist/] ...
.mbox end
.*
:CMT.This is from fl\sdimensi.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "DIMENSION Statement"
.mbox begin
      DIMENSION a(d) [,a(d)] ...
.mbox end
.*
:CMT.This is from fl\sdo.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard DO Statement"
.mbox begin
      DO s [,] i = e1, e2 [, e3]
.mbox end
.*
:CMT.This is from fl\sdo.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended DO Statement"
.mext begin
      DO [s[,]] i = e1, e2 [, e3]    [: block-label]
.mext end
.*
:CMT.This is from fl\selse.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "ELSE Statement"
.mbox begin
      ELSE
.mbox end
.*
:CMT.This is from fl\selseif.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "ELSE IF Statement"
.mbox begin
      ELSE IF (e) THEN
.mbox end
.*
:CMT.This is from fl\send.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END Statement"
.mbox begin
      END
.mbox end
.*
:CMT.This is from fl\sendfile.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "ENDFILE Statement"
.mbox begin
      ENDFILE u
      ENDFILE (alist)
.mbox end
.*
:CMT.This is from fl\sendif.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END IF Statement"
.mbox begin
      END IF
.mbox end
.*
:CMT.This is from fl\sentry.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "ENTRY Statement"
.mbox begin
      ENTRY name [( [d [, d] ...] )]
.mbox end
.*
:CMT.This is from fl\sequival.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "EQUIVALENCE Statement"
.mbox begin
      EQUIVALENCE (nlist) [,(nlist)] ...
.mbox end
.*
:CMT.This is from fl\sexterna.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "EXTERNAL Statement"
.mbox begin
      EXTERNAL p [,p] ...
.mbox end
.*
:CMT.This is from fl\sformat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "FORMAT Statement"
.mbox begin
      label    FORMAT fs
.mbox end
.*
:CMT.This is from fl\sfunctio.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard FUNCTION Statement"
.mbox begin
      [type] FUNCTION fun ( [d [, d] ...] )
.mbox end
.*
:CMT.This is from fl\sfunctio.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended FUNCTION Statement"
.mext begin
      [type[*len]] FUNCTION fun[*len] ( [d [, d] ...] )
.mext end
.*
:CMT.This is from fl\sgo.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Unconditional GO TO Statement"
.mbox begin
      GO TO s
.mbox end
.*
:CMT.This is from fl\sgo.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Computed GO TO Statement"
.mbox begin
      GO TO (s [,s]...) [,] i
.mbox end
.*
:CMT.This is from fl\sgo.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Assigned GO TO Statement"
.mbox begin
      GO TO i [[,] (s [,s]...)]
.mbox end
.*
:CMT.This is from fl\sif.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Arithmetic IF Statement"
.mbox begin
      IF (e) s1, s2, s3
.mbox end
.*
:CMT.This is from fl\sif.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Logical IF Statement"
.mbox begin
      IF (e) st
.mbox end
.*
:CMT.This is from fl\sif.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard Block IF Statement"
.mbox begin
      IF (e) THEN
.mbox end
.*
:CMT.This is from fl\sif.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended Block IF Statement"
.mext begin
      IF (e) THEN    [: block-label]
.mext end
.*
:CMT.This is from fl\simplici.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard IMPLICIT Statement"
.mbox begin
      IMPLICIT type (a [,a] ...) [,type (a [,a] ...)]...
.mbox end
.*
:CMT.This is from fl\simplici.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended IMPLICIT Statement"
.mext begin
      IMPLICIT type[*len] (a [,a] ...)
             [,type[*len] (a [,a] ...)] ...
.mext end
.*
:CMT.This is from fl\simplici.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "IMPLICIT NONE Statement"
.mext begin
      IMPLICIT NONE
.mext end
.*
:CMT.This is from fl\sinquire.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "INQUIRE by FILE"
.mbox begin
      INQUIRE (iflist)
.mbox end
.*
:CMT.This is from fl\sinquire.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "INQUIRE by UNIT"
.mbox begin
      INQUIRE (iulist)
.mbox end
.*
:CMT.This is from fl\sintrins.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "INTRINSIC Statement"
.mbox begin
      INTRINSIC f [,f] ...
.mbox end
.*
:CMT.This is from fl\sopen.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "OPEN Statement"
.mbox begin
      OPEN (oplist)
.mbox end
.*
:CMT.This is from fl\sparamet.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "PARAMETER Statement"
.mbox begin
      PARAMETER (p=e [,p=e] ...)
.mbox end
.*
:CMT.This is from fl\spause.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "PAUSE Statement"
.mbox begin
      PAUSE [n]
.mbox end
.*
:CMT.This is from fl\sprint.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard PRINT Statement"
.mbox begin
      PRINT f [,olist]
.mbox end
.*
:CMT.This is from fl\sprint.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended PRINT Statement"
.mext begin
      PRINT, olist
.mext end
.*
:CMT.This is from fl\sprogram.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "PROGRAM Statement"
.mbox begin
      PROGRAM pgm
.mbox end
.*
:CMT.This is from fl\sread.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard READ Statement"
.mbox begin
      READ (cilist) [ilist]
      READ f [,ilist]
.mbox end
.*
:CMT.This is from fl\sread.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard READ Statement"
.mext begin
      READ, ilist
.mext end
.*
:CMT.This is from fl\sreturn.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "RETURN Statement"
.mbox begin
      RETURN [e]
.mbox end
.*
:CMT.This is from fl\srewind.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "REWIND Statement"
.mbox begin
      REWIND u
      REWIND (alist)
.mbox end
.*
:CMT.This is from fl\ssave.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "SAVE Statement"
.mbox begin
      SAVE [a [,a] ...]
.mbox end
.*
:CMT.This is from fl\sstop.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "STOP Statement"
.mbox begin
      STOP [n]
.mbox end
.*
:CMT.This is from fl\ssubrout.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "SUBROUTINE Statement"
.mbox begin
      SUBROUTINE sub [( [d [, d] ...] )]
.mbox end
.*
:CMT.This is from fl\stype.gml and is in the FORTRAN 77 Language Reference
:CMT.This file generates sections for the various FORTRAN 77 types
:CMT.Each type has at least two, usually three, sub-topics
:CMT.Some sub-topics for some types include other files
:CMT.The symbol "typstmt" needs to be set for each type
:CMT.the symbol "typname" may need to be set for one or more types
:CMT.Note: stype.gml only deals with the "Standard" or "Simple" sections
.*
.sr typstmt=CHARACTER
:CMT.This is from fl\stype.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard CHARACTER Statement"
.mbox begin
      CHARACTER[*len [,]] name [,name] ...
.mbox end
.*
:CMT.This is from fl\stypdat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended CHARACTER Statement:  Data Initialization"
. .mext begin
      &typstmt name [/cl/] [,name[/cl/]] ...
. .mext end
.*
.sr typstmt=COMPLEX
:CMT.This is from fl\stype.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard COMPLEX Statement"
.mbox begin
      &typstmt name [,name] ...
.mbox end
.*
:CMT.This is from fl\stypdat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended COMPLEX Statement:  Data Initialization"
. .mext begin
      &typstmt[*len[,]] name [/cl/] [,name[/cl/]] ...
. .mext end
.*
:CMT.This is from fl\stypext.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended COMPLEX Statement:  Length Specification"
.mext begin
      &typstmt[*len[,]] name [,name] ...
.mext end
.*
.sr typstmt=DOUBLE COMPLEX
:CMT.This is from fl\stype.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Simple DOUBLE COMPLEX Statement"
.mext begin
      &typstmt name [,name] ...
.mext end
.*
:CMT.This is from fl\stypdat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "DOUBLE COMPLEX Statement:  Data Initialization"
. .mext begin
      &typstmt name [/cl/] [,name[/cl/]] ...
. .mext end
.*
.sr typstmt=DOUBLE PRECISION
:CMT.This is from fl\stype.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard DOUBLE PRECISION Statement"
.mbox begin
      &typstmt name [,name] ...
.mbox end
.*
:CMT.This is from fl\stypdat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended DOUBLE PRECISION Statement:  Data Initialization"
. .mext begin
      &typstmt name [/cl/] [,name[/cl/]] ...
. .mext end
.*
.sr typstmt=INTEGER
:CMT.This is from fl\stype.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard INTEGER Statement"
.mbox begin
      &typstmt name [,name] ...
.mbox end
.*
:CMT.This is from fl\stypdat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended INTEGER Statement:  Data Initialization"
. .mext begin
      &typstmt[*len[,]] name [/cl/] [,name[/cl/]] ...
. .mext end
.*
:CMT.This is from fl\stypext.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended INTEGER Statement:  Length Specification"
.mext begin
      &typstmt[*len[,]] name [,name] ...
.mext end
.*
.sr typstmt=LOGICAL
:CMT.This is from fl\stype.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard LOGICAL Statement"
.mbox begin
      &typstmt name [,name] ...
.mbox end
.*
:CMT.This is from fl\stypdat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended LOGICAL Statement:  Data Initialization"
. .mext begin
      &typstmt[*len[,]] name [/cl/] [,name[/cl/]] ...
. .mext end
.*
:CMT.This is from fl\stypext.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended LOGICAL Statement:  Length Specification"
.mext begin
      &typstmt[*len[,]] name [,name] ...
.mext end
.*
.sr typstmt=REAL
:CMT.This is from fl\stype.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Standard REAL Statement"
.mbox begin
      &typstmt name [,name] ...
.mbox end
.*
:CMT.This is from fl\stypdat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended REAL Statement:  Data Initialization"
. .mext begin
      &typstmt[*len[,]] name [/cl/] [,name[/cl/]] ...
. .mext end
.*
:CMT.This is from fl\stypext.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Extended REAL Statement:  Length Specification"
.mext begin
      &typstmt[*len[,]] name [,name] ...
.mext end
.*
:CMT.This is from fl\svolatil.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "VOLATILE Statement"
.mbox begin
      VOLATILE [a [,a] ...]
.mbox end
.*
:CMT.This is from fl\swrite.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "WRITE Statement"
.mbox begin
      WRITE (cilist) [olist]
.mbox end
.*
:CMT.This is from cg\cpwcc.gml and is in the C/C++ User's Guide
:CMT.Topic: "Open Watcom C/C++ Command Line Format"
.np
.ix 'command line format'
.ix 'compiling' 'command line format'
The formal &product command line syntax is shown below.
.ix 'invoking &product'
.mbigbox
compiler_name [options] [file_spec] [options] [@extra_opts]
.embigbox
.*
:CMT.This is from cg\cpwcc.gml and is in the C/C++ User's Guide
:CMT.Topic: "Environment Variables"
.hint
.ix 'user initialization file'
If you use the same compiler options all the time, you may find it
handy to define the environment variable in your user initialization
file.
.ix 'system initialization file' 'AUTOEXEC.BAT'
.ix 'AUTOEXEC.BAT'
If you are running DOS and you use the same compiler options all the
time, you may find it handy to define the environment variable in your
DOS system initialization file,
.fi AUTOEXEC.BAT.
.np
.ix 'system initialization' 'Windows NT'
.ix 'Windows NT' 'system initialization'
If you are running Windows NT, use the "System" icon in the
.bd Control Panel
to define environment variables.
.np
.ix 'system initialization file' 'CONFIG.SYS'
.ix 'CONFIG.SYS'
If you are running OS/2 and you use the same compiler options all the
time, you may find it handy to define the environment variable in your
OS/2 system initialization file,
.fi CONFIG.SYS.
.ehint
.*
:CMT.This is from cmn\bpatch.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "Applying a Patch"
.np
The format of the &patchcmdup command line is:
.ix '&patchcmdup' 'command line format'
.ix 'command line format' '&patchcmdup'
.mbigbox
&patchcmdup [options] patch_file
.embigbox
.*
:CMT.This is from cmn\exe2bin.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "The Open Watcom Exe2bin Utility"
.if &'lower(&syspdev) = ps .pa
.remark
Although &e2bname is capable of producing DOS ".COM" executables, this
functionality is only provided for compatibility with other tools. The
preferred way of generating ".COM" executables is to use
the &lnkname. with directive
.mono "format dos com".
Refer to the &lnkname Guide for details.
.eremark
.*
:CMT.This is from cmn\exe2bin.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "The Open Watcom Exe2bin Utility Command Line"
.np
The format of the &e2bname command line is as follows.
Items enclosed in square brackets ("[ ]") are optional.
.*
:cmt.The IN 1 was added to allow .mbigbox to have a valid left position
.in 1
.mbigbox
&e2bcmdup [options] exe_file [bin_file]
.embigbox
.*
:CMT.This is from cmn\exe2bin.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "The Open Watcom Exe2bin Utility Command Line"
.remark
Even if &e2bcmd. is sucessfully invoked with identical input files as in the
preceding examples (i.e. with vs. without &sw.x) the output files may differ.
This happens when cs:ip = 0x0000:0x0100 causes &e2bcmd. to skip additional
0x100 bytes from the input file, if the user did not specify &sw.x.
.eremark
.*
:CMT.This is from cmn\ide2make.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "The IDE2MAKE Utility"
.np
The &id2mkname command line syntax is:
.ix '&id2mkname' 'command line format'
.ix 'command line format' '&id2mkname'
.ix 'invoking &id2mkname'
.mbigbox
&id2mkcmd [options] [tgtfile]
.embigbox
.*
:CMT.This is from cmn\owcc.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "owcc Command Line Format"
.np
.ix 'owcc' 'command line format'
.ix 'command line format' 'owcc'
The format of the command line is:
.mbigbox
owcc [options] [files]
.embigbox
.*
:CMT.This is from cmn\wasm.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "The Open Watcom Assembler"
.np
The &asmname command line syntax is the following.
.ix '&asmcmdup' 'command line format'
.ix 'command line format' '&asmcmdup'
:cmt..if '&target' eq 'QNX' .do begin
.mbigbox
&asmcmdup [options] asm_file [options] [@env_var]
.embigbox
.if &'lower(&syspdev) = ps .pa
.mbigbox
&asmcmdup [options] [d:][path]filename[.ext] [options] [@env_var]
.embigbox
.*
:CMT.This is from cmn\wcl.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "WCL/WCL386 Command Line Format"
.np
.ix '&wclcmdup16' 'command line format'
.ix '&wclcmdup32' 'command line format'
.ix 'command line format' '&wclcmdup16'
.ix 'command line format' '&wclcmdup32'
The format of the command line is:
.mbigbox
&wclcmdup16 [files] [options]
&wclcmdup32 [files] [options]
.embigbox
.*
:CMT.This is from cmn\wmake.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "Open Watcom Make Command Line Format"
.np
.ix '&makcmdup command line' 'format'
The formal &makname command line syntax is shown below.
.ix '&makcmdup command line' 'invoking &makcmdup'
.ix 'command line format' '&makcmdup'
.mbigbox
&makcmdup [options] [macro_defs] [targets]
.embigbox
.*
:CMT.This is from cmn\wmake.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "Dependency Declarations"
.hint
A good habit to develop is to always put spaces around the colon so
that it will not be confused with drive specifications (e.g., a:).
.ehint
.remark
The terminology employed here is used by S.I.Feldman of Bell
Laboratories in
.us Make - A Program for Maintaining Computer Programs.
http://www.softlab.ntua.gr/facilities/documentation/unix/docs/make.txt
has a copy of this seminal article.
Confusion often arises from the use of the word "dependent".
In this context, it means "a subordinate part".
In the example, "LEDGER.DAT" is a subordinate part of the report
"BALANCE.LST".
.eremark
.*
:CMT.This is from cmn\wmake.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "Multiple Rules"
.hint
The term "updating", in this context, refers to the process of
checking the time-stamps of dependents and running the specified
command list whenever they are out-of-date.
Whenever a dependent is the target of some other rule, the dependent
must be brought up-to-date first.
Stated another way, if "A" depends on "B" and "B" depends on "C" and
"C" is younger than "B" then we must update "B" before we update "A".
.ehint
.*
:CMT.This is from cmn\wmake.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "Defining Recognized File Extensions (.EXTENSIONS)"
.hint
The default
.id &sysper.EXTENSIONS
declaration could have been coded as:
.np
:cmt. .millust
&sysper.EXTENSIONS:
.br
&sysper.EXTENSIONS: .exe
.br
&sysper.EXTENSIONS: .nlm .dsk .lan .exp
.br
&sysper.EXTENSIONS: .lib
.br
&sysper.EXTENSIONS: .obj
.br
&sysper.EXTENSIONS: .i .asm .c .cpp .cxx .cc
.br
&sysper.EXTENSIONS: .for .pas .cob
.br
&sysper.EXTENSIONS: .h .hpp .hxx .hh .fi .mif .inc
.br
&sysper.EXTENSIONS: .inc
:cmt. .emillust
.np
with identical results.
.ehint
.*
:CMT.This is from cmn\wmake.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "Implicit Rules"
.np
&makname is capable of accepting declarations of commonly used
dependencies.
.ix '&makcmdup' 'implicit rule'
.ix 'implicit rule'
These declarations are called "implicit rules" as opposed to "explicit
rules" which were discussed previously.
Implicit rules may be applied only in instances where you are able to
describe a dependency in terms of file extensions.
.hint
Recall that a file extension is the portion of the file name which
follows the period.
In the file specification:
.np
.fi c:\dos\ansi.sys
.np
the file extension is "SYS".
.ehint
.pc
This allows the implicit rule search to find the files with the
dependent extension.
.hint
A valid path specification is made up of directory specifications
separated by semicolons (";").
Here are some path specifications:
.sk 1
.cw !
.mono D:;C:\DOS;C:\UTILS;C:\WC
.br
.mono C:\SYS
.br
.mono A:\BIN;D:
.cw ;
.np
Notice that these path specifications are identical to the form
required by the operating system shell's "PATH" command.
.ehint
.hint
The simple heuristic method that &maksname uses for optimizing path
specification searches namely, keeping track of the last successful
sub-directory, is very effective in reducing the amount of disk
activity during the processing of a makefile.
A pitfall to avoid is having two files with the same name in the path.
The version of the file that is used to update the target depends on
the previous searches.
Care should be taken when using files that have the same name with
path specifications.
.ehint
.*
:CMT.This is from cmn\wmake.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "File Inclusion"
.pc
.ix '&makcmdup' 'reducing maintenance'
.ix 'reducing maintenance'
The use of the
.id !include
preprocessing directive is a simple way to reduce maintenance of
related makefiles.
.hint
Macros are expanded on
.id !include
preprocessor control lines.
This allows many benefits like:
.np
.ul
!include $(%env_var)
.pc
so that the files that &maksname will process can be controlled through
many different avenues like internal macros, command line macros, and
environment variables.
.np
Another way to access files is through the suffix path feature of
&maksname..
A definition like
.np
.ul
&sysper.mif : c:\mymifs;d:\some\more\mifs
.pc
will cause &maksname to search different paths for any make include
files.
.ehint
.*
:CMT.This is from cmn\wmodels.gml and is in the C/C++ User's Guide
:CMT.Topic: "16-bit Code Models"
.remark
If your program contains less than 64K of code, you should use a memory
model that employs the small code model.
This will result in smaller and faster code since near calls are
smaller instructions and are processed faster by the CPU.
.eremark
.*
:CMT.This is from cmn\wtouch.gml and is in the C/C++ Tools User's Guide
:CMT.Topic: "The Touch Utility"
.np
The &tchname command line syntax is:
.ix '&tchcmdup' 'command line format'
.ix 'command line format' '&tchcmdup'
.ix 'invoking &tchname'
:cmt.The IN 1 was added to allow .mbigbox to have a valid left position
.in 1
.mbigbox
&tchcmdup [options] file_spec [file_spec...]
.embigbox
.*
:CMT.This is from cw\guide.gml and is in the Causeway User's Guide
:CMT.Topic: "Using DLLs with Open Watcom C/C++"
.np
Following is an overview of the standard link file commands used to
create DLLs. You may also refer to the &product. Linker documentation for a
description of these commands.
.mbigbox
EXPORT function_name
.embigbox
.np
.mbigbox
IMPORT [local_name] module_name.function_name

IMPORT [local_name] module_name.ordinal
.embigbox
.mbigbox
NAME   module_name
.embigbox
.*
:CMT.This is from fg\fpwfc.gml and is in the FORTRAN 77 User's Guide
:CMT.Topic: "Open Watcom FORTRAN 77 Command Line Format"
.np
.ix 'command line format'
.ix 'compiling' 'command line format'
The formal &product command line syntax is shown below.
.ix 'invoking &product'
.mbigbox
&ccmdup16 [options] [d:][path]filename[.ext] [options]
&ccmdup32 [options] [d:][path]filename[.ext] [options]
.embigbox
.*
:CMT.This is from fg\fpwfc.gml and is in the FORTRAN 77 User's Guide
:CMT.Topic: "WFC/WFC386 Environment Variables"
.hint
.ix 'user initialization file'
If you use the same compiler options all the time, you may find it
handy to define the environment variable in your user initialization
file.
.ix 'system initialization file' 'AUTOEXEC.BAT'
.ix 'AUTOEXEC.BAT'
If you are running DOS and you use the same compiler options all the
time, you may find it handy to define the environment variable in your
DOS system initialization file,
.fi AUTOEXEC.BAT.
.np
.ix 'system initialization' 'Windows NT'
.ix 'Windows NT' 'system initialization'
If you are running Windows NT, use the "System" icon in the
.bd Control Panel
to define environment variables.
.np
.ix 'system initialization file' 'CONFIG.SYS'
.ix 'CONFIG.SYS'
If you are running OS/2 and you use the same compiler options all the
time, you may find it handy to define the environment variable in your
OS/2 system initialization file,
.fi CONFIG.SYS.
.ehint
.*
:CMT.This is from gui\wprof.gml and is in the Profiler Help
:CMT.Topic: "The Open Watcom Execution Sampler"
.np
.ix 'sampler' 'command line format'
.ix 'command line format' 'sampler'
The sampler's command line syntax is shown below.
.mbigbox
&smpcmdup. [options] program [arguments]
.embigbox
.*
:CMT.This is from gui\wprof.gml and is in the Profiler Help
:CMT.Topic: "Open Watcom Execution Profiler Command Line Format"
.np
.ix 'profiler' 'command line format'
.ix 'command line format' 'profiler'
.ix '&prfcmdup'
The formal profiler command line syntax is shown below.
.mbigbox
&prfcmdup [options] [sample_file]
.embigbox
.*
:CMT.This is from gui\wrc.gml and is in the Resource Compiler Help
:CMT.Topic: "The Open Watcom Resource Compiler"
.np
The &wrcname command line syntax is the following.
.ix '&wrccmdup' 'command line format'
.ix 'command line format' '&wrccmdup'
.ix '&wrcname' 'command line format'
.mbigbox
&wrccmdup {options} input-filename {options} [output-filename] {options}
.embigbox
.*
:CMT.This is from lg\ldalias.gml and is in the Linker Guide
:CMT.Topic: "The ALIAS Directive"
.np
The "ALIAS" directive is used to specify an equivalent name for a symbol name.
The format of the "ALIAS" directive (short form "A") is as follows.
.mbigbox
    ALIAS alias_name=symbol_name{, alias_name=symbol_name}
.embigbox
.*
:CMT.This is from lg\ldanonym.gml and is in the Linker Guide
:CMT.Topic: "The ANONYMOUSEXPORT Directive"
.np
The format of the "ANONYMOUSEXPORT" directive (short form "ANON") is
as follows.
.mbigbox
     ANONYMOUSEXPORT export{,export}
         or
     ANONYMOUSEXPORT =lbc_file

     export ::= entry_name[.ordinal][=internal_name]
.embigbox
.*
:CMT.This is from lg\ldautose.gml and is in the Linker Guide
:CMT.Topic: "The AUTOSECTION Directive"
.np
The format of the "AUTOSECTION" directive (short form "AUTOS") is as
follows.
.mbigbox
    AUTOSECTION [INTO ovl_file]
.embigbox
.*
:CMT.This is from lg\ldbegin.gml and is in the Linker Guide
:CMT.Topic: "The BEGIN Directive"
.np
The format of the "BEGIN" directive (short form "B") is as follows.
.mbigbox
    BEGIN
.embigbox
.np
The format of the "END" directive (short form "E") is as follows.
.mbigbox
    END
.embigbox
.*
:CMT.This is from lg\ldcmt.gml and is in the Linker Guide
:CMT.Topic: "The # Directive"
.np
.ix 'directives' 'comment'
.ix 'comment (#) directive'
The "#" directive is used to mark the start of a comment.
All text from the "#" character to the end of the line is considered a comment.
The format of the "#" directive is as follows.
.mbigbox
    # comment
.embigbox
.*
:CMT.This is from lg\ldcommit.gml and is in the Linker Guide
:CMT.Topic: "The COMMIT Directive"
.np
The format of the "COMMIT" directive (short form "COM") is as follows.
.mbigbox
     COMMIT mem_type

     mem_type ::= STACK=n | HEAP=n
.embigbox
.*
:CMT.This is from lg\lddebug.gml and is in the Linker Guide
:CMT.Topic: "The DEBUG Directive"
.np
The "DEBUG" directive is used to tell the &lnkname to generate
debugging information in the executable file.
This extra information in the executable file is used by the
&dbgname..
The format of the "DEBUG" directive (short form "D") is as follows.
.mbigbox
    DEBUG dbtype [dblist] |
    DEBUG [dblist]
~b
    dbtype ::= DWARF | WATCOM | CODEVIEW | NOVELL
    dblist ::= [db_option{,db_option}]
    db_option ::= LINES | TYPES | LOCALS | ALL
~b
DEBUG NOVELL only
    db_option ::= ONLYEXPORTS | REFERENCED
.embigbox
.remark
Except in rare cases, the most appropriate use of the "DEBUG" directive is
specifying "DEBUG ALL" (short form "D A") prior to any "FILE" or "LIBRARY"
directives. This will cause the &lnkname to emit all available debugging
information in the default format.
.eremark
.remark
The position of the "DEBUG" directive is important.
The level of debugging information specified in a "DEBUG" directive
only applies to object files and libraries that appear in
.us subsequent
"FILE" or "LIBRARY" directives.
For example, if "DEBUG WATCOM ALL" was the only "DEBUG" directive
specified and was also the last linker directive, no debugging
information would appear in the executable file.
.eremark
.*
:CMT.This is from lg\lddebug.gml and is in the Linker Guide
:CMT.Topic: "Line Numbering Information - DEBUG WATCOM LINES"
.remark
All modules for which line numbering information is requested must
have been compiled with the "d1" or "d2" option.
.eremark
.*
:CMT.This is from lg\lddebug.gml and is in the Linker Guide
:CMT.Topic: "Local Symbol Information - DEBUG WATCOM LOCALS"
.remark
All modules for which local symbol information is requested must have
been compiled with the "d2" option.
.eremark
.*
:CMT.This is from lg\lddebug.gml and is in the Linker Guide
:CMT.Topic: "Typing Information - DEBUG WATCOM TYPES"
.remark
All modules for which typing information is requested must have been
compiled with the "d2" option.
.eremark
.*
:CMT.This is from lg\lddebug.gml and is in the Linker Guide
:CMT.Topic: "Using DEBUG Directives"
.remark
A "DEBUG WATCOM" directive with no option suppresses the processing of
line numbering, local symbol and typing information for all subsequent
object modules.
.eremark
.remark
If you want all classes of debugging information for all files to
appear in the executable file you must specify "DEBUG WATCOM ALL"
before any "FILE" and "LIBRARY" directives.
.eremark
.*
:CMT.This is from lg\lddisabl.gml and is in the Linker Guide
:CMT.Topic: "The DISABLE Directive"
.np
The format of the "DISABLE" directive (short form "DISA") is as follows.
:cmt.The IN 1 was added to allow .mbigbox to have a valid left position
.in 1
.mbigbox
    DISABLE msg_num{, msg_num}
.embigbox
.*
:CMT.This is from lg\ldend.gml and is in the Linker Guide
:CMT.Topic: "The END Directive"
.np
The format of the "BEGIN" directive (short form "B") is as follows.
.mbigbox
    BEGIN
.embigbox
.np
The format of the "END" directive (short form "E") is as follows.
.mbigbox
    END
.embigbox
.*
:CMT.This is from lg\ldendlin.gml and is in the Linker Guide
:CMT.Topic: "The ENDLINK Directive"
.np
The "ENDLINK" directive is used to indicate the end of a new set of
linker commands that are to be processed after the current set of
commands has been processed.
The format of the "ENDLINK" directive (short form "ENDL") is as
follows.
.mbigbox
    ENDLINK
.embigbox
.*
:CMT.This is from lg\ldexport.gml and is in the Linker Guide
:CMT.Topic: "EXPORT - OS/2, Win16, Win32 only"
.np
The format of the "EXPORT" directive (short form "EXP") is as follows.
.mbigbox
     EXPORT export{,export}
         or
     EXPORT =lbc_file

OS/2 only:
     export ::= entry_name[.ordinal][=internal_name]
                [PRIVATE] [RESIDENT] [iopl_bytes]

Win16, Win32 only:
     export ::= entry_name[.ordinal][=internal_name]
                [PRIVATE] [RESIDENT]
.embigbox
.*
:CMT.This is from lg\ldexport.gml and is in the Linker Guide
:CMT.Topic: "EXPORT - ELF only"
.np
The "EXPORT" directive is used to tell the &lnkname which symbols are
available for import by other executables.
The format of the "EXPORT" directive (short form "EXP") is as follows.
.mbigbox
    EXPORT entry_name{,entry_name}
.embigbox
.*
:CMT.This is from lg\ldexport.gml and is in the Linker Guide
:CMT.Topic: "EXPORT - Netware only"
.np
The "EXPORT" directive is used to tell the &lnkname which symbols are
available for import by other NLMs.
The format of the "EXPORT" directive (short form "EXP") is as follows.
.mbigbox
    EXPORT entry_name{,entry_name}
.embigbox
.*
:CMT.This is from lg\ldfile.gml and is in the Linker Guide
:CMT.Topic: "The FILE Directive"
.np
The "FILE" directive is used to specify the object files and library
modules that the &lnkname is to process.
The format of the "FILE" directive (short form "F") is as follows.
.mbigbox
    FILE obj_spec{,obj_spec}

    obj_spec ::= obj_file[(obj_module)]
                       | library_file[(obj_module)]
.embigbox
.*
:CMT.This is from lg\ldfixedl.gml and is in the Linker Guide
:CMT.Topic: "The FIXEDLIB Directive"
.np
The "FIXEDLIB" directive can be used to explicitly place the modules
from a library file in the overlay section in which the "FIXEDLIB"
directive appears.
The format of the "FIXEDLIB" directive (short form "FIX" ) is as
follows.
.mbigbox
    FIXEDLIB library_file{,library_file}
.embigbox
.*
:CMT.This is from lg\ldforcev.gml and is in the Linker Guide
:CMT.Topic: "The FORCEVECTOR Directive"
.np
The "FORCEVECTOR" directive forces the &lnkname to generate an overlay
vector for the specified symbols.
The format of the "FORCEVECTOR" directive (short form "FORCEVE") is as
follows.
.mbigbox
    FORCEVECTOR symbol_name{,symbol_name}
.embigbox
.*
:CMT.This is from lg\ldformat.gml and is in the Linker Guide
:CMT.Topic: "The FORMAT Directive"
.if &'lower(&syspdev) = ps .pa
.np
The "FORMAT" directive is used to specify the format of the executable
file that the &lnkname is to generate.
The format of the "FORMAT" directive (short form "FORM") is as follows.
.mbigbox
    FORMAT form

    form ::= DOS [COM]
                | ZDOS [SYS | HWD | FSD]
                | RAW [BIN | HEX]
                | WINDOWS [win_dll] [MEMORY] [FONT]
                | WINDOWS VXD [DYNAMIC]
                | WINDOWS NT [TNT | RDOS] [dll_attrs]
                | OS2 [os2_type] [dll_attrs | os2_attrs]
                | PHARLAP [EXTENDED | REX | SEGMENTED]
                | NOVELL [NLM | LAN | DSK | NAM | 'number'] 'description'
                | QNX [FLAT]
                | ELF [DLL]

    win_dll ::= DLL [INITGLOBAL | INITINSTANCE]

    dll_attrs ::= DLL [INITGLOBAL | INITINSTANCE]
                      [TERMINSTANCE | TERMGLOBAL]

    os2_type ::= FLAT | LE | LX

    os2_attrs ::= PM | PMCOMPATIBLE | FULLSCREEN
                        | PHYSDEVICE | VIRTDEVICE
.embigbox
.*
:CMT.This is from lg\ldimport.gml and is in the Linker Guide
:CMT.Topic: "IMPORT - OS/2, Win16, Win32 only"
.np
The "IMPORT" directive describes a function that belongs to a Dynamic
Link Library.
The format of the "IMPORT" directive (short form "IMP") is as follows.
.mbigbox
     IMPORT import{,import}

     import ::= internal_name module_name[.entry_name | ordinal]
.embigbox
.*
:CMT.This is from lg\ldimport.gml and is in the Linker Guide
:CMT.Topic: "IMPORT - ELF only"
.np
The "IMPORT" directive is used to tell the &lnkname what symbols are
defined externally in other executables.
The format of the "IMPORT" directive (short form "IMP") is as follows.
.mbigbox
    IMPORT external_name{,external_name}
.embigbox
.*
:CMT.This is from lg\ldimport.gml and is in the Linker Guide
:CMT.Topic: "IMPORT - Netware only"
.np
The "IMPORT" directive is used to tell the &lnkname what symbols are
defined externally in other NLMs.
The format of the "IMPORT" directive (short form "IMP") is as follows.
.mbigbox
    IMPORT external_name{,external_name}
.embigbox
.*
:CMT.This is from lg\ldinclud.gml and is in the Linker Guide
:CMT.Topic: "The @ Directive"
.np
.ix 'directives' 'include'
.ix 'include directive'
The "@" directive instructs the &lnkname to process directives from an
alternate source.
The format of the "@" directive is as follows.
.mbigbox
    @directive_var
        or
    @directive_file
.embigbox
.*
:CMT.This is from lg\ldlangua.gml and is in the Linker Guide
:CMT.Topic: "The LANGUAGE Directive"
.np
The "LANGUAGE" directive is used to specify the language in which
strings in the &lnkname directives are specified.
The format of the "LANGUAGE" directive (short form "LANG") is as
follows.
.mbigbox
    LANGUAGE lang

    lang ::= JAPANESE | CHINESE | KOREAN
.embigbox
.*
:CMT.This is from lg\ldlibfil.gml and is in the Linker Guide
:CMT.Topic: "The LIBFILE Directive"
.np
The "LIBFILE" directive is used to specify the object files that the
&lnkname is to process.
The format of the "LIBFILE" directive (short form "LIBF") is as
follows.
.mbigbox
    LIBFILE obj_spec{,obj_spec}

    obj_spec ::= obj_file | library_file
.embigbox
.*
:CMT.This is from lg\ldlibpat.gml and is in the Linker Guide
:CMT.Topic: "The LIBPATH Directive"
.np
The "LIBPATH" directive is used to specify the directories that are to
be searched for library files appearing in subsequent "LIBRARY"
directives and object files appearing in subsequent "LIBFILE"
directives.
The format of the "LIBPATH" directive (short form "LIBP") is as
follows.
.mbigbox
    LIBPATH [path_name{&ps.path_name}]
.embigbox
.*
:CMT.This is from lg\ldlibrar.gml and is in the Linker Guide
:CMT.Topic: "The LIBRARY Directive"
.np
The "LIBRARY" directive is used to specify the library files to be
searched when unresolved symbols remain after processing all specified
input object files.
The format of the "LIBRARY" directive (short form "L") is as follows.
.mbigbox
    LIBRARY library_file{,library_file}
.embigbox
.*
:CMT.This is from lg\ldmodfil.gml and is in the Linker Guide
:CMT.Topic: "The MODFILE Directive"
.np
The "MODFILE" directive instructs the linker that only the specified
object files have changed.
The format of the "MODFILE" directive (short form "MODF") is as follows.
.mbigbox
    MODFILE obj_file{,obj_file}
.embigbox
.*
:CMT.This is from lg\ldmodtrc.gml and is in the Linker Guide
:CMT.Topic: "The MODTRACE Directive"
.np
The "MODTRACE" directive instructs the &lnkname to print a list of all
modules that reference the symbols defined in the specified modules.
The format of the "MODTRACE" directive (short form "MODT") is as
follows.
.mbigbox
    MODTRACE  module_name{,module_name}
.embigbox
.*
:CMT.This is from lg\ldmodule.gml and is in the Linker Guide
:CMT.Topic: "The MODULE Directive"
.np
The "MODULE" directive is used to specify the DLLs or NLMs to be
loaded before this executable is loaded.
The format of the "MODULE" directive (short form "MODU") is as
follows.
.mbigbox
    MODULE module_name{,module_name}
.embigbox
.warn
Versions 3.0 and 3.1 of the NetWare operating system do not
support the automatic loading of modules specified in the "MODULE"
directive.
You must load them manually.
.ewarn
.*
:CMT.This is from lg\ldname.gml and is in the Linker Guide
:CMT.Topic: "The NAME Directive"
.np
The "NAME" directive is used to provide a name for the executable file
generated by the &lnkname..
The format of the "NAME" directive (short form "N") is as follows.
.mbigbox
    NAME exe_file
.embigbox
.*
:CMT.This is from lg\ldnewseg.gml and is in the Linker Guide
:CMT.Topic: "The NEWSEGMENT Directive"
.np
The "NEWSEGMENT" directive provides an alternate method of grouping
code segments into physical segments.
By placing this directive after a sequence of "FILE" directives, all
code segments appearing in object modules specified by the sequence of
"FILE" directives will be packed into a physical segment.
Note that the size of a physical segment may vary in size.
The format of the "NEWSEGMENT" directive (short form "NEW") is as
follows.
.mbigbox
    NEWSEGMENT
.embigbox
.*
:CMT.This is from lg\ldnovect.gml and is in the Linker Guide
:CMT.Topic: "The NOVECTOR Directive"
.np
The "NOVECTOR" directive forces the &lnkname to not generate an
overlay vector for the specified symbols.
The format of the "NOVECTOR" directive (short form "NOV") is as
follows.
.mbigbox
    NOVECTOR symbol_name{,symbol_name}
.embigbox
.*
:CMT.This is from lg\ldoption.gml and is in the Linker Guide
:CMT.Topic: "The OPTION Directive"
.np
The "OPTION" directive is used to specify options to the &lnkname..
The format of the "OPTION" directive (short form "OP") is as follows.
.mbigbox
    OPTION option{,option}
.embigbox
.*
:CMT.This is from lg\ldoptlib.gml and is in the Linker Guide
:CMT.Topic: "The OPTLIB Directive"
.np
The "OPTLIB" directive is used to specify the library files to be
searched when unresolved symbols remain after processing all specified
input object files.
The format of the "OPTLIB" directive (no short form) is as follows.
.mbigbox
    OPTLIB library_file{,library_file}
.embigbox
.*
:CMT.This is from lg\ldorder.gml and is in the Linker Guide
:CMT.Topic: "The ORDER Directive"
.np
The "ORDER" directive is used to specify the order in which classes are
placed into the output image, and the order in which segments are linked
within a class. The directive can optionally also specify the starting
address of a class or segment, control whether the segment appears in the
output image, and facilitate copying of data from one segment to another.
The "ORDER" Directive is primarily intended for embedded (ROMable) targets
that do not run under an operating system, or for other special purpose
applications. The format of the "ORDER" directive (short form "ORD")
is as follows.
.mbigbox
    ORDER {CLNAME class_name [class_options]}+
~b
    class_options ::= [SEGADDR=n][OFFSET=n][copy_option][NOEMIT]{seglist}
    copy_option ::= [COPY source_class_name]
    seglist := {SEGMENT seg_name [SEGADDR=n][OFFSET=n][NOEMIT]}+
.embigbox
.*
:CMT.This is from lg\ldoutput.gml and is in the Linker Guide
:CMT.Topic: "The OUTPUT Directive"
.np
The "OUTPUT" directive overrides the normal operating system specific
executable format and creates either a raw binary image or an Intel Hex file.
 The format of the "OUTPUT" directive (short form "OUT") is as follows.
.mbigbox
    OUTPUT RAW|HEX [OFFSET=n][HSHIFT=n][STARTREC]
.embigbox
.*
:CMT.This is from lg\ldoverla.gml and is in the Linker Guide
:CMT.Topic: "The OVERLAY Directive"
.np
The "OVERLAY" directive allows you to specify the class of segments
which are to be overlayed.
The format of the "OVERLAY" directive (short form "OV") is as follows.
.mbigbox
    OVERLAY class{,class}
.embigbox
.warn
Care must be taken when overlaying data.
.ix 'overlaying data'
If a routine modifies data in an overlayed data segment,
it should not assume it contains that value if it is invoked again.
The data may have been overwritten by another overlay.
.ewarn
.*
:CMT.This is from lg\ldpath.gml and is in the Linker Guide
:CMT.Topic: "The PATH Directive"
.np
The "PATH" directive is used to specify the directories that are to be
searched for object files appearing in subsequent "FILE" directives.
When the "PATH" directive is specified, the current directory will no
longer be searched unless it appears in the "PATH" directive.
The format of the "PATH" directive (short form "P") is as follows.
:cmt.The IN 1 was added to allow .mbigbox to have a valid left position
.in 1
.mbigbox
    PATH path_name{&ps.path_name}
.embigbox
.*
:CMT.This is from lg\ldrefere.gml and is in the Linker Guide
:CMT.Topic: "The REFERENCE Directive"
.np
The format of the "REFERENCE" directive (short form "REF") is as
follows.
.mbigbox
    REFERENCE symbol_name{, symbol_name}
.embigbox
.*
:CMT.This is from lg\ldresour.gml and is in the Linker Guide
:CMT.Topic: "The RESOURCE Directive"
.np
The "RESOURCE" directive is used to specify
resource files to add to the executable file being generated.
.*
The format of the "RESOURCE" directive (short form "RES") is as follows.
.mbigbox
    RESOURCE resource_file{,resource_file}
.embigbox
.*
:CMT.This is from lg\ldresour.gml and is in the Linker Guide
:CMT.Topic: "The RESOURCE Directive"
.np
The "RESOURCE" directive is used to specify
resource files to add to the executable file being generated.
.*
The format of the "RESOURCE" directive (short form "RES") is as follows.
.mbigbox
    RESOURCE resource_file{,resource_file}
.embigbox
.*
:CMT.This is from lg\ldsectio.gml and is in the Linker Guide
:CMT.Topic: "The SECTION Directive"
.np
The "SECTION" directive is used to define the start of an overlay.
All object files in subsequent "FILE" directives, up to the next
"SECTION" or "END" directive, belong to that overlay.
The format of the "SECTION" directive (short form "S") is as follows.
.mbigbox
    SECTION [INTO ovl_file]
.embigbox
.*
:CMT.This is from lg\ldruntim.gml and is in the Linker Guide
:CMT.Topic: "RUNTIME - Win32 only"
.if &'lower(&syspdev) = ps .pa
.np
The "RUNTIME" directive specifies the environment under which the
application will run.
The format of the "RUNTIME" directive (short form "RU") is as follows.
.mbigbox
     RUNTIME  env[=major[.minor]]

     env ::= NATIVE | WINDOWS | CONSOLE | POSIX | OS2 | DOSSTYLE
.embigbox
.*
:CMT.This is from lg\ldruntim.gml and is in the Linker Guide
:CMT.Topic: "RUNTIME - PharLap only"
.np
The "RUNTIME" directive describes information that is used by
386|DOS-Extender to setup the environment for execution of the
program.
The format of the "RUNTIME" directive (short form "RU") is as follows.
.mbigbox
    RUNTIME run_option{,run_option}

    run_option ::= MINREAL=n | MAXREAL=n | CALLBUFS=n | MINIBuf=n
                           | MAXIBUF=n | NISTACK=n | ISTKSIZE=n
                           | REALBREAK=offset | PRIVILEGED | UNPRIVILEGED

    offset ::= n | symbol_name
.embigbox
.*
:CMT.This is from lg\ldruntim.gml and is in the Linker Guide
:CMT.Topic: "RUNTIME - ELF only"
.np
The "RUNTIME" directive specifies the Application Binary Interface (ABI)
type and version under which the application will run.
The format of the "RUNTIME" directive (short form "RU") is as follows.
.mbigbox
     RUNTIME  ABIVER[=abinum.abiversion] | abispec

     abispec ::= abiname[=abiversion]

     abiname ::= SVR4 | LINUX | FREEBSD | NETBSD | SOLARIS
.embigbox
.*
:CMT.This is from lg\ldsegmen.gml and is in the Linker Guide
:CMT.Topic: "The SEGMENT Directive"
.if &'lower(&syspdev) = ps .pa
.np
The "SEGMENT" directive is used to describe the attributes of code and
data segments.
The format of the "SEGMENT" directive (short form "SEG") is as
follows.
.np
:CMT. Force a page break; the table is too big and won't get properly
:CMT. moved to the next page automatically.
.mbigbox
    SEGMENT seg_desc{,seg_desc}

    seg_desc ::= seg_id {seg_attrs}+

    seg_id ::= 'seg_name' | CLASS 'class_name' | TYPE [CODE | DATA]
OS/2:
    seg_attrs ::= PRELOAD | LOADONCALL
                        | IOPL | NOIOPL
                        | EXECUTEONLY | EXECUTEREAD
                        | READONLY | READWRITE
                        | SHARED | NONSHARED
                        | CONFORMING | NONCONFORMING
                        | PERMANENT | NONPERMANENT
                        | INVALID | RESIDENT
                        | CONTIGUOUS | DYNAMIC
Win32:
    seg_attrs ::= PAGEABLE | NONPAGEABLE
                        | SHARED | NONSHARED

Win16:
    seg_attrs ::= PRELOAD | LOADONCALL
                        | EXECUTEONLY | EXECUTEREAD
                        | READONLY | READWRITE
                        | SHARED | NONSHARED
                        | MOVEABLE | FIXED
                        | DISCARDABLE

VxD:
    seg_attrs ::= PRELOAD | LOADONCALL
                        | IOPL | NOIOPL
                        | SHARED | NONSHARED
                        | DISCARDABLE | NONDISCARDABLE
                        | CONFORMING | NONCONFORMING
                        | RESIDENT

QNX:
    seg_attrs ::= EXECUTEONLY | EXECUTEREAD
                        | READONLY | READWRITE
.embigbox
.remark
Attributes specified for segments identified by a segment name
override attributes specified for segments identified by a class name.
.eremark
.*
:CMT.This is from lg\ldsort.gml and is in the Linker Guide
:CMT.Topic: "The SORT Directive"
.np
The "SORT" directive is used to sort the symbols in the "Memory Map"
section of the map file.
By default, symbols are listed on a per module basis in the order the
modules were encountered by the linker.
That is, a module header is displayed followed by the symbols defined
by the module.
.np
The format of the "SORT" directive (short form "SO") is as follows.
.mbigbox
    SORT [GLOBAL] [ALPHABETICAL]
.embigbox
.*
:CMT.This is from lg\ldstartl.gml and is in the Linker Guide
:CMT.Topic: "The STARTLINK Directive"
.np
The "STARTLINK" directive is used to indicate the start of a new set
of linker commands that are to be processed after the current set of
commands has been processed.
The format of the "STARTLINK" directive (short form "STARTL") is as
follows.
.mbigbox
    STARTLINK
.embigbox
.*
:CMT.This is from lg\ldsymtrc.gml and is in the Linker Guide
:CMT.Topic: "The SYMTRACE Directive"
.np
The "SYMTRACE" directive instructs the &lnkname to print a list of all
modules that reference the specified symbols.
The format of the "SYMTRACE" directive (short form "SYMT") is as
follows.
.mbigbox
    SYMTRACE  symbol_name{,symbol_name}
.embigbox
.*
:CMT.This is from lg\ldsystem.gml and is in the Linker Guide
:CMT.Topic: "The SYSTEM Directive"
.np
The first form of the "SYSTEM" directive (short form "SYS") is called a
system definition directive.
It allows you to associate a set of linker directives with a specified
name called the
.us system name.
.ix 'system name'
This set of linker directives is called a system definition block.
The format of a system definition directive is as follows.
.mbigbox
    SYSTEM BEGIN system_name {directive} END
.embigbox
.np
The second form of the "SYSTEM" directive is called a system deletion
directive.
It allows you to remove the association of a set of linker directives
with a
.us system name.
The format of a system deletion directive is as follows.
.mbigbox
    SYSTEM DELETE system_name
.embigbox
.np
The third form of the "SYSTEM" directive is as follows.
.mbigbox
    SYSTEM system_name
.embigbox
.*
:CMT.This is from lg\ldvector.gml and is in the Linker Guide
:CMT.Topic: "The VECTOR Directive"
.np
The format of the "VECTOR" directive (short form "VE") is as follows.
.mbigbox
    VECTOR symbol_name{,symbol_name}
.embigbox
.*
:CMT.This is from lg\opalignm.gml and is in the Linker Guide
:CMT.Topic: "The ALIGNMENT Option"
.np
The "ALIGNMENT" option specifies the alignment for segments in the
executable file.
The format of the "ALIGNMENT" option (short form "A") is as follows.
.mbigbox
    OPTION ALIGNMENT=n
.embigbox
.*
:CMT.This is from lg\oparea.gml and is in the Linker Guide
:CMT.Topic: "The AREA Option"
.np
The "AREA" option can be used to set the size of the memory pool in
which overlay sections are loaded by the dynamic overlay manager.
The format of the "AREA" option (short form "AR") is as follows.
.mbigbox
    OPTION AREA=n
.embigbox
.*
:CMT.This is from lg\opartifi.gml and is in the Linker Guide
:CMT.Topic: "The ARTIFICIAL Option"
.np
The format of the "ARTIFICIAL" option (short form "ART") is as follows.
.mbigbox
    OPTION ARTIFICIAL
.embigbox
.*
:CMT.This is from lg\opautoun.gml and is in the Linker Guide
:CMT.Topic: "The AUTOUNLOAD Option"
.np
The format of the "AUTOUNLOAD" option (short form "AUTOUN") is as
follows.
.if &'lower(&syspdev) = ps .pa
.mbigbox
    OPTION AUTOUNLOAD
.embigbox
.*
:CMT.This is from lg\opcache.gml and is in the Linker Guide
:CMT.Topic: "The CACHE Option"
.np
The format of the "CACHE" option (short form "CAC") is as follows.
.mbigbox
    OPTION CACHE
.embigbox
.np
The format of the "NOCACHE" option (short form "NOCAC") is as follows.
.mbigbox
    OPTION NOCACHE
.embigbox
.*
:CMT.This is from lg\opcaseex.gml and is in the Linker Guide
:CMT.Topic: "The CASEEXACT Option"
.np
The "CASEEXACT" option tells the &lnkname to respect case when
resolving references to global symbols.
That is, "ScanName" and "SCANNAME" represent two different symbols.
This is the default because the most commonly used languages
(C, C++, FORTRAN) are case sensitive.
The format of the "CASEEXACT" option (short form "C") is as follows.
.mbigbox
    OPTION CASEEXACT
.embigbox
.np
It is possible to override the default by using the "NOCASEEXACT" option.
The "NOCASEEXACT" option turns off case-sensitive linking.
The format of the "NOCASEEXACT" option (short form "NOCASE") is as
follows.
.mbigbox
    OPTION NOCASEEXACT
.embigbox
.*
:CMT.This is from lg\opcheck.gml and is in the Linker Guide
:CMT.Topic: "The CHECK Option"
.np
The format of the "CHECK" option (short form "CH") is as follows.
.mbigbox
    OPTION CHECK=symbol_name
.embigbox
.*
:CMT.This is from lg\opcopyri.gml and is in the Linker Guide
:CMT.Topic: "The COPYRIGHT Option"
.np
The "COPYRIGHT" option specifies copyright information that is placed
in the executable file.
The format of the "COPYRIGHT" option (short form "COPYR") is as
follows.
.mbigbox
    OPTION COPYRIGHT 'string'
.embigbox
.*
:CMT.This is from lg\opcsum.gml and is in the Linker Guide
:CMT.Topic: "The CHECKSUM Option"
.np
The "CHECKSUM" option specifies that the linker should create an MS-CRC32
checksum for the current image. This is primarily used for DLL's and device
drivers but can be applied to any PE format images.
The format of the "CHECKSUM" option (no short form) is as follows.
.mbigbox
    OPTION CHECKSUM
.embigbox
.*
:CMT.This is from lg\opcustom.gml and is in the Linker Guide
:CMT.Topic: "The CUSTOM Option"
.np
The format of the "CUSTOM" option (short form "CUST") is as follows.
.mbigbox
    OPTION CUSTOM=file_name
.embigbox
.*
:CMT.This is from lg\opcvpack.gml and is in the Linker Guide
:CMT.Topic: "The CVPACK Option"
.np
The format of the "CVPACK" option (short form "CVP") is as follows.
.mbigbox
    OPTION CVPACK
.embigbox
.*
:CMT.This is from lg\opdescri.gml and is in the Linker Guide
:CMT.Topic: "The DESCRIPTION Option"
.np
The "DESCRIPTION" option inserts the specified text into the
application or Dynamic Link Library.
This is useful if you wish to embed copyright information into an
application or Dynamic Link Library.
The format of the "DESCRIPTION" option (short form "DE") is as
follows.
.mbigbox
    OPTION DESCRIPTION 'string'
.embigbox
.*
:CMT.This is from lg\opdistri.gml and is in the Linker Guide
:CMT.Topic: "The DISTRIBUTE Option"
.np
The "DISTRIBUTE" option specifies that object modules extracted from
library files are to be distributed throughout the overlay structure.
The format of the "DISTRIBUTE" option (short form "DIS") is as
follows.
.mbigbox
    OPTION DISTRIBUTE
.embigbox
.*
:CMT.This is from lg\opdosseg.gml and is in the Linker Guide
:CMT.Topic: "The DOSSEG Option"
.np
The "DOSSEG" option tells the &lnkname to order segments in a special
way.
The format of the "DOSSEG" option (short form "D") is as follows.
.mbigbox
    OPTION DOSSEG
.embigbox
.*
:CMT.This is from lg\opdynami.gml and is in the Linker Guide
:CMT.Topic: "The DYNAMIC Option"
.np
The "DYNAMIC" option tells the &lnkname to use the dynamic overlay
manager.
The format of the "DYNAMIC" option (short form "DYN") is as follows.
.mbigbox
    OPTION DYNAMIC
.embigbox
.*
:CMT.This is from lg\opelimin.gml and is in the Linker Guide
:CMT.Topic: "The ELIMINATE Option"
.np
The format of the "ELIMINATE" option (short form "EL") is as follows.
.mbigbox
    OPTION ELIMINATE
.embigbox
.*
:CMT.This is from lg\opexit.gml and is in the Linker Guide
:CMT.Topic: "The EXIT Option"
.np
The format of the "EXIT" option (short form "EX") is as follows.
.mbigbox
    OPTION EXIT=symbol_name
.embigbox
.*
:CMT.This is from lg\opfarcal.gml and is in the Linker Guide
:CMT.Topic: "The FARCALLS Option"
.np
The "FARCALLS" option tells the &lnkname to optimize Far Calls.
This is the default setting for &lnkname.
The format of the "FARCALLS" option (short form "FAR") is as follows.
.mbigbox
    OPTION FARCALLS
.embigbox
.np
The "NOFARCALLS" option turns off Far Calls optimization.
The format of the "NOFARCALLS" option (short form "NOFAR") is as
follows.
.mbigbox
    OPTION NOFARCALLS
.embigbox
.*
:CMT.This is from lg\opfilchr.gml and is in the Linker Guide
:CMT.Topic: "The FILLCHAR Option"
.np
The "FILLCHAR" option (short form "FILL") specifies the byte value used to
fill gaps in the output image.
.mbigbox
    OPTION FILLCHAR=n
.embigbox
.*
:CMT.This is from lg\opfullh.gml and is in the Linker Guide
:CMT.Topic: "The FULLHEADER Option"
.np
This option is valid for 16-bit DOS "EXE" files.
By default, the &lnkname writes a "MZ" executable header which is just large
enough to contain all necessary data. 
The "FULLHEADER" option may be used to force the header size to 64 bytes, plus
the size of relocation records.
The format of the "FULLHEADER" option (short form "FULLH") is as
follows.
.mbigbox
    OPTION FULLHEADER
.embigbox
.*
:CMT.This is from lg\opheap.gml and is in the Linker Guide
:CMT.Topic: "The HEAPSIZE Option"
.np
The "HEAPSIZE" option specifies the size of the heap required by the
application.
The format of the "HEAPSIZE" option (short form "H") is as follows.
.mbigbox
    OPTION HEAPSIZE=n
.embigbox
.*
:CMT.This is from lg\ophelp.gml and is in the Linker Guide
:CMT.Topic: "The HELP Option"
.np
The format of the "HELP" option (short form "HE") is as follows.
.mbigbox
    OPTION HELP=help_file
.embigbox
.*
:CMT.This is from lg\ophshift.gml and is in the Linker Guide
:CMT.Topic: "The HSHIFT Option"
.np
The "HSHIFT" defines the relationship between segment and linear address
in a segmented executable. The format of the "HSHIFT" option is as follows.
.mbigbox
    OPTION HSHIFT=n
.embigbox
.*
:CMT.This is from lg\opimpfil.gml and is in the Linker Guide
:CMT.Topic: "The IMPFILE Option"
.np
The format of the "IMPFILE" option (short form "IMPF") is as follows.
.mbigbox
    OPTION IMPFILE[=imp_file]
.embigbox
.*
:CMT.This is from lg\opimplib.gml and is in the Linker Guide
:CMT.Topic: "The IMPLIB Option"
.np
.ix 'import library'
The "IMPLIB" option requests the linker to produce an import library
that corresponds to the DLL that is being generated.
The format of the "IMPLIB" option (short form "IMPL") is as follows.
.mbigbox
    OPTION IMPLIB[=imp_lib]
.embigbox
.remark
At present, the linker spawns the &libname to create the import
library file.
.eremark
.*
:CMT.This is from lg\opincrem.gml and is in the Linker Guide
:CMT.Topic: "The INCREMENTAL Option"
.np
The format of the "INCREMENTAL" option (short form "INC") is as follows.
:cmt.The IN 1 was added to allow .mbigbox to have a valid left position
.in 1
.mbigbox
    OPTION INCREMENTAL[=inc_file_name]
.embigbox
.remark
Only DWARF debugging information is supported with incremental
linking.
.eremark
.*
:CMT.This is from lg\opintern.gml and is in the Linker Guide
:CMT.Topic: "The INTERNALRELOCS Option"
.np
The format of the "INTERNALRELOCS" option (short form "INT") is as
follows.
.mbigbox
    OPTION INTERNALRELOCS
.embigbox
.*
:CMT.This is from lg\oplinear.gml and is in the Linker Guide
:CMT.Topic: "The LINEARRELOCS Option"
.np
The format of the "LINEARRELOCS" option (short form "LI") is as
follows.
.mbigbox
    OPTION LINEARRELOCS
.embigbox
.*
:CMT.This is from lg\oplnkver.gml and is in the Linker Guide
:CMT.Topic: "The LINKVERSION Option"
.np
The "LINKVERSION" option specifies that the linker should apply the given major
and minor version numbers to the PE format image header. If a version number is
not specified, then the built-in value of 2.18 is used.
The format of the "LINKVERSION" option (short form "LINKV") is as follows.
.mbigbox
    OPTION LINKVERSION = major[.minor]
.embigbox
.*
:CMT.This is from lg\oplongli.gml and is in the Linker Guide
:CMT.Topic: "The LONGLIVED Option"
.np
The format of the "LONGLIVED" option (short form "LO") is as follows.
.mbigbox
    OPTION LONGLIVED
.embigbox
.*
:CMT.This is from lg\opmangle.gml and is in the Linker Guide
:CMT.Topic: "The LONGLIVED Option"
.np
The format of the "MANGLEDNAMES" option (short form "MANG") is as follows.
.mbigbox
    OPTION MANGLEDNAMES
.embigbox
.*
:CMT.This is from lg\opmanyau.gml and is in the Linker Guide
:CMT.Topic: "The MANYAUTODATA Option"
.np
The "MANYAUTODATA" option specifies that a copy of the automatic data
segment (default data segment defined by the group "DGROUP"),
for the program module or Dynamic Link Library (DLL) being created,
is made for each instance.
The format of the "MANYAUTODATA" option (short form "MANY") is as
follows.
.mbigbox
    OPTION MANYAUTODATA
.embigbox
.*
:CMT.This is from lg\opmap.gml and is in the Linker Guide
:CMT.Topic: "The MAP Option"
.np
The "MAP" option controls the generation of a map file.
The format of the "MAP" option (short form "M") is as follows.
.mbigbox
    OPTION MAP[=map_file]
.embigbox
.*
:CMT.This is from lg\opmaxdat.gml and is in the Linker Guide
:CMT.Topic: "The MAXDATA Option"
.np
The format of the "MAXDATA" option (short form "MAXD") is as follows.
.mbigbox
    OPTION MAXDATA=n
.embigbox
.*
:CMT.This is from lg\opmaxerr.gml and is in the Linker Guide
:CMT.Topic: "The MAXERRORS Option"
.np
The format of the "MAXERRORS" option (short form "MAXE") is as
follows.
.mbigbox
    OPTION MAXERRORS=n
.embigbox
.*
:CMT.This is from lg\opmessag.gml and is in the Linker Guide
:CMT.Topic: "The MESSAGES Option"
.np
The format of the "MESSAGES" option (short form "MES") is as follows.
.mbigbox
    OPTION MESSAGES=msg_file
.embigbox
.*
:CMT.This is from lg\opmindat.gml and is in the Linker Guide
:CMT.Topic: "The MINDATA Option"
.np
The format of the "MINDATA" option (short form "MIND") is as follows.
.mbigbox
    OPTION MINDATA=n
.embigbox
.*
:CMT.This is from lg\opmixed.gml and is in the Linker Guide
:CMT.Topic: "The MIXED1632 Option"
.np
The format of the "MIXED1632" option (short form "MIX") is as
follows.
.mbigbox
    OPTION MIXED1632
.embigbox
.*
:CMT.This is from lg\opmodnam.gml and is in the Linker Guide
:CMT.Topic: "The MODNAME Option"
.np
The "MODNAME" option specifies a name to be given to the module being
created.
The format of the "MODNAME" option (short form "MODN") is as follows.
.mbigbox
    OPTION MODNAME=module_name
.embigbox
.*
:CMT.This is from lg\opmultil.gml and is in the Linker Guide
:CMT.Topic: "The MULTILOAD Option"
.np
The "MULTILOAD" option specifies that the module can be loaded more
than once by a "load" command.
The format of the "MULTILOAD" option (short form "MULTIL") is as
follows.
.mbigbox
    OPTION MULTILOAD
.embigbox
.*
:CMT.This is from lg\opnamele.gml and is in the Linker Guide
:CMT.Topic: "The NAMELEN Option"
.np
The format of the "NAMELEN" option (short form "NAMEL") is as follows.
.mbigbox
    OPTION NAMELEN=n
.embigbox
.*
:CMT.This is from lg\opnamele.gml and is in the Linker Guide
:CMT.Topic: "The NEWFILES Option"
.np
The "NEWFILES" option specifies that the application uses the
high-performance file system.
This option is applicable to 16-bit OS/2 applications only.
The format of the "NEWFILES" option (short form "NEWF") is as follows.
.mbigbox
    OPTION NEWFILES
.embigbox
.*
:CMT.This is from lg\opnlmfla.gml and is in the Linker Guide
:CMT.Topic: "The NLMFLAGS Option"
.np
The "NLMFLAGS" option is used to set bits in the flags field of the
header of the Netware executable file.
The format of the "NLMFLAGS" option (short form "NLMF") is as follows.
.mbigbox
    OPTION NLMFLAGS=some_value
.embigbox
.*
:CMT.This is from lg\opnoauto.gml and is in the Linker Guide
:CMT.Topic: "The NOAUTODATA Option"
.np
The "NOAUTODATA" option specifies that no automatic data segment
(default data segment defined by the group "DGROUP"), exists for the
program module or Dynamic Link Library being created.
This option applies to 16-bit applications only.
The format of the "NOAUTODATA" option (short form "NOA") is as
follows.
.mbigbox
    OPTION NOAUTODATA
.embigbox
.*
:CMT.This is from lg\opnodefa.gml and is in the Linker Guide
:CMT.Topic: "The NODEFAULTLIBS Option"
.np
The format of the "NODEFAULTLIBS" option (short form "NOD") is as
follows.
.mbigbox
    OPTION NODEFAULTLIBS
.embigbox
.*
:CMT.This is from lg\opnoexte.gml and is in the Linker Guide
:CMT.Topic: "The NOEXTENSION Option"
.np
The format of the "NOEXTENSION" option (short form "NOEXT") is as
follows.
.mbigbox
    OPTION NOEXTENSION
.embigbox
.*
:CMT.This is from lg\opnoindi.gml and is in the Linker Guide
:CMT.Topic: "The NOINDIRECT Option"
.np
The format of the "NOINDIRECT" option (short form "NOI") is as
follows.
.mbigbox
    OPTION NOINDIRECT
.embigbox
.*
:CMT.This is from lg\opnorelo.gml and is in the Linker Guide
:CMT.Topic: "The NORELOCS Option"
.np
The format of the "NORELOCS" option (short form "NOR") is as follows.
.mbigbox
    OPTION NORELOCS
.embigbox
.*
:CMT.This is from lg\opnostdc.gml and is in the Linker Guide
:CMT.Topic: "The NOSTDCALL Option"
.np
The "NOSTDCALL" option specifies that the characters unique to the
.kw __stdcall
calling convention be trimmed from all of the symbols that are
exported from the DLL being created.
The format of the "NOSTDCALL" option (short form "NOSTDC") is as
follows.
.mbigbox
    OPTION NOSTDCALL
.embigbox
.remark
Use the "IMPLIB" option to create an import library for the DLL which
can be used with software languages that add on the
.kw __stdcall
adornments.
.eremark
.*
:CMT.This is from lg\opnostub.gml and is in the Linker Guide
:CMT.Topic: "The NOSTUB Option"
.np
The "NOSTUB" option specifies that no "stub" program is to be placed at the
beginning of the executable file being generated.
The format of the "NOSTUB" option is as
follows.
:cmt.The IN 1 was added to allow .mbigbox to have a valid left position
.in 1
.mbigbox
    OPTION NOSTUB
.embigbox
.*
:CMT.This is from lg\opobjali.gml and is in the Linker Guide
:CMT.Topic: "The OBJALIGN Option"
.np
The "OBJALIGN" option specifies the alignment for objects in the
executable file.
The format of the "OBJALIGN" option (short form "OBJA") is as follows.
.mbigbox
    OPTION OBJALIGN=n
.embigbox
.*
:CMT.This is from lg\opoffset.gml and is in the Linker Guide
:CMT.Topic: "OFFSET - RAW only"
.np
The "OFFSET" option specifies the linear base address of the raw output image.
The format of the "OFFSET" option (short form "OFF") is as follows.
.mbigbox
    OPTION OFFSET=n
.embigbox
.*
:CMT.This is from lg\opoffset.gml and is in the Linker Guide
:CMT.Topic: "OFFSET - OS/2, Win32, ELF only"
.np
The format of the "OFFSET" option (short form "OFF") is as follows.
.mbigbox
    OPTION OFFSET=n
.embigbox
.*
:CMT.This is from lg\opoffset.gml and is in the Linker Guide
:CMT.Topic: "OFFSET - PharLap only"
.np
The "OFFSET" option specifies the offset in the program's segment in
which the first byte of code or data is loaded.
The format of the "OFFSET" option (short form "OFF") is as follows.
.mbigbox
    OPTION OFFSET=n
.embigbox
.*
:CMT.This is from lg\opoffset.gml and is in the Linker Guide
:CMT.Topic: "OFFSET - QNX only"
.np
The "OFFSET" option specifies the offset in the program's segment in
which the first byte of code or data is loaded.
This option does not apply to 16-bit QNX applications.
The format of the "OFFSET" option (short form "OFF") is as follows.
.mbigbox
    OPTION OFFSET=n
.embigbox
.*
:CMT.This is from lg\opoldlib.gml and is in the Linker Guide
:CMT.Topic: "The OLDLIBRARY Option"
.np
The "OLDLIBRARY" option is used to preserve the export ordinals for
successive versions of a Dynamic Link Library.
This ensures that any application that references functions in a
Dynamic Link Library by ordinal will continue to execute correctly.
The format of the "OLDLIBRARY" option (short form "OLD") is as
follows.
.mbigbox
    OPTION OLDLIBRARY=dll_name
.embigbox
.*
:CMT.This is from lg\oponeaut.gml and is in the Linker Guide
:CMT.Topic: "The ONEAUTODATA Option"
.np
The "ONEAUTODATA" option specifies that the automatic data segment
(default data segment defined by the group "DGROUP"), for the program
module or Dynamic Link Library (DLL) being created, will be shared by
all instances.
The format of the "ONEAUTODATA" option (short form "ONE") is as
follows.
.mbigbox
    OPTION ONEAUTODATA
.embigbox
.*
:CMT.This is from lg\oposdoma.gml and is in the Linker Guide
:CMT.Topic: "The OSDOMAIN Option"
.np
The format of the "OSDOMAIN" option (short form "OSD") is as follows.
.mbigbox
    OPTION OSDOMAIN
.embigbox
.*
:CMT.This is from lg\oposname.gml and is in the Linker Guide
:CMT.Topic: "The OSNAME Option"
.np
The "OSNAME" option can be used to set the name of the target
operating system of the executable file generated by the linker.
The format of the "OSNAME" option (short form "OSN") is as follows.
.mbigbox
    OPTION OSNAME='string'
.embigbox
.*
:CMT.This is from lg\oposver.gml and is in the Linker Guide
:CMT.Topic: "The OSVERSION Option"
.np
The "OSVERSION" option specifies that the linker should apply the given major
and minor version numbers to the PE format image header. This specifies the
major and minor versions of the operating system required to load this image.
If a version number is not specified, then the built-in value of 1.11 is used.
The format of the "OSVERSION" option (short form "OSV") is as follows.
.mbigbox
    OPTION OSVERSION = major[.minor]
.embigbox
.*
:CMT.This is from lg\oppackco.gml and is in the Linker Guide
:CMT.Topic: "The PACKCODE Option"
.np
This option is intended for 16-bit segmented applications.
By default, the &lnkname automatically groups logical code segments
into physical segments.
The "PACKCODE" option is used to specify the size of the physical
segment.
The format of the "PACKCODE" option (short form "PACKC") is as
follows.
.mbigbox
    OPTION PACKCODE=n
.embigbox
.*
:CMT.This is from lg\oppackda.gml and is in the Linker Guide
:CMT.Topic: "The PACKDATA Option"
.np
This option is intended for 16-bit segmented applications.
By default, the &lnkname automatically groups logical far data
segments into physical segments.
The "PACKDATA" option is used to specify the size of the physical
segment.
The format of the "PACKDATA" option (short form "PACKD") is as
follows.
.mbigbox
    OPTION PACKDATA=n
.embigbox
.*
:CMT.This is from lg\opprivil.gml and is in the Linker Guide
:CMT.Topic: "The PRIVILEGE Option"
.np
The "PRIVILEGE" option specifies the privilege level (0, 1, 2 or 3) at
which the application will run.
The format of the "PRIVILEGE" option (short form "PRIV") is as
follows.
.mbigbox
    OPTION PRIVILEGE=n
.embigbox
.*
:CMT.This is from lg\opprotmo.gml and is in the Linker Guide
:CMT.Topic: "The PROTMODE Option"
.np
The "PROTMODE" option specifies that the application will only run in
protected mode.
This option applies to 16-bit OS/2 applications only.
The format of the "PROTMODE" option (short form "PROT") is as follows.
.mbigbox
    OPTION PROTMODE
.embigbox
.*
:CMT.This is from lg\oppseudo.gml and is in the Linker Guide
:CMT.Topic: "The PSEUDOPREEMPTION Option"
The format of the "PSEUDOPREEMPTION" option (short form "PS") is as
follows.
.mbigbox
    OPTION PSEUDOPREEMPTION
.embigbox
.*
:CMT.This is from lg\opquiet.gml and is in the Linker Guide
:CMT.Topic: "The QUIET Option"
.np
The "QUIET" option tells the &lnkname to suppress all informational
messages.
Only warning, error and fatal messages will be issued.
By default, the &lnkname issues informational messages.
The format of the "QUIET" option (short form "Q") is as follows.
.mbigbox
    OPTION QUIET
.embigbox
.*
:CMT.This is from lg\opredefs.gml and is in the Linker Guide
:CMT.Topic: "The REDEFSOK Option"
.np
The format of the "REDEFSOK" option (short form "RED") is as follows.
.mbigbox
    OPTION REDEFSOK
.embigbox
.np
The format of the "NOREDEFSOK" option (short form "NORED") is as follows.
.mbigbox
    OPTION NOREDEFSOK
.embigbox
.*
:CMT.This is from lg\opreentr.gml and is in the Linker Guide
:CMT.Topic: "The REENTRANT Option"
.np
The "REENTRANT" option specifies that the module is reentrant.
That is, if an NLM is LOADed twice, the actual code in the server's
memory is reused.
The NLM's start procedure is called once for each LOAD.
The format of the "REENTRANT" option (short form "RE") is as follows.
.mbigbox
    OPTION REENTRANT
.embigbox
.*
:CMT.This is from lg\opresour.gml and is in the Linker Guide
:CMT.Topic: "RESOURCE - OS/2, Win16, Win32 only"
.np
.ix 'resource file'
The "RESOURCE" option requests the linker to add the specified
resource file to the executable file that is being generated.
The format of the "RESOURCE" option (short form "RES") is as follows.
.mbigbox
    OPTION RESOURCE[=resource_file]
.embigbox
.*
:CMT.This is from lg\opresour.gml and is in the Linker Guide
:CMT.Topic: "RESOURCE - QNX only"
.np
The "RESOURCE" option specifies the contents of the resource record in
QNX executable files.
The format of the "RESOURCE" option (short form "RES") is as follows.
.mbigbox
    OPTION RESOURCE resource_info

    resource_info ::= 'string' | =resource_file
.embigbox
.*
:CMT.This is from lg\oprwrelo.gml and is in the Linker Guide
:CMT.Topic: "The RWRELOCCHECK Option"
.np
The format of the "RWRELOCCHECK" option (short form "RWR") is as
follows.
.mbigbox
    OPTION RWRELOCCHECK
.embigbox
.*
:CMT.This is from lg\opscreen.gml and is in the Linker Guide
:CMT.Topic: "The SCREENNAME Option"
.np
The "SCREENNAME" option specifies the name of the first screen (the
screen that is automatically created when an NLM is loaded).
The format of the "SCREENNAME" option (short form "SCR") is as
follows.
.mbigbox
    OPTION SCREENNAME 'name'
.embigbox
.*
:CMT.This is from lg\opsharel.gml and is in the Linker Guide
:CMT.Topic: "The SHARELIB Option"
.np
The format of the "SHARELIB" option (short form "SHA") is as follows.
.mbigbox
    OPTION SHARELIB=shared_nlm
.embigbox
.*
:CMT.This is from lg\opshowde.gml and is in the Linker Guide
:CMT.Topic: "The SHOWDEAD Option"
.np
.ix 'dead code elimination'
.ix 'virtual functions'
The "SHOWDEAD" option instructs the linker to list, in the map file,
the symbols associated with dead code and unused C++ virtual functions
that it has eliminated from the link.
The format of the "SHOWDEAD" option (short form "SHO") is as follows.
.mbigbox
    OPTION SHOWDEAD
.embigbox
.*
:CMT.This is from lg\opsmall.gml and is in the Linker Guide
:CMT.Topic: "The SMALL Option"
.np
The "SMALL" option tells the &lnkname to use the standard overlay
manager (as opposed to the dynamic overlay manager) and that near
calls can be generated to overlay vectors corresponding to routines
defined in the overlayed portion of your program.
The format of the "SMALL" option (short form "SM") is as follows.
.mbigbox
    OPTION SMALL
.embigbox
.*
:CMT.This is from lg\opstack.gml and is in the Linker Guide
:CMT.Topic: "The STACK Option"
.np
The "STACK" option can be used to increase the size of the stack.
The format of the "STACK" option (short form "ST") is as follows.
.mbigbox
    OPTION STACK=n
.embigbox
.*
:CMT.This is from lg\opstanda.gml and is in the Linker Guide
:CMT.Topic: "The STANDARD Option"
.np
The "STANDARD" option instructs the &lnkname to use the standard
overlay manager (as opposed to the dynamic overlay manager).
Your application must be compiled for a big code memory model.
The format of the "STANDARD" option (short form "STAN") is as follows.
.mbigbox
    OPTION STANDARD
.embigbox
.*
:CMT.This is from lg\opstart.gml and is in the Linker Guide
:CMT.Topic: "The START Option"
.np
The format of the "START" option is as follows.
.mbigbox
    OPTION START=symbol_name
.embigbox
.*
:CMT.This is from lg\opstatic.gml and is in the Linker Guide
:CMT.Topic: "The STATICS Option"
.np
The format of the "STATICS" option (short form "STAT") is as follows.
.mbigbox
    OPTION STATICS
.embigbox
.*
:CMT.This is from lg\opstub.gml and is in the Linker Guide
:CMT.Topic: "The STUB Option"
.np
The "STUB" option specifies an executable file containing a "stub"
program that is to be placed at the beginning of the executable file
being generated.
The "stub" program will be executed if the module is executed under
DOS.
The format of the "STUB" option is as follows.
.mbigbox
    OPTION STUB=stub_name
.embigbox
.*
:CMT.This is from lg\opsymfil.gml and is in the Linker Guide
:CMT.Topic: "The SYMFILE Option"
.np
The "SYMFILE" option provides a method for specifying an alternate
file for debugging information.
The format of the "SYMFILE" option (short form "SYMF") is as follows.
.mbigbox
    OPTION SYMFILE[=symbol_file]
.embigbox
.*
:CMT.This is from lg\opsynchr.gml and is in the Linker Guide
:CMT.Topic: "The SYNCHRONIZE Option"
.np
The "SYNCHRONIZE" option forces an NLM to complete loading before
starting to load other NLMs.
Normally, the other NLMs are loading during the startup procedure.
The format of the "SYNCHRONIZE" option (short form "SY") is as
follows.
.mbigbox
    OPTION SYNCHRONIZE
.embigbox
.*
:CMT.This is from lg\opthread.gml and is in the Linker Guide
:CMT.Topic: "The THREADNAME Option"
.np
The "THREADNAME" option is used to specify the pattern to be used for
generating thread names.
The format of the "THREADNAME" option (short form "THR") is as
follows.
.mbigbox
    OPTION THREADNAME 'thread_name'
.embigbox
.*
:CMT.This is from lg\optoggle.gml and is in the Linker Guide
:CMT.Topic: "The TOGGLERELOCS Option"
.np
The format of the "TOGGLERELOCS" option (short form "TOG") is as
follows.
.mbigbox
    OPTION TOGGLERELOCS
.embigbox
.*
:CMT.This is from lg\opundefs.gml and is in the Linker Guide
:CMT.Topic: "The UNDEFSOK Option"
.np
The format of the "UNDEFSOK" option (short form "U") is as follows.
.mbigbox
    OPTION UNDEFSOK
.embigbox
.np
The format of the "NOUNDEFSOK" option (short form "NOU") is as
follows.
.mbigbox
    OPTION NOUNDEFSOK
.embigbox
.*
:CMT.This is from lg\opverbos.gml and is in the Linker Guide
:CMT.Topic: "The VERBOSE Option"
.np
The "VERBOSE" option controls the amount of information produced by
the &lnkname in the map file.
The format of the "VERBOSE" option (short form "V") is as follows.
.mbigbox
    OPTION VERBOSE
.embigbox
.*
:CMT.This is from lg\opversio.gml and is in the Linker Guide
:CMT.Topic: "The VERSION Option"
.np
The format of the "VERSION" option (short form "VERS") is as follows.
.mbigbox
OS/2, Win16, Win32:
    OPTION VERSION=major[.minor]
Netware:
    OPTION VERSION=major[.minor[.revision]]
.embigbox
.*
:CMT.This is from lg\opvfremo.gml and is in the Linker Guide
:CMT.Topic: "The VFREMOVAL Option"
.np
.ix 'virtual functions'
The "VFREMOVAL" option instructs the linker to remove unused C++
virtual functions.
The format of the "VFREMOVAL" option (short form "VFR") is as follows.
.mbigbox
    OPTION VFREMOVAL
.embigbox
.*
:CMT.This is from lg\opxdcdat.gml and is in the Linker Guide
:CMT.Topic: "The XDCDATA Option"
.np
The format of the "XDCDATA" option (short form "XDC") is as follows.
.mbigbox
    OPTION XDCDATA=rpc_file
.embigbox
.*
:CMT.This is from lg\tutmain.gml and is in the Linker Guide
:CMT.Topic: "Linking Executable Files for Various Systems"
.np
.ix '&lnkcmdup' 'command line format'
.ix '&lnkcmdup command line' 'invoking &lnkcmdup'
.ix 'command line format' '&lnkcmdup'
.ix 'invoking &lnkname'
The &lnkname command line format is as follows.
.mbigbox
&lnkcmd {directive}
.embigbox
.*
:CMT.This is from lg\tutqnx.gml and is in the 
:CMT.Open Watcom C/C++ Compiler and Tools User’s Guide for QNX
:CMT.Topic: "The Open Watcom Linker"
.np
.ix '&lnkcmdup' 'command line format'
.ix '&lnkcmdup command line' 'invoking &lnkcmdup'
.ix 'command line format' '&lnkcmdup'
.ix 'invoking &lnkname'
The &lnkname command line format is as follows.
.mbigbox
&lnkcmdup {directive}
.embigbox
.*
:CMT.This is from lg\wlbmain.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "The Open Watcom Library Manager Command Line"
.np
The following describes the &libname command line.
.ix '&libcmdup ' 'command line format'
.ix 'command line format ' '&libcmdup'
.mbigbox
&libcmdup [options_1] lib_file [options_2] [cmd_list]
.embigbox
.*
:CMT.This is from lg\wlbmain.gml and is in the 
:CMT.Open Watcom C/C++ Compiler and Tools User’s Guide for QNX
:CMT.Topic: "The Open Watcom Library Manager Command Line"
.np
The following describes the &libname command line.
.ix '&libcmdup ' 'command line format'
.ix 'command line format ' '&libcmdup'
.mbigbox
&libcmdup [options_1] lib_file [cmd_list]
.embigbox
.*
:CMT.This is from lg\wlfcopt.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "The Open Watcom Far Call Optimization Enabling Utility"
.np
The format of the &fcename is as follows.
Items enclosed in square brackets are optional; items enclosed in
braces may be repeated zero or more times.
.mbigbox
&fcecmdup { [option] [file] }
.embigbox
.*
:CMT.This is from lg\wlsyntax.gml and is in the Linker Guide and in the
:CMT.Open Watcom C/C++ Compiler and Tools User’s Guide for QNX
:CMT.The same text is used in many different sections, distinguished by target:
:CMT.DOS, ELF, Netware, OS/2, Phar Lap, QNX, RAW, Win16, Win32, WinVxD, ZDOS.
:CMT.Topic: "The [target] Executable File Format"
.np
.ix '&lnkcmdup' 'command line format'
.ix '&lnkcmdup command line' 'invoking &lnkcmdup'
.ix 'command line format' '&lnkcmdup'
.ix 'invoking &lnkname'
Input to the &lnkname is specified on the command line and can be
redirected to one or more files or environment strings.
The &lnkname command line format is as follows.
.mbigbox
&lnkcmdup {directive}
.embigbox
.*
:CMT.This is from lg\wlsyntax.gml and is in the
:CMT.Open Watcom C/C++ Compiler and Tools User’s Guide for QNX
:CMT.Topic: "The QNX Executable File Format"
.np
.ix '&lnkcmdup' 'command line format'
.ix '&lnkcmdup command line' 'invoking &lnkcmdup'
.ix 'command line format' '&lnkcmdup'
.ix 'invoking &lnkname'
Input to the &lnkname is specified on the command line and can be
redirected to one or more files or environment strings.
The &lnkname command line format is as follows.
.mbigbox
&lnkcmd {directive}
.embigbox
.*
:CMT.This is from lg\wstmain.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "The Open Watcom Strip Utility Command Line"
.np
The &stripname command line syntax is:
.ix '&stripcmdup.' 'command line format'
.ix 'command line format' '&stripcmdup'
.mbigbox
&stripcmdup [options] input_file [output_file] [info_file]
.embigbox
.*
:CMT.This is from pg\pgbldbnd.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "32-bit Windows:  Building and Running the GUI Application"
:CMT.     & "32-bit Windows:  Building and Running the Non-GUI Application"
.np
.ix 'binding 32-bit applications'
.ix 'Windows' 'binding 32-bit applications'
.ix 'WBIND'
The following describes the syntax of the
.fi WBIND
command.
.mbigbox
WBIND file_spec [-d] [-n] [-q] [-s supervisor] [-R rc_options]
.embigbox
.*
:CMT.This is from wd\wdbgos.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Debugging Under QNX Using the Postmortem Dump Facility"
.np
.ix 'postmortem dump' 'QNX'
.ix 'debugging' 'postmortem dump under QNX'
A limited form of debugging of an application that has terminated and
produced a postmortem dump can be done under QNX.
.ix 'dumper'
.ix 'dumper command'
In order to use this feature, you must start the QNX "dumper" program.
.mbigbox
dumper [-d path] [-p pid] &
.embigbox
.np
To examine the contents of the postmortem dump, the &dbgname may be
used.
.ix 'trap file'
The interface between the &dbgname and the postmortem dump is
contained in a special "trap" file.
The trap file is specified to the &dbgname using the
.sy TRap
option.
.*.ix 'pmd.trp'
.*.ix 'trap file' 'pmd.trp'
.mbigbox
&dbgcmd -TRap=pmd[;i] [:sym_file] file_spec
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Remote File Operations (DOS, OS/2 Only)"
.np
.ix 'RFX' 'running'
To run RFX, set up your machines as if you are about to do remote debugging.
Start the remote debug server, then start RFX using the the following syntax.
.mbigbox
RFX trap_file[;trap_parm] [rfx_cmd]
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Set Current Drive - drive:"
.mbigbox
drive:
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Change Directory - CHDIR, CD"
.mbigbox
chdir dir_spec
cd dir_spec
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Copy Files - COPY"
.mbigbox
copy [/s] src_spec [dst_spec] [/s]
.embigbox
.remark
If your default drive is set to one of the disks on the local
machine then the locale is local (e.g., @LC:, @LD:, @LE:, etc.).
If your default drive is set to one of the disks on the remote
machine then the locale is remote (e.g., @RC:, @RD:, @RE:, etc.).
If your DOS or OS/2 prompt contains the current drive and directory
then it will be easy to identify which locale is current.
.eremark
.remark
The "COPY" command is most effectively used when copying files from
one machine to the other.
Copying of large amounts of files from one place on the remote machine
to another place on the remote machine could be done more effectively
using the remote machine's DOS or OS/2.
This would eliminate the transfer of data from the remote machine to
the local machine and back to the remote machine.
.eremark
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "List Directory - DIR"
:cmt.The IN 1 was added to allow .mbigbox to have a valid left position
.in 1
.mbigbox
dir [/w] dir_spec [/w]
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Erase File - ERASE, DEL"
.mbigbox
erase [/s] file_spec [/s]
del [/s] file_spec [/s]
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Exit from RFX - EXIT"
.mbigbox
exit
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Make Directory - MKDIR, MD"
.mbigbox
mkdir dir_spec
md dir_spec
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Rename - RENAME, REN"
.mbigbox
rename file_spec new_name
ren file_spec new_name
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Remove Directory - RMDIR, RD"
.mbigbox
rmdir [/s] dir_spec [/s]
rd [/s] dir_spec [/s]
.embigbox
.*
:CMT.This is from wd\wdbgrfx.gml and is in the C/C++ Tools User’s Guide
:CMT.Topic: "Display File Contents - TYPE"
.mbigbox
type dir_spec
.embigbox
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "IF - ELSE - END IF"
.np
The
.kw ELSE
portion of this construct is optional, thus there are two possible
formats.
.mext begin
(a) IF( logical-expression )THEN    [: block-label]
        statement(s)
    END IF

(b) IF( logical-expression )THEN    [: block-label]
        statement(s)
    ELSE
        statement(s)
    END IF
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "ELSE IF"
.if &'lower(&syspdev) = ps .pa
.np
A further enhancement of the IF-THEN-ELSE construct is the
.kw ELSE IF
statement which may be used in the following two formats:
.mext begin
(a) IF( logical-expression-1 )THEN    [: block-label]
        statement(s)
    ELSE IF( logical-expression-2 )THEN
        statement(s)
    ...
    END IF

(b) IF( logical-expression-1 )THEN    [: block-label]
        statement(s)
    ELSE IF( logical-expression-2 )THEN
        statement(s)
    ...
    ELSE
        statement(s)
    END IF
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "DO - END DO"
.mext begin
  DO init-expr,end-value[,inc-value] [: block-label]
      statement(s)
  END DO
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "DO WHILE - END DO"
.mext begin
  DO WHILE (e) [: block-label]
      statement(s)
  END DO
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "LOOP - END LOOP"
.mext begin
  LOOP    [: block-label]
      statement(s)
  END LOOP
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "WHILE - END WHILE"
.mext begin
  WHILE( logical-expression )DO      [: block-label]
      statement(s)
  END WHILE
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "WHILE - Executable-statement"
.mext begin
  WHILE( logical-expression )stmt
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "UNTIL"
.mext begin
  LOOP    [: block-label]
      statement(s)
  UNTIL( logical-expression )
.mext end
.pc
or
.mext begin
  WHILE( logical-expression )DO      [: block-label]
      statement(s)
  UNTIL( logical-expression )
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "SELECT - END SELECT"
.mext begin
  SELECT [CASE] (e) [FROM]  [: block-label]
  CASE ( case-list )
      statement (s)
  CASE ( case-list )
      statement (s)
      .
      .
      .
  CASE DEFAULT
      statement(s)
  END SELECT
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "EXECUTE and REMOTE BLOCK"
.mext begin
  EXECUTE   name
      .
      .
      .
  REMOTE BLOCK name
      statement(s)
  END BLOCK
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "GUESS-ADMIT-END GUESS"
.mext begin
  GUESS    [: block-label]
      statement(s)
  ADMIT
      statement(s)
  ADMIT
      statement(s)
      .
      .
      .
  ADMIT
      statement(s)
  END GUESS
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "QUIT"
.mext begin
  QUIT    [ : block-label]
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "EXIT"
.mext begin
  EXIT    [ : block-label]
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "CYCLE"
.mext begin
  CYCLE   [ : block-label]
.mext end
.*
:CMT.This is from fl\fstruct.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "AT END"
.mext begin
  (READ statement)
  AT END DO [: block-label ]
      statement(s)
  END AT END
.mext end
.pc
or
.mext begin
  (READ statement)
  AT END, statement
.mext end
.*
:CMT.This is from fl\sadmit.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "ADMIT Statement"
.mext begin
      ADMIT
.mext end
.*
:CMT.This is from fl\sallocat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "ALLOCATE Statement"
.mext begin
ALLOCATE (array([l:]u[,[l:]u,...])[,...][,LOCATION=loc])
    or
ALLOCATE (array([l:]u[,[l:]u,...])[,...][,STAT=ierr])
    or
ALLOCATE (char*len)
.mext end
.*
:CMT.This is from fl\sat.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "AT END Statement"
.mext begin
      AT END DO    [: block-label]

           or

      AT END, stmt
.mext end
.*
:CMT.This is from fl\scase.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "CASE Statement"
.mext begin
      CASE cl
.mext end
.*
:CMT.This is from fl\scycle.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "CYCLE Statement"
.mext begin
      CYCLE   [: block-label]
.mext end
.*
:CMT.This is from fl\sdealloc.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "DEALLOCATE Statement"
.mext begin
      DEALLOCATE (arraylist [, STAT = ierr])
.mext end
.*
:CMT.This is from fl\sdowhile.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "DO WHILE Statement"
.mext begin
      DO [s[,]] WHILE (e)    [: block-label]
.mext end
.*
:CMT.This is from fl\sendaten.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END AT END Statement"
.mext begin
      END AT END
.mext end
.*
:CMT.This is from fl\sendbloc.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END BLOCK Statement"
.mext begin
      END BLOCK
.mext end
.*
:CMT.This is from fl\senddo.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END DO Statement"
.mext begin
      END DO
.mext end
.*
:CMT.This is from fl\sendgues.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END GUESS Statement"
.mext begin
      END GUESS
.mext end
.*
:CMT.This is from fl\sendloop.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END LOOP Statement"
.mext begin
      END LOOP
.mext end
.*
:CMT.This is from fl\sendmap.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END MAP Statement"
.mext begin
      END MAP
.mext end
.*
:CMT.This is from fl\sendsele.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END SELECT Statement"
.mext begin
      END SELECT
.mext end
.*
:CMT.This is from fl\sendstru.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END STRUCTURE Statement"
.mext begin
      END STRUCTURE
.mext end
.*
:CMT.This is from fl\sendunio.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END UNION Statement"
.mext begin
      END UNION
.mext end
.*
:CMT.This is from fl\sendwhil.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "END WHILE Statement"
.mext begin
      END WHILE
.mext end
.*
:CMT.This is from fl\sexecute.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "EXECUTE Statement"
.mext begin
      EXECUTE name
.mext end
.*
:CMT.This is from fl\sexit.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "EXIT Statement"
.mext begin
      EXIT    [: block-label]
.mext end
.*
:CMT.This is from fl\sguess.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "GUESS Statement"
.mext begin
      GUESS    [: block-label]
.mext end
.*
:CMT.This is from fl\sinclude.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "INCLUDE Statement"
.mext begin
      INCLUDE 'inc_spec'
.mext end
.*
:CMT.This is from fl\sloop.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "LOOP Statement"
.mext begin
      LOOP    [:block-label]
.mext end
.*
:CMT.This is from fl\smap.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "MAP Statement"
.mext begin
      MAP
.mext end
.*
:CMT.This is from fl\snamelis.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "NAMELIST Statement"
.mext begin
      NAMELIST /name/ vlist [[,]/name/ vlist] ...
.mext end
.*
:CMT.This is from fl\sotherwi.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "OTHERWISE Statement"
.mext begin
      OTHERWISE
.mext end
.*
:CMT.This is from fl\squit.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "QUIT Statement"
.mext begin
      QUIT    [: block-label]
.mext end
.*
:CMT.This is from fl\srecord.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "RECORD Statement"
.mext begin
      RECORD /typename/ name [,name] ...
.mext end
.*
:CMT.This is from fl\sremote.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "REMOTE BLOCK Statement"
.mext begin
      REMOTE BLOCK name
.mext end
.*
:CMT.This is from fl\sselect.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "SELECT Statement"
.mext begin
      SELECT [CASE] (e) [FROM]  [: block-label]
.mext end
.*
:CMT.This is from fl\sstructu.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "STRUCTURE Statement"
.mext begin
      STRUCTURE /typename/
.mext end
.*
:CMT.This is from fl\sunion.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "UNION Statement"
.mext begin
      UNION
.mext end
.*
:CMT.This is from fl\suntil.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "UNTIL Statement"
.mext begin
      UNTIL (e)
.mext end
.*
:CMT.This is from fl\swhile.gml and is in the FORTRAN 77 Language Reference
:CMT.Topic: "Block WHILE Statement"
.mext begin
      WHILE (e) DO    [: block-label]
.mext end
.*
:CMT.This is from cmn\complink.gml and is in the Tool User's Guide
:CMT.Topic: "WCL/WCL386 Environment Variables"
.hint
.ix 'system initialization file' 'AUTOEXEC.BAT'
.ix 'AUTOEXEC.BAT' 'system initialization file'
If you are running DOS and you use the same
.kw &wclcmdup16
or
.kw &wclcmdup32
options all the time, you may find it handy to place the
"&setcmdup &wclvarup16" or
"&setcmdup &wclvarup32"
command in your DOS system initialization file,
.fi AUTOEXEC.BAT.
.ix 'system initialization file' 'CONFIG.SYS'
.ix 'CONFIG.SYS' 'system initialization file'
If you are running OS/2 and you use the same
.kw &wclcmdup16
or
.kw &wclcmdup32
options all the time, you may find it handy to place the
"&setcmdup &wclvarup16" or
"&setcmdup &wclvarup32"
command in your OS/2 system initialization file,
.fi CONFIG.SYS.
.ehint
.*
:CMT.This is from cmn\wguiwide.gml and is in the IDE Help
:CMT.Topic: "A Simple Executable"
.if &'lower(&syspdev) = ps .pa
.hint
You can also invoke the &edname by double-clicking on
the source file that you want to edit.
.ehint
.*
:CMT.This is from cmn\wguiwide.gml and is in the IDE Help
:CMT.Topic: "Adding Source Files"
.hint
Even when entering wild cards, you can add multiple source file groups
by separating each entry with one space (e.g., sou*.c sou*.cpp).
.ehint
.*
:CMT.This is from cmn\wguiwide.gml and is in the IDE Help
:CMT.Topic: "Invoking Other Applications from the IDE"
.hint
.ix 'mouse' 'right button'
.ix 'right button'
An alternate way to view the contents of the Targets and Sources menu
is to click the right mouse button while your cursor is positioned in
the project window. If your cursor is positioned over a target, the
Targets menu appears. If your cursor is positioned over a source file,
the Sources menu appears.
.ehint
.*
:CMT.This is from pg\pgqa.gml and is in the Programmer's Guide
:CMT.Topic: "Conversion from IBM-compatible PC compilers"
.hint
(16-bit applications only)
Most 16-bit C compilers (including &cmpname) have a "large" memory
model which means that four byte pointers are used for both code and
data references.
A subtle point to watch out for involves differences between memory
model definitions of different compilers.
The "cdecl" calling convention allows functions to assume that the DS
segment register points to the group "DGROUP".
The &cmpname large memory model has what is called a "floating DS".
Any function used for the large memory model cannot assume that the DS
segment register points to the group "DGROUP".
There are a few possible recourses.
:cmt. the "note" macros are commented out as they have nothing to do with boxing
:cmt..autonote
:cmt..note
The assembly code could save and restore the DS segment register and
set DS to DGROUP in order to conform to the &cmpname convention.
If there are only a few accesses to DGROUP data, it is advisable to
use the SS segment register which points to DGROUP in the large memory
model.
.ix 'auxiliary pragma' 'loadds'
.ix 'loadds pragma option'
.ix 'pragma' 'loadds option'
:cmt..note
The assembly function could be described using a pragma that states
that DS should point to "DGROUP" before calling the function.
.ix 'DS segment register'
.ix 'segment registers' 'DS'
.np
.us #pragma aux _Setcolor parm loadds
.np
In the above example,
.us _Setcolor
is the sample function being described.
:cmt..note
.ix 'compile options' 'zdp'
The final alternative would be the use of the "zdp" compiler option.
The "zdp" option informs the code generator that the DS register must
always point to "DGROUP".
This is the default in the small, medium and flat memory models.
Note that "flat" is a 32-bit memory model only.
:cmt..endnote
.ehint
.*
:CMT.This is from pg\pgqa.gml and is in the Programmer's Guide
:CMT.Topic: "What you should know about optimization"
.hint
The definitive reference on compiler design is the "dragon" book
"Compilers - Principles, Techniques, and Tools", Alfred V. Aho, Ravi
Sethi, and Jeffrey D. Ullman, published by Addison-Wesley, Reading,
Massachusetts, 1986.
The authors of the "dragon" book advocate a conservative approach to
code generation where optimizations must preserve the semantics of the
original program.
The conservative approach is used throughout the &cmpname compiler to
ensure that programmers can use the compiler without worrying about
the semantics of their program being changed.
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
.* there is no "oa" option with FORTRAN
The programmer can request that potentially unsafe optimizations be
performed.
With regard to the "oa" (ignore aliasing) option provided by the
&cmpname compiler, the compiler only ignores aliasing of global
variables rather than ignore aliasing totally like other compilers.
.do end
.ehint
.hint
.ix 'debugging'
.ix 'd1'
.ix 'd2'
The "d2" compiler option will generate symbolic
information (for every local variable) and line number information for
the source file.
The "d1" compiler option will only generate line number information
for the source file.
The use of these options determines what kind of information will be
available for the particular module during the debugging session.
.ehint
.*
:CMT.This is from pg\pgqa.gml and is in the Programmer's Guide
:CMT.Topic: "What does "NULL assignment detected" mean in 16-bit applications?"
.hint
The &dbgname allows many commands to have short forms.
For instance, the "EXAMINE" command can be shortened to an "E".
We used the full commands in the examples for clarity.
.ehint
.*
:CMT.This is from pg\pgqa.gml and is in the Programmer's Guide
:CMT.Topic: "How source files can be seen in the debugger"
.hint
A subtle point to debugging information is that all the modules will
have global names available if any debugging directive is used.
In the above example, the module
.fi main
will have global name information even though it does not have a DEBUG
directive preceding it.
.ehint
.*
:CMT.This is from cg\ccall.gml and is in the C/C++ User's Guide
:CMT.Topic: "32-bit:  Returning Values from Functions"
.remark
.ix 'stack-based calling convention' 'returning values from functions'
.ix 'options' 'fpc'
Returning values from functions in the stack-based calling convention
is the same as returning values from functions in the register-based
calling convention when using the "fpc" option.
.eremark
.*
:CMT.This is from cg\ccall.gml and is in the C/C++ User's Guide
:CMT.Topic: "32-bit:  Calling Conventions for 80x87-based Applications"
.remark
.ix 'stack-based calling convention' '80x87 considerations'
When compiling using the "fpi" or "fpi87" options, the method used for
passing floating-point arguments in the stack-based calling convention
is identical to the method used in the register-based calling
convention.
However, when compiling using the "fpi" or "fpi87" options, the method
used for returning floating-point values in the stack-based calling
convention is different from the method used in the register-based
calling convention.
The register-based calling convention returns floating-point values in
ST(0), whereas the stack-based calling convention returns
floating-point values in EDX and EAX.
.eremark
.*
:CMT.This is from cg\cpheader.gml and is in the C/C++ User's Guide
:CMT.Topic: "When to Precompile Header Files"
.remark
Although you can use only one precompiled header (:FNAME..PCH:eFNAME.)
file per source file, you can use multiple
.fi .PCH
files in a project.
.eremark
.*
:CMT.This is from cg\cpopts.gml and is in the C/C++ User's Guide
:CMT.Topic: "fpi"
.remark
.ix 'floating-point' 'consistency of options'
When any module in an application is compiled with a particular
"floating-point" option, then all modules must be compiled with
the same option.
.np
If you wish to have floating-point emulation software included in
the application, you should select the "fpi" option.
A math coprocessor need not be present at run-time.
.np
Different math libraries are provided for applications which have been
compiled with a particular floating-point option.
:cmt.This line was reformatted because we don't have HDREF working yet.
:cmt.See the section entitled :HDREF refid='libmath'..
See the section entitled "libmath".
.eremark
.*
:CMT.This is from cg\seh.gml and is in the C/C++ User's Guide
:CMT.Topic: "Termination Handlers"
.remark
Kevin Goodman describes "unwinds" in his article.
"There are two types of unwinds: global and local. A global unwind
occurs when there are nested functions and an exception takes place. A
local unwind takes place when there are multiple handlers within one
function. Unwinding means that the stack is going to be clean by the
time your handler's code gets executed."
.eremark
.*
:CMT.This is from cmn\wmodl386.gml and is in the C/C++ User's Guide
:CMT.Topic: "32-bit Code Models"
.remark
If your program contains less than 4GB of code, you should use a
memory model that employs the small code model.
This will result in smaller and faster code since near calls are
smaller instructions and are processed faster by the CPU.
.eremark
.*
:CMT.This is from cmn\wmodl386.gml and is in the C/C++ User's Guide
:CMT.Topic: "32-bit Data Models"
.remark
If your program contains less than 4GB of data, you should use the
small data model.
This will result in smaller and faster code since references using
near pointers produce fewer instructions.
.eremark
.*
:CMT.This is from pg\fdosfile.gml and is in the FORTRAN 77 User's Guide
:CMT.Topic: "File Names in the FAT File System"
.remark
.ix 'file name' 'case sensitivity'
The file specification is case insensitive in that upper and lower
case letters can be used interchangeably.
.eremark
.*
:CMT.This is from fg\flib.gml and is in the FORTRAN 77 User's Guide
:CMT.Topic: "Subroutine FINTR"
.np
.ix 'assembler subprograms' 'subroutine FINTR'
.ix 'utility subprograms' 'subroutine FINTR'
.ix 'FINTR subroutine'
The subroutine
.id FINTR
allows the user to execute any software interrupt from a
FORTRAN 77 program.
.remark
This subroutine is only supported by the DOS and Windows libraries.
.eremark
.*
:CMT.This is from wd\wdbgwire.gml and is in the Debugger Guide
:CMT.Topic: "Parallel Port Wiring Considerations"
.remark
Although the wiring is different for all three cables, the &company
parallel communications software can determine which one is in use.
.eremark
.*
:set symbol="begthread" value="_beginthread".
:CMT.This is from pg\mthread.gml and is in the C/C++ Programmer's Guide
:CMT.Topic: "OS/2:  Creating Threads"
.warn
If any thread
calls a library function,
you must use the
.id &begthread
function to create the thread.
Do not use the
.id DosCreateThread
API function.
.ewarn
.*
:set symbol="begthread" value="_beginthread".
:CMT.This is from pg\mthread.gml and is in the C/C++ Programmer's Guide
:CMT.Topic: "NT:  Creating Threads"
.warn
If any thread
calls a library function,
you must use the
.id &begthread
function to create the thread.
Do not use the
.id CreateThread
API function.
.ewarn
.*
:set symbol="begthread" value="beginthread".
:CMT.This is from pg\mthread.gml and is in the FORTRAN 77 Programmer's Guide
:CMT.Topic: "OS/2:  Creating Threads"
If any thread
uses an I/O statement or calls an intrinsic function,
you must use the
.id &begthread
function to create the thread.
Do not use the
.id DosCreateThread
API function.
.ewarn
.*
:set symbol="begthread" value="beginthread".
:CMT.This is from pg\mthread.gml and is in the FORTRAN 77 Programmer's Guide
:CMT.Topic: "NT:  Creating Threads"
.warn
If any thread
uses an I/O statement or calls an intrinsic function,
you must use the
.id &begthread
function to create the thread.
Do not use the
.id CreateThread
API function.
.ewarn
.*
:CMT.This is from rsi\utilts.gml and is in the the C/C++ Programmer's Guide
:CMT.Topic: "DOS/4GW:  PMINFO"
:cmt.IN 1 is used to give .cbox a reasonble starting position
.if &'lower(&syspdev) = ps .pa
.in 1
.cbox begin
.in -2
C>pminfo
     Protected Mode and Extended Memory Performance Measurement -- 5.00
             Copyright (c) Tenberry Software, Inc. 1987 - 1993

DOS memory   Extended memory   CPU performance equivalent to 67.0 MHz 80486
----------   ---------------
       736              8012   K bytes configured (according to BIOS).
       640             15360   K bytes physically present (SETUP).
       651              7887   K bytes available for DOS/16M programs.
22.0 (3.0)        18.9 (4.0)   MB/sec word transfer rate (wait states).
42.9 (3.0)        37.0 (4.0)   MB/sec 32-bit transfer rate (wait states).

Overall cpu and memory performance (non-floating point) for typical
DOS programs is 10.36 ñ 1.04 times an 8MHz IBM PC/AT.

Protected/Real switch rate = 36156/sec (27 usec/switch, 15 up + 11 down),
DOS/16M switch mode 11 (VCPI).
.in +2
.cbox end
.*
:CMT.This is from rsi\utilts.gml and is in the the C/C++ Programmer's Guide
.if &'lower(&syspdev) = ps .pa
:CMT.Topic: "DOS/4GW:  RMINFO"
.cbox begin
.in -2
C>rminfo

DOS/16M Real Mode Information Program 5.00
Copyright (C) Tenberry Software, Inc. 1987 - 1993

Machine and Environment:
        Processor:              i386, coprocessor present
        Machine type:           10 (AT-compatible)
        A20 now:                enabled
        A20 switch rigor:       disabled
        DPMI host found
Switching Functions:
        To PM switch:           DPMI
        To RM switch:           DPMI
        Nominal switch mode:    0
        Switch control flags:   0000
Memory Interfaces:
        DPMI may provide:       16384K returnable
        Contiguous DOS memory:  463K
.in +2
.cbox end

