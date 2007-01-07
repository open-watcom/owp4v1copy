/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2007 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  wgml global variables.
*
****************************************************************************/


#ifndef GVARS_H_INCLUDED
#define GVARS_H_INCLUDED

#ifndef global
    #define global  extern
#endif

global  jmp_buf        *Environment;    // var for GSuicide()

global  char           *ScanCharPtr;   // used by character scanning routines
global  int             CurrChar;

global  int             SwitchChar;     // DOS switch character
global  char           *AltExt;         // alternate extension


global  FILELIST       *OPTFiles;       // Option files *.opt
global  char           *MasterFName;    // Primary input file name
global  char           *MasterFNameAttr;// Primary input file name attributes
global  FILECB         *FileCbs;        // GML files
global  char           *OutFile;        // output file name
global  char           *OutFileAttr;    // output file attributes (T:2222)


#define BUF_SIZE 1024

global  size_t          BufSize;
global  char           *Buffer;
global  char           *TokenBuf;
global  char           *TryFileName;
global  FILE           *Tryfp;

global  char           *Pathes;         // content of PATH Envvar
global  char           *GMLlibs;        // content of GMMLIB Envvar
global  char           *GMLincs;        // content of GMLINC Envvar

global  int             ErrCount;       // Overall Errrorcount
global  int             WngCount;       // Overall warning count



global  char            GMLChar;        // GML Keywword start char :
global  char            SCRChar;        // SCRIPT keywword start char .
global  char            CWSepChar;      // Control Word separator char ;
global  int             CPI;            // chars per inch
global  space_units     CPI_units;      // unit for chars per inch
global  int             LPI;            // lines per inch
global  space_units     LPI_units;      // unit for lines per inch
global  int             Bind;           // Bind value for even pages
global  space_units     Bind_units;     // unit for bind
global  int             BindOdd;        // Bind value for odd pages
global  space_units     BindOdd_units;  // unit for bind




global struct GlobalFlags {
    unsigned            quiet         : 1; // show Productinfo?
    unsigned            bannerprinted : 1; // Productinfo  shown
    unsigned            wscript       : 1; // enable WATCOM script extension (default)
    unsigned            free3         : 1;
    unsigned            free4         : 1;
    unsigned            free5         : 1;
    unsigned            free6         : 1;
    unsigned            free7         : 1;




} GlobalFlags;


#endif  /* GVARS_H_INCLUDED */

