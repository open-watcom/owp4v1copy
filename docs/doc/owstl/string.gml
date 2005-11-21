:H1.Introduction
:P.
The class template :CLASS.std::~string
provides dynamic strings of objects with a type given by the type
parameter :CLASS.CharT.
The behavior of :CLASS.CharT
objects is described by a suitable traits class. By default a
specialization of :CLASS.std::~char_traits< CharT >
is used. Specializations of :CLASS.std::~char_traits
for both character and wide character types are part of the library and
are used without any further intervention by the programmer.

:P.
Most of the methods in class template :CLASS.std::~string
are located in :PATH.hdr/watcom/string.mh.
This file is also used to generate the C library header :PATH.string.h
and the corresponding "new style" C++ library header :PATH.cstring.
This is accomplished by executing :TOOL.wsplice
over the file multiple times using different options. The material
that goes into the C++ library header :PATH.string
appears in :PATH.string.mh
below the C library material.

:P.
The class template :CLASS.std::~char_traits
along with its specializations for character and wide character, the
definition of :CLASS.std::~string,
and certain methods of :CLASS.std::~string
are located in :PATH.hdr/watcom/_strdef.mh.
This file generates the header :PATH._strdef.h
which is not to be directly included in user programs. It is, however,
included in :PATH.string
thus completing the contents of :PATH.string.
The reason for this separation of :PATH.string
is because of the exception classes. The standard exception classes use
strings and yet some of the methods of string throw standard exceptions. This
leads to circular inclusions which are clearly unacceptable. To resolve
this problem, the parts of :PATH.string
that are needed by the standard exception classes are split off into
:PATH._strdef.h.
These parts do not themselves need the standard exceptions and so the
circular reference is broken.

:H1. Status
:P.
:AUTHOR date='19 Oct 2005'.Peter C. Chapin
:P.
Reviewer: Not reviewed
:P.
Most of the required functionality has been implemented together with
moderately complete regression tests. There has so far been very little
user feedback, however.

:P.
The main componet that is missing is the I/O support for
:CLASS.std::~string.
Implementing this component has been put on hold until the iostreams
part of the library is reworked. In the meantime users will have to do
string I/O using C-style strings and convert them between
:CLASS.std::~string.
This is a significant issue; it is assumed that most standard programs
will do I/O on strings directly the library doesn't currently support
such programs no matter how complete the :CLASS.std::~string
implementation itself might be.

:P.
In addition to the problem above, the template methods of
:CLASS.std::~string
have not been implemented because the compiler does not yet support
template methods sufficiently well.

.H1. Design Details
:P.

.H2. Copy-On-Write?
:P.
This implementation of
:CLASS.std::~string
does not use a copy-on-write approach. Instead every string object
maintains its own independent representation. This was done, in
large measure, to simplify the implementation so that a reasonable
:CLASS.std::~string
could be offered quickly. It is easier to make independent strings
exception safe and thread safe. In fact, it is extremely difficult to
make
:CLASS.std::~string
thread safe in a reasonable way if a copy-on-write implementation
is used [references?]). Although the current implementation has not been
closely reviewed for either exception safety or thread safety, such a
review would not be too difficult considering the straight forward
implementation technique.

:P.
Concerns about the performance of this implementation have been
partially addressed by the results of some (minimal) benchmark tests.
See
:PATH.bld/bench/owstl.
These tests show that the current performance of
:CLASS.std::~string
is at least competitive with that offered by other implementations. More
complete benchmark testing is needed to verify this result.

:H2. Design Overview
:P.
This implementation of
:CLASS.std::~string
uses a single pointer and two counters to define the buffer space
allocated for a string. One counter measures the length of the allocated
space while the other measures the number of character units in that
space that are actually used. In order to meet the complexity
requirements of the standard,
:CLASS.string
allocates more space than it needs, increasing that amount of space by a
constant multiplicative factor whenever more is needed. This
implementation uses a multiplicative factor of two [note: other factors,
such as 1.5, might be more desirable; a factor of two causes somewhat
ineffecient memory reuse characteristics [reference?]]. The capacity of
a string is always an exact power of two. When a string is first created
it is given a particular minimum size for its capacity (currently 8) or
a capacity that is the smallest power of two larger than the new
string's length, whichever is larger.

:P.
A string's capacity is never reduced in this implementation. Once a
string's capacity is increased, the memory is not reclaimed until the
string is destroyed. This was done on the assumption that if a string
was once large it will probably be large again. Not returning memory
when a string's length shrinks reduces the total number of memory
allocation operations and reduces the chances of an out of memory
exception being thrown during a string operation. However, this design
choice is not particularly friendly to low memory systems. Considering that
Open Watcom targets some small machines, an alternative memory
management strategy might be worth offering as an option. In the
meantime programmers on such systems should be careful to destroy large
strings when they are no longer needed rather than, for example, just
calling 
:CODE.erase().

:H2. Relationship to vector
:P.
The
:CLASS.std::~string
template is very similar in many ways to the
:CLASS.std::~vector
template. In fact, in OWSTL both implementations use a similar
representation technique and a similar memory management approach.
However, the implementation of
:CLASS.std::~vector
is more complicated because the objects in a vector need not be of a POD
type (as is the case for string) so they need to be carefully copied and
initialized using appropriate methods. In contrast the
:CLASS.CharT
type used by
:CLASS.std::~string
can be copied and moved with low level memory copying functions (see
:CLASS.std::char_traits
).

:H2. Open Watcom Extensions
:P.
Because of the widespread demand for case insensitive string
manipulation, OWSTL provides a traits class that includes case
insensitive character comparisons. An instantiation of
:CLASS.std::~string,
called
:CLASS._watcom::~istring
is provided that uses this traits class.
