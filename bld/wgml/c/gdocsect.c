/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2008 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML tags for document sections :GDOC, :FRONTM, :BODY, ...
*
****************************************************************************/
#include    "wgml.h"
#include    "findfile.h"
#include    "gvars.h"



/***************************************************************************/
/*  error routine for wrong sequence of doc section tags                   */
/***************************************************************************/
static  void    g_err_doc_sect( doc_section  ds )
{
    static  char const  sect[12][9] = {
                                        "NONE",
                                        "GDOC",
                                        "FRONTM",
                                        "TITLEP",
                                        "ABSTRACT",
                                        "PREFACE",
                                        "TOC",
                                        "FIGLIST",
                                        "BODY",
                                        "APPENDIX",
                                        "BACKM",
                                        "INDEX"
                                     };

    err_count++;
    scan_err = true;
    g_err( err_doc_sect, sect[ds] );
    file_mac_info();
}

/***************************************************************************/
/*  general routine to process document section tags mostly dummy     TBD  */
/***************************************************************************/
static  void    gml_doc_xxx( doc_section  ds )
{
    char        *   p;

    scan_err = false;

    p = scan_start;
    if( ProcFlags.doc_sect >= ds ) {
        g_err_doc_sect( ds );
    } else {
        ProcFlags.doc_sect = ds;
    }
    scan_start = scan_stop + 1;
    return;
}


/***************************************************************************/
/*  Document section tags                                                  */
/*                                                                         */
/*  :GDOC                 only one with attributes                         */
/*  :FRONTM    optional                                                    */
/*  :TITLEP    optional                                                    */
/*  :ETITLEP   optional                                                    */
/*  :ABSTRACT  optional                                                    */
/*  :PREFACE   optional                                                    */
/*  :TOC       optional                                                    */
/*  :FIGLIST   optional                                                    */
/*  :BODY                                                                  */
/*  :APPENDIX  optional                                                    */
/*  :BACKM     optional                                                    */
/*  :INDEX     optional                                                    */
/*  :EGDOC     optional                                                    */
/***************************************************************************/
extern  void    gml_abstract( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_abstract );
}

extern  void    gml_appendix( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_appendix );
}

extern  void    gml_backm( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_backm );
}

extern  void    gml_body( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_body );
}

extern  void    gml_figlist( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_figlist );
}

extern  void    gml_frontm( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_frontm );
}

extern  void    gml_index( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_index );
}

extern  void    gml_preface( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_preface );
}

extern  void    gml_titlep( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_titlep );
}

extern  void    gml_etitlep( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_etitlep );
}

extern  void    gml_toc( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_toc );
}

extern  void    gml_egdoc( const gmltag * entry )
{
    gml_doc_xxx( doc_sect_egdoc );
}

extern  void    gml_gdoc( const gmltag * entry )
{
    char        *   p;

    scan_err = false;
    p = scan_start;
    p++;

    while( *p == ' ' ) {                // over WS to attribute
        p++;
    }
    if( !strnicmp( "sec", p, 3 ) ) {

        out_msg( "sec attribute not implemented\n" );
        wng_count++;
        file_mac_info();
    }

    gml_doc_xxx( doc_sect_gdoc );
    scan_start = scan_stop + 1;
    return;
}

