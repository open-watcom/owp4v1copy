#include "fail.h"

int main() {
    unsigned char       ch = 0xFF;
    unsigned short      s = 0xFFFF;
    unsigned int        i = 0xFFFFFFFF;
    unsigned __int8     i8 = ch;
    unsigned __int16    i16 = s;
    unsigned __int32    i32 = i;
    ch = i8;
    s = i16;
    i = i32;
    if( sizeof( char ) != sizeof( __int8 ) ) fail(__LINE__);
    if( sizeof( short ) != sizeof( __int16 ) ) fail(__LINE__);
    if( sizeof( int ) != sizeof( __int32 ) ) fail(__LINE__);
    _PASS;
}
