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
* Description:  CRC routines.
*
****************************************************************************/


#include "uac_crc.h"


ULONG crctable[256];
ULONG rd_crc=0;


void make_crctable(void)   // initializes CRC table
{
   ULONG r,
        i,
        j;

   for (i = 0; i <= 255; i++)
   {
      for (r = i, j = 8; j; j--)
         r = (r & 1) ? (r >> 1) ^ CRCPOLY : (r >> 1);
      crctable[i] = r;
   }
}

// Updates crc from addr till addr+len-1
//
ULONG getcrc(ULONG crc, UCHAR * addr, INT len)
{
   while (len--)
      crc = crctable[(unsigned char) crc ^ (*addr++)] ^ (crc >> 8);
   return (crc);
}

