
//
// This program illustrates what I [PeterC] believe to be a code
// generation bug in the compiler.
//
// The problem is inside the code generated for the template 'count'.
// The local variable 'number' is allocated at -04[ebp]. This is evident
// in the debugger; that location is given 'number's initial value and
// that location is the target of the 'inc' instruction that is used to
// increment 'number'. However, when the function returns, eax is loaded
// from -08[ebp]. Thus eax contains the wrong value on return and the
// calling program becomes confused. I'm using WinXP. To compile the
// test program I used: wcl386 -xs -d2 bug01.cpp.
//
// When this matter is resolved, uncomment the call to count_test in
// alg01.cpp (in function main).
//

#include <iostream>

#define typename

namespace xyz {
  struct input_iterator_tag  { };
  struct output_iterator_tag { };
  struct forward_iterator_tag       : public input_iterator_tag         { };
  struct bidirectional_iterator_tag : public forward_iterator_tag       { };
  struct random_access_iterator_tag : public bidirectional_iterator_tag { };

  template<class Iterator>
  struct iterator_traits {
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
    typedef typename Iterator::iterator_catagory iterator_catagory;
  };

  // template<>
  struct iterator_traits<char *> {
    typedef ptrdiff_t                  difference_type;
    typedef char                       value_type;
    typedef char                      *pointer;
    typedef char                      &reference;
    typedef random_access_iterator_tag iterator_catagory;
  };

  // Count
  template< class InputIterator, class Type >
  typename iterator_traits< InputIterator >::difference_type
    count( InputIterator first,
           InputIterator last,
           const Type &value )
  {
    typename iterator_traits< InputIterator >::difference_type number(0);
    while( first != last ) {
      if( *first == value ) ++number;
      ++first;
    }
    return( number );
  }
} // namespace xyz


bool count_test( )
{
  bool rc = true;

  char array[] = { 'a', 'b', 'b', 'd', 'e' };
  int n = -1;

  n = xyz::count( array, array + 5, 'b' );
  if( n != 2 ) {
    std::cout << "count FAIL 0001\n"; rc = false;
  }
  return( rc );
}

int main( )
{
  int rc = 0;
  try {
    if( !count_test( )      ) rc = 1;
  }
  catch( ... ) {
    std::cout << "Unexpected exception of unexpected type.\n";
    rc = 1;
  }

  return( rc );
}
