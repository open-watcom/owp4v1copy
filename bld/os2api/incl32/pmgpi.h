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
* Description:  OS/2 Graphics Programming Interface include file
*               for 32-bit development.
*
****************************************************************************/


#define INCL_GPIINCLUDED

#ifdef INCL_GPI
    #define INCL_GPILCIDS
    #define INCL_GPIPRIMITIVES
#endif

#if defined(INCL_GPILCIDS)

#define QF_PUBLIC     1
#define QF_PRIVATE    2
#define QF_NO_GENERIC 4
#define QF_NO_DEVICE  8

LONG   APIENTRY GpiCreateLogFont(HPS hps, PSTR8 pName, LONG lLcid, PFATTRS pfatAttrs);

LONG   APIENTRY GpiQueryFonts(HPS hps, ULONG flOptions, PCSZ pszFacename, PLONG plReqFonts,
                   LONG lMetricsLength, PFONTMETRICS afmMetrics);

#endif

#if defined(INCL_GPIPRIMITIVES)

#define CLR_ERROR      (-255)
#define CLR_FALSE        (-5)
#define CLR_TRUE         (-4)
#define CLR_DEFAULT      (-3)
#define CLR_WHITE        (-2)
#define CLR_BLACK        (-1)
#define CLR_BACKGROUND     0
#define CLR_BLUE           1
#define CLR_RED            2
#define CLR_PINK           3
#define CLR_GREEN          4
#define CLR_CYAN           5
#define CLR_YELLOW         6
#define CLR_NEUTRAL        7
#define CLR_DARKGRAY       8
#define CLR_DARKBLUE       9
#define CLR_DARKRED       10
#define CLR_DARKPINK      11
#define CLR_DARKGREEN     12
#define CLR_DARKCYAN      13
#define CLR_BROWN         14
#define CLR_PALEGRAY      15

#define FM_ERROR         (-1)
#define FM_DEFAULT         0
#define FM_OR              1
#define FM_OVERPAINT       2
#define FM_XOR             4
#define FM_LEAVEALONE      5
#define FM_AND             6
#define FM_SUBTRACT        7
#define FM_MASKSRCNOT      8
#define FM_ZERO            9
#define FM_NOTMERGESRC    10
#define FM_NOTXORSRC      11
#define FM_INVERT         12
#define FM_MERGESRCNOT    13
#define FM_NOTCOPYSRC     14
#define FM_MERGENOTSRC    15
#define FM_NOTMASKSRC     16
#define FM_ONE            17

#define BM_ERROR          (-1)
#define BM_DEFAULT          0
#define BM_OR               1
#define BM_OVERPAINT        2
#define BM_XOR              4
#define BM_LEAVEALONE       5
#define BM_AND              6
#define BM_SUBTRACT         7
#define BM_MASKSRCNOT       8
#define BM_ZERO             9
#define BM_NOTMERGESRC     10
#define BM_NOTXORSRC       11
#define BM_INVERT          12
#define BM_MERGESRCNOT     13
#define BM_NOTCOPYSRC      14
#define BM_MERGENOTSRC     15
#define BM_NOTMASKSRC      16
#define BM_ONE             17
#define BM_SRCTRANSPARENT  18
#define BM_DESTTRANSPARENT 19

#define TXTBOX_TOPLEFT     0
#define TXTBOX_BOTTOMLEFT  1
#define TXTBOX_TOPRIGHT    2
#define TXTBOX_BOTTOMRIGHT 3
#define TXTBOX_CONCAT      4
#define TXTBOX_COUNT       5

BOOL   APIENTRY GpiBeginArea(HPS hps, ULONG flOptions);
LONG   APIENTRY GpiBox(HPS hps, LONG lControl, PPOINTL pptlPoint, LONG lHRound, LONG lVRound);
LONG   APIENTRY GpiCharString(HPS hps, LONG lCount, PCH pchString);
LONG   APIENTRY GpiCharStringAt(HPS hps, PPOINTL pptlPoint, LONG lCount, PCH pchString);
LONG   APIENTRY GpiCharStringPos(HPS hps, PRECTL prclRect, ULONG flOptions, LONG lCount,
                   PCH pchString, PLONG alAdx);
LONG   APIENTRY GpiCharStringPosAt(HPS hps, PPOINTL pptlStart, PRECTL prclRect,
                   ULONG flOptions, LONG lCount, PCH pchString, PLONG alAdx);
BOOL   APIENTRY GpiComment(HPS hps, LONG lLength, PBYTE pbData);
LONG   APIENTRY GpiEndArea(HPS hps);
LONG   APIENTRY GpiFullArc(HPS hps, LONG lControl, FIXED fxMultiplier);

BOOL   APIENTRY GpiQueryTextAlignment(HPS hps, PLONG plHoriz, PLONG plVert);
BOOL   APIENTRY GpiQueryTextBox(HPS hps, LONG lCount1, PCH pchString,
                   LONG lCount2, PPOINTL aptlPoints);

BOOL   APIENTRY GpiSetCharSet(HPS hps, LONG llcid);
BOOL   APIENTRY GpiSetCharShear(HPS hps, PPOINTL pptlAngle);
BOOL   APIENTRY GpiSetColor(HPS hps, LONG lColor);
BOOL   APIENTRY GpiSetCurrentPosition(HPS hps, PPOINTL pptlPoint);

#endif
