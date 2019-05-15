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
* Description:  WGML predefined system symbols
*                   for the pic. macros see gsyssym.c
****************************************************************************/

#ifndef picka
    #error macro picka not defined
#endif
#ifndef pickc
    #error macro pickc not defined
#endif
#ifndef pickk
    #error macro pickk not defined
#endif
#ifndef pickl
    #error macro pickl not defined
#endif
//      symbol   next     flags
//      name     symbol
  pickl( ad,      adeven,  no_free+ro+access_fun+predefined          )
  pickl( adeven,  adodd,   no_free+ro+access_fun+predefined          )
  pickl( adodd,   amp,     no_free+ro+access_fun+predefined          )
  pickc( amp,     apage,   no_free+ro+predefined                     )
  pickl( apage,   bc,      no_free+ro+access_fun+predefined          )
  pickc( bc,      be,      no_free+ro+access_fun+predefined          )
  pickl( be,      bfonts,  no_free+ro+access_fun+predefined          )
  pickk( bfonts,  bm,      no_free+ro+access_fun+predefined          )
  pickl( bm,      bo,      no_free+ro+access_fun+predefined          )
  pickl( bo,      bs,      no_free+ro+access_fun+predefined          )
  pickc( bs,      bx,      no_free+ro+access_fun+predefined          )
  pickc( bx,      bxchar,  no_free+ro+access_fun+predefined          )
  pickl( bxchar,  cc,      no_free+ro+access_fun+predefined          )
  pickc( cc,      ccc,     no_free+ro+access_fun+predefined          )
  pickl( ccc,     cd,      no_free+ro+access_fun+predefined          )
  pickl( cd,      cdcount, no_free+ro+access_fun+predefined          )
  pickl( cdcount, chars,   no_free+ro+access_fun+predefined          )
  pickk( chars,   cl,      no_free+ro+access_fun+predefined          )
  pickl( cl,      co,      no_free+ro+access_fun+predefined          )
  pickk( co,      cont,    no_free+ro+access_fun+predefined          )
  pickc( cont,    cp,      no_free+ro+access_fun+predefined          )
  pickc( cp,      cpages,  no_free+ro+access_fun+predefined          )
  pickk( cpages,  cpc,     no_free+ro+access_fun+predefined          )
  pickl( cpc,     cpi,     no_free+ro+access_fun+predefined          )
  pickl( cpi,     cw,      no_free+ro+access_fun+predefined          )
  pickc( cw,      date,    no_free+ro+predefined                     )
  picka( date,    dayofm,  no_free+ro+predefined                     )
  picka( dayofm,  dayofw,  no_free+ro+predefined                     )
  picka( dayofw,  dayofy,  no_free+ro+predefined                     )
  picka( dayofy,  dfonts,  no_free+ro+predefined                     )
  pickk( dfonts,  dhset,   no_free+ro+access_fun+predefined          )
  pickl( dhset,   dpage,   no_free+ro+access_fun+predefined          )
  pickk( dpage,   duplex,  no_free+ro+access_fun+predefined          )
  pickk( duplex,  env,     no_free+ro+access_fun+predefined          )
  pickk( env,     fb,      no_free+ro+access_fun+predefined          )
  pickc( fb,      fbc,     no_free+ro+access_fun+predefined          )
  pickl( fbc,     fbf,     no_free+ro+access_fun+predefined          )
  pickl( fbf,     file,    no_free+ro+access_fun+predefined          )
  pickk( file,    fk,      no_free+ro+access_fun+predefined          )
  pickc( fk,      fkc,     no_free+ro+access_fun+predefined          )
  pickl( fkc,     flnest,  no_free+ro+access_fun+predefined          )
  pickl( flnest,  fm,      no_free+ro+access_fun+predefined          )
  pickl( fm,      fn,      no_free+ro+access_fun+predefined          )
  pickc( fn,      fnam,    no_free+ro+access_fun+predefined          )
  pickk( fnam,    fnc,     no_free+ro+access_fun+predefined          )
  pickl( fnc,     fnum,    no_free+ro+access_fun+predefined          )
  pickl( fnum,    fonts,   no_free+ro+access_fun+predefined          )
  pickk( fonts,   fs,      no_free+ro+access_fun+predefined          )
  pickl( fs,      gml,     no_free+ro+access_fun+predefined          )
  pickc( gml,     gutter,  no_free+ro+predefined                     )
  pickl( gutter,  hi,      no_free+ro+access_fun+predefined          )
  pickl( hi,      hm,      no_free+ro+access_fun+predefined          )
  pickl( hm,      hn,      no_free+ro+access_fun+predefined          )
  pickc( hn,      hnc,     no_free+ro+access_fun+predefined          )
  pickl( hnc,     hour,    no_free+ro+access_fun+predefined          )
  picka( hour,    hs,      no_free+ro+predefined                     )
  pickl( hs,      hy,      no_free+ro+access_fun+predefined          )
  pickk( hy,      hyc,     no_free+ro+access_fun+predefined          )
  pickl( hyc,     hyph,    no_free+ro+access_fun+predefined          )
  pickc( hyph,    in,      no_free+ro+access_fun+predefined          )
  pickl( in,      inr,     no_free+ro+access_fun+predefined          )
  pickl( inr,     ir,      no_free+ro+access_fun+predefined          )
  pickl( ir,      ixj,     no_free+ro+access_fun+predefined          )
  pickc( ixj,     ixref,   no_free+ro+access_fun+predefined          )
  pickl( ixref,   ju,      no_free+ro+access_fun+predefined          )
  pickk( ju,      layout,  no_free+ro+access_fun+predefined          )
  pickk( layout,  lc,      no_free+ro+access_fun+predefined          )
  pickl( lc,      li,      no_free+ro+access_fun+predefined          )
  pickc( li,      linb,    no_free+ro+access_fun+predefined          )
  pickc( linb,    line,    no_free+ro+access_fun+predefined          )
  pickl( line,    ll,      no_free+ro+access_fun+predefined          )
  pickl( ll,      lnum,    no_free+ro+access_fun+predefined          )
  pickl( lnum,    ls,      no_free+ro+access_fun+predefined          )
  pickl( ls,      lst,     no_free+ro+access_fun+predefined          )
  pickl( lst,     mc,      no_free+ro+access_fun+predefined          )
  pickc( mc,      mcs,     no_free+ro+access_fun+predefined          )
  pickc( mcs,     member,  no_free+ro+access_fun+predefined          )
  picka( member,  minute,  no_free+ro+predefined                     )
  picka( minute,  month,   no_free+ro+predefined                     )
  picka( month,   nodeid,  no_free+ro+predefined                     )
  pickk( nodeid,  of,      no_free+ro+access_fun+predefined          )
  pickl( of,      offline, no_free+ro+access_fun+predefined          )
  pickc( offline, online,  no_free+ro+access_fun+predefined          )
  pickc( online,  ooc,     no_free+ro+access_fun+predefined          )
  pickl( ooc,     out,     no_free+ro+access_fun+predefined          )
  pickk( out,     page,    no_free+ro+access_fun+predefined          )
  pickl( page,    paged,   no_free+ro+access_fun+predefined          )
  pickl( paged,   pagelm,  no_free+ro+access_fun+predefined          )
  pickl( pagelm,  pagerm,  no_free+ro+access_fun+predefined          )
  pickl( pagerm,  parm,    no_free+ro+access_fun+predefined          )
  picka( parm,    passno,  no_free+ro+predefined                     )
  pickl( passno,  passof,  no_free+ro+predefined                     )
  pickl( passof,  pdayofw, no_free+ro+predefined                     )
  picka( pdayofw, pdev,    no_free+ro+predefined                     )
  picka( pdev,    pgnuma,  no_free+ro+predefined                     )
  pickl( pgnuma,  pgnumad, no_free+ro+access_fun+predefined          )
  pickl( pgnumad, pgnumc,  no_free+ro+access_fun+predefined          )
  pickl( pgnumc,  pgnumcd, no_free+ro+access_fun+predefined          )
  pickl( pgnumcd, pgnumr,  no_free+ro+access_fun+predefined          )
  pickl( pgnumr,  pgnumrd, no_free+ro+access_fun+predefined          )
  pickl( pgnumrd, pi,      no_free+ro+access_fun+predefined          )
  pickc( pi,      pix,     no_free+ro+access_fun+predefined          )
  pickc( pix,     pl,      no_free+ro+access_fun+predefined          )
  pickl( pl,      pls,     no_free+ro+access_fun+predefined          )
  pickc( pls,     pmonth,  no_free+ro+access_fun+predefined          )
  picka( pmonth,  pn,      no_free+ro+predefined                     )
  pickl( pn,      ppage,   no_free+ro+access_fun+predefined          )
  pickl( ppage,   prs,     no_free+ro+access_fun+predefined          )
  pickc( prs,     prt,     no_free+ro+access_fun+predefined          )
  pickk( prt,     ps,      no_free+ro+access_fun+predefined          )
  pickc( ps,      pw,      no_free+ro+access_fun+predefined          )
  pickl( pw,      pyear,   no_free+ro+access_fun+predefined          )
  picka( pyear,   quiet,   no_free+ro+predefined                     )
  pickk( quiet,   rb,      no_free+ro+access_fun+predefined          )
  pickc( rb,      recno,   no_free+ro+access_fun+predefined          )
  pickl( recno,   resh,    no_free+ro+access_fun+predefined          )
  pickl( resh,    resv,    no_free+ro+access_fun+predefined          )
  pickl( resv,    ret,     no_free+ro+access_fun+predefined          )
  pickl( ret,     rmnest,  no_free+ro+access_fun+predefined          )
  pickl( rmnest,  sc,      no_free+ro+access_fun+predefined          )
  pickc( sc,      screen,  no_free+ro+access_fun+predefined          )
  pickc( screen,  second,  no_free+ro+access_fun+predefined          )
  picka( second,  seqno,   no_free+ro+predefined                     )
  pickl( seqno,   skcond,  no_free+ro+access_fun+predefined          )
  pickl( skcond,  sl,      no_free+ro+access_fun+predefined          )
  pickl( sl,      spcond,  no_free+ro+access_fun+predefined          )
  pickl( spcond,  su,      no_free+ro+access_fun+predefined          )
  pickk( su,      sys,     no_free+ro+access_fun+predefined          )
  pickk( sys,     tab,     no_free+ro+access_fun+predefined          )
  pickc( tab,     tb,      no_free+ro+access_fun+predefined          )
  pickc( tb,      termt,   no_free+ro+access_fun+predefined          )
  pickk( termt,   time,    no_free+ro+access_fun+predefined          )
  picka( time,    tiset,   no_free+ro+predefined                     )
  pickc( tiset,   tm,      no_free+ro+access_fun+predefined          )
  pickl( tm,      userid,  no_free+ro+access_fun+predefined          )
  pickk( userid,  version, no_free+ro+access_fun+predefined          )
  picka( version, wd,      no_free+ro+predefined                     )
  pickc( wd,      xtext,   no_free+ro+access_fun+predefined          )
  pickk( xtext,   year,    no_free+ro+access_fun+predefined          )
  picka( year,    ad,      no_free+ro+predefined                     )
//               &ad will be set to NULL in init routine for end of chain
#undef picka
#undef pickc
#undef pickk
#undef pickl
