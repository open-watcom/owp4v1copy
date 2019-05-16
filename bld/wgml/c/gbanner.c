/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2010 The Open Watcom Contributors. All Rights Reserved.
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
* Description: WGML implement processing for banner output
*
****************************************************************************/


#include "wgml.h"

static  banner_lay_tag  *   ban_top[max_ban][2];
static  banner_lay_tag  *   ban_bot[max_ban][2];
static  text_line           ban_line;           // for constructing banner line
static  uint32_t            ban_top_pos;        // top position of banner

/***************************************************************************/
/*  set banner pointers for head x heading                                 */
/***************************************************************************/

void set_headx_banners( int hx_lvl )
{
    bool    has_banners = false;

    static  int curr_hx_lvl;    // only valid when ProcFlags.heading_banner is true

    /* Determine if the current heading level has banners associated with it */

    if( (ban_top[hx_lvl + head0_ban][0] != NULL) ||
            (ban_top[hx_lvl + head0_ban][1] != NULL) ||
            (ban_bot[hx_lvl + head0_ban][0] != NULL) ||
            (ban_bot[hx_lvl + head0_ban][1] != NULL) ) {
        has_banners = true;        // at least one Hn banner exists
    }

    if( has_banners ) {
        if( ProcFlags.heading_banner ) {    // current banners are from a heading
            if( curr_hx_lvl >= hx_lvl ) {   // skip lower levels
                sect_ban_top[0] = ban_top[hx_lvl + head0_ban][0];
                sect_ban_top[1] = ban_top[hx_lvl + head0_ban][1];
                sect_ban_bot[0] = ban_bot[hx_lvl + head0_ban][0];
                sect_ban_bot[1] = ban_bot[hx_lvl + head0_ban][1];
                curr_hx_lvl = hx_lvl;
            }
        } else {                            // replace non-banner-related headers
            sect_ban_top[0] = ban_top[hx_lvl + head0_ban][0];
            sect_ban_top[1] = ban_top[hx_lvl + head0_ban][1];
            sect_ban_bot[0] = ban_bot[hx_lvl + head0_ban][0];
            sect_ban_bot[1] = ban_bot[hx_lvl + head0_ban][1];
            ProcFlags.heading_banner = true;
            curr_hx_lvl = hx_lvl;
        }
    }

    return;
}

/***************************************************************************/
/*  set banner pointers                                                    */
/***************************************************************************/
void set_banners( void )
{
    banner_lay_tag  *   ban;
    int                 k;

    static const struct {
        ban_docsect     ban_tag;
        e_tags          tag;
    }  ban_2_tag[max_ban] =  {
        { no_ban,       t_NONE     },   // dummy
        { abstract_ban, t_ABSTRACT },
        { appendix_ban, t_APPENDIX },
        { backm_ban,    t_BACKM    },
        { body_ban,     t_BODY     },
        { figlist_ban,  t_FIGLIST  },
        { index_ban,    t_INDEX    },
        { preface_ban,  t_PREFACE  },
        { toc_ban,      t_TOC      },
        { head0_ban,    t_H0       },
        { head1_ban,    t_H1       },
        { head2_ban,    t_H2       },
        { head3_ban,    t_H3       },
        { head4_ban,    t_H4       },
        { head5_ban,    t_H5       },
        { head6_ban,    t_H6       },
        { letfirst_ban, t_NONE     },   // dummy
        { letlast_ban,  t_NONE     },   // dummy
        { letter_ban,   t_NONE     },   // dummy
    };

    for( k = 0; k < max_ban; k++ ) {    // init banner list
        ban_top[k][0] = ban_top[k][1] = NULL;
        ban_bot[k][0] = ban_bot[k][1] = NULL;
    }

    for( ban = layout_work.banner; ban != NULL; ban = ban->next ) {

        if( ban->docsect > no_ban && ban->docsect < max_ban ) {
            for( k = 0; k < max_ban; k++ ) {
                if( ban->docsect == ban_2_tag[k].ban_tag ) {// tag found
                    switch( ban->place ) {
                    case   top_place :
                        ban_top[k][0] = ban;
                        ban_top[k][1] = ban;
                        break;
                    case   bottom_place :
                        ban_bot[k][0] = ban;
                        ban_bot[k][1] = ban;
                        break;

                    case   topodd_place :
                        ban_top[k][1] = ban;
                        break;
                    case   topeven_place :
                        ban_top[k][0] = ban;
                        break;

                    case   botodd_place :
                        ban_bot[k][1] = ban;
                        break;
                    case   boteven_place :
                        ban_bot[k][0] = ban;
                        break;
                    default:
                        break;
                    }
                    break;              // tag for banner found
                }
            }
        }
    }
}


/***************************************************************************/
/*  actually set a pgnum_style value from a given banner                   */
/***************************************************************************/

static bool do_set_pgnum_style( banner_lay_tag * ban, uint8_t index )
{
    bool    retval  = true;

    if( ban == NULL ) {         // nothing to do
        retval = false;
    } else if( ban->style == pgnuma_content ) {
        pgnum_style[index] = h_style;
    } else if( ban->style == pgnumad_content ) {
        pgnum_style[index] = h_style | xd_style;
    } else if( ban->style == pgnumr_content ) {
        pgnum_style[index] = r_style;
    } else if( ban->style == pgnumrd_content ) {
        pgnum_style[index] = r_style | xd_style;
    } else if( ban->style == pgnumc_content ) {
        pgnum_style[index] = c_style;
    } else if( ban->style == pgnumcd_content ) {
        pgnum_style[index] = c_style | xd_style;
    } else {    // banner content is not a page number style
        retval = false;
    }

    return(retval);
}


/***************************************************************************/
/*  set the pgnum_style array from the banner set arrays                   */
/*      first use a switch to identify the banners to use                  */
/*      then set the value by examining the banners in the order used by   */
/*      wgml 4.0                                                           */
/***************************************************************************/

void set_pgnum_style( void )
{
    int         i;
    ban_docsect ban_offset;

    for( i = 0; i < pns_max; i++ ) {
        switch( i ) {
        case pns_abstract :
            ban_offset = abstract_ban;
            break;
        case pns_appendix :
            ban_offset = appendix_ban;
            break;
        case pns_backm :
            ban_offset = backm_ban;
            break;
        case pns_body :
            ban_offset = body_ban;
            break;
        }
        if( do_set_pgnum_style( ban_top[ban_offset][0], i ) ) {
        } else if( do_set_pgnum_style( ban_top[ban_offset][1], i ) ) {
        } else if( do_set_pgnum_style( ban_bot[ban_offset][1], i ) ) {
        } else if( do_set_pgnum_style( ban_bot[ban_offset][0], i ) ) {
        } else {
            pgnum_style[i] = h_style;
        }
    }
    return;
}


/***************************************************************************/
/*  initialize final_content from the content of the region                */
/***************************************************************************/

static void content_reg( region_lay_tag * region )
{
    char            buf[BUF_SIZE];
    symsub      *   symsubval;
    int             k;
    int             rc;

    buf[0] = '\0';
    if( region->script_format ) {
        for( k = 0; k < 3; ++k ) {
            if( region->script_region[k].string != NULL ) {
                strcpy_s( &buf, strlen(region->script_region[k].string) + 1,
                          region->script_region[k].string );
                while( resolve_symvar_functions( &buf ) ); // loop until return shows no substitution done
                while( region->final_content[k].len < strlen( &buf ) ) {
                    if( region->final_content[k].string == NULL ) {
                        region->final_content[k].len = str_size;
                        region->final_content[k].string = mem_alloc( str_size );
                    } else {
                        region->final_content[k].len += str_size;
                        mem_realloc( region->final_content[k].string,
                                                                region->final_content[k].len );
                    }
                }
                if( region->final_content[k].len >  0 ) {   // buf actually has a string
                    strcpy_s( region->final_content[k].string, strlen( &buf ) + 1, &buf );
                    intrans( region->final_content[k].string, strlen( &buf ) + 1, region->font );
                }
            }
        }
    } else {    // not script format only normal string or keyword
        switch( region->contents.content_type ) {
        case   string_content:
            strcpy_s( &buf, strlen(region->contents.string) + 1, region->contents.string );
            while( resolve_symvar_functions( &buf ) ); // loop until return shows no substitution done
            break;
        case author_content :
            rc = find_symvar( &global_dict, "$author", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("author") + 1, "author" );
            }
            break;
        case bothead_content :
            rc = find_symvar( &global_dict, "$bothead", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$bothead") + 1, "$bothead" );
            }
            break;
        case date_content :
            /* This matches what wgml 4.0 actually does */
            rc = find_symvar( &global_dict, "date", no_subscript, &symsubval );
            if( rc == 2 ){  // tag DATE used
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {        // tag DATE not used
                rc = find_symvar( &global_dict, "$date", no_subscript, &symsubval );
                if( rc == 2 ) {
                    strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
                } else {
                    strcpy_s( &buf, strlen("$date") + 1, "$date" );
                }
            }
            break;
        case docnum_content :
            rc = find_symvar( &global_dict, "$docnum", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$docnum") + 1, "$docnum" );
            }
            break;
        case head0_content :
            rc = find_symvar( &global_dict, "$head0", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$head0") + 1, "$head0" );
            }
            break;
        case head1_content :
            rc = find_symvar( &global_dict, "$head1", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$head1") + 1, "$head1" );
            }
            break;
        case head2_content :
            rc = find_symvar( &global_dict, "$head2", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$head2") + 1, "$head2" );
            }
            break;
        case head3_content :
            rc = find_symvar( &global_dict, "$head3", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$head3") + 1, "$head3" );
            }
            break;
        case head4_content :
            rc = find_symvar( &global_dict, "$head4", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$head4") + 1, "$head4" );
            }
            break;
        case head5_content :
            rc = find_symvar( &global_dict, "$head5", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$head5") + 1, "$head5" );
            }
            break;
        case head6_content :
            rc = find_symvar( &global_dict, "$head6", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$head6") + 1, "$head6" );
            }
            break;
        case headnum0_content :
            rc = find_symvar( &global_dict, "$hnum0", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$hnum0") + 1, "$hnum0" );
            }
            break;
        case headnum1_content :
            rc = find_symvar( &global_dict, "$hnum1", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$hnum1") + 1, "$hnum1" );
            }
            break;
        case headnum2_content :
            rc = find_symvar( &global_dict, "$hnum2", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$hnum2") + 1, "$hnum2" );
            }
            break;
        case headnum3_content :
            rc = find_symvar( &global_dict, "$hnum3", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$hnum3") + 1, "$hnum3" );
            }
            break;
        case headnum4_content :
            rc = find_symvar( &global_dict, "$hnum4", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$hnum4") + 1, "$hnum4" );
            }
            break;
        case headnum5_content :
            rc = find_symvar( &global_dict, "$hnum5", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$hnum5") + 1, "$hnum5" );
            }
            break;
        case headnum6_content :
            rc = find_symvar( &global_dict, "$hnum6", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$hnum6") + 1, "$hnum6" );
            }
            break;
        case headtext0_content :
            rc = find_symvar( &global_dict, "$htext0", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$htext0") + 1, "$htext0" );
            }
            break;
        case headtext1_content :
            rc = find_symvar( &global_dict, "$htext1", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$htext1") + 1, "$htext1" );
            }
            break;
        case headtext2_content :
            rc = find_symvar( &global_dict, "$htext2", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$htext2") + 1, "$htext2" );
            }
            break;
        case headtext3_content :
            rc = find_symvar( &global_dict, "$htext3", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$htext3") + 1, "$htext3" );
            }
            break;
        case headtext4_content :
            rc = find_symvar( &global_dict, "$htext4", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$htext4") + 1, "$htext4" );
            }
            break;
        case headtext5_content :
            rc = find_symvar( &global_dict, "$htext5", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$htext5") + 1, "$htext5" );
            }
            break;
        case headtext6_content :
            rc = find_symvar( &global_dict, "$htext6", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$htext6") + 1, "$htext6" );
            }
            break;
        case pgnuma_content :
            rc = find_symvar( &global_dict, "$pgnuma", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$pgnuma") + 1, "$pgnuma" );
            }
            break;
        case pgnumad_content :
            rc = find_symvar( &global_dict, "$pgnumad", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$pgnumad") + 1, "$pgnumad" );
            }
            break;
        case pgnumc_content :
            rc = find_symvar( &global_dict, "$pgnumc", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$pgnumc") + 1, "$pgnumc" );
            }
            break;
        case pgnumcd_content :
            rc = find_symvar( &global_dict, "$pgnumcd", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$pgnumcd") + 1, "$pgnumcd" );
            }
            break;
        case pgnumr_content :
            rc = find_symvar( &global_dict, "$pgnumr", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$pgnumr") + 1, "$pgnumr" );
            }
            break;
        case pgnumrd_content :
            rc = find_symvar( &global_dict, "$pgnumrd", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$pgnumrd") + 1, "$pgnumrd" );
            }
            break;
        case sec_content :
            rc = find_symvar( &global_dict, "$sec", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$sec") + 1, "$sec" );
            }
            break;
        case stitle_content :
            rc = find_symvar( &global_dict, "$stitle", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$stitle") + 1, "$stitle" );
            }
            break;
        case title_content :
            rc = find_symvar( &global_dict, "$title", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$title") + 1, "$title" );
            }
            break;
        case time_content :
            rc = find_symvar( &global_dict, "$time", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$time") + 1, "$time" );
            }
            break;
        case tophead_content :
            rc = find_symvar( &global_dict, "$tophead", no_subscript, &symsubval );
            if( rc == 2 ) {
                strcpy_s( &buf, strlen(symsubval->value) + 1, symsubval->value );
            } else {
                strcpy_s( &buf, strlen("$tophead") + 1, "$tophead" );
            }
            break;
        case no_content :                   // empty region
            break;
        default:
            internal_err( __FILE__, __LINE__ );
        }
        /* still not script format only normal string or keyword */
        if( buf[0] != '\0' ) {       // assign to final_content depending on region_position
            if( region->region_position == pos_left ) {
                while( region->final_content[0].len < strlen( &buf ) ) {
                    if( region->final_content[0].string == NULL ) {
                        region->final_content[0].len = str_size;
                        region->final_content[0].string = mem_alloc( str_size );
                    } else {
                        region->final_content[0].len += str_size;
                        mem_realloc( region->final_content[0].string, region->final_content[0].len );
                    }
                }
                strcpy_s( region->final_content[0].string, strlen( &buf ) + 1, &buf );
                intrans( region->final_content[0].string, strlen( &buf ) + 1, region->font );
            } else if( region->region_position == pos_center ) {
                while( region->final_content[1].len < strlen( &buf ) ) {
                    if( region->final_content[1].string == NULL ) {
                        region->final_content[1].len = str_size;
                        region->final_content[1].string = mem_alloc( str_size );
                    } else {
                        region->final_content[1].len += str_size;
                        mem_realloc( region->final_content[1].string, region->final_content[1].len );
                    }
                }
                strcpy_s( region->final_content[1].string, strlen( &buf ) + 1, &buf );
                intrans( region->final_content[1].string, strlen( &buf ) + 1, region->font );
            } else if( region->region_position == pos_right ) {
                while( region->final_content[2].len < strlen( &buf ) ) {
                    if( region->final_content[2].string == NULL ) {
                        region->final_content[2].len = str_size;
                        region->final_content[2].string = mem_alloc( str_size );
                    } else {
                        region->final_content[2].len += str_size;
                        mem_realloc( region->final_content[2].string, region->final_content[2].len );
                    }
                }
                strcpy_s( region->final_content[2].string, strlen( &buf ) + 1, &buf );
                intrans( region->final_content[2].string, strlen( &buf ) + 1, region->font );
            } else {                                // position left if unknown
                while( region->final_content[0].len < strlen( &buf ) ) {
                    if( region->final_content[0].string == NULL ) {
                        region->final_content[0].len = str_size;
                        region->final_content[0].string = mem_alloc( str_size );
                    } else {
                        region->final_content[0].len += str_size;
                        mem_realloc( region->final_content[0].string, region->final_content[0].len );
                    }
                }
                strcpy_s( region->final_content[0].string, strlen( &buf ) + 1, &buf );
                intrans( region->final_content[0].string, strlen( &buf ) + 1, region->font );
            }
        }
    }
    return;
}


/***************************************************************************/
/*  output top / bottom banner                                             */
/*  on entry, ban->by_line contains the regions sorted in increasing order */
/*  by voffset + line_height of the region's font                          */
/*  multiline regions with enough text will produce additional regions     */
/*  until each region contains the text for one output line                */
/*  only then are the contents formed into lines and elements              */
/*  fully specified horizontal and vertical positions are computed here    */
/*  because set_positions() in docpage.c is specific to document text and  */
/*  banners require a different approach                                   */
/***************************************************************************/

static  void    out_ban_common( banner_lay_tag * ban, bool top )
{
    ban_reg_group   *   cur_grp;
    char            *   cur_p;
    doc_element     *   ban_doc_el;
    doc_element     *   last_doc_el;
    doc_element     *   old_doc_el;
    int                 k;
    region_lay_tag  *   cur_region;
    text_line       *   cur_line;
    uint32_t            cur_line_height;
    uint32_t            cur_width;
    uint32_t            cur_v_pos;
    uint32_t            text_width;

    ban_line.first = NULL;

    /***************************************************************************/
    /* For each region in the banner, fully resolve all symbols/functions      */
    /* Then determine how much of the text will fit and (for now) cut it off   */
    /* at the last space before that point.                                    */
    /* When multiline regions are implemented then, if the region has enough   */
    /* space for another line, create a new region with the remainder of the   */
    /* text and insert it at the proper position in cur_grp (voffset will      */
    /* always be larger by one line_height, so the new region will always sort */
    /* further down the list)                                                  */
    /* NOTE: regions with content "rule" will not be processed here            */
    /***************************************************************************/

    cur_grp = ban->by_line;
    while( cur_grp != NULL ) {
        cur_region = cur_grp ->first;
        while( cur_region != NULL ) {
            if( cur_region->contents.content_type != rule_content ) {
                content_reg( cur_region );      // load final_content array

                for( k = 0; k < 3; ++k ) {          // for all region parts
                    if( cur_region->final_content[k].string == NULL ) {
                        continue;                   // skip empty part
                    }
                    cur_width = 0;
                    for( cur_p = cur_region->final_content[k].string; *cur_p != '\0'; cur_p++ ) {
                        if( (cur_width + wgml_fonts[cur_region->font].width_table[(unsigned char) *cur_p]) <
                                cur_region->reg_width ) {
                            cur_width += wgml_fonts[cur_region->font].width_table[(unsigned char) *cur_p];
                        } else {
                            while( *cur_p != ' ' ) {
                                cur_p--;
                            }
                            *cur_p = '\0';     // This is where multiline support goes!!!
                            break;
                        }
                    }
                }
            }
            cur_region = cur_region->next;
        }
        cur_grp = cur_grp->next;
    }

    /***************************************************************************/
    /* convert each final_text into a text_chars and group those with the same */
    /* voffset into a single text_line.                                        */
    /* accumulate the text_lines into a doc_element                            */
    /* when a region with content "rule" is encountered and the device can     */
    /* draw a horizontal graphic line, then a new doc_element is created       */
    /* the result will be the same as what resolving multiple columns in       */
    /* normal text produces: a linked list of doc_elements which need merely   */
    /* be output in the given order                                            */
    /***************************************************************************/

    ban_doc_el = NULL;
    old_doc_el = NULL;
    cur_grp = ban->by_line;
    while( cur_grp != NULL ) {
        cur_region = cur_grp ->first;
        while( cur_region != NULL ) {
            if( (cur_region->contents.content_type == rule_content) &&
                    (bin_driver->hline.text != NULL) ) {        // page-oriented device: HLINE

                /*******************************************************************/
                /* This uses code written originally for use with control word BX  */
                /* That control word uses depth to indicate the amount by which at */
                /* the vertical position is to be adjusted after the hline is      */
                /* emitted, as it appears in the middle of the normal line depth   */
                /* Here, the line appears at the bottom of the line depth, but the */
                /* depth used must be 0 to prevent the next element from being     */
                /* placed one line too far down on the page                        */
                /*******************************************************************/

                if( ban_doc_el == NULL ) {
                    ban_doc_el = alloc_doc_el( el_hline );
                    old_doc_el = ban_doc_el;
                } else {
                    old_doc_el->next = alloc_doc_el( el_hline );
                    old_doc_el = old_doc_el->next;
                }
                old_doc_el->element.hline.ban_adjust = false;
                old_doc_el->element.hline.h_start = t_page.page_left + cur_region->reg_hoffset;
                old_doc_el->element.hline.h_len = cur_region->reg_width;
                old_doc_el->element.hline.v_start = ban_top_pos;
                if( bin_driver->y_positive == 0x00 ) {
                    old_doc_el->element.hline.v_start -= (cur_region->reg_voffset +
                                                        wgml_fonts[cur_region->font].line_height);
                } else {
                    old_doc_el->element.hline.v_start += (cur_region->reg_voffset +
                                                        wgml_fonts[cur_region->font].line_height);
                }
            } else {                    // all other regions
                if( (cur_region->contents.content_type == rule_content) &&
                        (bin_driver->hline.text == NULL) ) {    // character device: initialize text
                    line_buff.current = cur_region->reg_width;
                    while( line_buff.current > line_buff.length ) {
                        line_buff.length *= 2;
                        line_buff.text = mem_realloc( line_buff.text, line_buff.length + 1 );
                    }
                    memset( line_buff.text, bin_device->box.horizontal_line, line_buff.current );
                    line_buff.text[line_buff.current] = '\0';
                    while( cur_region->final_content[0].len < strlen( line_buff.text ) ) {
                        if( cur_region->final_content[0].string == NULL ) {
                            cur_region->final_content[0].string = mem_alloc( str_size );
                        } else {
                            mem_realloc( cur_region->final_content[0].string,
                                                    cur_region->final_content[0].len + str_size );
                        }
                    }
                    strcpy_s( cur_region->final_content[0].string, strlen( line_buff.text ) + 1,
                                                                            line_buff.text );
                }

                /* Initialize new doc_element, if appropriate */

                if( ban_doc_el == NULL ) {
                    ban_doc_el = alloc_doc_el( el_text );
                    old_doc_el = ban_doc_el;
                    cur_line = NULL;
                } else if( old_doc_el->type == el_hline ) {
                    old_doc_el->next = alloc_doc_el( el_text );
                    old_doc_el = old_doc_el->next;
                    cur_line = NULL;
                }

                /* Initialize new text_line, if appropriate */

                cur_line_height = wgml_fonts[cur_region->font].line_height;
                cur_v_pos = ban_top_pos;
                if( bin_driver->y_positive == 0x00 ) {
                    cur_v_pos -= (cur_region->reg_voffset + cur_line_height);
                } else {
                    cur_v_pos += (cur_region->reg_voffset + cur_line_height);
                }
                if( (cur_line == NULL) || (cur_line->y_address != cur_v_pos) ) {
                    if( old_doc_el->element.text.first == NULL ) {
                        cur_line = alloc_text_line();
                        old_doc_el->element.text.first = cur_line;
                    } else {
                        cur_line->next = alloc_text_line();
                        cur_line = cur_line->next;
                    }
                    cur_line->line_height = cur_line_height;
                    cur_line->y_address = cur_v_pos;
                }

                /* Convert the region into one or more text_chars */

                cur_region->final_content[0].hoffset = cur_region->reg_hoffset;
                cur_region->final_content[1].hoffset = cur_region->reg_hoffset;
                cur_region->final_content[2].hoffset = cur_region->reg_hoffset;
                if( cur_region->script_format ) {       // matches wgml 4.0
                    if( cur_region->final_content[2].string != NULL ) {
                        cur_region->final_content[2].hoffset += cur_region->reg_width -
                            cop_text_width( cur_region->final_content[2].string,
                            strlen( cur_region->final_content[2].string ), cur_region->font);
                    }
                    if( cur_region->final_content[1].string == NULL ) {
                        cur_region->final_content[1].hoffset = cur_region->final_content[2].hoffset;
                    } else {
                        cur_region->final_content[1].hoffset += (cur_region->reg_width / 2) -
                            (cop_text_width( cur_region->final_content[1].string,
                            strlen( cur_region->final_content[1].string ), cur_region->font) / 2);
                        if( cur_region->final_content[1].hoffset > cur_region->final_content[2].hoffset ) {
                            cur_region->final_content[1].hoffset = cur_region->final_content[2].hoffset;
                            cur_region->final_content[1].string[0] = '\0';
                        } else {
                            cur_width = 0;
                            for( cur_p = cur_region->final_content[1].string; *cur_p != '\0';
                                    cur_p++ ) {
                                if( (cur_width +
                                        wgml_fonts[cur_region->font].width_table[(unsigned char) *cur_p]) <
                                        cur_region->final_content[2].hoffset - cur_region->final_content[1].hoffset ) {
                                    cur_width += wgml_fonts[cur_region->font].width_table[(unsigned char) *cur_p];
                                } else {
                                    *cur_p = '\0';     // This is where multiline support goes!!!
                                    break;
                                }
                            }
                        }
                    }
                    if( cur_region->final_content[0].string != NULL ) {
                        if( cur_region->final_content[0].hoffset > cur_region->final_content[1].hoffset ) {
                            cur_region->final_content[0].string[0] = '\0';
                        } else {
                            cur_width = 0;
                            for( cur_p = cur_region->final_content[0].string; *cur_p != '\0';
                                    cur_p++ ) {
                                if( (cur_width +
                                        wgml_fonts[cur_region->font].width_table[(unsigned char) *cur_p]) <
                                        cur_region->final_content[1].hoffset - cur_region->final_content[0].hoffset ) {
                                    cur_width += wgml_fonts[cur_region->font].width_table[(unsigned char) *cur_p];
                                } else {
                                    *cur_p = '\0';     // This is where multiline support goes!!!
                                    break;
                                }
                            }
                        }
                    }
                    cur_p = cur_region->final_content[0].string;
                    if( (cur_p != NULL) && *cur_p ) {
                        if( cur_line->first == NULL ) {
                            cur_line->first = alloc_text_chars( cur_p, strlen( cur_p ),
                                                                                cur_region->font );
                            cur_line->last = cur_line->first;
                        } else {
                            cur_line->last->next = alloc_text_chars( cur_p, strlen( cur_p ),
                                                                                cur_region->font );
                            cur_line->last->next->prev = cur_line->last;
                            cur_line->last = cur_line->last->next;
                        }
                        cur_line->last->x_address = t_page.page_left + cur_region->final_content[0].hoffset;
                    }
                    cur_p = cur_region->final_content[1].string;
                    if( (cur_p != NULL) && *cur_p ) {
                        if( cur_line->first == NULL ) {
                            cur_line->first = alloc_text_chars( cur_p, strlen( cur_p ),
                                                                                cur_region->font );
                            cur_line->last = cur_line->first;
                        } else {
                            cur_line->last->next = alloc_text_chars( cur_p, strlen( cur_p ),
                                                                                cur_region->font );
                            cur_line->last->next->prev = cur_line->last;
                            cur_line->last = cur_line->last->next;
                        }
                        cur_line->last->x_address = t_page.page_left + cur_region->final_content[1].hoffset;
                    }
                    cur_p = cur_region->final_content[2].string;
                    if( (cur_p != NULL) && *cur_p ) {
                        if( cur_line->first == NULL ) {
                            cur_line->first = alloc_text_chars( cur_p, strlen( cur_p ),
                                                                                cur_region->font );
                            cur_line->last = cur_line->first;
                        } else {
                            cur_line->last->next = alloc_text_chars( cur_p, strlen( cur_p ),
                                                                                cur_region->font );
                            cur_line->last->next->prev = cur_line->last;
                            cur_line->last = cur_line->last->next;
                        }
                        cur_line->last->x_address = t_page.page_left + cur_region->final_content[2].hoffset;
                    }
                } else {
                    for( k = 0; k < 3; ++k ) {          // for all region parts
                        if( cur_region->final_content[k].string == NULL ) {
                            continue;                   // skip empty part
                        }
                        /* if k == 0, left-justify: already done */
                        text_width = cop_text_width( cur_region->final_content[k].string,
                                        strlen( cur_region->final_content[k].string ),
                                        cur_region->font);
                        if( k == 1 ) {  // center-justify
                            cur_region->final_content[1].hoffset += (cur_region->reg_width / 2) -
                                                                                text_width / 2;
                            if( text_width % 2 ) {
                                cur_region->final_content[1].hoffset--;
                            }
                        }
                        if( k == 2 ) {  // right-justify
                            cur_region->final_content[2].hoffset += cur_region->reg_width -
                                                                                text_width;
                        }
                        cur_p = cur_region->final_content[k].string;
                        if( cur_line->first == NULL ) {
                            cur_line->first = alloc_text_chars( cur_p, strlen( cur_p ),
                                                                                cur_region->font );
                            cur_line->last = cur_line->first;
                        } else {
                            cur_line->last->next = alloc_text_chars( cur_p, strlen( cur_p ),
                                                                                cur_region->font );
                            cur_line->last->next->prev = cur_line->last;
                            cur_line->last = cur_line->last->next;
                        }
                        cur_line->last->x_address = t_page.page_left + cur_region->final_content[k].hoffset;
                    }
                }
                if( top ) {
                    g_prev_font = cur_region->font;
                }
            }
            cur_region = cur_region->next;
        }
        cur_grp = cur_grp->next;
    }

    /****************************************************************/
    /*  insert the doc_elemets into t_page                          */
    /*  ban_doc_el is a linked list of doc_els, one per voffset     */
    /****************************************************************/

    last_doc_el = NULL;
    if( top ) {
        if( t_page.top_ban == NULL ){
            t_page.top_ban = ban_doc_el;
        } else {
            for( last_doc_el = t_page.top_ban; last_doc_el->next != NULL;
                last_doc_el = last_doc_el->next ) {}     // empty loop to find end of list
            last_doc_el->next = ban_doc_el;
        }
    } else {
        if( t_page.bot_ban == NULL ){
            t_page.bot_ban = ban_doc_el;
        } else {
            for( last_doc_el = t_page.top_ban; last_doc_el->next != NULL;
                last_doc_el = last_doc_el->next ) {}     // empty loop to find end of list
            last_doc_el->next = ban_doc_el;
        }
    }
}

/***************************************************************************/
/*  output top or bottom banner                                            */
/***************************************************************************/
void    out_ban_top( void )
{
    ban_top_pos = t_page.page_top;
    out_ban_common( t_page.top_banner, true );      // true for top banner
}

void    out_ban_bot( void )
{
    ban_top_pos = t_page.bot_ban_top;
    out_ban_common( t_page.bottom_banner, false );  // false for bottom banner
}

