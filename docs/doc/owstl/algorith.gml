:H1.Introduction
:P.
The algorithm header (truncated to :Q.algorith:eQ. for 8.3 file name 
compatibility) contains definitions of the algorthms from chaper 25 of 
the standard.


:H2.Status
:P.
:AUTHOR date='14 Jan 2006'.P Chapin, D Cletheroe
:P.
Reviewer: Not reviewed
:P.
The algorithms library was mainly written by Peter.
:P.
Missing members:
:UL.
:LI.adjacent_find( ForwardIterator, ForwardIterator )
:LI.adjacent_find( ForwardIterator, ForwardIterator, BinaryPredicate )
:LI.mismatch( InputIt1, InputIt1, InputIt2, BinaryPredicate )
:LI.search( FwdIterator1, FwdIterator1, FwdIterator2, FwdIterator2 )
:LI.search( FwdIt1, FwdIt1, FwdIt2, FwdIt2, BinaryPredicate )
:LI.search_n( FwdIterator, FwdIterator, Size, const T& )
:LI.search_n( FwdIt, FwdIt, Size, const T&, BinaryPredicate )
:LI.unique( ForwardIterator, ForwardIterator )
:LI.unique( ForwardIterator, ForwardIterator, BinaryPredicate pred )
:LI.unique_copy( InputIterator, InputIterator, OutputIterator )
:LI.unique_copy( InputIt, InputIt, OutputIt, BinaryPredicate )
:LI.rotate( ForwardIterator, ForwardIterator, ForwardIterator )
:LI.rotate_copy( FwdIterator, FwdIterator, FwdIterator, OutputIt )
:LI.partition( BidirectionalIterator, BidirectionalIterator, Predicate )
:LI.stable_partition( BidirectionalIterator, BidirectionalIterator, Predicate )
:LI.sort( RandomAccessIterator, RandomAccessIterator, Compare )
:LI.stable_sort( RandomAccessIterator, RandomAccessIterator )
:LI.stable_sort( RandomAccessIterator, RandomAccessIterator, Compare )
:LI.partial_sort( RAIterator, RAIterator, RAIterator )
:LI.partial_sort( RAIterator, RAIterator, RAIterator, Compare )
:LI.partial_sort_copy( InIterator, InIterator, RAIterator, RAIterator )
:LI.partial_sort_copy( InIt, InIt, RAIt, RAIt, Compare )
:LI.nth_element( RAIterator, RAIterator, RAIterator )
:LI.nth_element( RAIterator, RAIterator, RAIterator, Compare )
:LI.lower_bound( ForwardIterator, ForwardIterator, const T& )
:LI.lower_bound( ForwardIterator, ForwardIterator, const T&, Compare )
:LI.upper_bound( ForwardIterator, ForwardIterator, const T& )
:LI.upper_bound( ForwardIterator, ForwardIterator, const T& , Compare )
:LI.equal_range( ForwardIterator, ForwardIterator, const T& )
:LI.equal_range( ForwardIterator, ForwardIterator, const T&, Compare )
:LI.binary_search( ForwardIterator, ForwardIterator, const T& )
:LI.binary_search( ForwardIterator, ForwardIterator, const T&, Compare )
:LI.merge( InputIt1, InputIt1, InputIt2, InputIt2, OutputIt result )
:LI.merge( InputIt1, InputIt1, InputIt2, InputIt2, OutputIt result, Compare )
:LI.inplace_merge( BidirectionalIt, BidirectionalIt, BidirectionalIt )
:LI.inplace_merge( BidirectionalIt, BidirectionalIt, BidirectionalIt, Compare )
:LI.includes( InputIt1, InputIt1, InputIt2, InputIt )
:LI.includes( InputIt1, InputIt1, InputIt2, InputIt2, Compare )
:LI.set_union( InputIt1, InputIt1, InputIt2, InputIt2, OutputIt )
:LI.set_union( InputIt1, InputIt1, InputIt2, InputIt2, OutputIt, Compare )
:LI.set_intersection( InputIt1, InputIt1, InputIt2, InputIt2, OutputIt )
:LI.set_intersection( InputIt1, InputIt1, InputIt2, InputIt2, OutIt, Compare )
:LI.set_difference( InputIt1, InputIt1, InputIt2, InputIt2, OutputIt )
:LI.set_difference( InputIt1, InputIt1, InputIt2, InputIt2, OutIt, Compare )
:LI.set_symmetric_difference( InputIt1, InputIt1, InputIt2, InputIt2, OutIt )
:LI.set_symmetric_difference( InIt1, InIt1, InIt2, InIt2, OutIt, Compare )
:LI.lexicographical_compare( InputIt1, InputIt1, InputIt2, InputIt2 )
:LI.lexicographical_compare( InputIt1, InputIt1, InputIt2, InputIt2, Compare )
:LI.next_permutation( BidirectionalIterator, BidirectionalIterator )
:LI.next_permutation( BidirectionalIterator, BidirectionalIterator, Compare )
:LI.prev_permutation( BidirectionalIterator, BidirectionalIterator )
:LI.prev_permutation( BidirectionalIterator, BidirectionalIterator, Compare )
:eUL.

Completed members:
:UL.
:LI.for_each( InputIterator, InputIterator, Function )
:LI.find( InputIterator, InputIterator, const Type & )
:LI.find_if( InputIterator, InputIterator, Predicate )
:LI.find_end(FwdIterator1, FwdIterator1, FwdIterator2 , FwdIterator2 )
:LI.find_end(FwdIt1, FwdIt1, FwdIt2 , FwdIt2, BinaryPredicate )
:LI.find_first_of(FwdIterator1, FwdIterator1, FwdIterator2 , FwdIterator2 )
:LI.find_first_of(FwdIt1, FwdIt1, FwdIt2 , FwdIt2, BinaryPredicate )
:LI.equal( InputIterator1, InputIterator1, InputIterator2 )
:LI.equal( InputIterator1, InputIterator1, InputIterator2, BinaryPredicate )
:LI.copy( InputIterator, InputIterator, OutputIterator )
:LI.copy_backward( Bidirectional1, Bidirectional1, Bidirectional2 )
:LI.swap( Type &, Type & )
:LI.swap_ranges( ForwardIterator1, ForwardIterator1, ForwardIterator2 )
:LI.iter_swap( ForwardIterator1, ForwardIterator2 )
:LI.transform( InputIterator, InputIterator, OutputIterator, UnaryOperation )
:LI.transform( Input1, Input1, Input2, Output, BinaryOperation )
:LI.replace( ForwardIterator, ForwardIterator, const Type &, const Type & )
:LI.replace_if( ForwardIterator, ForwardIterator, Predicate, const Type & )
:LI.replace_copy( Input, Input, Output, const Type &, const Type & )
:LI.replace_copy_if( Input, Input, Output, Predicate, const Type & )
:LI.fill( ForwardIterator, ForwardIterator, const Type & )
:LI.fill_n( OutputIterator, Size, const Type & )
:LI.generate( ForwardIterator, ForwardIterator, Generator )
:LI.generate_n( OutputIterator, Size, Generator )
:LI.remove_copy( FwdIterator, FwdIterator, OutputIterator, Type const & )
:LI.remove_copy_if( FwdIterator, FwdIterator, OutputIterator, Predictate )
:LI.remove( ForwardIterator, ForwardIterator, Type const & )
:LI.remove_if( ForwardIterator, ForwardIterator, Predictate )
:LI.reverse( Bidirectional, Bidirectional )
:LI.reverse_copy( Bidirectional, Bidirectional, OutputIterator )
:LI.random_shuffle( RandomAccessIterator, RandomAccessIterator )
:LI.random_shuffle( RAIterator, RAIterator, RandomNumberGenerator& rand )
:LI.push_heap( RAIterator, RAIterator )
:LI.pop_heap( RAIterator, RAIterator )
:LI.make_heap( RAIterator, RAIterator )
:LI.sort_heap( RAIterator, RAIterator )
:LI.sort( RandomAccessIterator, RandomAccessIterator )
:LI.min( const Type &, const Type & )
:LI.max( const Type &, const Type & )
:LI.min( const Type &, const Type &, Compare )
:LI.max( const Type &, const Type &, Compare )
:LI.min_element( ForwardIterator, ForwardIterator )
:LI.min_element( ForwardIterator, ForwardIterator, Compare )
:LI.max_element( ForwardIterator, ForwardIterator )
:LI.max_element( ForwardIterator, ForwardIterator, Compare )
:eUL.

Written but not activated/finished:
:UL.
:LI.count( InputIterator, InputIterator, const Type & )
:LI.count_if( InputIterator, InputIterator, Predicate )
:eUL.

:H1.Design Details
:P.
Most of the standard algorithms are template functions that operate on
iterators to perform some common task. Each funtion template is quickly
addressed in the sections that follow. They are generally quite simple
and looking directly at the source may be the simplest form of
information.

:H2.*_heap
:P.
The functions
:CODE.push_heap, pop_heap, make_heap,
and
:CODE.sort_heap
support the manipulation of a heap data structure. Currently only
versions using an explicit
:CODE.operator<
have been implemented. The versions taking a comparison object
have yet to be created. Several heap related helper functions have been
implemented in namespace
:CODE.std::_ow.
These functions are not intended for general use.

:P.
There is a compiler bug that prevents the signature of the internal
:CODE.heapify
function from compiling. This has been worked around by providing the
necessary type as an additional template parameter. See the comments in
:PATH.algorith.mh
for more information.

:H2.remove remove_if
:P.
These functions :Q.remove:eQ. the value that compares equal or the
element at which the predicate evaluates != false. Because iterators
can't be used to access the underlying container the element can't
really be removed. It may well be the container is just a bit of stack
and the iterator a pointer. These functions instead copy elements from
the right (an incremented iterator) over the top of the element that is
:Q.removed:eQ. and then return an iterator identifying the new end of
the sequence. The initial implementation just called the
:CODE.remove_copy
and
:CODE.remove_copy_if
functions described below. This would perform unnecessary copies on top
of the same object if there any values at the begining of the container
that aren't to be removed. This could cause a bit of performance hit if
the object is large and there are lots of objects that don't need to be
removed, therefore these functions were re-written to be independent of
the the
:CODE.*_copy
versions and perform a check for this condition.

:H2.remove_copy remove_copy_if
:P.
This makes a copy of the elements that don't compare equal, or when the
predictate is false, starting at the location given by OutputIterator.
It is a simple while loop over the input iterator first to last, either
just skipping the element or copying it to the output.

:H2.find_first_of
:P.
There are two versions of this, one that uses
:CODE.operator==
and one that uses a binary predicate. There is a simple nested loop to
compare each element with each element indexed by the 2nd iterator
range.

:H2.find_end
:P.
There are two versions of this, one that uses
:CODE.operator==
and one that uses a binary predicate. The main loop executes two other
loops. The first loop finds an input1 element that matches the first
input2 element. When a match is found the second loop then checks to see
if it is complete match for the subsequence. If it is, the position the
subsequence started is noted and the main loop is iterated as there may
be another match later on. Note this can't search for the substring
backwards as the iterators are ForwardIterators.

:H2.random_shuffle
:P.
The
:CODE.random_shuffle
template with two arguments has been implemented using the C library
function
:CODE.rand.
However, the 1998/2003 standard is unclear about the source of random
numbers that
:CODE.random_shuffle
should use. There is an open library issue about this with the C++
standardization group. See
http://anubis.dkuug.dk/JTC1/SC22/WG21/docs/lwg-active.html, item #395.
The proposed resolution is to allow the implementation to use
:CODE.rand
without requiring it to do so (the source of random numbers is proposed
to be implementation defined).

:P.
The problem with
:CODE.rand
in this case is that Open Watcom's implementation of
:CODE.rand
is limited to 16 bits of output even on 32 bit platforms. This means
that
:CODE.random_shuffle
will malfunction on sequences larger than 32K objects. This is a problem
that needs to be resolved. The solution, probably, will be to provide 32
bit random number generators as an option (check this: has it already
been done?)

:H2.sort
:P.
The
:CODE.sort
template is implemented in terms of the standard heap support functions.
As a result the sorting algorithm being used is Heap-Sort. This
algorithm provides the required running time (even in the worst case)
but may have more overhead than the more traditional Quick-Sort.
Modifying the implementation of
:CODE.sort
at some future time to Quick-Sort might be worthwhile (be sure to
measure the performance before and after to verify that an improvement
actually occcurs).

:H2.add quick descriptions of other algorithms here...
:P.
