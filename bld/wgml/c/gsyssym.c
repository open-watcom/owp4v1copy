/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2009 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Implements predefined system variables SYSxxx and others
*               most are readonly and some are calculated when accessed
*                     incomplete                               TBD
****************************************************************************/


#include "wgml.h"


#define sys(x)  sys##x                  // construct symvar varname
#define sysf(x) sys##x##fun             // construct access function name
#define sys0(x) sys##x##0               // construct subscript 0 name
#define syss(x) sys##x##str             // construct name for string value


/***************************************************************************/
/*  declare dictionary entries for system variables                        */
/***************************************************************************/

#define picka( var, next, flag )    pickk( var, next, flag )
#define pickc( var, next, flag )    pickk( var, next, flag )
#define pickl( var, next, flag )    pickk( var, next, flag )
#define pickk( var, next, flag ) static symvar sys( var );
#include "gsyssym.h"

#define picka( var, next, flag )    pickk( var, next, flag )
#define pickc( var, next, flag )    pickk( var, next, flag )
#define pickl( var, next, flag )    pickk( var, next, flag )
#define pickk( var, next, flag ) static symsub sys0( var );
#include "gsyssym.h"

/***************************************************************************/
/*  declare the access functions for system variables                      */
/***************************************************************************/

#define picka( var, next, flag )
#define pickc( var, next, flag )    pickl( var, next, flag )
#define pickk( var, next, flag )    pickl( var, next, flag )
#define pickl( var, next, flag ) static void sysf( var )( symvar * entry );
#include "gsyssym.h"

/***************************************************************************/
/*  define char strings to hold the values of some system variables        */
/***************************************************************************/

#define picka( var, next, flag )
#define pickk( var, next, flag )
#define pickc( var, next, flag ) static char syss( var )[2];             // for single char values as string
#define pickl( var, next, flag ) static char syss( var )[MAX_L_AS_STR];  // for long as string and sysbxchar
#include "gsyssym.h"

/***************************************************************************/
/*  define the dictionary for the system variables                         */
/*  The dictionary is built at compile time                                */
/*  picka:      no access function, no separate string value               */
/*  pickc:         access function,    separate string value 2 chars       */
/*  pickl:         access function,    separate string value 12 chars      */
/*  pickk:         access function, no separate string value               */
/*                                  or 2 predefined values  ON OFF         */
/***************************************************************************/

#define pickc( var, next, flag )    pickl( var, next, flag )

#define pickl( var, next, flag )        \
        static symvar sys( var ) = {    \
            &sys( next ), "$" #var, 0L, 0L, NULL, &sys0( var ), sysf( var ), flag }; \
        static symsub sys0( var ) = { NULL, &sys( var ), no_subscript, syss( var ) };

#define picka( var, next, flag )        \
        static symvar sys( var ) = {    \
            &sys( next ), "$" #var, 0L, 0L, NULL, &sys0( var ), NULL, flag }; \
        static symsub sys0( var ) = { NULL, &sys( var ), no_subscript, NULL };

#define pickk( var, next, flag )        \
        static symvar sys( var ) = {    \
            &sys( next ), "$" #var, 0L, 0L, NULL, &sys0( var ), sysf( var ), flag }; \
        static symsub sys0( var ) = { NULL, &sys( var ), no_subscript, NULL };
#include "gsyssym.h"


/***************************************************************************/
/*  The sequence of the following strings must match the enum ju_enum      */
/***************************************************************************/

static  char    str[][8] = { "OFF", "ON", "HALF", "LEFT", "RIGHT", "CENTER",
                            "INSIDE", "OUTSIDE" };

static  char    dateval[20];
static  char    dayofmval[3];
static  char    dayofwval[2];
static  char    dayofyval[4];
static  char    hourval[3];
static  char    minuteval[3];
static  char    monthval[3];
static  char    pdayofwval[10];
static  char    pmonthval[12];
static  char    pyearval[5];
static  char    timeval[9];

/***************************************************************************/
/*  make new single char value known in dictionary                         */
/*  This is called from gsdccw.c and others                                */
/***************************************************************************/

void    add_to_sysdir( char * name, char char_val )
{
    symsub  *   dictval;

    find_symvar( &sys_dict, name, no_subscript, &dictval);
    *(dictval->value) = char_val;
}

/***************************************************************************/
/*  error routine for referenced, but not yet implemented variables        */
/*  the msg is only output for the first reference of every variable       */
/***************************************************************************/

static void var_wng( char * varname, symvar * e )
{
    if( !ProcFlags.no_var_impl_err ) {  // for full dict print no err msg
        e->varfunc = NULL;              // deactivate after first warning
        wng_count++;
        g_warn( err_var_not_impl, varname );
        file_mac_info();
    }
    return;
}

/***************************************************************************/
/*  access routines for some system symbols  mostly still dummy   TBD      */
/*  only those with comments are 'real'                                    */
/***************************************************************************/

static void sysadfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysadevenfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysadoddfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysapagefun( symvar * e )   // absolute page
{
    ulongtodec( apage + 1, sysapagestr );   // to match wgml 4.0
    return;
}

static void sysbefun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysbfontsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysbmfun( symvar * e )
{
    ulongtodec( bm, sysbmstr );
    return;
}

static void sysbofun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysbxfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysbxcharfun( symvar * e )  // box char always UNDefined TBD
{
    var_wng( e->name, e );
    return;
}

static void sysccfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syscccfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syscdfun( symvar * e )      // column count
{
    ulongtodec( g_cd, syscdstr );
    return;
}

static void syscdcountfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syscharsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysclfun( symvar * e )      // column length
{
    ulongtodec( g_cl, sysclstr );
    return;
}

static void syscofun( symvar * e )      // .co status
{
    if( ProcFlags.concat ) {
        sysco0.value = str[ju_on];
    } else {
        sysco0.value = str[ju_off];
    }
    return;
}

static void syscontfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syscpfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syscpagesfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syscpcfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syscpifun( symvar * e )     // cpi chars per inch
{
    ulongtodec( CPI, syscpistr );
    return;
}

static void sysdfontsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysdhsetfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysdpagefun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysduplexfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysenvfun( symvar * e )     // never seen another value TBD
{
    sysenv0.value = "BODY";
    return;
}

static void sysfbfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysfbcfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysfbffun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysfilefun( symvar * e )    // name of current input file/macro
{
    if( input_cbs == NULL ) {
        sysfile0.value = NULL;
    } else {
        if( input_cbs->fmflags & II_file ) {
            sysfile0.value = input_cbs->s.f->filename;
        } else {
            sysfile0.value = input_cbs->s.m->mac->name;
        }
    }
    return;
}

static void sysfkfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysfkcfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysflnestfun( symvar * e )  // include level file/macro
{
    ulongtodec( inc_level, sysflneststr );
    return;
}

static void sysfmfun( symvar * e )
{
    ulongtodec( fm, sysfmstr );
    return;
}

static void sysfnfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysfnamfun( symvar * e )  // name of current input file not macro
{
    inputcb * wk;

    if( input_cbs == NULL ) {
        sysfnam0.value = NULL;
    } else {
        wk = input_cbs;
        while( (wk != NULL) && !(wk->fmflags & II_file) ) {
            wk = wk->prev;
        }
        if( wk == NULL ) {
            sysfnam0.value = NULL;
        } else {
            sysfnam0.value = wk->s.f->filename;
        }
    }
    return;
}

static void sysfncfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysfnumfun( symvar * e )// lineno of current input file not macro
{
    inputcb     *wk;
    line_number l = 0;

    if( input_cbs != NULL ) {
        wk = input_cbs;
        while( (wk != NULL) && !(wk->fmflags & II_file) ) {
            wk = wk->prev;
        }
        if( wk != NULL ) {
            l = wk->s.f->lineno;
        }
    }
    ulongtodec( l, e->sub_0->value );
    return;
}

static void sysfontsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysfsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysgutterfun( symvar * e )  // gutter
{
    ulongtodec( g_gutter, sysgutterstr );
    return;
}

static void syshifun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syshmfun( symvar * e )
{
    ulongtodec( hm, syshmstr );
    return;
}

static void syshnfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syshncfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syshsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syshyfun( symvar * e )
{
    syshy0.value = str[ju_off];     // will need adjustment if hyphenation is implemented
    return;
}

static void syshycfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syshyphfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysinfun( symvar * e )      // .in indent value
{
    int32_t t_indent;                   // needed to make correction below

    t_indent = (t_page.cur_left * CPI) / g_resh;
    if( ((t_page.cur_left * CPI) - (t_indent * g_resh)) > (g_resh / 2) ) {  // rounding check
        t_indent++;
    }

    slongtodec( t_indent, sysinstr );   // in chars
    return;
}

static void sysinrfun( symvar * e )     // .in indentr indent right value
{
    int32_t t_indent;                   // needed to make correction below

    t_indent = (t_page.max_width * CPI) / g_resh;
    if( ((t_page.max_width *CPI) - (t_indent * g_resh)) > (g_resh / 2) ) {  // rounding check
        t_indent++;
    }

    slongtodec( t_indent, sysinrstr ); // in chars
    return;
}

static void sysirfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysixjfun( symvar * e )
{
    sysixj0.value = sysixjstr;
    return;
}

static void sysixreffun( symvar * e )   // $ixref
{
    sysixref0.value = sysixrefstr;
    return;
}

static void sysjufun( symvar * e )      // .ju status
{
    sysju0.value = str[ProcFlags.justify];
    return;
}

static void syslayoutfun( symvar * e ) // LAYOUT cmdline option or :LAYOUT tag seen
{
    if( ProcFlags.lay_specified ) {
        syslayout0.value = str[ju_on];
    } else {
        syslayout0.value = str[ju_off];
    }
    return;
}

static void syslcfun( symvar * e )      // remaining lines in column
{
    uint32_t    column_lines;

    column_lines = ((t_page.max_depth - t_page.cur_depth) * LPI) / g_resv;
    ulongtodec( column_lines, syslcstr );
    return;
}

static void syslifun( symvar * e )      // SCRIPT control word start char
{
    *syslistr = SCR_char;
    return;
}

static void syslinbfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syslinefun( symvar * e )    // current lineno on page
{
    ulongtodec( line, syslinestr );
    return;
}

static void sysllfun( symvar * e )
{
    ulongtodec( g_ll, sysllstr );
    return;
}

static void syslnumfun( symvar * e )  // lineno of current input file / macro
{
    line_number l;

    if( input_cbs == NULL ) {
        l = 0;
    } else {
        if( input_cbs->fmflags & II_file ) {
            l = input_cbs->s.f->lineno;
        } else {
            l = input_cbs->s.m->lineno;
        }
    }
    ulongtodec( l, e->sub_0->value );
    return;
}

static void syslsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syslstfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysmcfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysmcsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysnodeidfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysoffun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysofflinefun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysonlinefun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysoocfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysoutfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syspagefun( symvar * e )    // pageno in body
{
    ulongtodec( page, syspagestr );
    return;
}

static void syspagedfun( symvar * e )   // page depth
{
    ulongtodec( g_page_depth, syspagedstr );
    return;
}

static void syspagelmfun( symvar * e )  // page left margin
{
    ulongtodec( lm, syspagelmstr );
    return;
}

static void syspagermfun( symvar * e )  // page right margin
{
    ulongtodec( rm, syspagermstr );
    return;
}

static void syspgnumafun( symvar * e )  // pagenumber
{
    ulongtodec( page, syspgnumastr );
    return;
}

static void syspgnumadfun( symvar * e ) // pagenumber.
{
    ulongtodec( page, syspgnumadstr );
    strcat( syspgnumadstr, "." );
    return;
}

static void syspgnumcfun( symvar * e )  // roman page no UPPER
{
    int_to_roman( page, syspgnumcstr, sizeof( syspgnumcstr ) );
    strupr( syspgnumcstr );
    return;
}

static void syspgnumcdfun( symvar * e ) // roman page no UPPER.
{
    int_to_roman( page, syspgnumcdstr, sizeof( syspgnumcdstr ) );
    strupr( syspgnumcdstr );
    strcat( syspgnumcdstr, "." );
    return;
}

static void syspgnumrfun( symvar * e ) // roman page no
{
    int_to_roman( page, syspgnumrstr, sizeof( syspgnumrstr ) );
    return;
}

static void syspgnumrdfun( symvar * e ) // roman page no.
{
    int_to_roman( page, syspgnumrdstr, sizeof( syspgnumrdstr ) );
    strcat( syspgnumrdstr, "." );
    return;
}

static void syspifun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syspixfun( symvar * e )
{
    e->sub_0->value = syspixstr;
    return;
}

static void sysplfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysplsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syspnfun( symvar * e )      // page no
{
    ulongtodec( page, syspnstr );
    return;
}

static void sysppagefun( symvar * e )   // page no
{
    ulongtodec( page, sysppagestr );
    return;
}

static void sysprsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysprtfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syspsfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syspwfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysquietfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysrbfun( symvar * e )      // required blank
{                                       // can't get any functionality TBD
    *sysrbstr = sysrb0.value[0];
//    var_wng( e->name, e );
    return;
}

static void sysrecnofun( symvar * e )   // recno current input file / macro
{                                       // make it the same as &syslnum
    syslnumfun( e );                    // TBD
    return;
}

static void sysreshfun( symvar * e )    // horiz base units
{
    ulongtodec( g_resh, sysreshstr );
    return;
}

static void sysresvfun( symvar * e )    // vert base units
{
    ulongtodec( g_resv, sysresvstr );
    return;
}

static void sysretfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysrmnestfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysscfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysscreenfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysseqnofun( symvar * e )   // seqno current input record
{                                       // take &syslnum
    line_number l;                      // TBD

    if( input_cbs == NULL ) {
        l = 0;
    } else {
        if( input_cbs->fmflags & II_file ) {
            l = input_cbs->s.f->lineno;
        } else {
            l = input_cbs->s.m->lineno;
        }
    }
    sprintf_s( e->sub_0->value, MAX_L_AS_STR, "%.8lu", l );
    return;
}

static void sysskcondfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysslfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysspcondfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syssufun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syssysfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void systabfun( symvar * e )     // current user-defined tab character
{
    *systabstr = tab_char;
    return;
}

static void systbfun( symvar * e )     // current user-defined tab character
{
    *systbstr = tab_char;
    return;
}

static void systermtfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void systmfun( symvar * e )
{
    ulongtodec( tm, systmstr );
    return;
}

static void sysuseridfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void syswdfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

static void sysxtextfun( symvar * e )
{
    var_wng( e->name, e );
    return;
}

/***************************************************************************/
/*        dummy routines to avoid another picx macro                       */
/*        they deactivate themself at the first call                       */
/***************************************************************************/


static void sysampfun( symvar * e )     // dummy routine not needed
{
    e->varfunc = NULL;
    return;
}

static void sysbcfun( symvar * e )      // dummy routine not needed
{
    e->varfunc = NULL;
    return;
}

static void sysbsfun( symvar * e )      // dummy routine not needed
{
    e->varfunc = NULL;
    return;
}

static void syscwfun( symvar * e )      // dummy routine not needed
{
    e->varfunc = NULL;
    return;
}

static void sysgmlfun( symvar * e )     // dummy routine not needed
{
    e->varfunc = NULL;
    return;
}

static void syspassnofun( symvar * e )  // dummy routine not needed
{
    e->varfunc = NULL;
    return;
}

static void syspassoffun( symvar * e )  // dummy routine not needed
{
    e->varfunc = NULL;
    return;
}

static void systisetfun( symvar * e )   // dummy routine not needed
{
    e->varfunc = NULL;
    return;
}


/***************************************************************************/
/*  init_date_time  init several date / time system variables              */
/***************************************************************************/

static  void    init_date_time( void )
{
    time_t  now;
    struct  tm      tmbuf;
    char    *   p;

    now = time( NULL );
#ifdef _MSC_VER
    localtime_s( &tmbuf, &now );
#else
    localtime_s( &now, &tmbuf );
#endif
    strftime( dateval, sizeof( dateval ), "%B %d, %Y", &tmbuf );

    p = strstr( dateval, " 0" );        // search for leading zero
    if( p != NULL ) {                   // 'September 02, 2009'
        p++;
        *p = ' ';
        while( *p != '\0' ) {           // becomes
            *p = *(p + 1);              // 'September 2, 2009'
            p++;
        }
    }
    sysdate0.value = dateval;
    add_symvar( &global_dict, "date", dateval, no_subscript, 0 );

    strftime( dayofmval, sizeof( dayofmval ), "%e", &tmbuf );
    sysdayofm0.value = dayofmval;

    strftime( dayofwval, sizeof( dayofwval ), "%w", &tmbuf );
    dayofwval[0] += 1;                  // make 0-6 sun-sat 1-7
    sysdayofw0.value = dayofwval;

    strftime( dayofyval, sizeof( dayofyval ), "%j", &tmbuf );
    sysdayofy0.value = dayofyval;

    strftime( hourval, sizeof( hourval ), "%H", &tmbuf );
    syshour0.value = hourval;

    strftime( minuteval, sizeof( minuteval ), "%M", &tmbuf );
    sysminute0.value = minuteval;

    strftime( monthval, sizeof( monthval ), "%m", &tmbuf );
    sysmonth0.value = monthval;

    strftime( pdayofwval, sizeof( pdayofwval ), "%A", &tmbuf );
    syspdayofw0.value = pdayofwval;

    strftime( pmonthval, sizeof( pmonthval ), "%B", &tmbuf );
    syspmonth0.value = pmonthval;

    strftime( pyearval, sizeof( pyearval ), "%Y", &tmbuf );
    syspyear0.value = pyearval;
    sysyear0.value = &pyearval[2];      // year without century

    strftime( timeval, sizeof( timeval ), "%T", &tmbuf );
    systime0.value = timeval;
    syssecond0.value = &timeval[6];

    add_symvar( &global_dict, "time", timeval, no_subscript, 0 );

}

/***************************************************************************/
/*  init_predefined_symbols                                                */
/***************************************************************************/

static  void    init_predefined_symbols( void )
{
    char    wkstring[MAX_L_AS_STR];

    add_symvar( &global_dict, "amp", "&", no_subscript,
                is_AMP+predefined );

    wkstring[1] = '\0';
    wkstring[0] = GML_CHAR_DEFAULT;
    add_symvar( &global_dict, "gml", wkstring, no_subscript,
                predefined );

}

/***************************************************************************/
/*  init_sysparm     this is separate as only known after cmdline          */
/*                   processing                                            */
/***************************************************************************/

void    init_sysparm( char * cmdline, char * banner )
{
    char    *   p;

    p = strchr( cmdline, '(' );         // find parm start
    if( p == NULL ) {
        sysparm0.value = cmdline;       // empty cmdline
    } else {
        p++;
        SkipSpaces( p );                // over leading blanks
        sysparm0.value = p;

        p += strlen( p ) - 1;
        if( *p == ' ' ) {               // delete trailing blanks
            while( *p == ' ' ) {
                p--;
            }
            *++p = 0;                   // terminate string
        }
    }
    strupr( sysparm0.value );           // uppercase as wgml4 does

    syspdev0.value = dev_name;
    sysversion0.value = banner;
}


/***************************************************************************/
/*  init_sys_dict  initialize dictionary and some entries which do not     */
/*                 change very often                                       */
/***************************************************************************/

void    init_sys_dict( symvar * * dict )
{

    *dict           = &sysad;           // fill the dictionary ptr
    sysyear.next    = NULL;             // end of chain

    init_date_time();
    init_predefined_symbols();


    /***********************************************************************/
    /*  commented statements are perhaps  TBD                              */
    /***********************************************************************/

//  *sysadstr  =
//  *sysadevenstr  =
//  *sysadoddstr  =

    *sysampstr = '&';
    *(sysampstr + 1) = 0;
    *sysbcstr  = 'Y';
    *(sysbcstr + 1)  = 0;
//  *sysbestr  =
    *sysbsstr  = 0x16;
    *(sysbsstr + 1)  = 0;
    *sysbxstr  = 'N';
    *(sysbxstr + 1)  = 0;
    *sysbxcharstr       = 'U';
    *(sysbxcharstr + 1) = 'N';
    *(sysbxcharstr + 2) = 'D';
    *(sysbxcharstr + 3) = 0;
    *sysccstr  = 'N';
    *(sysccstr +1) = 0;
//  *syscccstr =
    syschars0.value = str[ju_off];
    sysco0.value    = str[ju_on];
    *syscpstr  = 'N';
    *(syscpstr + 1) = 0;
    *syscontstr = 0x03;
    *(syscontstr + 1) = 0;
//  *syscpagesstr  =
//  *syscpcstr =
//  *syscpistr =
    *syscwstr  = CW_SEP_CHAR_DEFAULT;
    *(syscwstr + 1) = 0;
//  *sysdfontsstr =
//  *sysdhsetstr =
//  *sysdpagestr =
    sysduplex0.value = str[ju_off];
    *sysfbstr = 'N';
    *(sysfbstr + 1) = 0;
//  *sysfbcstr =
//  *sysfbfstr =
    *sysfkstr = 'N';
    *(sysfkstr + 1) = 0;
//  *sysfkcstr =
    *sysfnstr = 'N';
    *(sysfnstr + 1) = 0;
//  *sysfncstr =
//  *sysfontsstr =
//  *sysfsstr =
    *sysgmlstr = GML_CHAR_DEFAULT;
    *(sysgmlstr + 1) = 0;
//  *sysgutterstr =
//  *syshistr =
    *syshnstr = 'N';
    *(syshnstr + 1) = 0;
//  *syshncstr =
//  *syshsstr =
    syshy0.value = str[ju_off];
//  *syshycstr =
    *syshyphstr = 'N';                // hyphenation OFF is default; hyphenation ON not implemented
    *(syshyphstr + 1) = 0;
//  *sysinstr =
//  *sysinrstr =
//  *sysirstr =
    *sysixjstr = '-';
    *(sysixjstr + 1) = 0;
    *sysixrefstr = ',';
    *(sysixrefstr + 1) = ' ';
    *(sysixrefstr + 2) = 0;
    sysju0.value = str[ju_on];
    syslayout0.value = str[ju_off];
    *syslistr = '.';
    *(syslistr + 1) = 0;
    *syslinbstr = ' ';
    *(syslinbstr + 1) = 0;
//  *syslsstr =
//  *syslststr =
    *sysmcstr = 'N';
    *(sysmcstr + 1) = 0;
    *sysmcsstr = '.';
    *(sysmcsstr + 1) = 0;
    sysmember0.value = NULL;            // member is never set
//  *sysnodeidstr =
//  *sysofstr =
    *sysofflinestr = 'N';
    *(sysofflinestr + 1) = 0;
    *sysonlinestr = 'Y';
    *(sysonlinestr + 1) = 0;
//  *sysoocstr =
//  *sysoutstr =
//  *syspagedstr =
//  *syspistr =
    *syspixstr = '*';
    *(syspixstr + 1) = 0;
//  *sysplstr =
    *sysplsstr = ',';
    *(sysplsstr + 1) = 0;
//  *sysppagestr =
    *sysprsstr = '-';
    *(sysprsstr + 1) = 0;
    sysprt0.value = str[ju_on];
    *syspsstr  = '%';
    *(syspsstr + 1) = 0;
//  *syspwstr =
    sysquiet0.value = str[ju_off];
    *sysrbstr    = ' ';
    *(sysrbstr + 1) = 0;
//  *sysrecnostr =
//  *sysretstr  =
//  *sysrmneststr  =
    *sysscstr   = 'N';
    *(sysscstr + 1) = 0;
    *sysscreenstr   = 'N';
    *(sysscreenstr + 1) = 0;
//  *sysseqnostr =
//  *sysskcondstr =
//  *sysslstr =
//  *sysspcondstr =
    syssu0.value = str[ju_on];
    syssys0.value = "DOS";
    *systabstr       = *systbstr       = 0x09;
    *(systabstr + 1) = *(systbstr + 1) = 0;
//  *systermtstr =
    *systisetstr = ' ';
    *(systisetstr + 1) = 0;
//  *systitlestr =
//  *sysuseridstr =
//  *syswdstr =
//  *sysxtextstr =

    return;
}

