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


#include "spy.h"
#include <stdio.h>
#include <string.h>
#include <dde.h>

#ifdef __NT__
#define NT_MSGS
#endif

#define msgpick( a, b, c, d, e, f )     { a, b, c, d, e, f }

message near MessageArray[] =  {
    #include "spyarray.h"
};

WORD MessageArraySize = sizeof( MessageArray ) / sizeof( message );

message near EditMessageArray[] = {
    msgpick( 1, 0, EM_GETSEL,               "EM_GETSEL",                MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETSEL,               "EM_SETSEL",                MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_GETRECT,              "EM_GETRECT",               MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETRECT,              "EM_SETRECT",               MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETRECTNP,            "EM_SETRECTNP",             MC_CONTROL, 0L ),
#ifdef NT_MSGS
    msgpick( 1, 0, EM_SCROLL,               "EM_SCROLL",                MC_CONTROL, 0L ),
#endif
    msgpick( 1, 0, EM_LINESCROLL,           "EM_LINESCROLL",            MC_CONTROL, 0L ),
#ifdef NT_MSGS
    msgpick( 1, 0, EM_SCROLLCARET,          "EM_SCROLLCARET",           MC_CONTROL, 0L ),
#endif
    msgpick( 1, 0, EM_GETMODIFY,            "EM_GETMODIFY",             MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETMODIFY,            "EM_SETMODIFY",             MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_GETLINECOUNT,         "EM_GETLINECOUNT",          MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_LINEINDEX,            "EM_LINEINDEX",             MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETHANDLE,            "EM_SETHANDLE",             MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_GETHANDLE,            "EM_GETHANDLE",             MC_CONTROL, 0L ),
#ifdef NT_MSGS
    msgpick( 1, 0, EM_GETTHUMB,             "EM_GETTHUMB",              MC_CONTROL, 0L ),
#endif
    msgpick( 1, 0, EM_LINELENGTH,           "EM_LINELENGTH",            MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_REPLACESEL,           "EM_REPLACESEL",            MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_GETLINE,              "EM_GETLINE",               MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_LIMITTEXT,            "EM_LIMITTEXT",             MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_CANUNDO,              "EM_CANUNDO",               MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_UNDO,                 "EM_UNDO",                  MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_FMTLINES,             "EM_FMTLINES",              MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_LINEFROMCHAR,         "EM_LINEFROMCHAR",          MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETTABSTOPS,          "EM_SETTABSTOPS",           MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETPASSWORDCHAR,      "EM_SETPASSWORDCHAR",       MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_EMPTYUNDOBUFFER,      "EM_EMPTYUNDOBUFFER",       MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_GETFIRSTVISIBLELINE,  "EM_GETFIRSTVISIBLELINE",   MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETREADONLY,          "EM_SETREADONLY",           MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETWORDBREAKPROC,     "EM_SETWORDBREAKPROC",      MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_GETWORDBREAKPROC,     "EM_GETWORDBREAKPROC",      MC_CONTROL, 0L ),
#ifndef NT_MSGS
    msgpick( 1, 0, EM_GETPASSWORDCHAR,      "EM_GETPASSWORDCHAR",       MC_CONTROL, 0L )
#else
    msgpick( 1, 0, EM_GETPASSWORDCHAR,      "EM_GETPASSWORDCHAR",       MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETMARGINS,           "EM_SETMARGINS",            MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_GETMARGINS,           "EM_GETMARGINS",            MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_GETLIMITTEXT,         "EM_GETLIMITTEXT",          MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_POSFROMCHAR,          "EM_POSFROMCHAR",           MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_CHARFROMPOS,          "EM_CHARFROMPOS",           MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_SETIMESTATUS,         "EM_SETIMESTATUS",          MC_CONTROL, 0L ),
    msgpick( 1, 0, EM_GETIMESTATUS,         "EM_GETIMESTATUS",          MC_CONTROL, 0L )
#endif
};

WORD EditMessageArraySize = sizeof( EditMessageArray ) / sizeof( message );

message near ButtonMessageArray[] = {
    msgpick( 1, 0, BM_GETCHECK,     "BM_GETCHECK",      MC_CONTROL, 0L ),
    msgpick( 1, 0, BM_SETCHECK,     "BM_SETCHECK",      MC_CONTROL, 0L ),
    msgpick( 1, 0, BM_GETSTATE,     "BM_GETSTATE",      MC_CONTROL, 0L ),
    msgpick( 1, 0, BM_SETSTATE,     "BM_SETSTATE",      MC_CONTROL, 0L ),
#ifndef NT_MSGS
    msgpick( 1, 0, BM_SETSTYLE,     "BM_SETSTYLE",      MC_CONTROL, 0L )
#else
    msgpick( 1, 0, BM_SETSTYLE,     "BM_SETSTYLE",      MC_CONTROL, 0L ),
    msgpick( 1, 0, BM_CLICK,        "BM_CLICK",         MC_CONTROL, 0L ),
    msgpick( 1, 0, BM_GETIMAGE,     "BM_GETIMAGE",      MC_CONTROL, 0L ),
    msgpick( 1, 0, BM_SETIMAGE,     "BM_SETIMAGE",      MC_CONTROL, 0L ),
    msgpick( 1, 0, BM_SETDONTCLICK, "BM_SETDONTCLICK",  MC_CONTROL, 0L ),
#endif
};

WORD ButtonMessageArraySize = sizeof( ButtonMessageArray ) / sizeof( message );

message near StaticMessageArray[] = {
    msgpick( 1, 0, STM_SETICON,     "STM_SETICON",  MC_CONTROL, 0L ),
#ifndef NT_MSGS
    msgpick( 1, 0, STM_GETICON,     "STM_GETICON",  MC_CONTROL, 0L )
#else
    msgpick( 1, 0, STM_GETICON,     "STM_GETICON",  MC_CONTROL, 0L ),
    msgpick( 1, 0, STM_SETIMAGE,    "STM_SETIMAGE", MC_CONTROL, 0L ),
    msgpick( 1, 0, STM_GETIMAGE,    "STM_GETIMAGE", MC_CONTROL, 0L )
#endif
};

WORD StaticMessageArraySize = sizeof( StaticMessageArray ) / sizeof( message );

message near ListBoxMessageArray[] = {
    msgpick( 1, 0, LB_ADDSTRING,            "LB_ADDSTRING",             MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_INSERTSTRING,         "LB_INSERTSTRING",          MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_DELETESTRING,         "LB_DELETESTRING",          MC_CONTROL, 0L ),
#ifdef NT_MSGS
    msgpick( 1, 0, LB_SELITEMRANGEEX,       "LB_SELITEMRANGEEX",        MC_CONTROL, 0L ),
#endif
    msgpick( 1, 0, LB_RESETCONTENT,         "LB_RESETCONTENT",          MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SETSEL,               "LB_SETSEL",                MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SETCURSEL,            "LB_SETCURSEL",             MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETSEL,               "LB_GETSEL",                MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETCURSEL,            "LB_GETCURSEL",             MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETTEXT,              "LB_GETTEXT",               MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETTEXTLEN,           "LB_GETTEXTLEN",            MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETCOUNT,             "LB_GETCOUNT",              MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SELECTSTRING,         "LB_SELECTSTRING",          MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_DIR,                  "LB_DIR",                   MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETTOPINDEX,          "LB_GETTOPINDEX",           MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_FINDSTRING,           "LB_FINDSTRING",            MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETSELCOUNT,          "LB_GETSELCOUNT",           MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETSELITEMS,          "LB_GETSELITEMS",           MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SETTABSTOPS,          "LB_SETTABSTOPS",           MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETHORIZONTALEXTENT,  "LB_GETHORIZONTALEXTENT",   MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SETHORIZONTALEXTENT,  "LB_SETHORIZONTALEXTENT",   MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SETCOLUMNWIDTH,       "LB_SETCOLUMNWIDTH",        MC_CONTROL, 0L ),
#ifdef NT_MSGS
    msgpick( 1, 0, LB_ADDFILE,              "LB_ADDFILE",               MC_CONTROL, 0L ),
#endif
    msgpick( 1, 0, LB_SETTOPINDEX,          "LB_SETTOPINDEX",           MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETITEMRECT,          "LB_GETITEMRECT",           MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETITEMDATA,          "LB_GETITEMDATA",           MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SETITEMDATA,          "LB_SETITEMDATA",           MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SELITEMRANGE,         "LB_SELITEMRANGE",          MC_CONTROL, 0L ),
#ifdef NT_MSGS
    msgpick( 1, 0, LB_SETANCHORINDEX,       "LB_SETANCHORINDEX",        MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETANCHORINDEX,       "LB_GETANCHORINDEX",        MC_CONTROL, 0L ),
#endif
    msgpick( 1, 0, LB_SETCARETINDEX,        "LB_SETCARETINDEX",         MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETCARETINDEX,        "LB_GETCARETINDEX",         MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SETITEMHEIGHT,        "LB_SETITEMHEIGHT",         MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETITEMHEIGHT,        "LB_GETITEMHEIGHT",         MC_CONTROL, 0L ),
#ifndef NT_MSGS
    msgpick( 1, 0, LB_FINDSTRINGEXACT,      "LB_FINDSTRINGEXACT",       MC_CONTROL, 0L )
#else
    msgpick( 1, 0, LB_FINDSTRINGEXACT,      "LB_FINDSTRINGEXACT",       MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SETLOCALE,            "LB_SETLOCALE",             MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETLOCALE,            "LB_GETLOCALE",             MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_SETCOUNT,             "LB_SETCOUNT",              MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_INITSTORAGE,          "LB_INITSTORAGE",           MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_ITEMFROMPOINT,        "LB_ITEMFROMPOINT",         MC_CONTROL, 0L ),
    msgpick( 1, 0, LB_GETLISTBOXINFO,       "LB_GETLISTBOXINFO",        MC_CONTROL, 0L )
#endif
};

WORD ListBoxMessageArraySize = sizeof( ListBoxMessageArray ) / sizeof( message );

message near ComboBoxMessageArray[] = {
    msgpick( 1, 0, CB_GETEDITSEL,           "CB_GETEDITSEL",            MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_LIMITTEXT,            "CB_LIMITTEXT",             MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SETEDITSEL,           "CB_SETEDITSEL",            MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_ADDSTRING,            "CB_ADDSTRING",             MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_DELETESTRING,         "CB_DELETESTRING",          MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_DIR,                  "CB_DIR",                   MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETCOUNT,             "CB_GETCOUNT",              MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETCURSEL,            "CB_GETCURSEL",             MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETLBTEXT,            "CB_GETLBTEXT",             MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETLBTEXTLEN,         "CB_GETLBTEXTLEN",          MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_INSERTSTRING,         "CB_INSERTSTRING",          MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_RESETCONTENT,         "CB_RESETCONTENT",          MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_FINDSTRING,           "CB_FINDSTRING",            MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SELECTSTRING,         "CB_SELECTSTRING",          MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SETCURSEL,            "CB_SETCURSEL",             MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SHOWDROPDOWN,         "CB_SHOWDROPDOWN",          MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETITEMDATA,          "CB_GETITEMDATA",           MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SETITEMDATA,          "CB_SETITEMDATA",           MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETDROPPEDCONTROLRECT, "CB_GETDROPPEDCONTROLRECT",
             MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SETITEMHEIGHT,        "CB_SETITEMHEIGHT",         MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETITEMHEIGHT,        "CB_GETITEMHEIGHT",         MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SETEXTENDEDUI,        "CB_SETEXTENDEDUI",         MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETEXTENDEDUI,        "CB_GETEXTENDEDUI",         MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETDROPPEDSTATE,      "CB_GETDROPPEDSTATE",       MC_CONTROL, 0L ),
#ifndef NT_MSGS
    msgpick( 1, 0, CB_FINDSTRINGEXACT,      "CB_FINDSTRINGEXACT",       MC_CONTROL, 0L )
#else
    msgpick( 1, 0, CB_FINDSTRINGEXACT,      "CB_FINDSTRINGEXACT",       MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SETLOCALE,            "CB_SETLOCALE",             MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETLOCALE,            "CB_GETLOCALE",             MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETTOPINDEX,          "CB_GETTOPINDEX",           MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SETTOPINDEX,          "CB_SETTOPINDEX",           MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETHORIZONTALEXTENT,  "CB_GETHORIZONTALEXTENT",   MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SETHORIZONTALEXTENT,  "CB_SETHORIZONTALEXTENT",   MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETDROPPEDWIDTH,      "CB_GETDROPPEDWIDTH",       MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_SETDROPPEDWIDTH,      "CB_SETDROPPEDWIDTH",       MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_INITSTORAGE,          "CB_INITSTORAGE",           MC_CONTROL, 0L ),
    msgpick( 1, 0, CB_GETCOMBOBOXINFO,      "CB_GETCOMBOBOXINFO",       MC_CONTROL, 0L )
#endif
};

WORD ComboBoxMessageArraySize = sizeof( ComboBoxMessageArray ) / sizeof( message );

#ifdef NT_MSGS
message near ScrollBarMessageArray[] = {
    msgpick( 1, 0, SBM_SETPOS,              "SBM_SETPOS",           MC_CONTROL, 0L ),
    msgpick( 1, 0, SBM_GETPOS,              "SBM_GETPOS",           MC_CONTROL, 0L ),
    msgpick( 1, 0, SBM_SETRANGE,            "SBM_SETRANGE",         MC_CONTROL, 0L ),
    msgpick( 1, 0, SBM_SETRANGEREDRAW,      "SBM_SETRANGEREDRAW",   MC_CONTROL, 0L ),
    msgpick( 1, 0, SBM_GETRANGE,            "SBM_GETRANGE",         MC_CONTROL, 0L ),
    msgpick( 1, 0, SBM_ENABLE_ARROWS,       "SBM_ENABLE_ARROWS",    MC_CONTROL, 0L ),
    msgpick( 1, 0, SBM_SETSCROLLINFO,       "SBM_SETSCROLLINFO",    MC_CONTROL, 0L ),
    msgpick( 1, 0, SBM_GETSCROLLINFO,       "SBM_GETSCROLLINFO",    MC_CONTROL, 0L ),
    msgpick( 1, 0, SBM_GETSCROLLBARINFO,    "SBM_GETSCROLLBARINFO", MC_CONTROL, 0L )
};

WORD ScrollBarMessageArraySize = sizeof( ScrollBarMessageArray ) / sizeof( message );
#endif

