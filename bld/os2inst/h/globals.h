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
* Description:  Global variable definitions.
*
****************************************************************************/


#ifndef __globals_h
#define __globals_h


#include "acestruc.h"
#include "unace.h"

//-------- Ace sign
extern const char *acesign;

//-------- Version string for program
extern const char *version;

//-------- header buffer and pointers
extern thead head;

extern tmhead *t_mhead;
extern tfhead *t_fhead;

//-------- buffers
extern ULONG *buf_rd ;
extern CHAR  *buf    ;
extern CHAR  *buf_wr ;
extern UCHAR *readbuf;

//-------- decompressor variables
extern
SHORT rpos          ,
      dcpr_do       ,
      dcpr_do_max   ,
      blocksize     ,
      dcpr_dic      ,
      dcpr_oldnum   ,
      bits_rd       ,
      dcpr_frst_file;
extern
USHORT dcpr_code_mn[1 << maxwd_mn],
       dcpr_code_lg[1 << maxwd_lg];
extern
UCHAR dcpr_wd_mn[maxcode + 2],
      dcpr_wd_lg[maxcode + 2],
      wd_svwd[svwd_cnt];
extern
ULONG dcpr_dpos      ,
      cpr_dpos2      ,
      dcpr_dicsiz    ,
      dcpr_dican     ,
      dcpr_size      ,
      dcpr_olddist[4],
      code_rd        ;
extern
CHAR *dcpr_text      ;

//-------- quicksort
extern USHORT sort_org[maxcode + 2];
extern UCHAR sort_freq[(maxcode + 2) * 2];

//-------- file handling
extern CHAR aname[PATH_MAX];
extern
INT  archan,
     wrhan;
extern LONG skipsize;

//-------- structures for archive handling
extern struct tadat adat;

//-------- flags
extern
INT  f_err      ,
     f_ovrall   ,
     f_allvol_pr,
     f_curpas   ,
     f_criterr  ;

int pipeit(char *format, ...);
void error(char *format, ...);
int confirm(char *format, ...);

#endif /* __globals_h */

