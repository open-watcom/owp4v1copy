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


enum {
  IO_ACCESS     = 1,    // ACCESS
  IO_ACTION,            // ACTION
  IO_BLANK,             // BLANK
  IO_BLOCKSIZE,         // BLOCKSIZE
  IO_CARRIAGECONTROL,   // CARRIAGECONTROL
  IO_DIRECT,            // DIRECT
  IO_END,               // END
  IO_ERR,               // ERR
  IO_EXIST,             // EXIST
  IO_FILE,              // FILE
  IO_FMT,               // FMT
  IO_FORM,              // FORM
  IO_FMTTED,            // FORMATTED
  IO_IOSTAT,            // IOSTAT
  IO_NAME,              // NAME
  IO_NAMED,             // NAMED
  IO_NXTREC,            // NEXTREC
  IO_NUMBER,            // NUMBER
  IO_OPENED,            // OPENED
  IO_REC,               // REC
  IO_RECL,              // RECL
  IO_RECORDTYPE,        // RECORDTYPE
  IO_SEQ,               // SEQUENTIAL
  IO_SHARE,             // SHARE
  IO_STATUS,            // STATUS
  IO_UNFMTD,            // UNFORMATTED
  IO_UNIT,              // UNIT

  IO_KW_MAX     = IO_UNIT,

//  end of io keywords

  IO_LIST_DIR,          // fmt=*
  IO_INTERNAL,          // internal file
  IO_NAMELIST           // NAMELIST-directed
};
