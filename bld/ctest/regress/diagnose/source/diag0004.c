/* Test comparison warnings. Tests for 'comparison is always 0/1'
 * and 'comparison equivalent to unsigned == 0' warnings.
 */

int main( int argc, char **argv )
{
    int                     ret;
    unsigned char           uc;
    unsigned short          us;
    unsigned int            ui;
    unsigned long           ul;
    unsigned long long      ull;

    ret = 0;
    uc = us = ui = ul = ull = argc;
    if( uc < 0 )    // always 0
        ret = 1;
    if( uc <= 0 )   // equivalent to uc == 0
        ret = 1;
    if( uc >= 0 )   // always 1
        ret = 1;
    if( uc > 0 )    // no problem here
        ret = 1;
    if( uc == 0 )   // no problem here
        ret = 1;
    if( uc != 0 )   // no problem here
        ret = 1;

    if( us < 0 )    // always 0
        ret = 1;
    if( us <= 0 )   // equivalent to uc == 0
        ret = 1;
    if( us >= 0 )   // always 1
        ret = 1;
    if( us > 0 )    // no problem here
        ret = 1;
    if( us == 0 )   // no problem here
        ret = 1;
    if( us != 0 )   // no problem here
        ret = 1;

    if( ui < 0 )    // always 0
        ret = 1;
    if( ui <= 0 )   // equivalent to uc == 0
        ret = 1;
    if( ui >= 0 )   // always 1
        ret = 1;
    if( ui > 0 )    // no problem here
        ret = 1;
    if( ui == 0 )   // no problem here
        ret = 1;
    if( ui != 0 )   // no problem here
        ret = 1;

    if( ul < 0 )    // always 0
        ret = 1;
    if( ul <= 0 )   // equivalent to uc == 0
        ret = 1;
    if( ul >= 0 )   // always 1
        ret = 1;
    if( ul > 0 )    // no problem here
        ret = 1;
    if( ul == 0 )   // no problem here
        ret = 1;
    if( ul != 0 )   // no problem here
        ret = 1;
    // The 64-bit comparisons aren't checked at the moment
    if( ull < 0 )   // always 0
        ret = 1;
    if( ull <= 0 )  // equivalent to uc == 0
        ret = 1;
    if( ull >= 0 )  // always 1
        ret = 1;
    if( ull > 0 )   // no problem here
        ret = 1;
    if( ull == 0 )  // no problem here
        ret = 1;
    if( ull != 0 )  // no problem here
        ret = 1;

    return( ui );
}
