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
* Description:  OS/2 syslevel file structures.
*
****************************************************************************/


#pragma pack(1)
/*--------------------------------------------------------------*\
 * SLFHEADER - This structure contains the information in a     *
 *         SYSLEVEL file header.  The first 2 fields are used   *
 *         to verify that the file is actually a SYSLEVEL file. *
 *         Also identified is the version, date, and offset to  *
 *         the SYSTABLE information -- see SYSTABLE             *
\*--------------------------------------------------------------*/

typedef struct _SLFHEADER {  /* slfh */
    USHORT    usSignature;              /* special # for id of syslevel file */
    CHAR      achSignature[8];  /* string to id slf file, must be 'SYSLEVEL' */
    CHAR      achJulian[5];                               /* date of version */
    USHORT    usSlfVersion;           /* version of syslevel file, must be 1 */
    USHORT    ausReserved[8];                                    /* reserved */
    ULONG     ulTableOffset;                           /* offset of SYSTABLE */
} SLFHEADER, *PSLFHEADER;

/*--------------------------------------------------------------*\
 * SYSTABLE - This structure contains the information about     *
 *         a specific component of a particular system. This    *
 *         info includes date and level information about the   *
 *         system /subsystem and the particular component.      *
\*--------------------------------------------------------------*/

typedef struct _SYSTABLE {  /* st */
    USHORT    usSysId;                        /* identifies system /subsytem */
    BYTE      bSysEdition;             /* edition of system, eg SE=00, EE=01 */
    BYTE      bSysVersion;                   /* version, eg 1.00=10, 1.10=11 */
    BYTE      bSysModify;                     /* modify, eg 1.00=00, 1.01=01 */
    USHORT    usSysDate;                                   /* date of system */
    CHAR      achCsdLevel[8];            /* subsytem CSD level, eg, XR?0000_ */
    CHAR      achCsdPrev[8];             /* as above, except for prev system */
    CHAR      achSysName[80];         /* Title of system / subsytem (ASCIIZ) */
    CHAR      achCompId[9];                      /* component ID of subsytem */
    BYTE      bRefreshLevel;
    CHAR      achType[9];            /* Null terminated type (8 chars +'\0') */
    BYTE      usReserved[6];                          /* reserved, must be 0 */
} SYSTABLE, *PSYSTABLE;
#pragma pack()

