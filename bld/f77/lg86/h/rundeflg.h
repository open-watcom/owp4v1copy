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


gbl_defn unsigned_16    RTFlags;        // Run-time flags
gbl_defn obj_ptr        ExLinePtr;      // current line # executing
gbl_defn iocb           *AltIOCB;       // alternate IOCB (/DEBUG, error msgs)
gbl_defn pointer        RListFile;      // pointer to listing file at run-time
gbl_defn uint           TotTime;        // total execution or compile time
gbl_defn signed_32      StmtRem;        // statements remaining
gbl_defn uint           PagesOut;       // # of pages of output to unit 6
gbl_defn char           *MsgBuff;       // error msg parm buffer
gbl_defn uint           LinesOut;       // # lines on this page of unit 6
gbl_defn unsigned_8     ArgChkFlags;    // argument checking flags
#ifndef _SA_LIBRARY
gbl_defn int            __RetCode;      // return code from program
gbl_defn bool           Executing;      // indicates executing or not
#endif
