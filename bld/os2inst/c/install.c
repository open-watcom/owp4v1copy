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
* Description:  SFX installer main module.
*
****************************************************************************/


#define INCL_WIN
#define INCL_DOS
#define INCL_BASE
#include <os2.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <process.h>
#include <sys/types.h>
#include "instutil.h"
#include "install.h"

#ifndef OS2_VER
#define OS2_VER "1.0.0"
#endif

#define MLE_BUFFER_SIZE     4096
#define REPLACE_BUFFER_SIZE 1024
#define THREAD_STACK_SIZE   32768
#define MAX_PACKAGES        128

/* Unusually many globals due to multiple dialogs */
HAB hab = 0;
HMQ hmq = 0;
HPS hps = 0;
HDC hdc = 0;
HBITMAP hbm = 0;
POINTL bmppoint;
SIZEL sizl;
QMSG qmsg;
HWND mainhwnd = 0, logohwnd = 0, hwndMLE = 0;
char tempPath[_MAX_PATH];
int installstate = NONE;
int installstage = 0;
int current_file = 0, success = 0;
unsigned long acepos = 0, aceoffset = 0;
int pixels = 0;
char confirmstring[1024];
/* I know I am being excessive but... better safe than sorry */
char *configsys[8196];
int configfilecount = -1;
int files = 0, files_deleted = 0, packagesize = 0, packagesselected[MAX_PACKAGES];
unsigned long freespacereq = 0;
/* Global flags often set by the user */
int driveselected, packagechosen, express = 1, driverstatus = -1, no_update = 0;
int licensechecked = 0, custom = 0, checkforupdate = 1, downloadsite = 0;
int updconfig = 1, usecurrent = 0, checking = 0, checkerror = 0, newerver = 0;
char wupdfilename[_MAX_PATH] = "", wupdversion[256] = "", wupddate[256] = "";
char wupdurl[4][_MAX_PATH] = { "", "", "", "" };

FILE *self;

/* These get loaded in loadheader */
char *INSTALLER_APPLICATION;
char *INSTALLER_VERSION;
char *INSTALLER_TITLE;
char *INSTALLER_PATH;
char *INSTALLER_FOLDER;
char *INSTALLER_PROGRAM;
char *INSTALLER_SHADOW;
char *INSTALLER_OBJECT;
char *INSTALLER_SETS;
char *INSTALLER_SYSVAR;
char *INSTALLER_SYSLINE;
char *INSTALLER_PACKAGES[MAX_PACKAGES];
int  INSTALLER_BITMAP_WIDTH;
int  INSTALLER_BITMAP_HEIGHT;
int  INSTALLER_PACKAGE_COUNT;

#define T_BINARY     3

int (*ftpget)(char *, char *, char *, char *, char *, char *, char *, int);
int ftpgetfound = 0;

/* Config.Sys -- Note the drive letter gets replaced with the boot drive letter
                 It is just a place holder. (For the next 3 entries)             */
char csfile[] = "C:\\CONFIG.SYS";
/* Backup Config.Sys filename */
char bufile[] = "C:\\CONFIG.OLD";
/* Installation Log Database -- Used for uninstallation and aborting */
char instlog[] = "C:\\OS2\\INSTALL\\OWINST.LOG";
char installdir[400] = "C:\\";

char productname[400] = "Product";
char bootdrive[2] = "C";
char winpath[400] = "C:\\OS2\\MDOS\\WINOS2";
char winjpath[400] = "C:\\OS2\\MDOS\\WINJOS2";
char wintpath[400] = "C:\\OS2\\MDOS\\WINTOS2";
char winhpath[400] = "C:\\OS2\\MDOS\\WINHOS2";
char browsedir[400] = "C:\\";
char installdir2[400] = "C:\\USER";
char empty_string[] = "";
char currentcf[400] = "";

FILE *logfile;

/* Function prototypes */
int installer_unpack(CHAR * filename, int operation);
void resetglobals(void);

typedef struct _replacements {
    char *replacestring, *replacevar;
} Replacements;

/* The variables in this array must be static buffers */
Replacements InstRep[] = {
    "%PRODUCT%",       productname,
    "%INSTALLPATH%",   installdir,
    "%BOOTDRIVE%",     bootdrive,
    "%ANYSTRING%",     empty_string,
    "%WINPATH%",       winpath,
    "%WINJPATH%",      winjpath,
    "%WINTPATH%",      wintpath,
    "%WINHPATH%",      winhpath,
    "%USERPATH%",      installdir2,
    "%VERSION%",       OS2_VER,
    "%WEB_VER%",       wupdversion,
    "%WEB_DATE%",      wupddate,
    "%WEB_LOCATION1%", wupdurl[0],
    "%WEB_LOCATION2%", wupdurl[1],
    "%WEB_LOCATION3%", wupdurl[2],
    "%WEB_LOCATION4%", wupdurl[3],
};

int replacemax = 16;

typedef struct _cnrentry {
    RECORDCORE  rc;
    HPOINTER    hptrIcon;
    char        *filename;
} CnrEntry;

/* In str1, str2 gets replaced by str3 */
char *replacestr(char *str1, char *str2, char *str3)
{
    char bigbuffer[4096];
    int z, x = 0, len1 = strlen(str1), len2 = strlen(str2), len3 = strlen(str3);

    for (z = 0; z < len1; z++) {
        if (len2 > 0 && strncmp(&str1[z], str2, len2) == 0) {
            int i;
            for (i = 0; i < len3; i++) {
                bigbuffer[x] = str3[i];
                x++;
            }
            z = z + (len2 - 1);
        } else {
            bigbuffer[x] = str1[z];
            x++;
        }
    }
    bigbuffer[x] = 0;
    return (char *)strdup(bigbuffer);
}

/* This function parses a string and replaces all the text in the
 * Replacement array with the current dynamic text */
char *replaceem(char *orig)
{
    char *tmp1 = NULL, *tmp2 = (char *)strdup(orig);
    int z;

    for (z = 0; z < replacemax; z++) {
        tmp1 = replacestr(tmp2, InstRep[z].replacestring, InstRep[z].replacevar);
        free(tmp2);
        tmp2 = tmp1;
        tmp1 = NULL;
    }
    return tmp2;
}

/* Returns path where the product was last installed (or default path otherwise) */
char *getPrevInstPath(void)
{
    char    *tmpbuf, *foundline;
    BOOL    found = FALSE;
    FILE    *f;

    tmpbuf = malloc(8196);
    foundline = malloc(8196);

    if ((tmpbuf == NULL) || (foundline == NULL))
        return installdir;

    /* We can extract the needed information from the install log */
    if ((f = fopen(instlog, "rb")) != NULL) {
        while (!feof(f)) {
            fgets(tmpbuf, 8196, f);
            if (tmpbuf[0] == '[' && (char *)strstr(tmpbuf, INSTALLER_APPLICATION) != NULL && !feof(f)) {
                fgets(tmpbuf, 8196, f); // This should be the Version line
                fgets(tmpbuf, 8196, f); // This should be the InstallDir line
                if ((char *)strstr(tmpbuf, "<InstallDir>")) {
                    /* We found an InstallDir line - but we remember it and
                     * keep looking for the last occurrence.
                     */
                    found = TRUE;
                    strcpy(foundline, tmpbuf);
                }
            }
        }
    }

    if (found) {
        /* The line we found looks like this:
         * <InstallDir>,c:\foo\bar
         * We need to get the string after the comma
         */
        strcpy(installdir, strchr(foundline, ',') + 1);
        stripcrlf(installdir);
        }

    free(tmpbuf);
    free(foundline);

    return installdir;
}


/*
 * Display an informational dialog box to the user with the given text.
 */
int mesg(char *format, ...) {
    va_list args;
    char outbuf[4096];

    va_start(args, format);
    vsprintf(outbuf, format, args);
    va_end(args);

    WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, outbuf, INSTALLER_TITLE, 0, MB_OK | MB_INFORMATION | MB_MOVEABLE);

    return strlen(outbuf);
}

int checktext(char *text, char *buffer, int buflen)
{
    int z, len = strlen(text);

    for (z = 0; z < (buflen - len); z++) {
        if (memcmp(text, &buffer[z], len) == 0)
            return z;
    }
    return -1;
}

/*
 * Returns the offset within the executable to the specified text.
 */
long findtext(char *text)
{
    char buffer[512];
    int offset;
    unsigned long curpos = 0;

    fseek(self, 0, SEEK_SET);
    fread(buffer, 1, 512, self);
    if((offset = checktext(text, buffer, 512)) > -1)
        return offset;
    while(!feof(self))
    {
        memcpy(buffer, &buffer[256], 256);
        fread(&buffer[256], 1, 256, self);
        curpos += 256;
        if((offset = checktext(text, buffer, 512)) > -1)
            return offset+curpos;

    }
    return -1;
}

/* We encode archive search text so we don't get confused
 * by the string table itself when looking for header. */
char *decode(char *input)
{
    char    *result;
    int i = 0;

    result = (char *)malloc(strlen(input) / 2 + 1);

    while (input[0] && input[1]) {
        result[i] = ((input[0] - 0x41) << 4) | (input[1] - 0x41);
        input += 2;
        i++;
    }
    result[i] = '\0';

    return result;
}

/*
 * Window Procedure for the viewer window.
 */
MRESULT EXPENTRY ViewWndProc(HWND hWnd, ULONG msg, MPARAM mp1,  MPARAM mp2)

{
    SWP swp;

    int th, wh, ww;
    static int bw, bh;

    switch ( msg ) {
    case WM_CREATE:
        bw = WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
        bh = WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
        break;
    case WM_SIZE:
        WinQueryWindowPos(hWnd, &swp);
        wh = swp.cy;
        ww = swp.cx;
        swp.cy = 0;
        WinQueryWindowPos(WinWindowFromID(hWnd, FID_TITLEBAR), &swp);
        th = swp.cy;
        WinSetWindowPos(hwndMLE, NULLHANDLE, bw, bh, ww, wh - th, SWP_MOVE | SWP_SIZE);
        /*WinQueryWindowPos(hwndMLE, &swp);
        mesg("x %d y %d cx %d cy %d ww %d wh %d th %d hwndMLE %d", swp.x, swp.y, swp.cx, swp.cy, ww, wh, th, hwndMLE);*/
        return(WinDefWindowProc(hWnd, msg, mp1, mp2));
        break;
    default :
        return(WinDefWindowProc(hWnd, msg, mp1, mp2));
    }
    return(0L);
}

/*
 * Opens a Window which contains the text specfied by given filename.
 */
void viewfile(char *filename, HAB localhab)
{
    HWND hwndFrame, /*hwndMLE,*/ hwndClient;
    ULONG flStyle = FCF_MINMAX | FCF_SYSMENU | FCF_TITLEBAR |
                    FCF_SIZEBORDER | FCF_SHELLPOSITION | FCF_TASKLIST;
    char deffont[] = "10.System Monospaced";

    WinRegisterClass(localhab, "VIEW", ViewWndProc, CS_SIZEREDRAW, 32);

    hwndFrame = WinCreateStdWindow(HWND_DESKTOP, WS_VISIBLE | WS_MAXIMIZED,
                                  &flStyle, "VIEW", filename, 0L,
                                  NULLHANDLE, 0L, &hwndClient);

    hwndMLE = WinCreateWindow(hwndFrame, WC_MLE, "",
                              WS_VISIBLE | MLS_VSCROLL | MLS_WORDWRAP | MLS_READONLY | MLS_BORDER,
                              0,0,100,100, hwndFrame, HWND_TOP, 999L, NULL, NULL);

    WinSetWindowPos(hwndFrame, HWND_TOP, 0, 0, 0, 0, SWP_MAXIMIZE | SWP_ZORDER);
    WinSetWindowPos(hwndMLE, HWND_TOP, 0, 0, 0, 0, SWP_SHOW);

    if (!hwndMLE) {
        mesg("Error %lu while creating MLE window.", WinGetLastError(localhab));
    }
    else {
        FILE *f;
        char buffer[MLE_BUFFER_SIZE];
        ULONG bytes, point = -1;

        WinSetPresParam(hwndMLE, PP_FONTNAMESIZE, strlen(deffont) + 1, deffont);

        if ((f = fopen(filename, "rb")) != NULL) {
            WinSendMsg(hwndMLE, MLM_SETIMPORTEXPORT, MPFROMP(buffer), MPFROMLONG(MLE_BUFFER_SIZE));
            while (!feof(f)) {
                bytes = fread(buffer, 1, MLE_BUFFER_SIZE, f);
                    WinSendMsg(hwndMLE, MLM_IMPORT, MPFROMP(&point), MPFROMLONG(bytes));
            }
            fclose(f);
        }
    }
}

/*
 * Find the offset withing the executable of the ace data for use.
 */
int aceseek_entry(int num)
{
    long headerstart;
    char headerbuf[20], packageconfig[100];
    int z, start = 0, entry = 0, packageoffset=0;
    /* Decode DBSOFT-ACE - small memleak but
     * install should not be running for very long. :) */
    sprintf(headerbuf, "%s%d", decode("EEECFDEPEGFECNEBEDEF"), num);
    if ((headerstart = findtext(headerbuf)) < 0) {
        mesg("Could not find ACE header in executable.");
        exit(2);
    }

    fseek(self, headerstart+strlen(headerbuf), SEEK_SET);
    fread(packageconfig, 1, 100, self);

    for (z = 0; z < 100; z++) {
        if (packageconfig[z] == '-' || packageconfig[z] == '~') {
            char cur = packageconfig[z];
            packageconfig[z] = 0;
            switch (entry) {
            case 0:
                current_file = 0;
                files = atoi(&packageconfig[start]);
                break;
            case 1:
                packagesize = atoi(&packageconfig[start]);
                break;
            }
            start = z+1;
            if (cur == '~') {
                packageoffset = z + 1;
                z = 100;
            }
            entry++;
        }
    }
    aceoffset=headerstart+strlen(headerbuf)+packageoffset;
    fseek(self, aceoffset, SEEK_SET);
    return TRUE;
}

/*
 * Reads the embedded .cfg data from the executable and place it in the global
 * INSTALL_* variables for use.
 */
int loadheader(void)
{
    char *buffer;
    long headerstart;
    int  z, start = 0, entry = 0;

    buffer = malloc(8096 * 4);

    /* Decode DBSOFT-HEADER */
    if ((headerstart = findtext(decode("EEECFDEPEGFECNEIEFEBEEEFFC"))) < 0) {
        mesg("Could not find Selfinstaller header in executable.");
        exit(2);
    }
    fseek(self, headerstart + 13, SEEK_SET);

    fread(buffer, 1, 8096 * 4, self);
    for (z = 0; z < 8096 * 4; z++) {
        if (buffer[z] == '-' || buffer[z] == '~') {
            char cur = buffer[z];
            buffer[z] = 0;
            switch (entry) {
            case 0:
                INSTALLER_APPLICATION = (char *)strdup(&buffer[start]);
                break;
            case 1:
                INSTALLER_VERSION = (char *)strdup(&buffer[start]);
                break;
            case 2:
                INSTALLER_TITLE = (char *)strdup(&buffer[start]);
                break;
            case 3:
                INSTALLER_PATH = (char *)strdup(&buffer[start]);
                break;
            case 4:
                INSTALLER_FOLDER = (char *)strdup(&buffer[start]);
                break;
            case 5:
                INSTALLER_PROGRAM = (char *)strdup(&buffer[start]);
                break;
            case 6:
                INSTALLER_SHADOW = (char *)strdup(&buffer[start]);
                break;
            case 7:
                INSTALLER_OBJECT = (char *)strdup(&buffer[start]);
                break;
            case 8:
                INSTALLER_SETS = (char *)strdup(&buffer[start]);
                break;
            case 9:
                INSTALLER_SYSVAR = (char *)strdup(&buffer[start]);
                break;
            case 10:
                INSTALLER_SYSLINE = (char *)strdup(&buffer[start]);
                break;
            case 11:
                INSTALLER_BITMAP_WIDTH = atoi(&buffer[start]);
                break;
            case 12:
                INSTALLER_BITMAP_HEIGHT = atoi(&buffer[start]);
                break;
            case 13:
                INSTALLER_PACKAGE_COUNT = atoi(&buffer[start]);
                break;
            default:
                INSTALLER_PACKAGES[entry-14] = malloc((z-start)+1);
                strcpy(INSTALLER_PACKAGES[entry-14], &buffer[start]);
                break;
            }
            start = z + 1;
            if (cur == '~') {
                free(buffer);
                return TRUE;
            }
            entry++;
        }
    }
    free(buffer);
    return FALSE;
}

/*
 * Functions to work on an ace file embedded within the archive in an
 * abstract manner.
 */
int aceread(void *buf, size_t count)
{
    unsigned long curpos = ftell(self);
    size_t readit;

    if(count >  (packagesize-(curpos-aceoffset)))
        readit = (packagesize-(curpos-aceoffset));
    else
        readit = count;

    return fread(buf, 1, readit, self);
}

off_t acelseek(off_t offset, int whence)
{
    switch (whence) {
    case SEEK_SET:
        fseek(self, aceoffset+offset, SEEK_SET);
        break;
    case SEEK_CUR:
        fseek(self, offset, SEEK_CUR);
        break;
    }
    return acepos;
}

int aceopen(const char *path, int flags)
{
    fseek(self, aceoffset, SEEK_SET);
    return 0;
}

int aceclose(int fd)
{
    fseek(self, aceoffset, SEEK_SET);
    return  0;
}

/*
 * Read the generated log file and remove any files installed.
 */
void delete_files(void)
{
    char tmpbuf[8196], *fileptr;
    FILE *tmplf;
    int  linenum = 0, found = -1, z;

    files_deleted = 1;

    if ((tmplf = fopen(instlog, "rb")) == NULL)
        return;

    while (!feof(tmplf)) {
        fgets(tmpbuf, 8196, tmplf);
        linenum++;
        if (tmpbuf[0] == '[' && (char *)strstr(tmpbuf, INSTALLER_APPLICATION) != NULL && !feof(tmplf)) {
            fgets(tmpbuf, 8196, tmplf);
            linenum++;
            if ((char *)strstr(tmpbuf, "<Version>") != NULL && (char *)strstr(tmpbuf, INSTALLER_VERSION) != NULL)
                found = linenum;
        }
    }
    if (found != -1) {
        rewind(tmplf);
        for (z = 0; z < found; z++)
            fgets(tmpbuf, 8196, tmplf);
        while (!feof(tmplf)) {
            fgets(tmpbuf, 8196, tmplf);
            if ((char *)strstr(tmpbuf, "<FileInst>") != NULL) {
                fileptr = (char *)strchr(tmpbuf, ',')+1;
                /* Remove trailing CRLFs */
                if(fileptr[strlen(fileptr)-1] == '\r' || fileptr[strlen(fileptr)-1] == '\n')
                    fileptr[strlen(fileptr)-1]=0;
                if(fileptr[strlen(fileptr)-1] == '\r' || fileptr[strlen(fileptr)-1] == '\n')
                    fileptr[strlen(fileptr)-1] = 0;
                remove(fileptr);
                current_file--;
                WinSendMsg(mainhwnd, WM_USER, 0, 0);
            }
            if ((char *)strstr(tmpbuf, "<End>") != NULL) {
                fclose(tmplf);
                return;
            }
        }
    }
    fclose(tmplf);
    return;
}

/*
 * Removes any carriage returns or line feeds from the buffer.
 */
void stripcrlf(char *buffer)
{
    int z, len = strlen(buffer);

    for (z = 0; z < len; z++) {
        if (buffer[z] == '\r' || buffer[z] == '\n') {
            buffer[z] = 0;
            return;
        }
    }
}

/*
 * Reads a config file into memory for editing with updatesys, updateset, etc.
 */
int readconfigfile(char *filename)
{
    char tmpbuf[8196];
    FILE *tmpcs;

    /* Reset this value when restarting */
    configfilecount = -1;

    if ((tmpcs = fopen(filename, "rb")) == NULL) {
        strcpy(currentcf, empty_string);
        return 1;
    }
    else
        strcpy(currentcf, filename);

    while (!feof(tmpcs)) {
        configfilecount++;
        fgets(tmpbuf, 8196, tmpcs);
        configsys[configfilecount] = malloc(strlen(tmpbuf)+1);
        strcpy(configsys[configfilecount], tmpbuf);
        stripcrlf(configsys[configfilecount]);
    }

    fclose(tmpcs);
    return 0;
}

/*
 * Write the updated config file to disk and backup the original.
 */
int writeconfigfile(char *filename, char *backup)
{
    FILE *tmpcs;
    int i;

    if (backup) {
        remove(backup);
        DosMove(filename, backup);
    }
    else
        remove(filename);

    if ((tmpcs = fopen(filename, "wb")) == NULL)
        return 1;

    for (i = 0; i < configfilecount; i++) {
        if (configsys[i]) {
            fwrite(configsys[i], 1, strlen(configsys[i]), tmpcs);
            /* Add the CRLF that got stripped in the reading stage. */
            fwrite("\r\n", 1, 2, tmpcs);
            free(configsys[i]);
        }
    }

    fclose(tmpcs);
    return 0;
}

/*
 * Adds or replaces a SET variable based on the flags (CONFIG.SYS)
 */
void updateset(char *setname, char *newvalue, int flag)
{
    char *cmpbuf1, *cmpbuf2, *tmpptr, *tmpptr2, *nv;
    int i, z, t;

    nv = replaceem(newvalue);

    cmpbuf1=malloc(strlen(setname)+2);
    strcpy(cmpbuf1, setname);
    strcat(cmpbuf1, "=");
    for (i = 0; i < configfilecount; i++) {
        if (strlen(cmpbuf1) <= strlen(configsys[i])) {
            tmpptr = (char *)strdup(configsys[i]);
            strupr(tmpptr);
            if ((tmpptr2 = (char*)strstr(tmpptr, "SET ")) != NULL) {
                tmpptr2 += 4;
                cmpbuf2 = malloc(strlen(tmpptr2)+1);
                /* Remove any spaces from the string */
                z = 0;
                for (t = 0; t < strlen(tmpptr2) && z < strlen(cmpbuf1); t++) {
                    if (tmpptr2[t] != ' ') {
                        cmpbuf2[z]=tmpptr2[t];
                        z++;
                    }
                }
                cmpbuf2[z] = 0;
                if (stricmp(cmpbuf1, cmpbuf2) == 0) {
                    /* Ok we found the entry, and if UPDATE_ALWAYS change it to the
                     new entry, otherwise exit */
                    if (flag == UPDATE_ALWAYS) {
                        fprintf(logfile, "<CFRemLine>,%s,%s\r\n", currentcf, configsys[i]);
                        free(configsys[i]);
                        configsys[i] = malloc(strlen(setname)+strlen(nv)+6);
                        strcpy(configsys[i], "SET ");
                        strcat(configsys[i], setname);
                        strcat(configsys[i], "=");
                        strcat(configsys[i], nv);
                        fprintf(logfile, "<CFAddLine>,%s,%s\r\n", currentcf, configsys[i]);
                        free(cmpbuf1);free(cmpbuf2);free(tmpptr);
                    }
                    return;
                }
                free(cmpbuf2);
            }
            free(tmpptr);
        }
    }
    /* Couldn't find the line so we'll add it */
    configsys[configfilecount] = malloc(strlen(cmpbuf1)+strlen(nv)+6);
    strcpy(configsys[configfilecount], "SET ");
    strcat(configsys[configfilecount], setname);
    strcat(configsys[configfilecount], "=");
    strcat(configsys[configfilecount], nv);
    fprintf(logfile, "<CFAddLine>,%s,%s\r\n", currentcf, configsys[configfilecount]);
    configfilecount++;
    free(cmpbuf1);
}

/*
 * Adds an entry to a system variable (CONFIG.SYS)
 */
void updatesys(char *sysname, char *newvalue)
{
    char *cmpbuf1, *cmpbuf2, *tmpptr, *tmpptr2, *capbuf1, *capbuf2, *nv, *brian;
    int i, z, t;

    nv = replaceem(newvalue);

    cmpbuf1 = malloc(strlen(sysname) + 2);
    /* Remove any spaces from the string */
    for (i = 0, z = 0; i < strlen(sysname); i++)
        if (sysname[i] != ' ')
            cmpbuf1[z++] = sysname[i];
    cmpbuf1[z] = '\0';
    strcat(cmpbuf1, "=");
    for (i = 0; i < configfilecount; i++) {
        if (strlen(cmpbuf1) <= strlen(configsys[i])) {
            cmpbuf2 = malloc(strlen(configsys[i]) + 1);
            /* Remove any spaces from the string */
            z = 0;
            for (t = 0; t < strlen(configsys[i]) && z < strlen(cmpbuf1); t++) {
                if (configsys[i][t] != ' ') {
                    cmpbuf2[z] = configsys[i][t];
                    z++;
                }
            }
            cmpbuf2[z] = 0;
            if (stricmp(cmpbuf1, cmpbuf2) == 0) {
                /* Do a case insensitive comparison but preserve the case */
                tmpptr = &configsys[i][t];
                capbuf1 = malloc(strlen(tmpptr) + 1);
                capbuf2 = malloc(strlen(nv) + 1);
                strcpy(capbuf1, tmpptr);
                strcpy(capbuf2, nv);
                strupr(capbuf1);
                strupr(capbuf2);
                /* Ok, we found the line, and it doesn't have an entry so we'll add it */
                if ((tmpptr2 = (char *)strstr(capbuf1, capbuf2)) == NULL) {
                    fprintf(logfile, "<CFRemLine>,%s,%s\r\n", currentcf, configsys[i]);
                    brian = configsys[i];
                    configsys[i] = malloc(strlen(configsys[i])+strlen(nv)+4);
                    strcpy(configsys[i], brian);
                    free(brian);
                    /* Remove any trailing CRLFs */
                    if (configsys[i][strlen(configsys[i]) - 1] != ';')
                        strcat(configsys[i], ";");
                    strcat(configsys[i], nv);
                    strcat(configsys[i], ";");
                    fprintf(logfile, "<CFAddLine>,%s,%s\r\n", currentcf, configsys[i]);
                }
                free(cmpbuf1);
                free(cmpbuf2);
                free(capbuf1);
                free(capbuf2);
                return;
            }
            free(cmpbuf2);
        }
    }
    /* Couldn't find the line so we'll add it */
    configsys[configfilecount] = malloc(strlen(sysname) + strlen(nv) + 4);
    strcpy(configsys[configfilecount], sysname);
    strcat(configsys[configfilecount], "=");
    strcat(configsys[configfilecount], nv);
    strcat(configsys[configfilecount], ";");
    fprintf(logfile, "<CFAddLine>,%s,%s\r\n", currentcf, configsys[configfilecount]);
    configfilecount++;
    free(cmpbuf1);
    if (nv)
        free(nv);
}

/*
 * Removes a line from a config file.
 */
void removeline(char *text)
{
    int z;
    for (z = 0; z < configfilecount; z++) {
        if (stricmp(configsys[z], text) == 0) {
            int t;

            fprintf(logfile, "<CFRemLine>,%s,%s\r\n", currentcf, configsys[z]);
            free(configsys[z]);
            for(t=z;t<(configfilecount-1);t++)
                configsys[t] = configsys[t+1];
            configfilecount--;
        }
    }
}


/*
 * Display a fatal error message and set the abort flag in case we are in a secondary thread.
 */
void error(char *format, ...) {
    va_list args;
    char errstring[1024];

    va_start(args, format);
    vsprintf(errstring, format, args);
    va_end(args);

    success = 1;
    installstate = ABORTED;
    WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, errstring, INSTALLER_TITLE, 0, MB_OK | MB_ERROR | MB_MOVEABLE | MB_SYSTEMMODAL);
}

/*
 * The Window procedure for the confirmation dialog.
 */
MRESULT EXPENTRY ConfirmDlgProc(HWND hWnd, ULONG msg, MPARAM mp1,   MPARAM mp2)
{
    SWP winpos;

    switch (msg)
    {
    case WM_INITDLG:
        WinSetWindowText(hWnd, INSTALLER_TITLE);
        WinEnableWindow(WinWindowFromID(mainhwnd, I_Cancel), FALSE);
        WinSetDlgItemText(hWnd, I_Confirm, confirmstring);
        WinQueryWindowPos(mainhwnd, &winpos);
        WinSetWindowPos(hWnd, HWND_TOP, winpos.x+30, winpos.y+30, 0, 0, SWP_MOVE | SWP_ZORDER);
        break;
    case WM_COMMAND:
        WinEnableWindow(WinWindowFromID(mainhwnd, I_Cancel), TRUE);
        switch ( SHORT1FROMMP(mp1) )
        {
        case I_Ja:
            WinDismissDlg(hWnd, 0);
            break;
        case I_Alle:
            WinDismissDlg(hWnd, 1);
            break;
        case I_Nein:
            WinDismissDlg(hWnd, 2);
            break;
        case I_Halt:
            success=2;
            installstate=ABORTED;
            WinDismissDlg(hWnd, 3);
            break;
        }
        break;
    default :
        return(WinDefDlgProc(hWnd, msg, mp1, mp2));
    }
    return(0L);
}
/*
 * Display a confirmation dialog with the options: YES NO ALL CANCEL
 * Returns: 0 for YES, 1 for ALL, 2 for NO and 3 for CANCEL
 */
int confirm(char *format, ...) {
    va_list args;

    va_start(args, format);
    vsprintf(confirmstring, format, args);
    va_end(args);

    return WinDlgBox(HWND_DESKTOP, HWND_DESKTOP, &ConfirmDlgProc, NULLHANDLE, I_Karte, NULL);
}

/*
 * Make the TEMP directory the current directory, or the root directory of the boot drive.
 */
void settempdir(void)
{
    int len;

    strcpy(tempPath, replaceem("%BOOTDRIVE%:\\"));
    len = strlen(tempPath);
    if (len > 3 && tempPath[len-1] == '\\')
        tempPath[len-1] = 0;
    strupr(tempPath);
    setdrivedir(tempPath);
}

/*
 * A function to grab a file from an embedded archive and extract it to the TEMP directory.
 */
void grabfile(char *filename)
{
    no_update = 1;
    settempdir();
    remove(filename);
    aceseek_entry(0);
    resetglobals();
    installer_unpack(filename, 2);
    no_update = 0;
}

void getbootdrive(void)
{
    ULONG   bootdrv = 0;       /* System Information Data Buffer */

    DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &bootdrv, sizeof(ULONG));

    bootdrive[0]=installdir[0]=instlog[0]=csfile[0]=bufile[0]=(char)('A'+(bootdrv-1));
}
/*
 * Start an application using CMD.EXE.
 */
int cmdrun(char *progname)
{
    STARTDATA SData       = {0};
    PSZ       PgmTitle    = "",
              PgmName     = "CMD.EXE";
    APIRET    rc          = NO_ERROR;
    PID       pid         = 0;
    ULONG     ulSessID    = 0;
    CHAR      achObjBuf[256] = {0};
    HQUEUE hqQueue;
    REQUESTDATA rdRequest;
    ULONG ulSzData;
    BYTE bPriority;
    PVOID pvData;

    SData.Length  = sizeof(STARTDATA);
    SData.Related = SSF_RELATED_CHILD;
    SData.FgBg    = SSF_FGBG_BACK;
    SData.TraceOpt = SSF_TRACEOPT_NONE;
    SData.PgmTitle = PgmTitle;
    SData.PgmName = PgmName;
    SData.PgmInputs = progname;

    SData.TermQ = "\\QUEUES\\CHILD.QUE";
    SData.Environment = 0;
    SData.InheritOpt = SSF_INHERTOPT_SHELL;
    SData.SessionType = SSF_TYPE_WINDOWABLEVIO;
    SData.IconFile = 0;
    SData.PgmHandle = 0;

    SData.PgmControl = SSF_CONTROL_INVISIBLE;
    SData.Reserved = 0;
    SData.ObjectBuffer = achObjBuf;
    SData.ObjectBuffLen = (ULONG) sizeof(achObjBuf);

    if (!(rc = DosCreateQueue(&hqQueue, QUE_FIFO | QUE_CONVERT_ADDRESS,"\\QUEUES\\CHILD.QUE"))) {
        if (!(rc = DosStartSession(&SData, &ulSessID, &pid)))
            if (!(rc=DosReadQueue(hqQueue, &rdRequest, &ulSzData, &pvData, 0, 0, &bPriority, 0)))
                DosFreeMem(pvData);
        DosCloseQueue(hqQueue);
    }

    if (rc != NO_ERROR) {
       mesg("Error %d while attempting to run %s!", rc, progname);
       return 1;
    }
    return NO_ERROR;
}

/*
 * This thread runs along side the main thread allowing the user to cancel the process.
 */
void install_thread(void *param)
{
    HAB ihab = 0;
    HMQ ihmq = 0;
    char tmpinstallpath[1024];
    int  k, j, installcount=0, installed=0;

    (void)param;
    ihab = WinInitialize(0);
    ihmq = WinCreateMsgQueue(ihab, 0);

    installstate = INSTALLING;

    if ((logfile = fopen(instlog, "ab")) == NULL) {
        error("Log file \"%s\" open failed! Installation aborted!", instlog);
        exit(1);
    }

    fprintf(logfile, "[%s]\r\n<Version>,%s\r\n<InstallDir>,%s\r\n<Start>\r\n", INSTALLER_APPLICATION, INSTALLER_VERSION, installdir);

    /* Create nested subdirectories if necessary. */
    strcpy(tmpinstallpath, installdir);
    for (k = 3; k < strlen(installdir); k++) {
        if (tmpinstallpath[k] == '\\') {
            tmpinstallpath[k] = 0;
            if(DosCreateDir(tmpinstallpath, NULL)==NO_ERROR)
                fprintf(logfile, "<NewDir>,%s\r\n", tmpinstallpath);
            tmpinstallpath[k] = '\\';
        }
    }

    if (DosCreateDir(installdir, NULL) == NO_ERROR)
        fprintf(logfile, "<NewDir>,%s\r\n", installdir);

    if (strlen(installdir) > 0 && installdir[0] > 'a'-1 && installdir[0] < 'z'+1)
        installdir[0] = installdir[0] - ('a'-'A');
    if (strlen(installdir) > 2 && installdir[1]==':' && installdir[2]=='\\')
        DosSetDefaultDisk((int)(installdir[0] - 'A'+1));

    DosSetCurrentDir(installdir);

    /* Unpack files to destination directory */
    for (j = 0; j < INSTALLER_PACKAGE_COUNT; j++) {
        if (packagesselected[j] == TRUE)
            installcount++;
    }
    if (installcount == 0) {
        mesg("No packages selected for installation!");
    }
    else {
        for (j = 0; j < INSTALLER_PACKAGE_COUNT; j++) {
            if (packagesselected[j] == TRUE) {
                char statustext[512];
                aceseek_entry(j);
                resetglobals();
#if SELECTPACKAGES
                sprintf(statustext,"Copying Files for %s %d/%d, Press \"Exit Installation\" to Abort.", INSTALLER_PACKAGES[j], installed+1, installcount);
#else
                sprintf(statustext,"Copying Files...\n Press \"Exit Installation\" to Abort.", INSTALLER_PACKAGES[j], installed+1, installcount);
#endif
                WinSetDlgItemText(mainhwnd, I_Info3, statustext);
                /* filename parameter when null is all files */
                installer_unpack(NULL, 2);
                installed++;
            }
        }
    }

    if (installstate == ABORTED) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "User aborted installation!", INSTALLER_TITLE, 0, MB_OK | MB_ERROR | MB_MOVEABLE | MB_SYSTEMMODAL);
    }
    else if (success == 1) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Error unpacking files, Installer may be corrupt!", INSTALLER_TITLE, 0, MB_OK | MB_ERROR | MB_MOVEABLE | MB_SYSTEMMODAL);
    }

    if (installstate != ABORTED)
        installstate = COMPLETED;

#ifdef EXTRAINST
    /* Run a custom batch file */
    setdrivedir(replaceem("%USERPATH%\\disk"));

    /* This is where we start messing with CONFIG.SYS, so now's    */
    /* a good time to make a backup copy.                          */
    DosCopy(csfile, bufile, DCPY_EXISTING);

    if (updconfig && installstate == COMPLETED) {
        char *t = replaceem("/c %USERPATH%\\disk\\setup.cmd foo");

        cmdrun(t);

        free(t);
    }
#endif

    WinSendMsg(mainhwnd, WM_USER + 1, NULL, NULL);

    WinDestroyMsgQueue(ihmq);
    WinTerminate(ihab);
}

/*
 * Use the information from the .cfg file which is embedded in the EXE to update the
 * CONFIG.SYS settings.
 */
void configsys_update(void)
{
    char *arg1, *arg2, *arg3;
    char temp[5000];
    int  z, argn;

    if (readconfigfile(csfile))
        return;

    /* Update Miscellaneous lines */
    if (strlen(INSTALLER_SYSLINE) > 0) {
        char *tmpptr = &temp[0];
        int len;

        strcpy(temp, INSTALLER_SYSLINE);
        temp[4999] = 0;
        len = strlen(temp);

        for (z = 0; z < len; z++) {
            if (temp[z] == ',') {
                char *tmpptr2;

                temp[z] = 0;
                tmpptr2 = replaceem(tmpptr);
                tmpptr = &temp[z + 1];
                removeline(tmpptr2);
                configsys[configfilecount] = tmpptr2;
                fprintf(logfile, "<CFAddLine>,%s,%s\r\n", currentcf, tmpptr2);
                configfilecount++;
            }
        }
        if (tmpptr && *tmpptr) {
            char *tmpptr2;

            tmpptr2 = replaceem(tmpptr);
            removeline(tmpptr2);
            configsys[configfilecount] = tmpptr2;
            fprintf(logfile, "<CFAddLine>,%s,%s\r\n", currentcf, tmpptr2);
            configfilecount++;
        }

    }
    /* Update SET variables */
    argn = 0;
    if (strlen(INSTALLER_SETS) > 0) {
        strcpy(temp, INSTALLER_SETS);
        arg1 = &temp[0];
        arg2 = arg3 = NULL;
        for (z = 0; z < strlen(INSTALLER_SETS); z++) {
            if (temp[z] == ',') {
                argn++;
                temp[z] = 0;
                switch (argn) {
                case 1:
                    arg2 = &temp[z+1];
                    break;
                case 2:
                    arg3 = &temp[z+1];
                    break;
                case 3:
                    argn = 0;
                    updateset(arg1, arg2, (int)(arg3[0] - '0'));
                    arg1 = &temp[z + 1];
                    arg2 = arg3 = NULL;
                    break;
                }
            }
        }
        updateset(arg1, arg2, (int)arg3-'0');
    }

    /* Update system variables */
    argn = 0;
    if (strlen(INSTALLER_SYSVAR) > 0) {
        strcpy(temp, INSTALLER_SYSVAR);
        arg1 = &temp[0];
        arg2 = NULL;
        for (z = 0; z < strlen(INSTALLER_SYSVAR); z++) {
            if (temp[z] == ',') {
                argn++;
                temp[z] = 0;
                switch (argn) {
                case 1:
                    arg2 = &temp[z + 1];
                    break;
                case 2:
                    argn = 0;
                    updatesys(arg1, arg2);
                    arg1 = &temp[z + 1];
                    arg2 = NULL;
                    break;
                }
            }
        }
        updatesys(arg1, arg2);
    }

    writeconfigfile(csfile, bufile);
}

void create_wps_objects(void)
{
    char *arg1, *arg2, *arg3, *arg4, *arg5, *arg6;
    char temp[5000];
    char zerotext[2] = "";
    int z, argn, len;

    /* Create Folder Objects */
    if (strlen(INSTALLER_FOLDER)>0) {
        strcpy(temp, replaceem(INSTALLER_FOLDER));
        argn=0;
        arg1=&temp[0];
        arg2=arg3=arg4=arg5=&zerotext[0];
        len = strlen(temp);
        for (z = 0; z < len; z++) {
            if (temp[z]==',') {
                argn++;
                temp[z]=0;
                switch (argn) {
                case 1:
                    arg2=&temp[z+1];
                    break;
                case 2:
                    arg3=&temp[z+1];
                    break;
                case 3:
                    arg4=&temp[z+1];
                    break;
                case 4:
                    arg5=&temp[z+1];
                    break;
                case 5:
                    argn=0;
                    MakeFolder(arg1, arg2, arg3, arg4, arg5);
                    fprintf(logfile, "<WPSFolderAdd>,%s,%s,%s,%s,%s\r\n", arg1, arg2,arg3,arg4,arg5);
                    arg1=&temp[z+1];
                    arg2=arg3=arg4=arg5=&zerotext[0];
                    break;
                }
            }
        }
        MakeFolder(arg1, arg2, arg3, arg4, arg5);
        fprintf(logfile, "<WPSFolderAdd>,%s,%s,%s,%s,%s\r\n", arg1, arg2,arg3,arg4,arg5);
    }

    /* Create Program Objects */
    if (strlen(INSTALLER_PROGRAM)>0) {
        strcpy(temp, replaceem(INSTALLER_PROGRAM));
        argn=0;
        arg1=&temp[0];
        arg2=arg3=arg4=arg5=arg6=&zerotext[0];
        len = strlen(temp);
        for (z = 0; z < len; z++) {
            if (temp[z]==',') {
                argn++;
                temp[z]=0;
                switch (argn) {
                case 1:
                    arg2=&temp[z+1];
                    break;
                case 2:
                    arg3=&temp[z+1];
                    break;
                case 3:
                    arg4=&temp[z+1];
                    break;
                case 4:
                    arg5=&temp[z+1];
                    break;
                case 5:
                    arg6=&temp[z+1];
                    break;
                case 6:
                    argn=0;
                    MakeProgram(arg1, arg2, arg3, arg4, arg5, arg6);
                    fprintf(logfile, "<WPSProgramAdd>,%s,%s,%s,%s,%s,%s\r\n", arg1,arg2,arg3,arg4,arg5,arg6);
                    arg1=&temp[z+1];
                    arg2=arg3=arg4=arg5=arg6=&zerotext[0];
                    break;
                }
            }
        }
        MakeProgram(arg1, arg2, arg3, arg4, arg5, arg6);
        fprintf(logfile, "<WPSProgramAdd>,%s,%s,%s,%s,%s,%s\r\n", arg1, arg2,arg3,arg4,arg5,arg6);
    }

    /* Create Shadow Objects */
    if (strlen(INSTALLER_SHADOW)>0) {
        strcpy(temp, replaceem(INSTALLER_SHADOW));
        argn=0;
        arg1=&temp[0];
        arg2=arg3=arg4=&zerotext[0];
        len = strlen(temp);
        for (z = 0; z < len; z++) {
            if (temp[z]==',') {
                argn++;
                temp[z]=0;
                switch (argn) {
                case 1:
                    arg2=&temp[z+1];
                    break;
                case 2:
                    arg3=&temp[z+1];
                    break;
                case 3:
                    arg4=&temp[z+1];
                    break;
                case 4:
                    argn=0;
                    MakeShadow(arg1, arg2, arg3, arg4);
                    fprintf(logfile, "<WPSShadowAdd>,%s,%s,%s,%s\r\n", arg1,arg2,arg3,arg4);
                    arg1=&temp[z+1];
                    arg2=arg3=arg4=&zerotext[0];
                    break;
                }
            }
        }
        MakeShadow(arg1, arg2, arg3, arg4);
        fprintf(logfile, "<WPSShadowAdd>,%s,%s,%s,%s\r\n", arg1,arg2,arg3,arg4);
    }

    /* Create Generic Objects */
    if (strlen(INSTALLER_OBJECT)>0) {
        strcpy(temp, replaceem(INSTALLER_OBJECT));
        argn=0;
        arg1=&temp[0];
        arg2=arg3=arg4=arg5=&zerotext[0];
        len = strlen(temp);
        for (z = 0; z < len; z++) {
            if (temp[z]==',') {
                argn++;
                temp[z]=0;
                switch (argn) {
                case 1:
                    arg2=&temp[z+1];
                    break;
                case 2:
                    arg3=&temp[z+1];
                    break;
                case 3:
                    arg4=&temp[z+1];
                    break;
                case 4:
                    arg5=&temp[z+1];
                    break;
                case 5:
                    argn=0;
                    MakeObject(arg1, arg2, arg3, arg4, arg5);
                    fprintf(logfile, "<WPSObjectAdd>,%s,%s,%s,%s,%s\r\n", arg1,arg2,arg3,arg4,arg5);
                    arg1=&temp[z+1];
                    arg2=arg3=arg4=arg5=&zerotext[0];
                    break;
                }
            }
        }
        MakeObject(arg1, arg2, arg3, arg4, arg5);
        fprintf(logfile, "<WPSObjectAdd>,%s,%s,%s,%s,%s\r\n", arg1, arg2,arg3,arg4,arg5);
    }
}

/*
 * Reads a line from a file and returns it in raw.
 */
void getline(FILE *f, char *raw)
{
    memset(raw, 0, 256);
    fgets(raw, 255, f);
    stripcrlf(raw);
}

/*
 * Removes a character from a buffer by advancing the buffer to the left.
 */
void removechar(char *buffer, int thisone, int len)
{
    int x;

    for (x = thisone; x < len; x++)
        buffer[x] = buffer[x + 1];
}

/*
 * Breaks up a line into its components.
 */
void parseline(char *raw, char comment, char delimiter, char quotes, char *entry, char *entrydata, char *entrydata2)
{
    char in[256];
    int z, len, in_quotes = 0, entrynum=0, last = 0;

    strcpy(entry, empty_string);
    strcpy(entrydata, empty_string);
    strcpy(entrydata2, empty_string);
    strcpy(in, raw);

    if (in[strlen(in)-1] == '\n')
        in[strlen(in)-1] = 0;

    if (in[0] != comment) {
        len=strlen(in);
        for (z = 0; z < len; z++) {
            if (!in_quotes && in[z] == delimiter) {
                in[z] = 0;
                /* Strip any other delimiters */
                z++;
                while (in[z] == delimiter && z < len)
                    z++;
                if (!entrynum)
                    strcpy(entry, in);
                else {
                    strcpy(entrydata, &in[last]);
                    strcpy(entrydata2, &in[z]);
                }
                last = z;
                if(entrynum)
                    return;
                entrynum++;
            }
            if (in[z] == quotes) {
                removechar(in, z, len);
                z--;
                len--;
                if(in_quotes)
                    in_quotes = 0;
                else
                    in_quotes = 1;
            }
        }
        if (!entrynum)
            strcpy(entry, in);
        else
            strcpy(entrydata, &in[last]);
    }
}

/*
 * Reads a line from the file and splits it up into it's components.
 */
void getparseline(FILE *f, char comment, char delimiter, char quotes, char *raw, char *entry, char *entrydata, char *entrydata2)
{
    getline(f, raw);
    parseline(raw, comment, delimiter, quotes, entry, entrydata, entrydata2);
}

#if 0
/*
 * Removes an entry in a windows ini file based on the value of entry.
 */
void removewinentry(char *section, char *entry)
{
    int z;
    char sec[400];
    char *tmp = replacestr(entry, "%ANYSTRING%", "\n");

    sprintf(sec, "[%s]", section);

    for (z = 0; z < configfilecount; z++) {
        if (stricmp(configsys[z], sec) == 0) {
            int x;

            for (x = 0; x < strlen(tmp); x++)
                if (tmp[x] == '\n')
                    tmp[x] = 0;
            if (strnicmp(tmp, configsys[z], strlen(tmp)) == 0) {
                /* Found it :) */
                int k;

                fprintf(logfile, "<WIRemLine>,%s,%s\r\n", currentcf, configsys[z]);
                free(configsys[z]);

                for (k = z; k < (configfilecount - 1); k++)
                    configsys[k] = configsys[k+1];
            }
        }
    }

}

/*
 * Adds an entry in a windows ini file.
 */
void addwinentry(char *section, char *entry, char *entrydata)
{
    int z;
    char sec[400], buffer[1024];

    sprintf(sec, "[%s]", section);

    for (z = 0; z < configfilecount; z++) {
        if (stricmp(configsys[z], sec) == 0) {
            int t;
            /* This code looks for an existing entry an replaces it if it finds one */
            for (t = z + 1; t < configfilecount; t++) {
                if (configsys[t][0] == '[')
                    t = configfilecount;
                else {
                    char tmp[1024];
                    int k, len = strlen(configsys[t]);

                    strcpy(tmp, configsys[t]);
                    for (k = 0; k < len; k++) {
                        if (tmp[k] == '=') {
                            tmp[k] = 0;
                            if (stricmp(entry, tmp)==0) {
                                fprintf(logfile, "<WIRemLine>,%s,%s\r\n", currentcf, configsys[t]);
                                free(configsys[t]);
                                sprintf(buffer, "%s=%s", entry, entrydata);
                                configsys[t] = replaceem(buffer);
                                fprintf(logfile, "<WIAddLine>,%s,%s\r\n", currentcf, configsys[t]);
                                return;
                            }
                        }
                    }
                }
            }
            /* This code adds a new entry if we couldn't find an existing one */
            for (t = configfilecount; t > z; t--)
                configsys[t+1] = configsys[t];
            configfilecount++;
            sprintf(buffer, "%s=%s", entry, entrydata);
            configsys[z+1] = replaceem(buffer);
            fprintf(logfile, "<WIAddLine>,%s,%s\r\n", currentcf, configsys[z+1]);
            return;
        }
    }

}

#endif

/*
 * Window procedure for the package selection dialog when there are multiple
 * packages included in an archive.
 */
MRESULT EXPENTRY PackageDlgProc(HWND hWnd, ULONG msg, MPARAM mp1,   MPARAM mp2)
{
    int j, i, count = 0;

    switch(msg) {
    case WM_INITDLG:
        for (j = 0; j < INSTALLER_PACKAGE_COUNT; j++)
            WinSendMsg(WinWindowFromID(hWnd, PACKAGES), LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP(INSTALLER_PACKAGES[j]));
        break;
        /* Process push button selections */
    case WM_CLOSE:
        for (j = 0; j < INSTALLER_PACKAGE_COUNT; j++)
            packagesselected[j] = FALSE;
        i = (int)WinSendMsg(WinWindowFromID(hWnd, PACKAGES),
                            LM_QUERYSELECTION,
                            MPFROMSHORT(LIT_FIRST),0L);
        while (i != LIT_NONE) {
            packagesselected[i] = TRUE;
            i = (int)WinSendMsg(WinWindowFromID(hWnd, PACKAGES),
                                LM_QUERYSELECTION,
                                (MPARAM)i,0L);
        }
        for (j = 0; j < INSTALLER_PACKAGE_COUNT; j++) {
            if (packagesselected[j] == TRUE)
                count++;
        }
        if (count == 0)
            packagechosen = FALSE;
        else
            packagechosen = TRUE;

        WinDismissDlg(hWnd, FALSE);
        break;
    case WM_COMMAND:
        switch (SHORT1FROMMP(mp1)) {
        case PB_OK:
            for (j = 0; j < INSTALLER_PACKAGE_COUNT; j++)
                packagesselected[j] = FALSE;
            i = (int)WinSendMsg(WinWindowFromID(hWnd, PACKAGES),
                                LM_QUERYSELECTION,
                                MPFROMSHORT(LIT_FIRST),0L);
            while (i != LIT_NONE) {
                packagesselected[i] = TRUE;
                i = (int)WinSendMsg(WinWindowFromID(hWnd, PACKAGES),
                                    LM_QUERYSELECTION,
                                    (MPARAM)i,0L);
            }
            for (j = 0; j < INSTALLER_PACKAGE_COUNT; j++) {
                if (packagesselected[j] == TRUE)
                    count++;
            }
            if (count == 0)
                packagechosen=FALSE;
            else
                packagechosen=TRUE;

            WinDismissDlg(hWnd, FALSE);
            break;
        case PB_CANCEL:
            WinDismissDlg(hWnd, FALSE);
            break;
        case PB_SELECTALL:
            for (j = 0; j < INSTALLER_PACKAGE_COUNT; j++)
                WinSendMsg(WinWindowFromID(hWnd, PACKAGES), LM_SELECTITEM, MPFROMSHORT(j), (MPARAM)TRUE);
            break;
        case PB_DESELECTALL:
            WinSendMsg(WinWindowFromID(hWnd, PACKAGES), LM_SELECTITEM, MPFROMSHORT(LIT_NONE), FALSE);
            break;
        }
        break;
        /* Pass through unhandled messages */
    default :
        return(WinDefDlgProc(hWnd, msg, mp1, mp2));
    }
    return(0L);
}

/*
 * Configures the container window passed to it for use with populatedir()
 */
void setdir(HWND hwnd)
{
    PFIELDINFO details, first, left = NULL;
    FIELDINFOINSERT detin;
    CNRINFO cnri;
    int z;
    ULONG flData[2] = {  CFA_BITMAPORICON | CFA_CENTER | CFA_HORZSEPARATOR | CFA_SEPARATOR,
    CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR };
    ULONG offStruct[2] = {
        offsetof(CnrEntry, hptrIcon),
        offsetof(CnrEntry, filename),
    };

    first = details = (PFIELDINFO)WinSendMsg(hwnd, CM_ALLOCDETAILFIELDINFO, MPFROMLONG(2), 0L);
    if (!first) {
        mesg("Error creating Directory container!");
        return;
    }
    for (z = 0; z < 2; z++) {
        if (z == 1)
            left = details;
        details->cb = sizeof(FIELDINFO);
        details->flData = flData[z];
        details->flTitle = CFA_FITITLEREADONLY;
        switch (z) {
        case 0:
            details->pTitleData = "Icon";
            break;
        case 1:
            details->pTitleData = "Name";
            break;
        }
        details->offStruct = offStruct[z];
        details = details->pNextFieldInfo;
    }

    cnri.cb = sizeof(CNRINFO);
    cnri.pFieldInfoLast = left;
    cnri.xVertSplitbar  = 150;
    cnri.flWindowAttr = CV_DETAIL | CV_MINI;
    cnri.slBitmapOrIcon.cx = 15;
    cnri.slBitmapOrIcon.cy = 15;

    WinSendMsg(hwnd, CM_SETCNRINFO, MPFROMP(&cnri),  MPFROMLONG(CMA_FLWINDOWATTR | CMA_SLBITMAPORICON /*| CMA_PFIELDINFOLAST | CMA_XVERTSPLITBAR*/));

    detin.cb = sizeof(FIELDINFOINSERT);
    detin.fInvalidateFieldInfo = FALSE;
    detin.pFieldInfoOrder = (PFIELDINFO) CMA_FIRST;
    detin.cFieldInfoInsert = 2;

    WinSendMsg(hwnd, CM_INSERTDETAILFIELDINFO, MPFROMP(first), MPFROMP(&detin));
}

/*
 * Reads the directory list from the directory specified in the global variable
 * browserdir and populates the container window passed to it.
 */
void populatedir(HWND hwnd)
{
    FILEFINDBUF3    ffb;
    APIRET          rc;
    HPOINTER        fileicon, foldericon;
    RECORDINSERT    recin;
    HDIR            hdir = HDIR_CREATE;
    ULONG           findcount = 1;
    CnrEntry        *direntry, *first;
    int             count = 0;
    char            tmpbuf[201];

    fileicon = WinLoadPointer(HWND_DESKTOP,NULLHANDLE,FILEICON);
    foldericon = WinLoadPointer(HWND_DESKTOP,NULLHANDLE,FOLDERICON);

    strcpy(tmpbuf, browsedir);
    if (tmpbuf[strlen(tmpbuf)-1] == '\\')
        strcat(tmpbuf, "*");
    else
        strcat(tmpbuf, "\\*");

    WinSendMsg(hwnd, CM_REMOVERECORD, (MPARAM)0L, MPFROM2SHORT(0, CMA_INVALIDATE | CMA_FREE));

    if ((rc = DosFindFirst(tmpbuf, &hdir, MUST_HAVE_DIRECTORY, &ffb, sizeof(FILEFINDBUF3), &findcount, FIL_STANDARD)) == NO_ERROR) {
        while (rc != ERROR_NO_MORE_FILES) {
            if (strcmp(ffb.achName, ".") != 0)
                count++;
            rc = DosFindNext(hdir, &ffb, sizeof(FILEFINDBUF3), &findcount);
        }
    }
    DosFindClose(hdir);

    hdir = HDIR_CREATE;
    findcount = 1;

    first = direntry = (CnrEntry *)WinSendMsg(hwnd, CM_ALLOCRECORD, MPFROMLONG(sizeof(CnrEntry) - sizeof(RECORDCORE)), MPFROMLONG(count));

    if (!first) {
        mesg("Error populating directory");
        return;
    }

    if ((rc = DosFindFirst(tmpbuf, &hdir, MUST_HAVE_DIRECTORY, &ffb, sizeof(FILEFINDBUF3), &findcount, FIL_STANDARD)) == NO_ERROR) {
        while (rc != ERROR_NO_MORE_FILES) {
            if (strcmp(ffb.achName, ".") != 0) {
                direntry->hptrIcon = direntry->rc.hptrIcon = foldericon;
                direntry->filename = (char *)strdup(ffb.achName);
                direntry->rc.pszIcon = direntry->filename;
                direntry->rc.pszName = direntry->filename;
                direntry->rc.pszText = direntry->filename;

                direntry = (CnrEntry *)direntry->rc.preccNextRecord;
            }

            rc = DosFindNext(hdir, &ffb, sizeof(FILEFINDBUF3), &findcount);
        }
    }

    DosFindClose(hdir);

    recin.cb = sizeof(RECORDINSERT);
    recin.pRecordOrder = (PRECORDCORE)CMA_END;
    recin.pRecordParent = NULL;
    recin.zOrder = CMA_TOP;
    recin.fInvalidateRecord = TRUE;
    recin.cRecordsInsert = count;

    WinSendMsg(hwnd, CM_INSERTRECORD, MPFROMP(first), MPFROMP(&recin));

    WinDestroyPointer(foldericon);
    WinDestroyPointer(fileicon);
}

/*
 * Thread to spawn the readme file.
 */
void readme(void *param)
{
    HAB zhab;
    HMQ zhmq;
    char *filename = (char *)param;

    zhab = WinInitialize(0);
    zhmq = WinCreateMsgQueue(zhab, 0);

    viewfile(filename, zhab);
    remove(filename);

    while (WinGetMsg(zhab, &qmsg, (HWND)NULL, 0, 0))
        WinDispatchMsg(zhab, &qmsg);

    WinSendMsg(mainhwnd, WM_USER + 2, NULL, NULL);
    WinDestroyMsgQueue(zhmq);
    WinTerminate(zhab);
}

/*
 * Thread to download the new version and start it up.
 */
void getupdate(void *param)
{
    if (strnicmp(wupdurl[downloadsite], "ftp", 3) == 0) {
        char site[256], *path = empty_string;

        if (strlen(wupdurl[downloadsite]) > 6) {
            int z, len = strlen(&wupdurl[downloadsite][6]);
            for (z = 6; z < len; z++) {
                if (wupdurl[downloadsite][z] == '/') {
                    strncpy(site, &wupdurl[downloadsite][6], z-6);
                    path = &wupdurl[downloadsite][z];
                    z = len;
                }

            }
            remove(wupdfilename);
            if (!ftpget("ftp.update.com", "anonymous", "install@update.com", NULL, wupdfilename, path, "w", T_BINARY))
                error("Error downloading new version.");
            else {
                spawnlp(P_NOWAIT, wupdfilename, wupdfilename, NULL);
                exit(-1);
                }
        }
        else
            error("Unable to determine path to file.");
    }
}

/*
 * Thread to download the status file and check the version.
 */
void checkforupdates(void *param)
{
    HAB xhab = 0;
    HMQ xhmq = 0;

    xhab = WinInitialize(0);
    xhmq = WinCreateMsgQueue(xhab, 0);

    settempdir();
    if (ftpget("ftp.update.com", "anonymous", "install@update.com", NULL, "update.ini", "/update/update.ini", "w", T_BINARY)) {
        int savestate = installstate;
        error("Unable to connect to vendor's server.  We cannot tell if you have the current version.");
        success=0;
        installstate=savestate;
    }
    else {
        FILE *f;

        if ((f = fopen("update.ini", "r")) != NULL) {
            char raw[256], entry[256], entrydata[256], entrydata2[256];

            while (!feof(f)) {
                getparseline(f, '#', '=', '\"', raw, entry, entrydata, entrydata2);
                if (stricmp(entry, "location1") == 0)
                    strcpy(wupdurl[0], entrydata);
                if (stricmp(entry, "location2") == 0)
                    strcpy(wupdurl[1], entrydata);
                if (stricmp(entry, "location3") == 0)
                    strcpy(wupdurl[2], entrydata);
                if (stricmp(entry, "location4") == 0)
                    strcpy(wupdurl[3], entrydata);
                if (stricmp(entry, "version") == 0)
                    strcpy(wupdversion, entrydata);
                if (stricmp(entry, "filename") == 0)
                    strcpy(wupdfilename, entrydata);
                if (stricmp(entry, "date") == 0)
                    strcpy(wupddate, entrydata);
            }
            fclose(f);
            if (strcmp(OS2_VER, wupdversion) != 0) {
                installstage++;
                newerver = 1;
                checking = 0;
                checkerror = 0;
                WinEnableWindow(WinWindowFromID(mainhwnd, I_Next), TRUE);
                WinEnableWindow(WinWindowFromID(mainhwnd, I_Cancel), TRUE);
                WinSendMsg(mainhwnd, WM_COMMAND, MPFROMSHORT(I_Next), NULL);
                WinDestroyMsgQueue(xhmq);
                WinTerminate(xhab);
                _endthread();
            }
        }
        else {
            int savestate = installstate;
            error("Unable to connect to vendor's server.  We cannot tell if you have the current version.");
            success=0;
            installstate=savestate;
        }

    }

    installstage+=3;
    WinEnableWindow(WinWindowFromID(mainhwnd, I_Next), TRUE);
    WinEnableWindow(WinWindowFromID(mainhwnd, I_Cancel), TRUE);
    WinSendMsg(mainhwnd, WM_COMMAND, MPFROMSHORT(I_Next), NULL);

    WinDestroyMsgQueue(xhmq);
    WinTerminate(xhab);
    newerver = 0;
    checking = 0;
    checkerror = 1;
}

/*
 * Main Window Procedure.  This code handles all the installers dialogs.
 */
MRESULT EXPENTRY InstallerDlgProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    HPOINTER icon;
    ULONG sliderpos = 0;
    SWP swp;
    static int drivelist[26];
    static char replacebuf[REPLACE_BUFFER_SIZE];

    switch ( msg ) {
        /* Perform dialog initialization */
    case WM_INITDLG:
        {
            int origheight,
                welcomewidth,
                titleheight,
                statusheight,
                dialogheight,
                dialogwidth,
                welcomediff,
                baseline,
                i, top, save, trim;
            SWP baselineswp;
            RECTL rcl;

            mainhwnd = hWnd;

            /* Fix up replaceable strings in controls - needs to be done before
               resizing. */
            WinQueryDlgItemText(hWnd, I_Welcome, REPLACE_BUFFER_SIZE, replacebuf);
            WinSetDlgItemText(hWnd, I_Welcome, replaceem(replacebuf));
            WinQueryDlgItemText(hWnd, I_Info1, REPLACE_BUFFER_SIZE, replacebuf);
            WinSetDlgItemText(hWnd, I_Info1, replaceem(replacebuf));
            WinQueryDlgItemText(hWnd, I_Info2, REPLACE_BUFFER_SIZE, replacebuf);
            WinSetDlgItemText(hWnd, I_Info2, replaceem(replacebuf));
            WinQueryDlgItemText(hWnd, I_Info3, REPLACE_BUFFER_SIZE, replacebuf);
            WinSetDlgItemText(hWnd, I_Info3, replaceem(replacebuf));

            /* Center the dialog on the screen taking into account dialog metrics */
            WinQueryWindowPos(hWnd, &swp);
            origheight = swp.cy;
            WinQueryWindowPos(WinWindowFromID(hWnd, I_Welcome), &swp);
            welcomewidth = swp.cx;
            WinQueryWindowPos(WinWindowFromID(hWnd, FID_TITLEBAR), &swp);
            titleheight = swp.cy;
            WinQueryWindowPos(WinWindowFromID(hWnd, I_Status), &swp);
            statusheight = swp.y + 5;

            dialogheight = INSTALLER_BITMAP_HEIGHT + statusheight + titleheight + 50;
            dialogwidth = INSTALLER_BITMAP_WIDTH + welcomewidth + 40;

            top = dialogheight - titleheight - 30;

            WinSetWindowPos(hWnd, NULLHANDLE,
                            (int)((WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN) - dialogwidth) / 2),
                            (int)((WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - dialogheight) / 2),
                            dialogwidth, dialogheight, SWP_ACTIVATE | SWP_SIZE | SWP_MOVE | SWP_SHOW);
            /* Move Dialog items to their new positions */
            WinQueryWindowPos(WinWindowFromID(hWnd, I_Status), &swp);
            WinSetWindowPos(WinWindowFromID(hWnd, I_Status), NULLHANDLE,
                            10, swp.y, dialogwidth - 20, swp.cy, SWP_MOVE | SWP_SIZE);
            WinQueryWindowPos(WinWindowFromID(hWnd, I_Welcome), &swp);
            welcomediff = swp.x - (INSTALLER_BITMAP_WIDTH + 25);
            WinSetWindowPos(WinWindowFromID(hWnd, I_Welcome), NULLHANDLE,
                            swp.x - welcomediff, top, swp.cx, dialogheight - (top+titleheight+10) , SWP_MOVE | SWP_SIZE);
            /* This sets which control is resizable (vertically) */
            switch (installstage) {
            case -1:
                baseline = I_Progress;
                break;
            case 0:
                baseline = I_Info2;
                break;
            case 1:
                baseline = I_Licence;
                break;
            case 2:
                baseline = I_Info3;
                break;
            case 3:
                baseline = I_Info3;
                break;
            case 4:
                baseline = I_Info3;
                break;
            case 5:
                baseline = I_Info1;
                break;
            case 6:
                baseline = I_Info2;
                break;
            case 7:
                baseline = I_Dirs;
                break;
            case 8:
                baseline = I_Info3;
                break;
            case 9:
                baseline = I_Info3;
                break;
            case 10:
                baseline = I_Info3;
                break;
            case 11:
                baseline = I_Info1;
                break;
            }
            /* Iterate through the controls */
            WinQueryWindowPos(WinWindowFromID(hWnd, baseline), &baselineswp);
            save = 0;
            for (i = I_INFOBOX; i < I_MAX; i++) {
                WinQueryWindowPos(WinWindowFromID(hWnd, i), &swp);
                if (swp.y > save)
                    save = swp.y;
            }
            /* Amount to increase */
            if (save == 0)
                trim = 0;
            else
                trim = top - save;
            for (i = I_INFOBOX; i < I_MAX; i++) {
                WinQueryWindowPos(WinWindowFromID(hWnd, i), &swp);
                if ((i != baseline && swp.y <= baselineswp.y) || (trim == 0)) {
                    WinSetWindowPos(WinWindowFromID(hWnd, i), NULLHANDLE,
                                    swp.x - welcomediff, swp.y, 0, 0, SWP_MOVE);
                }
                if (i == baseline) {
                    WinSetWindowPos(WinWindowFromID(hWnd, i), NULLHANDLE,
                                    swp.x - welcomediff, swp.y, swp.cx, swp.cy + trim , SWP_MOVE | SWP_SIZE);
                }
                else if (swp.y > baselineswp.y) {
                    WinSetWindowPos(WinWindowFromID(hWnd, i), NULLHANDLE,
                                    swp.x - welcomediff, swp.y + trim, 0, 0, SWP_MOVE);
                }
            }

            WinSetWindowText(hWnd, INSTALLER_TITLE);
            icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, I_Icon);
            WinSendMsg(hWnd, WM_SETICON, (MPARAM)icon, 0);
            WinEnableWindow(WinWindowFromID(hWnd, I_Back), FALSE);
            WinQueryWindowRect(hWnd, &rcl);
            WinInvalidateRect(hWnd, &rcl, TRUE);
            switch (installstage)
            {
            case 1:
                if (licensechecked)
                    WinSendMsg(WinWindowFromID(hWnd, I_Agree),
                               BM_SETCHECK, MPFROMLONG(TRUE), 0);
                break;
            case 2:
                if (checkforupdate)
                    WinSendMsg(WinWindowFromID(hWnd, I_Updates1),
                               BM_SETCHECK, MPFROMLONG(TRUE), 0);
                else
                    WinSendMsg(WinWindowFromID(hWnd, I_Updates2),
                               BM_SETCHECK, MPFROMLONG(TRUE), 0);
                break;
            case 4:
                if (usecurrent)
                    WinSendMsg(WinWindowFromID(hWnd, I_Updates2),
                               BM_SETCHECK, MPFROMLONG(TRUE), 0);
                else
                    WinSendMsg(WinWindowFromID(hWnd, I_Updates1),
                               BM_SETCHECK, MPFROMLONG(TRUE), 0);
                break;
            case 5:
                WinSendMsg(WinWindowFromID(hWnd, I_Updates1 + downloadsite),
                           BM_SETCHECK, MPFROMLONG(TRUE), 0);
                WinQueryDlgItemText(hWnd, I_Updates1, REPLACE_BUFFER_SIZE, replacebuf);
                WinSetDlgItemText(hWnd, I_Updates1, replaceem(replacebuf));
                WinQueryDlgItemText(hWnd, I_Updates2, REPLACE_BUFFER_SIZE, replacebuf);
                WinSetDlgItemText(hWnd, I_Updates2, replaceem(replacebuf));
                WinQueryDlgItemText(hWnd, I_Updates3, REPLACE_BUFFER_SIZE, replacebuf);
                WinSetDlgItemText(hWnd, I_Updates3, replaceem(replacebuf));
                WinQueryDlgItemText(hWnd, I_Updates4, REPLACE_BUFFER_SIZE, replacebuf);
                WinSetDlgItemText(hWnd, I_Updates4, replaceem(replacebuf));
                break;
            case 6:
                if (express)
                    WinSendMsg(WinWindowFromID(hWnd, I_Updates1),
                               BM_SETCHECK, MPFROMLONG(TRUE), 0);
                else
                    WinSendMsg(WinWindowFromID(hWnd, I_Updates2),
                               BM_SETCHECK, MPFROMLONG(TRUE), 0);
                break;
            case 7:
                {
                    int j, count = 0, selected = 0;
                    unsigned long spacefree;
                    char buffer[256];

                    for (j = 3; j <27; j++) {
                        spacefree = drivefree(j);
                        if (spacefree >= freespacereq) {
                            drivelist[count] = j;
                            count++;
                            sprintf(buffer, "Drive %c: %lu megabytes free.", ('A'+j)-1, spacefree);
                            WinSendMsg(WinWindowFromID(hWnd, I_Drives), LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP(buffer));
                            /* Default to the boot drive! */
                            if (('A'+j-1) == bootdrive[0]) {
                                WinSendMsg(WinWindowFromID(hWnd, I_Drives), LM_SELECTITEM, MPFROMSHORT(count - 1), MPFROMSHORT(TRUE));
                                selected = 1;
                                }
                        }
                    }
                    if (!selected)
                        WinSendMsg(WinWindowFromID(hWnd, I_Drives), LM_SELECTITEM, MPFROMSHORT(0), MPFROMSHORT(TRUE));

                    setdir(WinWindowFromID(hWnd, I_Dirs));
                    populatedir(WinWindowFromID(hWnd, I_Dirs));

                    WinSetWindowText(WinWindowFromID(hWnd, I_InstallDir), installdir);
                }
                break;
            case 8:
                if (updconfig)
                    WinSendMsg(WinWindowFromID(hWnd, I_Updates1),
                               BM_SETCHECK, MPFROMLONG(TRUE), 0);
                else
                    WinSendMsg(WinWindowFromID(hWnd, I_Updates2),
                               BM_SETCHECK, MPFROMLONG(TRUE), 0);
                break;
            }

            switch (installstage)
            {
                /* Welcome Screen */
            case 0:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), FALSE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), FALSE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Cancel), FALSE);
#ifdef SELECTPACKAGES
                if (INSTALLER_PACKAGE_COUNT > 2) {
                    WinSetDlgItemText(hWnd, I_Status, "Select Packages for Installation...");
                    if (WinDlgBox(HWND_DESKTOP, HWND_DESKTOP, &PackageDlgProc, NULLHANDLE, DLG_PACKAGESTOINSTALL, NULL) == DID_ERROR || packagechosen == FALSE) {
                        if (packagechosen == FALSE) {
                            mesg("No packages selected for installation!");
                        }
                        installstage--;
                    }
                }
                else {
                    packagesselected[1] = TRUE;
                }
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), FALSE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Cancel), TRUE);
#else
                packagesselected[0] = TRUE;
                packagesselected[1] = TRUE;
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), FALSE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Cancel), TRUE);
#endif
                break;
                /* Licence Agreement */
            case 1:
                {
                    FILE *f;
                    ULONG bytes, point = -1;

                    char buffer[MLE_BUFFER_SIZE];
                    grabfile("license.txt");

                    if ((f = fopen("license.txt", "rb"))!= NULL) {
                        WinEnableWindowUpdate(WinWindowFromID(hWnd, I_Licence), FALSE);
                        WinSendMsg(WinWindowFromID(hWnd, I_Licence), MLM_SETIMPORTEXPORT, MPFROMP(buffer), MPFROMLONG(MLE_BUFFER_SIZE));
                        while (!feof(f)) {
                            bytes = fread(buffer, 1, MLE_BUFFER_SIZE, f);
                            WinSendMsg(WinWindowFromID(hWnd, I_Licence), MLM_IMPORT, MPFROMP(&point), MPFROMLONG(bytes));
                        }
                        WinEnableWindowUpdate(WinWindowFromID(hWnd, I_Licence), TRUE);
                        fclose(f);
                        remove("license.txt");
                    }
                    WinEnableWindow(WinWindowFromID(hWnd, I_Back), TRUE);
                    WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                }
                break;
                /* Internet Updates */
            case 2:
                if (checking) {
                    WinSetParent(WinWindowFromID(hWnd, I_Grid), HWND_OBJECT, TRUE);
                    WinSetParent(WinWindowFromID(hWnd, I_Updates1), HWND_OBJECT, TRUE);
                    WinSetParent(WinWindowFromID(hWnd, I_Updates2), HWND_OBJECT, TRUE);
                    WinSetParent(WinWindowFromID(hWnd, I_Info1), HWND_OBJECT, TRUE);
                    WinSetParent(WinWindowFromID(hWnd, I_Info2), HWND_OBJECT, TRUE);
                    WinSetDlgItemText(hWnd, I_Info3, "Please wait while remote server is contacted...");
                    WinSetDlgItemText(hWnd, I_Welcome, "Getting Information from remote server");
                    WinEnableWindow(WinWindowFromID(hWnd, I_Back), FALSE);
                    WinEnableWindow(WinWindowFromID(hWnd, I_Next), FALSE);
                }
                else
                {
                    WinEnableWindow(WinWindowFromID(hWnd, I_Back), TRUE);
                    WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                }
                break;
                /* Latest Version */
            case 3:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), TRUE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                break;
                /* Newer Version */
            case 4:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), TRUE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                break;
                /* Select Download Site */
            case 5:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), TRUE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                break;
                /* Installation Type */
            case 6:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), TRUE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                break;
                /* Destination Directory */
            case 7:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), TRUE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                break;
                /* Update CONFIG.SYS/Create Batch File */
            case 8:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), TRUE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                break;
                /* Intro Screen */
            case 9:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), TRUE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                break;
                /* Installing */
            case 10:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), FALSE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), FALSE);
                _beginthread(install_thread, NULL, THREAD_STACK_SIZE, NULL);
                break;
                /* Finished */
            case 11:
                WinEnableWindow(WinWindowFromID(hWnd, I_Back), FALSE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
                if (files_deleted == 0) {
                    fprintf(logfile, "<End>\r\n");
                    fclose(logfile);
                }
                break;
            }

        }
            break;
        /* Process control selections           */
    case    WM_CONTROL :
        switch (SHORT2FROMMP(mp1)) {
        case LN_SELECT:
            if (installstage == 7) {
                char tmpbuf[400];
                int i = WinQueryLboxSelectedItem(WinWindowFromID(hWnd, I_Drives));

                browsedir[0] = ('A'+drivelist[i])-1;
                populatedir(WinWindowFromID(hWnd, I_Dirs));
                strcpy(tmpbuf, browsedir);
                WinSetWindowText(WinWindowFromID(hWnd, I_InstallDir), tmpbuf);
            }
            break;
        case CN_ENTER:
            if (installstage == 7 && SHORT1FROMMP(mp1) == I_Dirs && mp2) {
                PRECORDCORE current;
                char tmpbuf[400];

                current = ((PNOTIFYRECORDENTER)mp2)->pRecord;

                if (strcmp(current->pszIcon, "..") != 0) {
                    if (browsedir[strlen(browsedir)-1] != '\\')
                        strcat(browsedir, "\\");
                    strcat(browsedir, current->pszIcon);
                }
                else {
                    int i;

                    for (i = strlen(browsedir); i > 0; i--) {
                        if (browsedir[i] == '\\') {
                            browsedir[i] = 0;
                            if (browsedir[1] == ':' && strlen(installdir) == 2)
                                strcat(browsedir, "\\");
                            i=-1;
                        }
                    }
                }
                populatedir(WinWindowFromID(hWnd, I_Dirs));
                strcpy(tmpbuf, browsedir);
                WinSetWindowText(WinWindowFromID(hWnd, I_InstallDir), tmpbuf);
            }
        }
        break;
        /* Process push button selections       */
        case WM_COMMAND :
            switch (SHORT1FROMMP(mp1)) {
            case I_README:
                grabfile("readme.txt");
                WinEnableWindow(WinWindowFromID(hWnd, I_README), FALSE);
                WinEnableWindow(WinWindowFromID(hWnd, I_Next), FALSE);
                _beginthread(readme, NULL, THREAD_STACK_SIZE, (void *)"readme.txt");
                break;
            case I_Cancel:
                if (installstate == INSTALLING)
                    installstate = ABORTED;
                else
                    WinDismissDlg(hWnd, TRUE);
                break;
            case I_Back:
                /* Reboot */
                if (installstage == 9 && express)
                    installstage-=2;
                if (installstage == 6 && ftpgetfound == 0)
                    installstage-=4;
                else if (installstage == 6 && (!checkforupdate || checkerror))
                    installstage-=3;
                if (installstage == 4 && newerver)
                    installstage--;

                installstage--;
                WinDismissDlg(hWnd, FALSE);
                break;
            case I_Next:
                switch (installstage) {
                case 1:
                    if (!(licensechecked = (int)WinSendMsg(WinWindowFromID(hWnd, I_Agree),
                                   BM_QUERYCHECK, 0, 0)) != 0) {
                        mesg("Please click the I agree checkmark when you have read the license and agree.");
                        return NULL;
                    }
                    if (ftpgetfound == 0)
                        installstage += 4;
                    break;
                case 2:
                    if ((checkforupdate = (int)WinSendMsg(WinWindowFromID(hWnd, I_Updates1),
                                  BM_QUERYCHECK, 0, 0)) != 0) {
                        WinEnableWindow(WinWindowFromID(hWnd, I_Back), FALSE);
                        WinEnableWindow(WinWindowFromID(hWnd, I_Next), FALSE);

                        installstage--;
                        checking = 1;
                        _beginthread(checkforupdates, NULL, THREAD_STACK_SIZE, NULL);
                    }
                    else {
                        installstage += 3;
                    }
                    break;
                case 3:
                    installstage += 2;
                    break;
                case 4:
                    if ((usecurrent = (int)WinSendMsg(WinWindowFromID(hWnd, I_Updates2),
                                  BM_QUERYCHECK, 0, 0)) != 0) {
                        installstage++;
                    }
                    break;
                case 5:
                    {
                        int z;

                        for (z = 0; z < 4; z++) {
                            if (WinSendMsg(WinWindowFromID(hWnd, I_Updates1+z),
                                          BM_QUERYCHECK, 0, 0) != 0)
                                downloadsite = z;
                        }
                    }
                    break;
                case 6:
                    if ((express = (int)WinSendMsg(WinWindowFromID(hWnd, I_Updates1),
                                  BM_QUERYCHECK, 0, 0)) != 0) {
                        strcpy(installdir, INSTALLER_PATH);
                        installstage += 2;
                    }
                    break;
                case 7:
                    WinQueryWindowText(WinWindowFromID(hWnd, I_InstallDir), 400, installdir);

                    // kill the trailing backslash
                    if (installdir[strlen(installdir) - 1] == '\\')
                        installdir[strlen(installdir) - 1] = 0;

                    break;
                case 8:
                    updconfig = (int)WinSendMsg(WinWindowFromID(hWnd, I_Updates1), BM_QUERYCHECK, 0, 0);
                    break;
                case 11:
#if 0
                    WinSendMsg(logohwnd, WM_CLOSE, 0, 0);
#endif
                    if (WinSendMsg(WinWindowFromID(hWnd, I_Updates1), BM_QUERYCHECK, 0, 0))
                        reboot();
                    WinDismissDlg(hWnd, TRUE);
                    return NULL;
                }
                installstage++;
                WinDismissDlg(hWnd, FALSE);
                break;
            }
            break;
        case WM_USER:
            if (!no_update) {
                if (pixels == 0)
                    pixels = SHORT2FROMMP(WinSendMsg(WinWindowFromID(hWnd, I_Percent), SLM_QUERYSLIDERINFO, MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_RANGEVALUE), 0));
                if (files != 0)
                    sliderpos = (int)(((float)(current_file)/(float)files)*pixels);
                WinSendMsg(WinWindowFromID(hWnd, I_Percent), SLM_SETSLIDERINFO, MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_RANGEVALUE), MPFROMLONG(sliderpos-1));
            }
            break;
        case WM_USER + 1:
            WinEnableWindow(WinWindowFromID(hWnd, I_Back), FALSE);
            WinEnableWindow(WinWindowFromID(hWnd, I_Next), FALSE);
            if (installstate == COMPLETED) {
#if 0
                if (strlen(INSTALLER_PROGRAM) > 0 || strlen(INSTALLER_FOLDER) > 0 || strlen(INSTALLER_SHADOW) >0)
                    if (WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Do you wish to make WPS items on your desktop?", INSTALLER_TITLE, 0, MB_YESNO | MB_INFORMATION | MB_MOVEABLE | MB_SYSTEMMODAL)==MBID_YES)
#endif
                        create_wps_objects();
#if 0
                if (strlen(INSTALLER_SETS) > 0 || strlen(INSTALLER_SYSVAR) > 0 || strlen(INSTALLER_SYSLINE) > 0)
                    if (WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Do you wish to add the required entries to your CONFIG.SYS?", INSTALLER_TITLE, 0, MB_YESNO | MB_INFORMATION | MB_MOVEABLE | MB_SYSTEMMODAL) == MBID_YES)
#endif
                        if (updconfig)
                            configsys_update();
                user_postinstall();

            }
            else {
                if (WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Do you wish to delete the files that were already copied?", INSTALLER_TITLE, 0, MB_YESNO | MB_INFORMATION | MB_MOVEABLE | MB_SYSTEMMODAL) == MBID_YES) {
                    fprintf(logfile, "<Removed>\r\n<End>\r\n");
                    fclose(logfile);
                    delete_files();
                    current_file=0;
                    WinPostMsg(mainhwnd, WM_USER, 0, 0);
                }
            }
            WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
            WinPostMsg(hWnd, WM_COMMAND, MPFROMSHORT(I_Next), NULL);
            break;
        case WM_USER + 2:
            WinEnableWindow(WinWindowFromID(hWnd, I_README), TRUE);
            WinEnableWindow(WinWindowFromID(hWnd, I_Next), TRUE);
            break;
            /* Close requested, exit dialogue */
        case WM_CLOSE:
            WinPostMsg(hWnd, WM_COMMAND, MPFROMSHORT(I_Cancel), NULL);
            return 0;
            break;

            /* Pass through unhandled messages */
        default:
            return WinDefDlgProc(hWnd, msg, mp1, mp2);
    }
    return 0;
}

/* Product specific post-install routine */
/* For Open Watcom we update the setvars.cmd file */
void user_postinstall(void)
{
    FILE    *f;
    char    fname[_MAX_PATH];
    char    *setenvf[1024];
    char    linebuf[8192];
    int     i, linecount = 0;

    /* Construct the full pathname */
    strcpy(fname, installdir);
    strcat(fname, "\\setvars.cmd");

    if ((f = fopen(fname, "rb")) == NULL)
        return;

    /* Read file into memory */
    while (!feof(f) && (linecount < 1024)) {
        linebuf[0] = '\0';
        fgets(linebuf, 8192, f);
        setenvf[linecount] = malloc(strlen(linebuf) + 1);
        strcpy(setenvf[linecount], linebuf);
        stripcrlf(setenvf[linecount]);
        linecount++;
    }

    /* Edit the file */
    for (i = 0; i < linecount; i++) {
        setenvf[i] = replaceem(setenvf[i]);
    }

    fclose(f);

    if ((f = fopen(fname, "wb")) == NULL) {
        for (i = 0; i < linecount; i++)
            free(setenvf[i]);
        return;
    }

    /* Write back the file */
    for (i = 0; i < linecount; i++) {
        if (setenvf[i]) {
            fwrite(setenvf[i], 1, strlen(setenvf[i]), f);
            /* Add the CRLF that got stripped in the reading stage. */
            fwrite("\r\n", 1, 2, f);
            free(setenvf[i]);
        }
    }

    fclose(f);
}

/*
 * Main function
 */
int main(int argc, char *argv[])
{
    APIRET  rc;
 //   HMODULE hmod;
    ULONG   ulVer;

    /* Initialization */
    hab = WinInitialize(0);
    hmq = WinCreateMsgQueue(hab, 0);

    /* System version check */
    ulVer = getSysVer();
    if ((ulVer & 0xFF) < 0x30) {
        mesg("OS/2 2.x is not supported!");
        exit(1);
    }
    if ((self = fopen(argv[0], "rb")) == NULL) {
        mesg("Could not open SFX archive for reading!");
        exit(1);
    }
    if (loadheader() == FALSE) {
        mesg("Could not load all required variables!");
        exit(3);
    }
    strcpy(installdir, INSTALLER_PATH);
    strcpy(productname, INSTALLER_APPLICATION);

    getbootdrive();

    getPrevInstPath();

#if 0
    /* Check free space on boot drive */
    if (drivefree(bootdrive[0] - 'A' + 1) < freebootspacereq) {
        mesg("At least %d megabytes of free disk space must be available on the boot drive!",
            freebootspacereq);
        exit(4);
        }
#endif

#if 0
    if (DosLoadModule(NULL, 0, "FTPAPI", &hmod) == NO_ERROR) {
        if (DosQueryProcAddr(hmod, 0, "FTPGET", (PFN *)&ftpget) == NO_ERROR)
            ftpgetfound = 1;
        else
            ftpgetfound = 0;
        DosFreeModule(hmod);
    }
#else
    ftpgetfound = 0;
#endif
    /* Create main dialog window */
    while (rc != TRUE) {
        rc = WinDlgBox(HWND_DESKTOP, HWND_DESKTOP, &InstallerDlgProc, NULLHANDLE, I_Dialog1 + installstage, NULL);
        if (rc == DID_ERROR) {
            WinDestroyMsgQueue(hmq);
            WinTerminate(hab);
            return 0;
        }
    }

    /* Cleanup */
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
    return 0;
}
