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
* Description:  Packinst - creates a self-extracting archive
*
****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_PACKAGES 128

extern int files;

FILE *cfgfile, *installfile;
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
int INSTALLER_BITMAP_WIDTH;
int INSTALLER_BITMAP_HEIGHT;
int INSTALLER_PACKAGE_COUNT=0;

int packagefiles[MAX_PACKAGES];
char *packagefilename[MAX_PACKAGES];

void resetglobals(void);

int include_unpack(char *aname);

void append_file(char *filename)
{
    FILE *appendfile;
    int  amnt;
    char buffer[512];

    if ((appendfile = fopen(filename, "rb")) == NULL) {
        printf("Error opening %s for reading!\n", filename);
        exit(3);
    }
    while (!feof(appendfile)) {
         amnt = fread(buffer, 1, 512, appendfile);
         fwrite(buffer, 1, amnt, installfile);
    }
    fclose(appendfile);
}

void getline(FILE *f, char *entry, char *entrydata)
{
    char in[4096];
    int z;

    memset(in, 0, 4096);
    fgets(in, 4095, f);

    if (in[strlen(in)-1] == '\n')
        in[strlen(in)-1] = 0;

    if (in[0] != '#') {
        for (z = 0; z < strlen(in); z++) {
            if(in[z] == '=') {
                in[z] = 0;
                strcpy(entry, in);
                strcpy(entrydata, &in[z+1]);
                return;
            }
        }
    }
    strcpy(entry, "");
    strcpy(entrydata, "");
}

int main(int argc, char *argv[])
{
    char entry[8096], entrydata[8096];
    int z;

    printf("Packinst - version 2.1\n\n");
    if (argc != 2) {
        printf("Usage: Packinst <filename.cfg>\n");
        exit(1);
    }
    if ((cfgfile = fopen(argv[1], "r")) == NULL) {
        printf("Error opening config file \"%s\"!\n", argv[1]);
        exit(2);
    }
    if ((installfile = fopen("sfx.exe", "wb")) == NULL) {
        printf("Error opening sfx.exe for writing!\n");
        exit(3);
    }
#if 0
    printf("Adding resources to sfx.exe...\n");
    system("rc install.rc sfx.exe > NUL");
#endif
    printf("Creating installer...\n");
    append_file("install.exe");
    fwrite("DBSOFT-HEADER", 1, 13, installfile);
    while (!feof(cfgfile)) {
        getline(cfgfile, entry, entrydata);
        if (stricmp(entry, "INSTALLER_APPLICATION") == 0)
            INSTALLER_APPLICATION = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_VERSION") == 0)
            INSTALLER_VERSION = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_TITLE") == 0)
            INSTALLER_TITLE = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_PATH") == 0)
            INSTALLER_PATH = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_FOLDER") == 0)
            INSTALLER_FOLDER = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_PROGRAM") == 0)
            INSTALLER_PROGRAM = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_SHADOW") == 0)
            INSTALLER_SHADOW = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_OBJECT") == 0)
            INSTALLER_OBJECT = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_SETS") == 0)
            INSTALLER_SETS = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_SYSVAR") == 0)
            INSTALLER_SYSVAR = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_SYSLINE") == 0)
            INSTALLER_SYSLINE = (char *)strdup(entrydata);
        if (stricmp(entry, "INSTALLER_PACKAGE") == 0) {
            for (z = 0; z < strlen(entrydata); z++) {
                if (entrydata[z] == ',') {
                    entrydata[z] = 0;
                    packagefilename[INSTALLER_PACKAGE_COUNT] = (char *)strdup(entrydata);
                    INSTALLER_PACKAGES[INSTALLER_PACKAGE_COUNT] = (char *)strdup(&entrydata[z+1]);
                }
            }
            resetglobals();
            files = 0;
            include_unpack(packagefilename[INSTALLER_PACKAGE_COUNT]);
            packagefiles[INSTALLER_PACKAGE_COUNT] = files;
            INSTALLER_PACKAGE_COUNT++;
        }
        if (stricmp(entry, "INSTALLER_BITMAP_WIDTH") == 0)
            INSTALLER_BITMAP_WIDTH = atoi(entrydata);
        if (stricmp(entry, "INSTALLER_BITMAP_HEIGHT") == 0)
            INSTALLER_BITMAP_HEIGHT = atoi(entrydata);
    }
    fclose(cfgfile);
    fprintf(installfile, "%s-%s-%s-%s-%s-%s-%s-%s-%s-%s-%s-%d-%d-%d",
        INSTALLER_APPLICATION, INSTALLER_VERSION, INSTALLER_TITLE,
        INSTALLER_PATH, INSTALLER_FOLDER, INSTALLER_PROGRAM,
        INSTALLER_SHADOW, INSTALLER_OBJECT, INSTALLER_SETS, INSTALLER_SYSVAR,
        INSTALLER_SYSLINE, INSTALLER_BITMAP_WIDTH,
        INSTALLER_BITMAP_HEIGHT, INSTALLER_PACKAGE_COUNT);
    for (z = 0; z < INSTALLER_PACKAGE_COUNT; z++)
        fprintf(installfile, "-%s", INSTALLER_PACKAGES[z]);
    fwrite("~", 1, 1, installfile);
    for (z = 0; z < INSTALLER_PACKAGE_COUNT; z++) {
         struct stat st;

         stat(packagefilename[z], &st);
         fprintf(installfile, "DBSOFT-ACE%d%d-%d~", z, packagefiles[z], st.st_size);
         append_file(packagefilename[z]);
         printf("Package: %s - %d files in %s (%u bytes).\n", INSTALLER_PACKAGES[z], packagefiles[z], packagefilename[z], st.st_size);
    }
    fclose(installfile);
    fclose(cfgfile);
    return 0;
}
