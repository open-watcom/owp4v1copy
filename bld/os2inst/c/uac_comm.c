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
* Description:  Decompress and output archive comment if present.
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <stdio.h>    // printf()

#include "globals.h"
#include "uac_dcpr.h"
#include "uac_comm.h"

INT  comm_cpr_size=0;
CHAR *comm;

void comment_out(CHAR *top)      // outputs comment if present
{
   INT  i;

   if (head.HEAD_FLAGS & ACE_COMM)
   {                             // comment present?
      if (head.HEAD_TYPE == MAIN_BLK)
      {                          // get begin and size of comment data
         comm = (CHAR *)MCOMM;
         comm_cpr_size = MCOMM_SIZE;
      }
      else
      {
         comm = (CHAR *)FCOMM;
         comm_cpr_size = FCOMM_SIZE;
      }                          // limit comment size if too big
      i = sizeof(head) - (INT)(comm - (CHAR*) &head);
      if (comm_cpr_size > i)
         comm_cpr_size = i;
      dcpr_comm(i);              // decompress comment

      pipeit("%s\n\n%s\n\n", top, comm); // output comment
   }
}
