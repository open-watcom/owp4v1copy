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
* Description:  Miscellaneous installer utility functions.
*
****************************************************************************/


#define INCL_WIN
#define INCL_DOS
#define INCL_BASE
#include <os2.h>
#include <string.h>
#include <stdlib.h>

#include "syslev.h"
#include "instutil.h"


/*
 * Returns the space free on a given drive... where 0 is A: in MB
 */
unsigned long drivefree(int drive)
{
    ULONG   aulFSInfoBuf[40] = {0};
    APIRET  rc               = NO_ERROR;
    double  bytesFree;

    DosError(FERR_DISABLEHARDERR);
    rc = DosQueryFSInfo(drive, FSIL_ALLOC, (PVOID)aulFSInfoBuf, sizeof(aulFSInfoBuf));

    DosError(FERR_ENABLEHARDERR);
    if (rc != NO_ERROR)
        return 0;

    bytesFree = (double)aulFSInfoBuf[3] * (double)aulFSInfoBuf[1] * (USHORT)aulFSInfoBuf[4];
    return (unsigned long)(bytesFree / (1024.0 * 1024.0));
}

void setdrivedir(char *drivedir)
{
    DosSetDefaultDisk((int)(drivedir[0]-'A'+1));
    DosSetCurrentDir(drivedir);
}


/*
 * Makes a folder on the desktop.
 */
void MakeFolder(char *Title, char *Icon, char *dest, char *id, char *setup)
{
    char szArg[200];

    memset(szArg,0,sizeof(szArg));

    if ((Icon != NULL) && (strlen(Icon) != 0)) {
        strcat(szArg,"ICONFILE=");
        strcat(szArg,Icon);
    }

    if ((id != NULL) && (strlen(id) != 0)) {
        strcat(szArg,";OBJECTID=");
        strcat(szArg,id);
    }

    if ((setup != NULL) && (strlen(setup) != 0)) {
        strcat(szArg,";");
        strcat(szArg,setup);
    }

    WinCreateObject("WPFolder",Title,szArg,dest,CO_UPDATEIFEXISTS);
}

/*
 * Makes a Program object on the desktop.
 */
void MakeProgram(char *Title, char *Program, char *Icon, char *dest, char *id, char *setup)
{
    char szArg[200];

    memset(szArg,0,sizeof(szArg));

    strcat(szArg,"EXENAME=");
    strcat(szArg,Program);

    if ((Icon != NULL) && (strlen(Icon) != 0)) {
        strcat(szArg,";ICONFILE=");
        strcat(szArg,Icon);
    }

    if ((id != NULL) && (strlen(id) != 0)) {
        strcat(szArg,";OBJECTID=");
        strcat(szArg,id);
    }

    if ((setup != NULL) && (strlen(setup) != 0)) {
        strcat(szArg,";");
        strcat(szArg,setup);
    }

    WinCreateObject("WPProgram",Title,szArg,dest,CO_UPDATEIFEXISTS);
}
/*
 * Makes a user defined object on the desktop.
 */
void MakeObject(char *Title, char *oclass, char *dest, char *id, char *setup)
{
    char szArg[200];

    memset(szArg,0,sizeof(szArg));

    if ((oclass == NULL) || (strlen(oclass) == 0))
        return;

    if ((id != NULL) && (strlen(id) != 0)) {
        strcat(szArg,"OBJECTID=");
        strcat(szArg,id);
    }

    if ((setup != NULL) && (strlen(setup) != 0)) {
        if ((id != NULL) && (strlen(id) != 0))
            strcat(szArg,";");
        strcat(szArg,setup);
    }

    WinCreateObject(oclass,Title,szArg,dest,CO_UPDATEIFEXISTS);
}
/*
 * Makes a shadow on the desktop.
 */
void MakeShadow(char *Title, char *reference, char *dest, char *id)
{
    char szArg[400];

    memset(szArg,0,sizeof(szArg));

    strcpy(szArg,"SHADOWID=");
    strcat(szArg,reference);
    if ((id != NULL) && (strlen(id) != 0)) {
        strcat(szArg,";OBJECTID=");
        strcat(szArg,id);
    }
    strcat(szArg,";");
    WinCreateObject("WPShadow",Title,szArg,dest,CO_UPDATEIFEXISTS);
}

/*
 * Return OS/2 version - hi word is FP level, lo byte is version.
 * For instance Warp 4 FP15 will return 0x000F0040, MCP will
 * return 0x00000045.
 */
ULONG getSysVer(void)
{
    CHAR      szSyslevel[] = "C:\\OS2\\INSTALL\\SYSLEVEL.OS2";
    CHAR      szFpklevel[] = "C:\\OS2\\INSTALL\\SYSLEVEL.FPK";
    ULONG     ulAction;
    ULONG     ulPosition;
    SLFHEADER slfh;
    SYSTABLE  st;
    HFILE     hf;
    ULONG     ulBootDrive, ulVer;
    CHAR      szCsdLevel[9];

    DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof(ulBootDrive));

    szSyslevel[0] = (CHAR)(ulBootDrive + 'A' - 1);
    szFpklevel[0] = (CHAR)(ulBootDrive + 'A' - 1);

    if (!DosOpen(szSyslevel, &hf, &ulAction, 0L, FILE_NORMAL,
         FILE_OPEN, OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE, 0L)) {
        DosRead(hf, &slfh, sizeof(SLFHEADER), &ulAction);
        DosChgFilePtr(hf, slfh.ulTableOffset, FILE_BEGIN, &ulPosition);
        DosRead(hf, &st, sizeof(SYSTABLE), &ulAction);
        DosClose(hf);
        strncpy(szCsdLevel, st.achCsdLevel, 8);
        ulVer = st.bSysVersion | (atoi(&szCsdLevel[4]) << 16);

        /* If we're on older Warp 4 FP level, we need to check SYSLEVEL.FPK
         * too - argh! The older FixPaks didn't update SYSLEVEL.OS2 properly.
         */
        if (((ulVer & 0xFF) == 0x40) && ((ulVer >> 16) < 5)) {
            if (!DosOpen(szFpklevel, &hf, &ulAction, 0L, FILE_NORMAL,
                         FILE_OPEN, OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE, 0L)) {
                DosRead(hf, &slfh, sizeof(SLFHEADER), &ulAction);
                DosChgFilePtr(hf, slfh.ulTableOffset, FILE_BEGIN, &ulPosition);
                DosRead(hf, &st, sizeof(SYSTABLE), &ulAction);
                DosClose(hf);
                strncpy(szCsdLevel, st.achCsdLevel, 8);
                ulVer = 0x40 | (atoi(&szCsdLevel[4]) << 16);
            }
        }
        return ulVer;
    }
    // If we failed to read SYSLEVEL.OS2, just assume things will work.
    return 0x45;
}

/*
 * Reboot the system
 */
void reboot(void)
{
#define SYSFUNC               0xD5
#define REBOOT                0xAB
#define REBOOTDEV             "\\DEV\\DOS$"

    APIRET rc;
    HFILE  hREBOOT;
    ULONG  ulAction;

    rc = DosOpen(REBOOTDEV,
                 &hREBOOT,
                 &ulAction,
                 0L,
                 FILE_NORMAL,
                 FILE_OPEN,
                 OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE,
                 0L);
    if (rc == 0) {
        DosDevIOCtl(hREBOOT, SYSFUNC, REBOOT, NULL,
                    0L, NULL, NULL, 0L, NULL);
        DosClose(hREBOOT);
    }
}

