/* Test pointer assignment warnings, especially null pointer assignments
 */


/* A litle extract from Windows 3.x headers */
#define NEAR                    __near
#define DECLARE_HANDLE(name)    struct name##__ { int unused; }; \
                                typedef const struct name##__ NEAR* name
DECLARE_HANDLE(HMENU);


#define NVOID   (void __near *)
#define FVOID   (void __far *)

void foo( HMENU *hm )
{
}

int main( int argc, char **argv )
{
    HMENU   hm;

    hm = 0;                 // OK
    hm = 0L;                // OK
    hm = NVOID 0;           // OK
    hm = NVOID (1 - 1);     // OK
    hm = NVOID 1;           // OK
    hm = FVOID 0;           // OK - in MSC since 6.0
    hm = FVOID (1 - 1);     // OK - also null pointer constant
    hm = FVOID 1;           // pointer truncated

    foo( 0 );               // OK
    foo( 0L );              // OK
    foo( NVOID 0 );         // OK
    foo( NVOID (1 - 1) );   // OK
    foo( NVOID 1 );         // OK
    foo( FVOID 0 );         // OK - in MSC since 6.0
    foo( FVOID (1 - 1) );   // OK - also null pointer constant
    foo( FVOID 1 );         // pointer truncated
    
    return( 0 );
}
