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
* Description:  Functions controlling GUI attributes.
*
****************************************************************************/


#include "gui.h"
#include "guidlg.h"
#include "guistr.h"
#include "setup.h"
#include "setupinf.h"
#include "resource.h"
#include <string.h>
extern char *GetVariableStrVal( const char *vbl_name );
extern gui_colour_set   MainColours[];
extern  void            GUISetJapanese();
extern int              Invisible;
gui_window      *MainWnd = NULL;
int             NominalButtonWidth = 11;

char *Bolt[] = {
"",
"               ________                          ________              ",
"              /:::::::/                         /:::::::/              ",
"             /:::::::/__                       /:::::::/__             ",
"            /::::::::::/                      /::::::::::/             ",
"            -----/::::/__                     -----/::::/__            ",
"                /......./                         /......./            ",
"               -----/./                          -----/./              ",
"                   /./                               /./               ",
"                  //                                //                 ",
"                 /                                 /                   ",
"                                                                       ",
"                ________                          ________             ",
"               /:::::::/                         /:::::::/             ",
"              /:::::::/__                       /:::::::/__            ",
"             /::::::::::/                      /::::::::::/            ",
"             -----/::::/__                     -----/::::/__           ",
"                 /......./                         /......./           ",
"                -----/./                          -----/./             ",
"                   /./                               /./               ",
"                  //                                //                 ",
"                 /                                 /                   ",
"                                                                       "
};

#define WND_APPROX_SIZE 10000


gui_resource WndGadgetArray[] = {
    BITMAP_SPLASH, "splash",
};

gui_ord BitMapBottom;
gui_coord               BitMapSize;

extern GUICALLBACK WndMainEventProc;
bool WndMainEventProc( gui_window * gui, gui_event event, void *parm )
{
    int                 i;

    parm=parm;
    switch( event ) {
    case GUI_PAINT:
        if( GUIIsGUI() ) {
            gui_rect            rect;
            gui_text_metrics    metrics;
            gui_ord             indent;
            gui_ord             topdent;
            int                 row;

            GUIGetClientRect( gui, &rect );
            GUIGetTextMetrics( gui, &metrics );
            indent = ( rect.width - BitMapSize.x ) / 2;
            if( BitMapSize.x > rect.width )
                indent = 0;
            topdent = metrics.avg.y; // ( rect.height - BitMapSize.y ) / 2;
            BitMapBottom = BitMapSize.y + metrics.avg.y;
            if( BitMapSize.y > rect.height )
                topdent = 0;
            row = topdent / metrics.max.y;
            GUIDrawHotSpot( gui, 1, row, indent, GUI_BACKGROUND );
        } else {
            for( i = 0; i < sizeof( Bolt ) / sizeof( Bolt[0] ); ++i ) {
                GUIDrawTextExtent( gui, Bolt[i], strlen( Bolt[i] ), i, 0, GUI_BACKGROUND, WND_APPROX_SIZE );
            }
        }
        break;
    default:
        break;
    }
    return( TRUE );
}

gui_coord               GUIScale;

extern bool SetupInit( void )
/***************************/
{
    gui_rect            rect;
    gui_create_info     init;

    // Cancel button may be wider in other languages
    NominalButtonWidth = strlen( LIT( Cancel ) ) + 5;

    GUIWndInit( 300, GUI_PLAIN ); // 300 uS mouse dbl click rate, graphics mouse
    GUISetCharacter( GUI_SCROLL_SLIDER, 177 );
    GUISetBetweenTitles( 2 );
    GUIScale.x = WND_APPROX_SIZE;
    GUIScale.y = WND_APPROX_SIZE;
    GUIGetRoundScale( &GUIScale );
    rect.x = 0;
    rect.y = 0;
    rect.width = GUIScale.x;
    rect.height = GUIScale.y;
    GUISetScale( &rect );

    memset( &init, 0, sizeof( init ) );
    init.rect = rect;
    init.scroll = 0;
    init.style = 0;
    init.text = "";
    if( Invisible ) {
        init.style |= GUI_INIT_INVISIBLE;
    } else {
        init.style |= GUI_VISIBLE | GUI_MAXIMIZE | GUI_MINIMIZE;
    }
#ifdef _UI
    init.style |= GUI_NOFRAME;
#endif
    init.parent = NULL;
    init.num_menus = 0;
    init.menu = NULL;
    init.num_attrs = WND_NUMBER_OF_COLORS;
    init.colours = MainColours;
    init.call_back = WndMainEventProc;
    init.extra = NULL;

    GUIInitHotSpots( 1, WndGadgetArray );
    GUIGetHotSpotSize( 1, &BitMapSize );
    MainWnd = GUICreateWindow( &init );

#if defined(__NT__)
    /*
     * GUI Toolkit now works such that default system colors are used instead of
     * RGB colors. Since we really want nice blue background, we have to hack
     * around that by specifying a specific RGB color and re-setting the background
     * to use that color.
     */
    GUISetRGB( GUI_BRIGHT_BLUE, 0x00ff0000 );
    GUISetWndColour( MainWnd, GUI_BACKGROUND, &MainColours[GUI_BACKGROUND] );
#endif

    return( TRUE );
}


extern void SetupTitle()
/**********************/
{
    char        buff[MAXBUF];

    ReplaceVars( buff, GetVariableStrVal( "Appname" ) );
    GUISetWindowText( MainWnd, buff );
}


extern void SetupFini( void )
/***************************/
{
    if( MainWnd != NULL ) GUIDestroyWnd( MainWnd );
}


extern void SetupError( char *msg )
/*********************************/
{
//    MsgBox( NULL, "IDS_ERROR", GUI_OK, msg );
    MsgBox( NULL, msg, GUI_OK );
}
