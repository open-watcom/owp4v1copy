/* Test C99 'for' loop declaration diagnostics */

/* correct usage */
int foo( void )
{
    int a = 0;
    int i = 4;

    for( int i = 2, j = 1; i < 10; ++i )
        a += i + j;     // i in scope opened by 'for'

    return( a - i );    // back to the i in function scope
}


int main( int argc, char **argv )
{
    int     z = 0;

    for( register int i = 1; i < 2; ++i ) ;     // OK
    for( auto int i = 1; i < 2; ++i ) ;         // OK
    for( double i = 1; i < 2; ++i ) ;           // OK
    for( extern int i; i < 2; ++i ) ;           // bad storage class
    for( static int i = 1; i < 2; ++i ) ;       // bad storage class
    for( typedef int i; z < 2; ++z ) ;          // bad storage class
    for( register i = 1; i < 2; ++i ) ;         // missing type
    for( int i = 1, j(); i < 2; ++i ) ;         // j is not object

    // TODO: more tests for structs, enums, and other such nonsense

    return( foo() );
}
