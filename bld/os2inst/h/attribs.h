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
* Description:  File attribute constants.
*
****************************************************************************/


#ifndef __attribs_h
#define __attribs_h


#ifndef _A_SUBDIR
  #define _A_SUBDIR 0x10        // MS-DOS directory constant
#endif

#ifndef S_IEXEC
  #ifdef S_IEXECUTE
    #define S_IEXEC S_IEXECUTE
  #else
    #define S_IEXEC 0
  #endif
#endif

#ifndef S_IDELETE
  #define S_IDELETE 0
#endif

#ifndef S_IRGRP
  #define S_IRGRP 0
  #define S_IWGRP 0
#endif

#ifndef S_IROTH
  #define S_IROTH 0
  #define S_IWOTH 0
#endif


#endif /* __attribs_h */

