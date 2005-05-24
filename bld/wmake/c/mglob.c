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
* Description:  Global variables for wmake.
*
****************************************************************************/


#if defined( DEVELOPMENT ) || defined( INTERNAL_VERSION )
    /* just because this is compiled with -d2 - we get all the debugging
     * info we'll ever need this way :)
     */
#   include "macros.h"
#   include "make.h"
#   include "massert.h"
#   include "mcache.h"
#   include "mmemory.h"
#   include "mexec.h"
#   include "mhash.h"
#   include "mmisc.h"
#   include "mrcmsg.h"
#   include "msg.h"
#   include "mparse.h"
#   include "mpreproc.h"
#   include "mstream.h"
#   include "msuffix.h"
#   include "mlex.h"
#   include "mtarget.h"
#   include "mtypes.h"
#   include "mupdate.h"
#   include "mvecstr.h"
#   include "msysdep.h"
#if defined( M_I86 ) || defined( M_I386 )
#   include "tinyio.h"
#endif
#else
#   include "mtypes.h"
#   include "make.h"
#endif


struct Glob Glob;

/*
 * Be careful that these declarations doesn't exceed 2048 characters.
 * Note that this is just one big string - not an array of strings.
 */
const char FAR *BuiltIns = {
    "__MAKEOPTS__=%s\n"
    "__MAKEFILES__=\n"
    "__VERSION__=14\n"
#ifdef DLLS_IMPLEMENTED
    "__LOADDLL__=\n"
#endif

#if defined( __DOS__ )
    "__MSDOS__=\n"

#elif defined( __NT__ )
    "__NT__=\n"
    #if defined(__386__)
        "__NT386__=\n"
    #elif defined(__AXP__)
        "__NTAXP__=\n"
    #else
        #error Unknown CPU architecture
    #endif

#elif defined( __OS2__ )
    "__OS2__=\n"

#elif defined( __QNX__ )
    "__QNX__=\n"
    "__UNIX__=\n"

#elif defined( __LINUX__ )
    "__LINUX__=\n"
    "__UNIX__=\n"
    #if defined(__386__)
        "__LINUX386__=\n"
    #elif defined(__PPC__)
        "__LINUXPPC__=\n"
    #elif defined(__MIPS__)
        "__LINUXMIPS__=\n"
    #else
        #error Unknown CPU architecture
    #endif

#endif
};

const char FAR *SuffixList = {
    ".SUFFIXES: "
        ".exe .nlm .dsk .lan .exp "             /* executable extensions */
        ".lib .obj "                            /* object file extensions */
        ".i "                                   /* preprocessor output exts */
        ".asm .c .cpp .cxx .cc .for .pas .cob " /* source file extensions */
        ".h .hpp .hxx .hh .fi .mif .inc\n"      /* include file extensions */
};

const char FAR *MSSuffixList = {
    ".SUFFIXES: "
        ".exe .obj .asm .c .cpp .cxx .bas .cbl .for .f .f90 .pas .res .rc"
};


const char FAR *UNIXSuffixList = {
    ".SUFFIXES: "
        ".exe .obj .c .y .l .f"
};

/*
 * Be careful that this doesn't exceed 2048 characters.  Note that this is just
 * one big string - not an array of strings.
 */
const char FAR* MSBuiltIn = {
    "AS=ml\n"
    "BC=bc\n"
    "CC=cl\n"
    "COBOL=cobol\n"
    "CPP=cl\n"
    "CXX=cl\n"
    "FOR=fl\n"
    "PASCAL=pl\n"
    "RC=rc\n"
    ".asm.exe:\n"
    "    $(AS) $(AFLAGS) $*.asm\n"
    ".asm.obj:\n"
    "    $(AS) $(AFLAGS) /c $*.asm\n"
    ".c.exe:\n"
    "    $(CC) $(CFLAGS) $*.c\n"
    ".c.obj:\n"
    "    $(CC) $(CFLAGS) /c $*.c\n"
    ".cpp.exe:\n"
    "    $(CPP) $(CPPFLAGS) $*.cpp\n"
    ".cpp.obj:\n"
    "    $(CPP) $(CPPFLAGS) /c $*.cpp\n"
    ".cxx.exe:\n"
    "    $(CXX) $(CXXFLAGS) $*.cxx\n"
    ".cxx.obj:\n"
    "    $(CXX) $(CXXFLAGS) $*.cxx\n"
    ".bas.obj:\n"
    "    $(BC) $(BFLAGS) $*.bas\n"
    ".cbl.exe:\n"
    "    $(COBOL) $(COBFLAGS) $*.cbl, $*.exe;\n"
    ".cbl.obj:\n"
    "    $(COBOL) $(COBFLAGS) $*.cbl;\n"
    ".f.exe:\n"
    "    $(FOR) $(FFLAGS) $*.f\n"
    ".f.obj:\n"
    "    $(FOR) /c $(FFLAGS) $*.f\n"
    ".f90.exe:\n"
    "    $(FOR) $(FFLAGS) $*.f90\n"
    ".f90.obj:\n"
    "    $(FOR) /c $(FFLAGS) $*.f90\n"
    ".for.exe:\n"
    "    $(FOR) $(FFLAGS) $*.for\n"
    ".for.obj:\n"
    "    $(FOR) /c $(FFLAGS) $*.for\n"
    ".pas.exe:\n"
    "    $(PASCAL) $(PFLAGS) $*.pas\n"
    ".pas.obj:\n"
    "    $(PASCAL) /c $(PFLAGS) $*.pas\n"
    ".rc.res:\n"
    "    $(RC) $(RFLAGS) /r $*\n"

};

const char FAR* UNIXBuiltIn = {
     "YACC=yacc\n"
     "YFLAGS=\n"
     "LEX=lex\n"
     "LFLAGS=\n"
     "LDFLAGS=\n"
     "CC=cl\n"
     "FC=fl\n"
     "CFLAGS=-nologo\n"
     ".c.exe:\n"
     "    $(CC) $(CFLAGS) $(LDFLAGS) $<\n"
     ".f.exe:\n"
     "    $(FC) $(FFLAGS) $(LDFLAGS) $<\n"
     ".c.obj:\n"
     "    $(CC) $(CFLAGS) -c $<\n"
     ".f.obj:\n"
     "    $(FC) $(FFLAGS) -c $<\n"
     ".y.obj:\n"
     "    $(YACC) $(YFLAGS) $<\n"
     "    $(CC) $(CFLAGS) -c y.tab.c\n"
     "    del y.tab.c\n"
     "    move y.tab.obj $@\n"
     ".l.obj:\n"
     "    $(LEX) $(LFLAGS) $<\n"
     "    $(CC) $(CFLAGS) -c lex.yy.c\n"
     "    del lex.yy.c\n"
     "    move lex.yy.obj $@\n"
     ".y.c:\n"
     "    $(YACC) $(YFLAGS) $<\n"
     "    move y.tab.c $@\n"
     ".l.c:\n"
     "    $(LEX) $(LFLAGS) $<\n"
     "    move lex.yy.c $@\n"
};

/*
 * This is the table indexed by users of the is... functions.
 * The program 'cretype.exe' is used to rebuild this table.
 */
const UINT8 IsArray[] = {
#include "isarray.gh"
};
