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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#define _DEVELOPMENT    _OFF    // are we developing?
#define _MEMTRACK       _OFF    // memory tracking during development?
#define _CSET           _ASCII  // character set?

#if defined( __386__ ) || defined( M_I86 )
  #if defined( __FPI__ )                // 8087 present?
    #define _8087               _ON
  #else
    #define _8087               _OFF
  #endif
#elif defined( __AXP__ )
#elif defined( __PPC__ )
#else
  #error Unknown platform
#endif

#if defined( __OS2__ )          // operating system?
  #define _OPSYS        _OS2
#elif defined( __NT__ )
  #define _OPSYS        _NT
#elif defined( __NETWARE__ )
  #define _OPSYS        _NETWARE
#elif defined( __LINUX__ )
  #define _OPSYS        _LINUX
#elif defined( __QNX__ )
  #define _OPSYS        _QNX
#else
  #define _OPSYS        _PCDOS
#endif

#if defined( __386__ )          // target machine?
  #define _TARGET       _80386
#elif defined( M_I86 )
  #define _TARGET       _8086
#elif defined( __AXP__ )
  #define _TARGET       _AXP
#elif defined( __PPC__ )
  #define _TARGET       _PPC
#else
  #error unknown platform
#endif

#define __INCL_ERRMSGS__        // include error messages

#define _SysMemAlloc    malloc  // define system memory manager
#define _SysMemFree     free

#if _TARGET != _AXP
#pragma aux _SysMemInit =
#pragma aux _SysMemFini =
#endif
