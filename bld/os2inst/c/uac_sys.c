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
* Description:  Basic UNACE system support stuff.
*
****************************************************************************/


#include "os.h"

#include <signal.h>  // signal()
#include <stdio.h>   // fprintf() fflush() getch() putc()
#include <conio.h>

#include "globals.h"
#include "uac_sys.h"


void memset16(USHORT * dest, SHORT val, INT len)  // fills short-array with
{                                                 // value
   while (len--)
      *(dest++) = val;
}

INT  cancel(void)               // checks whether to interrupt the program
{
#ifdef DOS
   while (kbhit())
   {
      if (getch() == 27)
         f_err = ERR_USER;
   }
#endif
   return (f_err);
}

INT  wrask(CHAR * s)            // prompt-routine
{
   INT  ch;

   fprintf(stderr, "\n %s (Yes,Always,No,Cancel) ", s);
   fflush(stderr);
   do
   {
      ch = getch();
      ch = upcase(ch);
   }
   while (ch != 'Y' && ch != 'A' && ch != 'N' && ch != 'C' && ch != 27);
   fprintf(stderr, "%s", ch == 'Y' ? "Yes" : (ch == 'A' ? "Always" : (ch == 'N' ? "No" : "Cancel")));
   fflush(stderr);
   return (ch == 'Y' ? 0 : (ch == 'A' ? 1 : (ch == 'N' ? 2 : 3)));
}

void beep(void)                           // makes some noise
{
#ifdef DOS
   sound(800);
   delay(250);
   nosound();
#endif
}

void my_signalhandler(INT sig_number)     // sets f_err if ctrl+c or ctrl+brk
{
   f_err = ERR_USER;
   error("\nUser break\n");
}

#ifdef DOS                                // handles hardware errors
#ifdef __BORLANDC__
INT harderrhandler(UINT deverr, UINT errc, UINT * devhdr)
#else
INT __far harderrhandler(UINT deverr, UINT errc, UINT * devhdr)
#endif
{
   f_criterr = 'A' + deverr & 0xff;
   f_err = ERR_OTHER;
   return (0x3);
}
#endif

void set_handler(void)                    // initializes handlers
{
#if defined(DOS) && !defined(__BORLANDC__)
   signal(SIGBREAK, my_signalhandler);    // set ctrl-break/-c handlers
#endif
   signal(SIGINT, my_signalhandler);
#if defined(DOS) && !defined(__CONSOLE__) // set hardware error handler
#ifdef __BORLANDC__
   harderr(harderrhandler);
#else
   _harderr(harderrhandler);
#endif
#endif
}

