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
* Description:  OS/2 Dialog related semantic actions.
*
****************************************************************************/


/*
    The data structures for OS/2 dialog templates are as follows:

struct DLGTEMPLATE {
    USHORT      cbTemplate;         // Template size
    USHORT      type;               // Dialog type - always zero?
    USHORT      codepage;           // Dialog codepage
    USHORT      offadlgti;          // Offset to first item
    USHORT      fsTemplateStatus;   // Template status flags
    USHORT      iItemFocus;         // Index to initial focus window
    USHORT      coffPresParams;     // Offset to presentation parameters
    DLGTIITEM   adlgti[1];          // Array of item templates
}

struct DLGTITEM {
    USHORT      fsItemStatus;       // Item status flags
    USHORT      cChildren;          // Number of child windows
    USHORT      cchClassName;       // Number of chars in class name
    USHORT      offClassName;       // Offset to class name
    USHORT      cchText;            // Number of chars in window text
    USHORT      offText;            // Offset to window text
    ULONG       flStyle;            // Window style
    USHORT      x;                  // X coordinate
    USHORT      y;                  // Y coordinate
    USHORT      cx;                 // Width
    USHORT      cy;                 // Height
    USHORT      id;                 // Window ID
    USHORT      offPresParams;      // Offset to presentation parameters
    USHORT      offCtlData;         // Offset to class specific data
}

*/

#include <stddef.h>
#include <string.h>
#include "wresall.h"
#include "rcmem.h"
#include "global.h"
#include "errors.h"
#include "ytab.gh"
#include "semantic.h"
#include "semdiag.h"

#include "reserr.h"

static int ResOS2WriteDlgTemplate( char *tmpldata, int size, WResFileID handle )
/******************************************************************************/
{
    int     numwrote;

    numwrote = WRESWRITE( handle, tmpldata, size );
    if( numwrote != size ) {
        WRES_ERROR( WRS_WRITE_FAILED );
        return( TRUE );
    } else {
        return( FALSE );
    }
}

static void InitOS2DialogBoxHeader( DialogHeaderOS2 *header )
/**********************************************************/
{
    header->Size             = 0;
    header->Type             = 0;
    header->Codepage         = 850; // TODO
    header->OffsetFirstTmpl  = 14;
    header->TemplateStatus   = 1;
    header->ItemFocus        = -1;
    header->OffsetPresParams = 0;
}

#if 0
static void AddDiagOption( DialogBoxHeader * head, FullDialogOptions * opt )
/***************************************************************************/
{
    switch (opt->token) {
    case Y_MENU:
        if (head->MenuName != NULL) {
            RcMemFree( head->MenuName );
        }
        head->MenuName = opt->Opt.Name;
        break;
    case Y_FONT:
        head->Style |= DS_SETFONT;
        head->PointSize = opt->Opt.Font.PointSize;
        if (head->FontName != NULL) {
            RcMemFree( head->FontName );
        }
        head->FontName = opt->Opt.Font.FontName;
        break;
    }
} /* AddDiagOptions */
#endif

#if 0
static FullDialogBoxHeader * SemDiagOptions( FullDialogBoxHeader * head,
                                            FullDialogOptions * opt )
/**********************************************************************/
{
    AddDiagOption( &head->u.Head, opt );
//    if( opt->token == Y_STYLE ) {
//        head->StyleGiven = TRUE;
//    }

    return( head );
}
#endif

static FullDiagCtrlListOS2 * SemOS2EmptyDiagCtrlList( void ) {
/*******************************************************/
    FullDiagCtrlListOS2 *      newlist;

    newlist = RcMemMalloc( sizeof(FullDiagCtrlListOS2) );
    newlist->head = NULL;
    newlist->tail = NULL;
    newlist->numctrls = 0;
    return( newlist );
}

extern FullDiagCtrlListOS2 * SemOS2NewDiagCtrlList( FullDialogBoxControlOS2 * ctrl,
                                                    DataElemList * list )
/*********************************************************************************/
{
    FullDiagCtrlListOS2 *      newlist;

    newlist = SemOS2EmptyDiagCtrlList();
    return( SemOS2AddDiagCtrlList( newlist, ctrl, list ) );
}

extern FullDiagCtrlListOS2 * SemOS2AddDiagCtrlList( FullDiagCtrlListOS2 * list,
                    FullDialogBoxControlOS2 * ctrl, DataElemList * dataList )
/*****************************************************************************/
{
    if( ctrl != NULL ) {
        ctrl->dataListHead = dataList;
        ResAddLLItemAtEnd( (void **) &(list->head), (void **) &(list->tail), ctrl );
        list->numctrls++;
    }
    return( list );
}

extern FullDialogBoxControlOS2 * SemOS2InitDiagCtrl( void )
/***************************************************/
{
    FullDialogBoxControlOS2 *  newctrl;

    newctrl = RcMemMalloc( sizeof(FullDialogBoxControlOS2) );
    newctrl->next = NULL;
    newctrl->prev = NULL;
    newctrl->children = NULL;
    newctrl->framectl = 0;

    return( newctrl );
}

/* These are the default styles used for OS/2 dialog box controls  */
/* statments except the CONTROL statement (see rcos2.y for that).  */

#define DEF_AUTOCHECKBOX     (OS2_BS_AUTOCHECKBOX|OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_AUTORADIOBUTTON  (OS2_BS_AUTORADIOBUTTON|OS2_WS_VISIBLE)
#define DEF_CHECKBOX         (OS2_BS_CHECKBOX|OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_COMBOBOX         (OS2_CBS_SIMPLE|OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_CONTAINER        (OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_CTEXT            (OS2_SS_TEXT|OS2_DT_CENTER|OS2_WS_VISIBLE|OS2_WS_GROUP)
#define DEF_DEFPUSHBUTTON    (OS2_BS_DEFAULT|OS2_BS_PUSHBUTTON|OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_EDITTEXT         (OS2_ES_AUTOSCROLL|OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_GROUPBOX         (OS2_SS_GROUPBOX|OS2_WS_VISIBLE|OS2_WS_GROUP)
#define DEF_ICON             (OS2_SS_ICON|OS2_WS_VISIBLE)
#define DEF_LISTBOX          (OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_LTEXT            (OS2_SS_TEXT|OS2_DT_LEFT|OS2_WS_VISIBLE|OS2_WS_GROUP)
#define DEF_MLE              (OS2_MLS_BORDER|OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_NOTEBOOK         (OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_PUSHBUTTON       (OS2_BS_PUSHBUTTON|OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_RADIOBUTTON      (OS2_BS_RADIOBUTTON|OS2_WS_VISIBLE)
#define DEF_RTEXT            (OS2_SS_TEXT|OS2_DT_RIGHT|OS2_WS_VISIBLE|OS2_WS_GROUP)
#define DEF_SLIDER           (OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_SPINBUTTON       (OS2_WS_VISIBLE|OS2_WS_TABSTOP)
#define DEF_VALUESET         (OS2_WS_VISIBLE|OS2_WS_TABSTOP)

extern FullDialogBoxControlOS2 * SemOS2NewDiagCtrl( uint_8 token,
                                    FullDiagCtrlOptionsOS2 opts )
/***************************************************************/
{
    FullDialogBoxControlOS2 *   newctrl;
    uint_32                     style_mask; /* for the style of the control */
    uint_32                     style_value;
    uint_32                     defstyle;
    uint_32                     style;
    uint_16                     ctlClass;
    ControlClass *              cont_class;

    switch (token) {
    case Y_AUTOCHECKBOX:
        ctlClass = OS2_WC_BUTTON;
        defstyle = DEF_AUTOCHECKBOX;
        break;
    case Y_AUTORADIOBUTTON:
        ctlClass = OS2_WC_BUTTON;
        defstyle = DEF_AUTORADIOBUTTON;
        break;
    case Y_CHECKBOX:
        ctlClass = OS2_WC_BUTTON;
        defstyle = DEF_CHECKBOX;
        break;
    case Y_COMBOBOX:
        ctlClass = OS2_WC_COMBOBOX;
        defstyle = DEF_COMBOBOX;
        break;
    case Y_CONTAINER:
        ctlClass = OS2_WC_CONTAINER;
        defstyle = DEF_CONTAINER;
        break;
    case Y_CTEXT:
        ctlClass = OS2_WC_STATIC;
        defstyle = DEF_CTEXT;
        break;
    case Y_DEFPUSHBUTTON:
        ctlClass = OS2_WC_BUTTON;
        defstyle = DEF_DEFPUSHBUTTON;
        break;
    case Y_EDITTEXT:
        ctlClass = OS2_WC_ENTRYFIELD;
        defstyle = DEF_EDITTEXT;
        break;
    case Y_GROUPBOX:
        ctlClass = OS2_WC_STATIC;
        defstyle = DEF_GROUPBOX;
        break;
    case Y_ICON:
        ctlClass = OS2_WC_STATIC;
        defstyle = DEF_ICON;
        break;
    case Y_LISTBOX:
        ctlClass = OS2_WC_LISTBOX;
        defstyle = DEF_LISTBOX;
        break;
    case Y_LTEXT:
        ctlClass = OS2_WC_STATIC;
        defstyle = DEF_LTEXT;
        break;
    case Y_MLE:
        ctlClass = OS2_WC_MLE;
        defstyle = DEF_MLE;
        break;
    case Y_NOTEBOOK:
        ctlClass = OS2_WC_NOTEBOOK;
        defstyle = DEF_NOTEBOOK;
        break;
    case Y_PUSHBUTTON:
        ctlClass = OS2_WC_BUTTON;
        defstyle = DEF_PUSHBUTTON;
        break;
    case Y_RTEXT:
        ctlClass = OS2_WC_STATIC;
        defstyle = DEF_RTEXT;
        break;
    case Y_RADIOBUTTON:
        ctlClass = OS2_WC_BUTTON;
        defstyle = DEF_RADIOBUTTON;
        break;
    case Y_SLIDER:
        ctlClass = OS2_WC_SLIDER;
        defstyle = DEF_SLIDER;
        break;
    case Y_SPINBUTTON:
        ctlClass = OS2_WC_SPINBUTTON;
        defstyle = DEF_SPINBUTTON;
        break;
    case Y_VALUESET:
        ctlClass = OS2_WC_VALUESET;
        defstyle = DEF_VALUESET;
        break;
    }

    newctrl = SemOS2InitDiagCtrl();
    cont_class = ResNumToControlClass( ctlClass );

    style_mask  = opts.Style.Mask;
    style_value = opts.Style.Value;
    style = (style_mask & style_value) | (~style_mask & defstyle);

    newctrl->ctrl.ID      = opts.ID;
    newctrl->ctrl.Size    = opts.Size;
    newctrl->ctrl.Text    = opts.Text;
    newctrl->ctrl.ClassID = cont_class;
    newctrl->ctrl.Style   = style;
    /* ExtraBytes is 0 for all predefined controls */
    newctrl->ctrl.ExtraBytes = 0;

    return( newctrl );
} /* SemOS2NewDiagCtrl */

static void SemOS2FreeDiagCtrlList( FullDiagCtrlListOS2 * list )
/********************************************************/
{
    FullDialogBoxControlOS2 * ctrl;
    FullDialogBoxControlOS2 * oldctrl;

    ctrl = list->head;
    while (ctrl != NULL) {
        /* free the contents of pointers within the structure */
        if (ctrl->ctrl.ClassID != NULL) {
            RcMemFree( ctrl->ctrl.ClassID );
        }
        if (ctrl->ctrl.Text != NULL) {
            RcMemFree( ctrl->ctrl.Text );
        }

        oldctrl = ctrl;

        if( ctrl->children != NULL )
            SemOS2FreeDiagCtrlList( ctrl->children );

        SemFreeDataElemList( ctrl->dataListHead );

        ctrl = ctrl->next;

        RcMemFree( oldctrl );
    }

    RcMemFree( list );
} /* SemOS2FreeDiagCtrlList */

static uint_16 SemOS2CountBytes( DataElemList *list )
/***************************************************/
{
    DataElemList     *travptr;
    uint_16           bytes = 0;
    int               i;

    for( travptr = list; travptr != NULL; travptr = travptr->next ) {
        for( i = 0; i < travptr->count; i++ ) {
            if( travptr->data[i].IsString ) {
                bytes += travptr->data[i].StrLen + 1;
            } else {
                bytes += 2;
            }
        }
    }

    return( bytes );
}

static uint_16 SemOS2DumpCtlData( char *ptr, DataElemList *list )
/***************************************************************/
{
    DataElemList     *travptr;
    uint_16           bytes = 0;
    uint_16           *data;
    uint_16           len;
    int               i;

    for( travptr = list; travptr != NULL; travptr = travptr->next ) {
        for( i = 0; i < travptr->count; i++ ) {
            if( travptr->data[i].IsString ) {
                len = travptr->data[i].StrLen + 1;
                strncpy( ptr, travptr->data[i].Item.String, len );
                bytes += len;
                ptr   += len;
            } else {
                data   = (uint_16*)ptr;
                *data  = (uint_16)travptr->data[i].Item.Num;
                bytes += 2;
                ptr   += 2;
            }
        }
    }

    return( bytes );
}

static int SemOS2CalcControlSize( FullDiagCtrlListOS2 *ctrls )
/***************************************************************/
{
    FullDialogBoxControlOS2 *ctrl;
    DialogBoxControl        *control;
    int                     size = 0;

    for( ctrl = ctrls->head; ctrl != NULL; ctrl = ctrl->next ) {
        control = &(ctrl->ctrl);
        size += sizeof( DialogTemplateItemOS2 );
        if( !(control->ClassID->Class & 0x80) )  // Class name length if provided
            size += strlen( control->ClassID->ClassName ) + 1;
        if( control->Text != NULL )             // Conrol text if provided
            if( control->Text->ord.fFlag == 0xFF )          // We have ID
                size += 3;
            else
                size += strlen( control->Text->name ) + 1;  // We have name
        else
            size += 1;

        // TODO: PRESPARAMS

        size += control->ExtraBytes;
        size += SemOS2CountBytes( ctrl->dataListHead );

        if( ctrl->children != NULL )        // Add size of all children
            size += SemOS2CalcControlSize( ctrl->children );
    }

    return size;
}

static char *SemOS2BuildTemplateArray( char *ptr, FullDiagCtrlListOS2 *ctrls )
/***********************************************************************/
{
    FullDialogBoxControlOS2 *ctrl;
    DialogBoxControl        *control;
    DialogTemplateItemOS2   *tmpl;

    /* Create the DLGTITEM array and fill in whatever data
       we know at this point.
     */
    tmpl = (DialogTemplateItemOS2 *)ptr;
    for( ctrl = ctrls->head; ctrl != NULL; ctrl = ctrl->next ) {
        control = &(ctrl->ctrl);
        tmpl->fsItemStatus  = 0;
        tmpl->offClassName  = 0;
        tmpl->offText       = 0;
        tmpl->flStyle       = control->Style;
        tmpl->x             = control->Size.x;
        tmpl->y             = control->Size.y;
        tmpl->cx            = control->Size.width;
        tmpl->cy            = control->Size.height;
        tmpl->id            = control->ID;
        tmpl->offPresParams = -1;
        tmpl->offCtlData    = -1;

        if( !(control->ClassID->Class & 0x80) ) {
            tmpl->cchClassName = strlen( control->ClassID->ClassName );
        }
        else {
            tmpl->cchClassName = 0;
            tmpl->offClassName = control->ClassID->Class & 0x7F;
        }

        if( control->Text != NULL ) {
            if( control->Text->ord.fFlag == 0xFF )
                tmpl->cchText = 3;
            else
                tmpl->cchText = strlen( control->Text->name );
        }
        else
            tmpl->cchText = 0;

        // TODO: PRESPARAMS

        ctrl->tmpl = tmpl;  // Save the pointer to DLGTITEM so we can update it later

        ptr += sizeof( *tmpl );
        if( ctrl->children != NULL ) {   // Process all children
            ptr = SemOS2BuildTemplateArray( ptr, ctrl->children );
            tmpl->cChildren = ctrl->children->numctrls;
        }
        else
            tmpl->cChildren = 0;

        tmpl = (DialogTemplateItemOS2 *)ptr;
    }

    return ptr;
}

static char *SemOS2DumpTemplateData( char *base, char *ptr,
                                     FullDiagCtrlListOS2 *ctrls )
/***************************************************************/
{
    FullDialogBoxControlOS2 *ctrl;
    DialogBoxControl        *control;
    DialogTemplateItemOS2   *tmpl;

    /* Dump all strings etc. into the resource image and update the
       offsets as appropriate.
     */
    for( ctrl = ctrls->head; ctrl != NULL; ctrl = ctrl->next ) {
        control = &(ctrl->ctrl);
        tmpl    = ctrl->tmpl;

        // Write out class name if provided
        if( !(control->ClassID->Class & 0x80) ) {
            strcpy( ptr, control->ClassID->ClassName );
            tmpl->offClassName = ptr - base;
            ptr += tmpl->cchClassName + 1;
        }

        // IBM's RC always stores at least one character of text
        // even if no text is provided; it could be a buglet but we'll
        // do the same for compatibility.
        tmpl->offText = ptr - base;
        if( control->Text != NULL ) {
            if( control->Text->ord.fFlag == 0xFF ) {
                memcpy( ptr, control->Text->name, 3 );
                ptr += 3;
            }
            else {
                strcpy( ptr, control->Text->name );
                ptr += tmpl->cchText + 1;
            }
        }
        else {
            *ptr = '\0';
            ptr++;
        }

        // TODO: PRESPARAMS

        // Write out class data if provided
        if( control->ExtraBytes ) {
            memcpy( ptr, &ctrl->framectl, sizeof( uint_32 ) );
            tmpl->offCtlData = ptr - base;
            ptr += control->ExtraBytes;
            // Write out class data if provided
            ptr += SemOS2DumpCtlData( ptr, ctrl->dataListHead );
        }
        else {
            if( ctrl->dataListHead ) {
                tmpl->offCtlData = ptr - base;
                ptr += SemOS2DumpCtlData( ptr, ctrl->dataListHead );
            }
        }

        if( ctrl->children != NULL )    // Process all children
            ptr = SemOS2DumpTemplateData( base, ptr, ctrl->children );
    }

    return ptr;
}

/* The OS/2 dialog templates present us with a problem because the
   template items contain a number of offsets that are not known until
   the template is processed; this means we cannot just start spitting
   the data into a file. Instead we build an in-memory image of the
   resource (the size must be < 64K) and then dump the entire resource
   into the file - which certainly shouldn't hurt performance either.
*/
extern void SemOS2WriteDialogTemplate( WResID * name, ResMemFlags flags,
                                       FullDiagCtrlListOS2 *ctrls )
/**********************************************************************/
{
    ResLocation              loc;
    int                      err_code;
    int                      error;
    int                      size;
    DialogHeaderOS2          *head = NULL;
    char                     *tmpl;
    char                     *ptr;

    size = sizeof( DialogHeaderOS2 ) + SemOS2CalcControlSize( ctrls );
    if( size > 65536 ) {
        // TODO: Error, template is too big
    }

    tmpl = RcMemMalloc( size );

    head = (DialogHeaderOS2*)tmpl;
    InitOS2DialogBoxHeader( head );
    head->Size = size;
    ptr = tmpl + sizeof( DialogHeaderOS2 );

    // Create the DLGTITEM array in memory
    ptr = SemOS2BuildTemplateArray( ptr, ctrls );

    // Dump all other data into memory and update the offsets
    SemOS2DumpTemplateData( tmpl, ptr, ctrls );

    // Write the resource to file
    loc.start = SemStartResource();

    error = ResOS2WriteDlgTemplate( tmpl, size, CurrResFile.handle );
    if( error ) {
        err_code = LastWresErr();
        goto OutputWriteError;
    }

    RcMemFree( tmpl );

    loc.len = SemEndResource( loc.start );
    SemAddResourceFree( name, WResIDFromNum( OS2_RT_DIALOG ), flags, loc );

    SemOS2FreeDiagCtrlList( ctrls );

    return;

OutputWriteError:
    RcError( ERR_WRITTING_RES_FILE, CurrResFile.filename,
                strerror( err_code )  );
    ErrorHasOccured = TRUE;
    SemOS2FreeDiagCtrlList( ctrls );
    return;
} /* SemOS2WriteDialogTemplate */


extern FullDialogBoxControlOS2 * SemOS2SetControlData( ResNameOrOrdinal *name,
                    uint_32 id, DialogSizeInfo size, ResNameOrOrdinal * ctlclassname,
                    IntMask style, FullDiagCtrlListOS2 * childctls )
/***********************************************************************************/
{
    FullDialogBoxControlOS2 *control;

    control = SemOS2InitDiagCtrl();

    control->ctrl.ID         = id;
    control->ctrl.Size       = size;
    control->ctrl.Text       = name;
    control->ctrl.ClassID    = ResNameOrOrdToControlClass( ctlclassname );
    control->ctrl.Style      = style.Value;
    control->children        = childctls;
    control->ctrl.ExtraBytes = 0;
    RcMemFree( ctlclassname );

    return( control );
}

extern FullDialogBoxControlOS2 * SemOS2SetWindowData( FullDiagCtrlOptionsOS2 opts,
                    IntMask framectl, FullDiagCtrlListOS2 * childctls )
/**********************************************************************************/
{
    FullDialogBoxControlOS2 *control;
    uint_32                 style;
    uint_32                 defstyle;
    uint_32                 style_mask; /* for the style of the control */
    uint_32                 style_value;

    control = SemOS2InitDiagCtrl();

    defstyle    = OS2_WS_CLIPSIBLINGS | OS2_WS_SAVEBITS | OS2_FS_DLGBORDER;
    style_mask  = opts.Style.Mask;
    style_value = opts.Style.Value;
    style = (style_mask & style_value) | (~style_mask & defstyle);

    control->ctrl.ID         = opts.ID;
    control->ctrl.Size       = opts.Size;
    control->ctrl.Text       = opts.Text;
    control->ctrl.ClassID    = ResNumToControlClass( OS2_WC_FRAME );
    control->ctrl.Style      = style;
    control->children        = childctls;
    control->ctrl.ExtraBytes = 4;   // Frame control data
    control->framectl        = framectl.Value;

    return( control );
}

extern void SemOS2AddDlgincResource( WResID * name, char * filename )
/*******************************************************************/
{
    ResLocation loc;
    int         error, err_code;

    loc.start = SemStartResource();
    error = ResWriteString( filename, FALSE, CurrResFile.handle );
    if(error) {
        err_code = LastWresErr();
        goto OutputWriteError;
    }
    loc.len = SemEndResource( loc.start );
    SemAddResourceFree( name, WResIDFromNum( OS2_RT_DLGINCLUDE ),
                        MEMFLAG_DISCARDABLE | MEMFLAG_MOVEABLE | MEMFLAG_PURE,
                        loc );
    RcMemFree( filename );
    return;

OutputWriteError:
    RcError( ERR_WRITTING_RES_FILE, CurrResFile.filename,
             strerror( err_code ) );
    ErrorHasOccured = TRUE;
    RcMemFree( filename );
    return;
}
