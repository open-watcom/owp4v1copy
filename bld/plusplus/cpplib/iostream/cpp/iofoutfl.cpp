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
* Description:  Converts a float value to string representation for stream
*               output.
*
****************************************************************************/

#ifdef __SW_FH
#include "iost.h"
#else
#include "variety.h"
#include <iostream>
#endif
#include "ioutil.h"
#include "lock.h"
#include "iofhdr.h"


enum {
    FLOAT_OVERHEAD  = 7,            // 2 signs, decimal, E, 3-digit exponent
    EXTRA_DIG       = 10,           // some extra for useless mantissa digits
    MAX_PREC        = LDBL_DIG + EXTRA_DIG,
    LONGEST_FLOAT   = LDBL_DIG + EXTRA_DIG + FLOAT_OVERHEAD,
    NEW_FLOAT_BUFSZ = 64            // just use a fixed buffer size
};

#define MAX_DIGITS LDBL_DIG+EXTRA_DIG

namespace std {

  // Write a "long double" floating-point value.

  ostream &ostream::__outfloat( long double const &f ) {

    int         digit_offset;
    int         precision;
    char        buf[NEW_FLOAT_BUFSZ+1];
    int         i, j, k;
    CVT_INFO    cvt;
    long_double ld;
    auto char   stkbuf[34];
#ifdef _LONG_DOUBLE_
    double      double_value;
#endif
    ::memset( buf, 0, sizeof( buf ) );
    __lock_it( __i_lock );
    precision = this->precision();
    if( precision > MAX_PREC ) 
        precision = MAX_PREC;
        
#ifdef _LONG_DOUBLE_
    /* convert this double into a long double */
    double_value = f;   //*f;
    __EFG__FDLD( (double _WCNEAR *)&double_value, (long_double _WCNEAR *)&ld );
#else
        ld.value = f;       //*f;
#endif  /* #fi 0 */
    
    if( (flags() & (std::ios::scientific|ios::fixed)) == std::ios::scientific ) {
        cvt.flags = E_FMT;
        cvt.scale = 1;
    } else if( (flags() & (std::ios::scientific|ios::fixed)) == std::ios::fixed ) {
        cvt.flags = F_FMT;
        cvt.scale = 0;
    } else {
        cvt.flags = G_FMT;
        cvt.scale = 1;
        if( precision == 0 ) {
            precision = 1;
        }
    }
    if( (flags() & std::ios::showpoint) ) {
        cvt.flags |= F_DOT;
    }
    cvt.ndigits = precision;
    cvt.expchar = (flags() & std::ios::uppercase) ? 'E' : 'e';
    cvt.expwidth = 0;

    /* Do the converstion to CVT_INFO struct */
    __EFG_LDcvt( &ld, &cvt, stkbuf );
    
    // put all the pieces together
    if( cvt.sign < 0 ) {
        buf[0] = '-';
        digit_offset = 1;
    } else if( flags() & std::ios::showpos ) {
        buf[0] = '+';
        digit_offset = 1;
    } else {
        digit_offset = 0;
    }
    
    if( cvt.n1 != 0 ) {
        memcpy( &buf[digit_offset], &stkbuf[0], cvt.n1 );
        buf[digit_offset + cvt.n1] = '\0';
    }
    
    /* moved above
    if( buf[0] == '-' || buf[0] == '+' ) {
        digit_offset = 1;
    } else {
        digit_offset = 0;
    } */
    
    if( opfx() ) {
        /* Write leading digits */
        setstate( __WATCOM_ios::writeitem( *this, buf, ::strlen(buf), digit_offset ) );
        if(!good()){
            osfx();
            return(*this);
        }
    
        /* Write trailing zeroes */
        j = cvt.nz1 / (sizeof(buf) - 1);
        k = cvt.nz1 % (sizeof(buf) - 1);
        for( i=0 ; i<j ; i++ ){
            memset(buf, '0', (sizeof(buf) - 1));
            buf[sizeof(buf) - 1] = '\0';
            setstate( __WATCOM_ios::writeitem( *this, buf, ::strlen(buf), 0 ) );
            if(!good()){
                osfx();
                return(*this);
            }
        }
        memset(buf, '0', k);
        buf[k] = '\0';
        setstate( __WATCOM_ios::writeitem( *this, buf, ::strlen(buf), 0 ) );
        if(!good()){
            osfx();
            return(*this);
        }

        /* Write next set of digits (and decimal point) */
        if( cvt.n2 != 0 ) {
            memcpy( buf, &stkbuf[cvt.n1], cvt.n2 );
            buf[cvt.n2] = '\0';
            setstate( __WATCOM_ios::writeitem( *this, buf, ::strlen(buf), 0 ) );
            if(!good()){
                osfx();
                return(*this);
            }
        }

        /* Write trailing zeroes */
        j = cvt.nz2 / (sizeof(buf) - 1);
        k = cvt.nz2 % (sizeof(buf) - 1);
        for( i=0 ; i<j ; i++ ){
            memset(buf, '0', (sizeof(buf) - 1));
            buf[sizeof(buf) - 1] = '\0';
            setstate( __WATCOM_ios::writeitem( *this, buf, ::strlen(buf), 0 ) );
            if(!good()){
                osfx();
                return(*this);
            }
        }
        
        memset(buf, '0', k);
        buf[k] = '\0';
        setstate( __WATCOM_ios::writeitem( *this, buf, ::strlen(buf), 0 ) );
        if(!good()){
            osfx();
            return(*this);
        }

        // suffix and out
        osfx();
    } /* fi opfx() */

    return( *this );
  }

}   /* end namespace std */

