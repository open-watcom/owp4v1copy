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
* Description:  Linux trap file communication functions.
*
****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "linuxcomm.h"
#include "trpimp.h"

void print_msg( const char *format, ... )
{
    va_list     args;
    static char buf[2048];

    va_start(args,format);
    vsprintf(buf,format,args);
    write( 1, buf, strlen(buf) );
    va_end(args);
}

unsigned TryOnePath( char *path, struct stat *tmp, char *name, char *result )
{
    char        *end;
    char        *ptr;

    if( path == NULL ) return( 0 );
    ptr = result;
    for( ;; ) {
        switch( *path ) {
        case ':':
        case '\0':
            if( ptr != result && ptr[-1] != '/' )
                *ptr++ = '/';
            end = strcpy( name, ptr );
            if( stat( result, tmp ) == 0 )
                return( end - result );
            if( *path == '\0' )
                return( 0 );
            ptr = result;
            break;
        case ' ':
        case '\t':
            break;
        default:
            *ptr++ = *path;
            break;
        }
        ++path;
    }
}

unsigned FindFilePath( int exe, char *name, char *result )
{
    struct stat tmp;
//    unsigned    len;
    char        *end;

    if( stat( (char *)name, &tmp ) == 0 ) {
        end = strcpy( name, result );
        return( end - result );
    }
    // TODO: Need to find out how to get at the environment for the
    //       debug server process (I think!).
/*    if( exe ) {
        return( TryOnePath( getenv( "PATH" ), &tmp, name, result ) );
    } else {
        len = TryOnePath( getenv( "WD_PATH" ), &tmp, name, result );
        if( len != 0 ) return( len );
        len = TryOnePath( getenv( "HOME" ), &tmp, name, result );
        if( len != 0 ) return( len );
        return( TryOnePath( "/usr/watcom/wd", &tmp, name, result ) );
    }*/
    return 0;
}

unsigned ReqRead_user_keyboard()
{
    // TODO: Implement this for Linux!
#if 0
    struct _console_ctrl    *con;
    unsigned                con_num;
    int                     con_hdl;
    int                     con_mode;
    char                    chr;
    //NYI: what about QNX windows?
    static char             con_name[] = "/dev/conXX";
    unsigned                timeout;
    read_user_keyboard_req  *acc;
    read_user_keyboard_ret  *ret;

#   define FIRST_DIGIT (sizeof( con_name ) - 3)

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    timeout = acc->wait * 10;
    if( timeout == 0 ) timeout = -1;
    ret->key = '\0';
    con = console_open( 2, O_WRONLY );
    if( con == NULL ) {
        return( sizeof( *ret ) );
    }
    con_num = console_active( con, -1 );
    console_close( con );
    con_name[ FIRST_DIGIT + 0 ] = (con_num / 10) + '0';
    con_name[ FIRST_DIGIT + 1 ] = (con_num % 10) + '0';

    con_hdl = open( con_name, O_RDONLY );
    if( con_hdl < 0 ) {
        if( timeout == -1 ) timeout = 50;
        sleep( timeout / 10 );
        return( sizeof( *ret ) );
    }
    con_mode = dev_mode( con_hdl, 0, _DEV_MODES );
    if( dev_read( con_hdl, &chr, 1, 1, 0, timeout, 0, 0 ) == 1 ) {
        if( chr == 0xff ) {
            read( con_hdl, &chr, 1 );
            chr = '\0';
        }
        ret->key = chr;
    }
    dev_mode( con_hdl, con_mode, _DEV_MODES );
    close( con_hdl );
    return( sizeof( *ret ) );
#endif
    read_user_keyboard_req  *acc;
    read_user_keyboard_ret  *ret;
    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    ret->key = ' ';
    return( sizeof( *ret ) );
}

unsigned ReqGet_err_text()
{
    get_err_text_req    *acc;
    char                *err_txt;

    acc = GetInPtr( 0 );
    err_txt = GetOutPtr( 0 );
    strcpy( err_txt, strerror( acc->err ) );
    return( strlen( err_txt ) + 1 );
}

unsigned ReqSplit_cmd()
{
    char                *cmd;
    char                *start;
    split_cmd_ret       *ret;
    unsigned            len;

    cmd = GetInPtr( sizeof( split_cmd_req ) );
    ret = GetOutPtr( 0 );
    start = cmd;
    len = GetTotalSize() - sizeof( split_cmd_req );
    for( ;; ) {
        if( len == 0 ) goto done;
        switch( *cmd ) {
        case '/':
        case '=':
        case '(':
        case ';':
        case ',':
            goto done;
        case '\0':
        case ' ':
        case '\t':
            ret->parm_start = cmd - start + 1;
            ret->cmd_end = cmd - start;
            return( sizeof( *ret ) );
        }
        ++cmd;
        --len;
    }
done:
    ret->parm_start = cmd - start;
    ret->cmd_end = cmd - start;
    return( sizeof( *ret ) );
}

unsigned ReqGet_next_alias()
{
    get_next_alias_ret  *ret;

    ret = GetOutPtr( 0 );
    ret->seg = 0;
    ret->alias = 0;
    return( sizeof( *ret ) );
}

unsigned ReqSet_user_screen()
{
    return( 0 );
}

unsigned ReqSet_debug_screen()
{
    return( 0 );
}

