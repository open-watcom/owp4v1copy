/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Conversion from string to tbyte format (long double)
*                      it doesn't depend on platform
*
****************************************************************************/

#include <string.h>
#include <ctype.h>
#include "tbyte.h"
#include "watcom.h"

typedef unsigned __int64 u64;

typedef union {
    u64 m64[2];
    unsigned_32 m32[4];
} b128;

#define EXPONENT_BIAS 0x3FFF

#define SET_SIGN(x,y) if( y < 0 ) x->e |= 0x8000

#define MAX_EXP_INDEX 13

TB_LD tab_plus_exp[MAX_EXP_INDEX] = {
    { 0xA000000000000000UL, 0x4002 }, // 1e1L
    { 0xC800000000000000UL, 0x4005 }, // 1e2L
    { 0x9C40000000000000UL, 0x400C }, // 1e4L
    { 0xBEBC200000000000UL, 0x4019 }, // 1e8L
    { 0x8E1BC9BF04000000UL, 0x4034 }, // 1e16L
    { 0x9DC5ADA82B70B59EUL, 0x4069 }, // 1e32L
    { 0xC2781F49FFCFA6D5UL, 0x40D3 }, // 1e64L
    { 0x93BA47C980E98CE0UL, 0x41A8 }, // 1e128L
    { 0xAA7EEBFB9DF9DE8EUL, 0x4351 }, // 1e256L
    { 0xE319A0AEA60E91C7UL, 0x46A3 }, // 1e512L
    { 0xC976758681750C17UL, 0x4D48 }, // 1e1024L
    { 0x9E8b3B5DC53D5DE5UL, 0x5A92 }, // 1e2048L
    { 0xC46052028A20979BUL, 0x7525 }  // 1e4096L
};

TB_LD tab_minus_exp[MAX_EXP_INDEX] = {
    { 0xCCCCCCCCCCCCCCCDUL, 0x3FFB }, // 1e-1L
    { 0xA3D70A3D70A3D70AUL, 0x3FF8 }, // 1e-2L
    { 0xD1B71758E219652CUL, 0x3FF1 }, // 1e-4L
    { 0xABCC77118461CEFDUL, 0x3FE4 }, // 1e-8L
    { 0xE69594BEC44DE15BUL, 0x3FC9 }, // 1e-16L
    { 0xCFB11EAD453994BAUL, 0x3F94 }, // 1e-32L
    { 0xA87FEA27A539E9A5UL, 0x3F2A }, // 1e-64L
    { 0xDDD0467C64BCE4A1UL, 0x3E55 }, // 1e-128L
    { 0xC0314325637A193AUL, 0x3CAC }, // 1e-256L
    { 0x9049EE32DB23D21CUL, 0x395A }, // 1e-512L
    { 0xA2A682A5DA57C0BEUL, 0x32B5 }, // 1e-1024L
    { 0xCEAE534F34362DE4UL, 0x256B }, // 1e-2048L
    { 0xA6DD04C8D2CE9FDEUL, 0x0AD8 }  // 1e-4096L
};

int check_u64_overflow( u64 *x, unsigned int c) {
    if( *x > 0xffffffffffffffffUL / 10UL ) {
        return 1;
    } else {
        *x *= 10;
         if ( 0xffffffffffffffffUL - *x >= c ) {
             *x += c;
         } else {
             *x = 0xffffffffffffffffUL;
         }
         return 0;
    }
}

int bitsize64(u64 x) {
    int i;
    
    for( i = 64; i > 0 ; i-- ) {
        if( x & 0x8000000000000000UL ) break;
        x <<= 1;
    }
    return i;
}

int U64LD(u64 u64_value, TB_LD *ld) {
    int bs;

    bs = bitsize64(u64_value);
    if( bs == 0 ) {
        ld->e = 0;
    } else {
        ld->e = bs - 1 + EXPONENT_BIAS;
        u64_value <<= 64 - bs;
    }
    ld->m = u64_value;
    return 0;
}

int normalize(b128 *r1) {
    int bs;

    bs = bitsize64(r1->m64[1]);
    if( bs == 0 ) {
        r1->m64[1] = r1->m64[0];
        r1->m64[0] = 0;
        bs = bitsize64(r1->m64[1]);
        return -64;
    }
    if( bs > 0 && bs < 64 ) {
        r1->m64[1] <<= 64 - bs;
        r1->m64[1] |= r1->m64[0] >> bs;
        return bs - 64;
    }
    return 0;
}

int add128(b128 *r1, u64 x1) {
    u64 cy;
    int i;

    cy = ((b128*)&x1)->m32[0];
    for( i = 1; i < 4; i++ ) {
        cy = r1->m32[i] + cy;
        r1->m32[i] = cy;
        cy >>= 32;
    }
    cy = ((b128*)&x1)->m32[1];
    for( i = 2; i < 4; i++ ) {
        cy = r1->m32[i] + cy;
        r1->m32[i] = cy;
        cy >>= 32;
    }
    return 0;
}

int TB_FLDM(TB_LD *op1, TB_LD *op2, TB_LD *res) {
    u64 x1;
    b128 r1;
    long exp;

    exp = (long)(op1->e & 0x7fff) + (long)(op2->e & 0x7fff) - EXPONENT_BIAS + 1;
    r1.m64[0] = (u64)(((b128*)&op1->m)->m32[0]) * (u64)(((b128*)&op2->m)->m32[0]);
    r1.m64[1] = (u64)(((b128*)&op1->m)->m32[1]) * (u64)(((b128*)&op2->m)->m32[1]);
    x1 = (u64)(((b128*)&op1->m)->m32[1]) * (u64)(((b128*)&op2->m)->m32[0]);
    add128(&r1, x1);
    x1 = (u64)(((b128*)&op1->m)->m32[0]) * (u64)(((b128*)&op2->m)->m32[1]);
    add128(&r1, x1);
    exp += normalize(&r1);
    if( r1.m32[1] & 0x80000000 ) {
        x1 = 0x100000000UL;
        add128(&r1, x1);
    }
    res->m = r1.m64[1];
    res->e = exp;
    return 0;
}

int TB_create(u64 u64_value, long exp_value, TB_LD *ld) {
    TB_LD *tabExp;
    int i;

    if( exp_value < 0 ) {
        exp_value = -exp_value;
        tabExp = tab_minus_exp;
    } else {
        tabExp = tab_plus_exp;
    }
    U64LD(u64_value, ld);
    for( i = 0; i < MAX_EXP_INDEX; i++) {
        if ( exp_value & 1 ) {
            TB_FLDM(ld, tabExp + i, ld);
        }
        if( (exp_value >>= 1) == 0 ) break;
    }
    if( exp_value != 0 ) {
        // exponent overflow
    }
    return 0;
}

TB_LD * strtotb(char *p, TB_LD * ld, char negative) {
    int              sign = +1;
    int              exp_sign = +1;
    long             exp_value;
    long             u64_exponent;
    int              u64_overflow;
    u64              u64_value;
    long             exponent;
    u64              value;
    long             exp1;

    while ( isspace(*p) ) p++;
    switch (*p) {
    case '-':
        sign = -1;
    case '+':
        p++;
    default :
        break;
    }
    if( negative ) {
        sign = (sign > 0) ? -1 : +1;
    }
    value = 0;
    exponent = 0;
    exp1 = 0;
    u64_value = 0;
    u64_exponent = 0;
    u64_overflow = 0;
    while ( (unsigned int)(*p - '0') < 10u ) {
        if( u64_overflow ) {
            u64_exponent++;
        } else if( check_u64_overflow(&u64_value, *p - '0') ) {
            u64_overflow = 1;
        }
        if( *p == '0' ) {
            exp1++;
        } else {
            value = u64_value;
            exp1 = 0;
        }
        p++;
    }
    exponent = exp1;
    if ( *p == '.' ) {
        p++;
        while ( (unsigned int)(*p - '0') < 10u ) {
            if( u64_overflow == 0 ) {
                if( check_u64_overflow(&u64_value, *p - '0') ) {
                    u64_overflow = 1;
                } else {
                    u64_exponent--;
                }
                if( *p != '0' ) {
                    value = u64_value;
                    exponent = u64_exponent;
                }
            }
            p++;
        }
    }
    exp_value   = 0;
    if ( (*p | 0x20) == 'e' ) {
        switch (*++p) {
        case '-': 
            exp_sign = -1;
        case '+': p++;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            break;
        default : 
            ld->m = 0;
            ld->e = 0;
            SET_SIGN(ld, sign);
            return ld;
        }
        while ( (unsigned int)(*p - '0') < 10u )
            exp_value = 10 * exp_value + (*p++ - '0');
        if( exp_sign < 0 )
            exp_value = -exp_value;
    }
    exp_value += exponent;
    TB_create(value, exp_value, ld);
    SET_SIGN(ld, sign);
    return ld;
}
