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

/*
 was: \samples\win\datactl\*.C

    This file defines the control data library of functions.

*/
// (heavily modified and tweaked to fit for VIW)
// modifications done by Felix Fontein (felix@amuq.net)

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "ctltype.h"
#include "dyntpl.h"

/******* Leave this stuff alone. To alter the data controls that
         are desired, change the '#defines' at the top of this file (which got removed now :) ) */

BOOL ctl_check_start( ctl_elt *, HANDLE, HWND, void *, BOOL );
BOOL ctl_radio_start( ctl_elt *, HANDLE, HWND, void *, BOOL );
BOOL ctl_text_start( ctl_elt *, HANDLE, HWND, void *, BOOL );
BOOL ctl_combo_start( ctl_elt *, HANDLE, HWND, void *, BOOL );
BOOL ctl_dcombo_start( ctl_elt *, HANDLE, HWND, void *, BOOL );
BOOL ctl_int_start( ctl_elt *, HANDLE, HWND, void *, BOOL );
BOOL ctl_float_start( ctl_elt *, HANDLE, HWND, void *, BOOL );
BOOL ctl_rint_start( ctl_elt *, HANDLE, HWND, void *, BOOL );
BOOL ctl_rfloat_start( ctl_elt *, HANDLE, HWND, void *, BOOL );

BOOL ctl_check_finish( ctl_elt *, HANDLE, HWND, void *, finish_type );
BOOL ctl_radio_finish( ctl_elt *, HANDLE, HWND, void *, finish_type );
BOOL ctl_text_finish( ctl_elt *, HANDLE, HWND, void *, finish_type );
BOOL ctl_combo_finish( ctl_elt *, HANDLE, HWND, void *, finish_type );
BOOL ctl_dcombo_finish( ctl_elt *, HANDLE, HWND, void *, finish_type );
BOOL ctl_int_finish( ctl_elt *, HANDLE, HWND, void *, finish_type );
BOOL ctl_float_finish( ctl_elt *, HANDLE, HWND, void *, finish_type );
BOOL ctl_rint_finish( ctl_elt *, HANDLE, HWND, void *, finish_type );
BOOL ctl_rfloat_finish( ctl_elt *, HANDLE, HWND, void *, finish_type );

BOOL ctl_check_modified( ctl_elt *, int, LONG );
BOOL ctl_text_modified( ctl_elt *, int, LONG );
BOOL ctl_combo_modified( ctl_elt *, int, LONG );
BOOL ctl_radio_modified( ctl_elt *, int, LONG );

ctl_action Ctl_int_actions[]={
    { NULL, NULL, NULL }, // empty dummy, since we are starting to count from one
    { ctl_check_start, ctl_check_finish, ctl_check_modified },
    { ctl_radio_start, ctl_radio_finish, ctl_radio_modified },
    { ctl_text_start, ctl_text_finish, ctl_text_modified },
    { ctl_combo_start, ctl_combo_finish, ctl_combo_modified },
    { ctl_dcombo_start, ctl_dcombo_finish, ctl_combo_modified },
    { ctl_int_start, ctl_int_finish, ctl_text_modified },
    { ctl_float_start, ctl_float_finish, ctl_text_modified },
    { ctl_int_start, ctl_rint_finish, ctl_text_modified },
    { ctl_float_start, ctl_rfloat_finish, ctl_text_modified },
    // missing: CTL_ESCOMBO,        // editable string combo box (drop down)
    // missing: CTL_DHCOMBO,        // dynamic combo box, with HWND parm
};

BOOL ctl_dlg_init( WPI_INST inst, HWND dlg, void *ptr, void *ctl_ptr)
//BOOL ctl_dlg_init( HANDLE inst, HWND dlg, void *ptr, void *ctl_ptr )

/******************************************************************/
/* initialize a data control. Returns TRUE if it gets up OK. Could return
   FALSE if a data error occurs */

{
    int                 num;
    ctl_elt             *elt;
    clt_def             *ctl;

    ctl = ctl_ptr;      // so app doesn't have to do type cast

    for( num = ctl->num_ctls, elt = ctl->elts; num > 0; --num, ++elt ) {
            if( !(Ctl_int_actions[elt->type].setup)( elt, inst, dlg, ptr, TRUE ) ) {
                return( FALSE );
            }
        elt->modified = FALSE;
    }

    return( TRUE );
}

BOOL ctl_dlg_done( WPI_INST inst, HWND dlg, void *ptr, void *ctl_ptr)
//BOOL ctl_dlg_done( HANDLE inst, HWND dlg, void *ptr, void *ctl_ptr )

/******************************************************************/
/* finalize a data control. Returns TRUE if all field verification is ok,
   FALSE otherwise */

{
    int                 num;
    ctl_elt             *elt;
    clt_def             *ctl;

    ctl = ctl_ptr;      // so app doesn't have to do type cast

    for( num = ctl->num_ctls, elt = ctl->elts; num > 0; --num, ++elt ) {
        if( elt->modified ) {
                if( !(Ctl_int_actions[elt->type].finish)( elt, inst,
                                                                dlg, ptr, FINISH_DONE ) ) {
                    return( FALSE );
                }
        }
    }

    return( TRUE );
}

void ctl_dlg_process( void *ctl_ptr, WPI_PARAM1 wparam, WPI_PARAM2 lparam )
//void ctl_dlg_process( void *ctl_ptr, UINT wparam, LONG lparam )

/**************************************************************/
/* this routine must be called for all WM_COMMAND events sent to the
   dialog. */

{
    clt_def             *ctl;
    ctl_elt             *elt;
    int                 num;
    BOOL                mod;

    ctl = ctl_ptr;      // so app doesn't have to do type cast

    for( num = ctl->num_ctls, elt = ctl->elts; num > 0; --num, ++elt ) {
            mod = (Ctl_int_actions[elt->type].modified)( elt, wparam, lparam );
            elt->modified |= mod;
            if( mod ) {
                break;
            }
    }
}

BOOL ctl_dlg_reset( WPI_INST ___a, HWND dlg, void *ptr, void *ctl_ptr, BOOL ___b)
{
    return ctl_dlg_init( ___a, dlg, ptr, ctl_ptr);
    //return FALSE;
}

////////////////////////////////////////////////

void dyn_tpl_init( void *dyn_def, HWND dlg_hld )
{
    int                 num;
    dyn_tpl_dim         *dim;
    dyn_tpl_dlg         *dyn = dyn_def; // so app doesn't have to do type cast
    char                *dimptr = (char*)&dyn->tpls;
    int                 *int_ctr;
    int                 i;

    for( num = dyn->num_tpls; num > 0; --num)
    {
        dim = (dyn_tpl_dim*)dimptr;
        dimptr += sizeof(dyn_tpl_dim);
        int_ctr = (int*)dimptr;
        while (*int_ctr++ != -1) ;
        dimptr = (char*)int_ctr;
        if (dim->info.use)
        {
            dyn_dim_type state = dim->info.tpl_state(dlg_hld, TRUE);
            dim->info.state = state;
                    // first, there comes a list in form "begin,end" which is zero terminated
                    for (i = 0; dim->dyn_tpl[i] != 0; i += 2)
                    {
                        int b = dim->dyn_tpl[i], e = dim->dyn_tpl[i + 1];
                        for (; b <= e; ++b)
                        {
                            HWND h = GetDlgItem(dlg_hld, b);
                            ShowWindow(h, (dim->info.state == DYN_INVISIBLE) ? SW_HIDE : SW_SHOW);
                            EnableWindow(h, (dim->info.state == DYN_VISIBLE) ? TRUE : FALSE);
                        }
                    }
                    // then there comes a list in form "item" which is zero terminated
                    for (++i; dim->dyn_tpl[i] != 0; ++i)
                    {
                        HWND h = GetDlgItem(dlg_hld, dim->dyn_tpl[i]);
                        ShowWindow(h, (dim->info.state == DYN_INVISIBLE) ? SW_HIDE : SW_SHOW);
                        EnableWindow(h, (dim->info.state == DYN_VISIBLE) ? TRUE : FALSE);
                    }
                    // i don't know what comes now, just take it as another list
                    for (++i; dim->dyn_tpl[i] != -1; ++i)
                    {
                        HWND h = GetDlgItem(dlg_hld, dim->dyn_tpl[i]);
                        ShowWindow(h, (dim->info.state == DYN_INVISIBLE) ? SW_HIDE : SW_SHOW);
                        EnableWindow(h, (dim->info.state == DYN_VISIBLE) ? TRUE : FALSE);
                    }
        }
    }
}

void dyn_tpl_process( void *dyn_def, HWND dlg_hld, WPI_PARAM1 parm1, WPI_PARAM2 parm2 )
{
    int                 num;
    dyn_tpl_dim         *dim;
    dyn_tpl_dlg         *dyn = dyn_def; // so app doesn't have to do type cast
    char                *dimptr = (char*)&dyn->tpls;
    int                 *int_ctr;
    int                 i;

    for( num = dyn->num_tpls; num > 0; --num )
    {
        dim = (dyn_tpl_dim*)dimptr;
        dimptr += sizeof(dyn_tpl_dim);
        int_ctr = (int*)dimptr;
        while (*int_ctr++ != -1) ;
        dimptr = (char*)int_ctr;
        if (dim->info.use)
            if (dim->info.tpl_check(parm1, parm2, dlg_hld))
            {
                dyn_dim_type state = dim->info.tpl_state(dlg_hld, TRUE);
                if (dim->info.state != state)
                {
                    dim->info.state = state;
                    // first, there comes a list in form "begin,end" which is zero terminated
                    for (i = 0; dim->dyn_tpl[i] != 0; i += 2)
                    {
                        int b = dim->dyn_tpl[i], e = dim->dyn_tpl[i + 1];
                        for (; b <= e; ++b)
                        {
                            HWND h = GetDlgItem(dlg_hld, b);
                            ShowWindow(h, (dim->info.state == DYN_INVISIBLE) ? SW_HIDE : SW_SHOW);
                            EnableWindow(h, (dim->info.state == DYN_VISIBLE) ? TRUE : FALSE);
                        }
                    }
                    // then there comes a list in form "item" which is zero terminated
                    for (++i; dim->dyn_tpl[i] != 0; ++i)
                    {
                        HWND h = GetDlgItem(dlg_hld, dim->dyn_tpl[i]);
                        ShowWindow(h, (dim->info.state == DYN_INVISIBLE) ? SW_HIDE : SW_SHOW);
                        EnableWindow(h, (dim->info.state == DYN_VISIBLE) ? TRUE : FALSE);
                    }
                    // i don't know what comes now, just take it as another list
                    for (++i; dim->dyn_tpl[i] != -1; ++i)
                    {
                        HWND h = GetDlgItem(dlg_hld, dim->dyn_tpl[i]);
                        ShowWindow(h, (dim->info.state == DYN_INVISIBLE) ? SW_HIDE : SW_SHOW);
                        EnableWindow(h, (dim->info.state == DYN_VISIBLE) ? TRUE : FALSE);
                    }
                }
            }
    }
}

////////////////////////////////////////////////////////////////////////////

BOOL ctl_check_start( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, BOOL ___b )
/********************************************************************/
/* start check field */

{
    inst = inst;

    CheckDlgButton( dlg, elt->control, _value_bool( ptr, elt ) );

    return( TRUE );
}

BOOL ctl_check_finish( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, finish_type ___f )
/*********************************************************************/
/* end check field */

{
    inst = inst;

    _value_bool( ptr, elt ) = IsDlgButtonChecked( dlg, elt->control );

    return( TRUE );
}

BOOL ctl_check_modified( ctl_elt *elt, UINT wparam, LONG lparam )
/***************************************************************/

{
    WORD        cmd;
    WORD        id;


    lparam = lparam;
    id = LOWORD( wparam );
    cmd = GET_WM_COMMAND_CMD( wparam, lparam );
    if( id == elt->control &&
        ( cmd == BN_CLICKED || cmd == BN_DOUBLECLICKED ) ) {
        return( TRUE );
    }

    return( FALSE );
}

WORD ctl_combo_add_msg( HWND, int );
WORD ctl_combo_sel_msg( HWND, int );
WORD ctl_combo_get_msg( HWND, int );

BOOL ctl_combo_start( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, BOOL ___b )
/********************************************************************/
/* start a combo list box */

{
    char                value[50];
    WORD                id;
    int                 choose;
    int                 max;

    choose = _value_int( ptr, elt ) - elt->info.combo.origin;

    if( choose < 0 ) {
        choose = 0;
    } else {
        max = elt->info.combo.end_id - elt->info.combo.start_id;
        if( choose > max ) {
            choose = max;
        }
    }

    for( id = elt->info.combo.start_id; id <= elt->info.combo.end_id; ++id ) {

        LoadString( inst, id, value, 50 );
        value[49] = '\0';
        SendDlgItemMessage( dlg, elt->control,
                        ctl_combo_add_msg( dlg, elt->control ) , 0,
                                                    (DWORD)(LPSTR) value );
    }
    SendDlgItemMessage( dlg, elt->control, ctl_combo_sel_msg( dlg,
                                            elt->control ), choose, 0 );

    return( TRUE );
}

BOOL ctl_combo_finish( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, finish_type ___f )
/*********************************************************************/
/* finish a combo list box */

{
    inst = inst;

    _value_int( ptr, elt ) = elt->info.combo.origin +
            SendDlgItemMessage( dlg, elt->control,
                        ctl_combo_get_msg( dlg, elt->control ), 0, 0 );

    return( TRUE );
}

BOOL ctl_combo_modified( ctl_elt *elt, UINT wparam, LONG lparam )
/***************************************************************/

{
    WORD        id;
    WORD        cmd;

    lparam = lparam;
    id = LOWORD( wparam );
    cmd = GET_WM_COMMAND_CMD( wparam, lparam );
    if( id == elt->control &&
        ( cmd == CBN_SELCHANGE || cmd == LBN_SELCHANGE ) ) {
        return( TRUE );
    }

    return( FALSE );
}

static BOOL is_listbox( HWND dlg, int ctl )
/*****************************************/

{
    char                buf[100];

    GetClassName( GetDlgItem( dlg, ctl ), buf, 100 );
    if( 0 == stricmp( buf, "listbox" ) ) {
        return( TRUE );
    }

    return( FALSE );
}

WORD ctl_combo_add_msg( HWND dlg, int ctl )
/*****************************************/

{
    if( is_listbox( dlg, ctl ) ) {
        return( LB_ADDSTRING );
    } else {
        return( CB_ADDSTRING );
    }
}

WORD ctl_combo_sel_msg( HWND dlg, int ctl )
/*****************************************/

{
    if( is_listbox( dlg, ctl ) ) {
        return( LB_SETCURSEL );
    } else {
        return( CB_SETCURSEL );
    }
}

WORD ctl_combo_get_msg( HWND dlg, int ctl )
/*****************************************/

{
    if( is_listbox( dlg, ctl ) ) {
        return( LB_GETCURSEL );
    } else {
        return( CB_GETCURSEL );
    }
}

WORD ctl_combo_add_msg( HWND, int );
WORD ctl_combo_sel_msg( HWND, int );
WORD ctl_combo_get_msg( HWND, int );

BOOL ctl_dcombo_start( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, BOOL ___b )
/*********************************************************************/
/* start a dynamic combo list box */

{
    char                *str;
    int                 i;
    int                 value;

    inst = inst;

    value = _value_int( ptr, elt ) - elt->info.dcombo.origin;

    for( i = 0;; ++i ) {
        str = (elt->info.dcombo.fetch)( i );
        if( str == NULL ) {
            break;
        }

        SendDlgItemMessage( dlg, elt->control,
                        ctl_combo_add_msg( dlg, elt->control ) , 0,
                                                    (DWORD)(LPSTR) str );
    }

    if( value >= i ) {
        value = i - 1;
    }

    SendDlgItemMessage( dlg, elt->control, ctl_combo_sel_msg( dlg,
                                            elt->control ), value, 0 );

    return( TRUE );
}

BOOL ctl_dcombo_finish( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, finish_type ___f )
/**********************************************************************/
/* finish a dynamic combo list box */

{
    inst = inst;

    _value_int( ptr, elt ) = elt->info.dcombo.origin +
            SendDlgItemMessage( dlg, elt->control,
                        ctl_combo_get_msg( dlg, elt->control ), 0, 0 );

    return( TRUE );
}

BOOL ctl_float_start( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, BOOL ___b )
/********************************************************************/
/* start a float field */

{
    char                buf[50];
    char                *str;
    int                 dec;
    float               value;

    inst = inst;

    value = _value_float( ptr, elt );
    sprintf( buf, "%f", value );
    for( str = buf + strlen( buf ) - 1; *str == '0'; --str );
    for( dec = 0; *str != '.'; --str, ++dec );
    if( dec < 2 ) {
        dec = 2;
    }

    sprintf( buf, "%-.*f", dec, value );

    SetDlgItemText( dlg, elt->control, buf );

    return( TRUE );
}

BOOL ctl_float_finish( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, finish_type ___f )
/*********************************************************************/
/* end a float field */

{
    char                str[50];
    float               value;

    inst = inst;

    GetDlgItemText( dlg, elt->control, str, 50 );
    str[49] = '\0';

    if( 1 == sscanf( str, "%f", &value ) ) {
        _value_float( ptr, elt ) = value;
    } else {
        SetFocus( GetDlgItem( dlg, elt->control ) );
        MessageBox( dlg, "Invalid value: please re-enter it", NULL,
                                        MB_APPLMODAL | MB_ICONHAND | MB_OK );
        return( FALSE );
    }

    return( TRUE );
}

BOOL ctl_int_start( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, BOOL ___b )
/******************************************************************/
/* start an integer field */

{
    inst = inst;

    SetDlgItemInt( dlg, elt->control, _value_int( ptr, elt ), TRUE );

    return( TRUE );
}

BOOL ctl_int_finish( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, finish_type ___f )
/*******************************************************************/
/* end an int field */

{
    BOOL                ok;

    inst = inst;

    _value_int( ptr, elt ) = GetDlgItemInt( dlg, elt->control, &ok, TRUE );

    if( !ok ) {
        SetFocus( GetDlgItem( dlg, elt->control ) );
        MessageBox( dlg, "Invalid integer: please re-enter it", NULL,
                                        MB_APPLMODAL | MB_ICONHAND | MB_OK );
        return( FALSE );
    }

    return( TRUE );
}

BOOL ctl_radio_start( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, BOOL ___b )
/********************************************************************/
/* start a radio button */

{
    int                 value;

    inst = inst;

    value = _value_int( ptr, elt );

    if( value != 0 ) {
        CheckRadioButton( dlg, elt->control, elt->info.radio.end_control,
                                elt->control + _value_int( ptr, elt ) - 1 );
    }

    return( TRUE );
}

BOOL ctl_radio_finish( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, finish_type ___f )
/*********************************************************************/
/* finish a radio button */

{
    int                 control;

    inst = inst;

    for( control = elt->info.radio.end_control; control >= elt->control;
                                                            --control ) {
        if( IsDlgButtonChecked( dlg, control ) ) {
            _value_int( ptr, elt ) = control - elt->control + 1;

            break;
        }
    }

    return( TRUE );
}

BOOL ctl_radio_modified( ctl_elt *elt, UINT wparam, LONG lparam )
/************************************************************/

{
    WORD        id;
    WORD        cmd;

    lparam = lparam;
    id = LOWORD( wparam );
    cmd = GET_WM_COMMAND_CMD(wparam, lparam );
    if( id >= elt->control && id <= elt->info.radio.end_control &&
                ( cmd == BN_CLICKED || cmd == BN_DOUBLECLICKED ) ) {
        return( TRUE );
    }

    return( FALSE );
}

BOOL ctl_float_finish( ctl_elt *, HANDLE, HWND, void *, finish_type ___f );

BOOL ctl_rfloat_finish( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, finish_type ___f )
/**********************************************************************/
/* end an float field */

{
    float               value;
    char                str[100];
    BOOL                any_max;

    if( !ctl_float_finish( elt, inst, dlg, ptr, ___f ) ) {
        return( FALSE );
    }

    value = _value_float( ptr, elt );

    any_max = elt->info.rfloat.max >= elt->info.rfloat.min;
    if( value < elt->info.rfloat.min || ( any_max &&
                                        value > elt->info.rfloat.max ) ) {
        SetFocus( GetDlgItem( dlg, elt->control ) );
        if( any_max ) {
            sprintf( str, "Value must be in the range %.2f to %.2f",
                            elt->info.rfloat.min, elt->info.rfloat.max );
        } else {
            sprintf( str, "Value must be greater than or equal to %.2f",
                                                    elt->info.rfloat.min );
        }

        MessageBox( dlg, str, NULL, MB_APPLMODAL | MB_ICONHAND | MB_OK );

        return( FALSE );
    }

    return( TRUE );
}

BOOL ctl_int_finish( ctl_elt *, HANDLE, HWND, void *, finish_type ___f );

BOOL ctl_rint_finish( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, finish_type ___f )
/********************************************************************/
/* end an int field */

{
    int                 value;
    char                str[100];
    BOOL                any_max;

    if( !ctl_int_finish( elt, inst, dlg, ptr, ___f ) ) {
        return( FALSE );
    }

    value = _value_int( ptr, elt );

    any_max = elt->info.rint.max >= elt->info.rint.min;
    if( value < elt->info.rint.min || ( any_max &&
                                        value > elt->info.rint.max ) ) {
        SetFocus( GetDlgItem( dlg, elt->control ) );
        if( any_max ) {
            sprintf( str, "Integer must be in the range %d to %d",
                                    elt->info.rint.min, elt->info.rint.max );
        } else {
            sprintf( str, "Integer must be greater than or equal to %d",
                                                        elt->info.rint.min );
        }

        MessageBox( dlg, str, NULL, MB_APPLMODAL | MB_ICONHAND | MB_OK );

        return( FALSE );
    }

    return( TRUE );
}

BOOL ctl_text_start( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, BOOL ___b )
/*******************************************************************/
/* start a text field */

{
    inst = inst;

    SetDlgItemText( dlg, elt->control, _str_ptr( ptr, elt ) );

    return( TRUE );
}

BOOL ctl_text_finish( ctl_elt *elt, HANDLE inst, HWND dlg, void *ptr, finish_type ___f )
/********************************************************************/
/* end a text field */

{
    char                *str;

    inst = inst;

    str = _str_ptr( ptr, elt );

    GetDlgItemText( dlg, elt->control, str, elt->info.text.text_size );

    str[elt->info.text.text_size - 1]= '\0'; // in case of overflow

    return( TRUE );
}

BOOL ctl_text_modified( ctl_elt *elt, UINT wparam , LONG lparam )
/***********************************************************/

{
    WORD        id;
    WORD        cmd;

    id = LOWORD( wparam );
    cmd = GET_WM_COMMAND_CMD( wparam, lparam );

    if( id == elt->control && cmd == EN_CHANGE ) {
        return( TRUE );
    }

    return( FALSE );
}
