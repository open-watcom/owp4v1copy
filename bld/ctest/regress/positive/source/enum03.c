#include "fail.h"
#include <stdio.h>

typedef enum {
    en81_3 = -0x7F,
    en81_1 = 0x7F,
    en81_2 = -0x7F
} enum_s81;

typedef enum {
    en82_3 = 0x7F,
    en82_1 = 0xFF,
    en82_2 = 0x7F
} enum_u82;

typedef enum {
    en161_3 = -0x7F,
    en161_1 = 0xFF,
    en161_2 = -0x7F
} enum_s161;

typedef enum {
    en162_3 = -0x7FFF,
    en162_1 = 0x7FFF,
    en162_2 = -0x7FFF
} enum_s162;

typedef enum {
    en163_3 = 0x7FFF,
    en163_1 = 0xFFFF,
    en163_2 = 0x7FFF
} enum_u163;

typedef enum {
    en321_3 = -0x7FFF,
    en321_1 = 0xFFFF,
    en321_2 = -0x7FFF
} enum_s321;

typedef enum {
    en322_3 = -0x7FFFFFFF,
    en322_1 = 0x7FFFFFFF,
    en322_2 = -0x7FFFFFFF
} enum_s322;

typedef enum {
    en323_3 = 0x7FFFFFFF,
    en323_1 = 0xFFFFFFFF,
    en323_2 = 0x7FFFFFFF
} enum_u323;

typedef enum {
    en641_3 = -0x7FFFFFFF,
    en641_1 = 0xFFFFFFFF,
    en641_2 = -0x7FFFFFFF
} enum_s641;

typedef enum {
    en642_3 = -0x7FFFFFFFFFFFFFFF,
    en642_1 = 0x7FFFFFFFFFFFFFFF,
    en642_2 = -0x7FFFFFFFFFFFFFFF
} enum_s642;

typedef enum {
    en643_3 = -0x7FFFFFFFFFFFFFFF,
    en643_1 = 0xFFFFFFFFFFFFFFFF,
    en643_2 = -0x7FFFFFFFFFFFFFFF
} enum_u643;

typedef enum {
    x = 1UI64 << 30,
    y = 1UI64 << 31,
    z = 1UI64 << 32,
} my_enum;

typedef enum {
    x2 = 1I64 << 29,
    y2 = -1 << 15,
    z2 = 1I64 << 31,
} my_enum2;

int main( int argc, char **argv )
{
    __int64	i = 1UI64 << 30;
    __int64	j = 1UI64 << 31;
    __int64	k = 1UI64 << 32;

    __int64	i2 = 1I64 << 29;
    __int64	j2 = -1 << 15;
    __int64	k2 = 1I64 << 31;

#ifndef __SW_EI
    if( sizeof( enum_s81 ) != 1 ) fail(__LINE__);
    if( sizeof( enum_u82 ) != 1 ) fail(__LINE__);
#endif
#if !defined( __SW_EI ) || defined( __I86__ )
    if( sizeof( enum_s161 ) != 2 ) fail(__LINE__);
    if( sizeof( enum_s162 ) != 2 ) fail(__LINE__);
    if( sizeof( enum_u163 ) != 2 ) fail(__LINE__);
#endif
    if( sizeof( enum_s321 ) != 4 ) fail(__LINE__);
    if( sizeof( enum_s322 ) != 4 ) fail(__LINE__);
    if( sizeof( enum_u323 ) != 4 ) fail(__LINE__);
    if( sizeof( enum_s641 ) != 8 ) fail(__LINE__);
    if( sizeof( enum_s642 ) != 8 ) fail(__LINE__);
    if( sizeof( enum_u643 ) != 8 ) fail(__LINE__);
    if( i != x ) fail(__LINE__);
    if( j != y ) fail(__LINE__);
    if( k != z ) fail(__LINE__);

    if( i2 != x2 ) fail(__LINE__);
    if( j2 != y2 ) fail(__LINE__);
    if( k2 != z2 ) fail(__LINE__);

    _PASS;
}
