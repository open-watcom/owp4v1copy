#include <stdio.h>
#include <inttypes.h>

void main()
{
    intmax_t    x, y;

    x = -500000000000;
    y = imaxabs( x );
    printf( "imaxabs(%"PRIdMAX") = %"PRIdMAX"\n", x, y );
}
