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
* Description:  Determine the flavour du jour of editor.
*
****************************************************************************/


#include "control.h"
#if defined( __WINDOWS_386__ )
char    TITLE[] = "vi/win386";
#elif defined( __WINDOWS__ )
char    TITLE[] = "vi/win";
#elif defined( __OS2V2__ )
char    TITLE[] = "vi/os2v2";
#elif defined( __OS2__ )
char    TITLE[] = "vi/os2";
#elif defined( __NT__ )
#if defined( __WIN__ )
char    TITLE[] = "vi/ntwin";
#else
char    TITLE[] = "vi/nt";
#endif
#elif defined( __PHAR__ ) || defined( __4G__ )
char    TITLE[] = "vi/386";
#elif defined( __QNX__ )
char    TITLE[] = "vi/qnx";
#elif defined( __LINUX__ )
char    TITLE[] = "vi/linux";
#elif defined( __UNIX__ )
char    TITLE[] = "vi/unix";
#elif defined( __286__ )
char    TITLE[] = "vi/286";
#else
char    TITLE[] = "vi/86";
#endif
