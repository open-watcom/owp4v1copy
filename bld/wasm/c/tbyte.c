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
* Description:  Conversion from string with real number to tbyte format
*
****************************************************************************/

// FIXME: long_double is not supported on all platforms as tbyte, it works with double

#include <ctype.h>
#include "tbyte.h"

long_double * strtotb(const char* s, long_double * ld, char** endptr, char negative) {
    const char    *p = s;
    int           sign = +1;
    unsigned int  expo;
    long_double   value;
    long_double   factor;
    long_double   *pfactor;
    long_double   ld1;
    long_double   const_10;
    long_double   const_0_1;

    // const_10 = 10.0L;
    __I4LD(10, &const_10);
    // const_0_1 = 0.1L;
    __I4LD(1, &ld1);
    __FLDD(&ld1, &const_10, &const_0_1);

    while ( isspace(*p) ) p++;

    switch (*p) {
    case '-': sign = -1;
    case '+': p++;
    default : break;
    }
    if( negative ) {
        sign = (sign > 0) ? -1 : +1;
    }

    // value = 0.0L;
    __I4LD(0, &value);
    while ( (unsigned int)(*p - '0') < 10u ) {
        // value = value * 10 + (*p++ - '0') * factor;
        __I4LD((*p++ - '0'), &ld1);
        __FLDM(&value, &const_10, &value);
        __FLDA(&value, &ld1, &value);
    }

    if ( *p == '.' ) {
        // factor = 1.0L;
        __I4LD(1, &factor);
        p++;
        while ( (unsigned int)(*p - '0') < 10u ) {
            // factor *= 0.1L;
            __FLDM(&factor, &const_0_1, &factor);
            // value += (*p++ - '0') * factor;
            __I4LD((*p++ - '0'), &ld1);
            __FLDM(&ld1, &factor, &ld1);
            __FLDA(&value, &ld1, &value);
        }
    }

    if ( (*p | 0x20) == 'e' ) {
        expo   = 0;
        pfactor = &const_10;
        switch (*++p) {
        case '-': pfactor = &const_0_1;
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
                  // value = 0.0L;
                  __I4LD(0, &value);
                  p = s;
                  goto done;
        }
        while ( (unsigned int)(*p - '0') < 10u )
            expo = 10 * expo + (*p++ - '0');
        while ( 1 ) {
            if ( expo & 1 )
                // value *= factor;
                __FLDM(&value, pfactor, &value);
            if ( (expo >>= 1) == 0 )
                break;
            // factor *= factor;
            __FLDM(pfactor, pfactor, pfactor);
        }
    }

done:
    if ( endptr != NULL )
        *endptr = (char*)p;

    // return value * sign;
    __I4LD(sign, &ld1);
    __FLDM(&value, &ld1, ld);
    return ld;
}
