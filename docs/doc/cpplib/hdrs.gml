The following header files are supplied with the C++ class library.
When an object from the library is to be manipulated with a function,
the related header file should be included in the source file.  The related
header file is shown in the synopsis for the function.
The header files provide the proper declarations for the functions and for
the number and types of arguments used with them.  Constant values and
enumerations used in conjunction with the functions are also declared.  The
files can be included in any order.
:P.
The following files are provided with the software.  The header files are
all located in the
:MONO.\WATCOM\H
or
:MONO./usr/include
(QNX only)
directory.
:DL termhi=2.
:DT.complex.h
:DD.
.ix 'header files' 'complex'
This header file defines the
:MONO.Complex
class.
The
:MONO.Complex
class is used to perform complex arithmetic.
The
:MONO.Complex
member functions are declared.
The related functions that manipulate
:MONO.Complex
objects are declared.
Inline member functions for the
:MONO.Complex
class are defined.
:DT.except.h
:DD.
.ix 'header files' 'except'
.ix 'exception handling'
This header file provides declarations to be used with the
exception handling mechanism.
:DT.fstream.h
:DD.
.ix 'header files' 'fstream'
This header file defines the
:MONO.filebuf
:CONT.,
:MONO.fstreambase
:CONT.,
:MONO.ifstream
:CONT.,
:MONO.ofstream
:CONT., and
:MONO.fstream
classes.
These classes are used to perform C++ file input and output operations.
The various class members are declared and inline member functions for
the classes are defined.
:DT.generic.h
:DD.
.ix 'header files' 'generic'
This header file is part of the macro support required prior to the
implementation of templates in the C++ language.  It is retained for
backwards compatibility.
:DT.iomanip.h
:DD.
.ix 'header files' 'iomanip'
This header file contains the macro definitions required to implement the
parameterized manipulators in the absence of templates.   It is retained
for backwards compatibility.
:DT.iostream.h
:DD.
This header file defines the
:MONO.ios
:CONT.,
:MONO.istream
:CONT.,
:MONO.ostream
:CONT., and
:MONO.iostream
classes.
These classes form the basis of the C++ formatted input and output support.
The various class members are declared and
inline member functions for the classes are defined.
The &cin., &cout., &cerr., and &clog. predefined objects are declared
along with the non-parameterized manipulators.
:DT.new.h
:DD.
.ix 'header files' 'new'
This header file provides declarations to be used with the
intrinsic
:MONO.operator~bnew
and
:MONO.operator~bdelete
memory management functions.
:DT.stdiobuf.h
:DD.
.ix 'header files' 'stdiobuf'
This header file defines the
:MONO.stdiobuf
class which provides the support for the C++ input and output operations to
standard input, standard output, and standard error streams.
The various class members are declared and
inline member functions for the classes are defined.
:DT.streambu.h
:DD.
.ix 'header files' 'streambu'
This header file defines the
:MONO.streambuf
class which provides the support for buffering of input and output operations.
The various class members are declared and
inline member functions for the classes are defined.
This header file is automatically included by the
:MONO.iostream.h
header file.
:DT.string.hpp
:DD.
.ix 'header files' 'string'
This header file defines the
:MONO.String
class.
The
:MONO.String
class is used to manipulate character strings.
The
:MONO.String
member functions are declared.
The related functions that manipulate
:MONO.String
objects are declared.
Inline member functions for the
:MONO.String
class are defined.
Note that the
:MONO.hpp
extension is used to avoid colliding with the ANSI C
:MONO.string.h
header file.
:DT.strstrea.h
:DD.
.ix 'header files' 'strstrea'
This header files defines the
:MONO.strstreambuf
:CONT.,
:MONO.strstreambase
:CONT.,
:MONO.istrstream
:CONT.,
:MONO.ostrstream
:CONT., and
:MONO.strstream
classes.
These classes are used to perform C++ in-memory formatting.
The various class members are declared and
inline member functions for the classes are defined.
:DT.wcdefs.h
:DD.
.ix 'header files' 'wcdefs'
This header file contains definitions used by the container libraries.
If a container class needs any of these definitions, the file is
automatically included.
:DT.wclbase.h
:DD.
.ix 'header files' 'wclbase'
This header file defines the base classes which are used
by the list containers.
:DT.wclcom.h
:DD.
.ix 'header files' 'wclcom'
This header file defines the classes which are common
to the list containers.
:DT.wclibase.h
:DD.
.ix 'header files' 'wclibase'
This header file defines the base classes which are used
by the list iterators.
:DT.wclist.h
:DD.
.ix 'header files' 'wclist'
This header file defines the
:MONO.list
.ix 'list containers'
container classes.
The available list container classes are single and double linked versions
of intrusive, value and pointer lists.
:DT.wclistit.h
:DD.
.ix 'header files' 'wclistit'
This header file defines the
:MONO.iterator
.ix 'iterator classes'
classes that correspond to the list containers.
:DT.wcqueue.h
:DD.
.ix 'header files' 'wcqueue'
This header file defines the
:MONO.queue
class.
Entries in a queue class are accessed first in, first out.
:DT.wcstack.h
:DD.
.ix 'header files' 'wcstack'
This header file defines the
:MONO.stack
class.
Entries in a stack class are accessed last in, first out.
:eDL.
