/* Test shift amout warnings. Need to test left/right shifts as well as
 * >>= and <<= operators. Must work with shift amounts which are constant
 * expressions and must work with shift expressions which are themselves
 * constant. Needs to be careful about long long shift amounts.
 */

int main( void )
{
    int         i  = 1;
    long        l  = 2;
    char        c  = 3;
    long long   ll = 4;

    i = i << -1;                        // negative shift
    i = 1 >> 20 * 10;                   // too big shift, will be const folded
    i = i << -3 + 8;                    // no problem here
    i <<= 0x100000002ll;                // 64-bit signed shift with high bits set
    i <<= 0x800000000ull;               // 64-bit unsigned shift with high bits set
    i >>= -3ll;                         // negative 64-bit signed shift
    i >>= -3ll + 5;                     // no problem here
    i = i << (sizeof( i ) * 8);         // too big shift
    l = l >> (sizeof( l ) * 8);         // too big shift
    c <<= sizeof( c ) * 8;              // loss of precision but shift OK
    i <<= sizeof( c ) * 8;              // no problem here ('c' converted to int)
    ll = ll << (sizeof( ll ) * 8ll);    // too big shift
    ll = ll << (sizeof( ll ) * 8 - 1);  // no problem here

    return( i );
}
