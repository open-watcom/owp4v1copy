
//
// This program illustrates what I [PeterC] believe to be a template bug
// in the compiler.
//
// The compiler says that it can't instantiate xyz::f and appears to
// consider its third parameter (the complicated one involving
// iterator_traits) to be an "error".
//
// When this matter is resolved the heap support in algorithm can be
// exposed and the corresponding tests in alg02.cpp can be uncommented.

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

  template< class Iterator >
  void f( Iterator first,
          Iterator last,
          typename iterator_traits< Iterator >::value_type x )
  {
    std::cout << "x = " << x << "\n";
  }

} // namespace xyz


int main( )
{
  char *p = "Hello";

  xyz::f( p, p + 5, 'x' );

  return( 0 );
}

