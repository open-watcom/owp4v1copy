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
* Description:  Routines to create/replace files or directories.
*
****************************************************************************/


#include "os.h"

// Ask user for confirmation if we're overwriting newer files
#define CHECK_DATETIME

#ifdef __EMX__
#include <sys/types.h>
#include <sys/dirent.h>
#else
#include <direct.h>
#endif
#include <io.h>
#include <fcntl.h>
#include <stdio.h>     // printf() remove()
#include <string.h>    // strncpy()
#include <sys/types.h>
#include <sys/stat.h>  // struct stat
#ifdef CHECK_DATETIME
#define INCL_WIN       /* Window Manager Functions     */
#define INCL_DOS
#define INCL_BASE
#include <os2.h>
#include <time.h>
#include "install.h"
#define OS2_H_INCLUDED
#endif

#if defined(DOS) || defined(WINNT) || defined(WIN16)
 #include <io.h>       // access()
#if defined(__WATCOMC__)
 #include <direct.h>      // mkdir()
#else
 #include <dir.h>      // mkdir()
#endif
#endif

#include "attribs.h"
#include "globals.h"
#include "uac_crt.h"
#include "uac_sys.h"

extern FILE *logfile;
extern char installdir[400];

/* gets file name from header
 */
CHAR *ace_fname(CHAR * s, thead * head, INT nopath)
{
   INT  i;
   char *cp;

   strncpy(s, (CHAR*)(*(tfhead *) head).FNAME, i = (*(tfhead *) head).FNAME_SIZE);
   s[i] = 0;

   if (nopath)
   {
      cp=strrchr(s, '\\');
      if (cp)
         memmove(s, cp+1, strlen(cp));
   }
#ifdef __UNIX__
   else
   {                                // by current OS seperator
      cp=s;
      while ((cp=strchr(cp, '\\'))!=NULL)
         *cp++='/';
   }
#endif

   return s;
}

void check_ext_dir(CHAR * f)        // checks/creates path of file
{
    char d[1024];
    char buffer[1024];
    int z, flag = 0, len = strlen(f);

    strcpy(buffer, f);
    for(z=len;z>-1;z--)
    {
        if(buffer[z] == '\\')
        {
            buffer[z+1] = 0;
            flag = 1;
            z = -1;
        }
    }
    if(!flag)
        return;
    for(z=0;z<strlen(buffer);z++)
    {
        if(buffer[z] == '\\')
        {
            if(!(z == 2 && buffer[1] == ':'))
            {
                strcpy(d, buffer);
                d[z] = 0;
                if (!fileexists(d))
                {
#ifdef __EMX__
                    if (mkdir(d, 0))
#else
                    if (mkdir(d))
#endif
                    {
                        f_err = ERR_WRITE;
                        error("Error while creating directory \"%s\".", d);
                    }
                    else
                    {
                        if(logfile)
                        {
                            if(strlen(d) > 1 && d[1] == ':')
                                fprintf(logfile, "<NewDir>,%s\r\n", d);
                            else
                            {
                                if(installdir[strlen(installdir)-1] == '\\')
                                    fprintf(logfile, "<NewDir>,%s%s\r\n", installdir, d);
                                else
                                    fprintf(logfile, "<NewDir>,%s\\%s\r\n", installdir, d);
                            }
                        }
                    }
                }
            }
        }

    }
}

INT  ovr_delete(CHAR * n)           // deletes directory or file
{
   if (remove(n) && rmdir(n))
   {
       DosReplaceModule(n, NULL, NULL);
       if (remove(n) && rmdir(n))
       {
           error("Could not delete file or directory: \"%s\" Access denied.", n);
           return (1);
       }
   }
   return (0);
}

INT  create_dest_file(CHAR * file, INT a)  // creates file or directory
{
   INT  han,
        i  = 0,
        ex = fileexists(file);
   struct stat st;
   extern int no_update;

   check_ext_dir(file);
   if (f_err)
      return (-1);
   if (a & _A_SUBDIR)
   {                                // create dir or file?
      if (ex)
      {
          stat(file, &st);
      }

#ifdef __EMX__
      if ((!ex && mkdir(file, 0)) || (ex && !(st.st_mode & S_IFDIR)))
#else
      if ((!ex && mkdir(file)) || (ex && !(st.st_mode & S_IFDIR)))
#endif
      {
         error("Could not create directory %s\\%s.", installdir, file);
         return (-1);
      }
      else
      {
          if(logfile)
              fprintf(logfile, "<NewDir>,%s\\%s\r\n", installdir, file);
      }

#ifdef DOS
      _dos_setfileattr(file, a);    // set directory attributes
#endif
      return (-1);
   }
   else
   {
      if (ex)
      {                             // does the file already exist
#ifdef CHECK_DATETIME
           static int overwriteall = 0;
           FILESTATUS3 fileinfo;

           f_ovrall = 1;

           DosQueryPathInfo(file, FIL_STANDARD, &fileinfo, sizeof(FILESTATUS3));
           if (!overwriteall) {
               FDATE fdate;
               FTIME ftime;
               struct tm    tc, tc2;
               time_t       tt, tt2;

               *((USHORT*)&fdate) = (USHORT)(fhead.FTIME >> 16);
               *((USHORT*)&ftime) = (USHORT)fhead.FTIME;

               tc.tm_year = fileinfo.fdateLastWrite.year + 80;
               tc.tm_mon = fileinfo.fdateLastWrite.month - 1;
               tc.tm_mday = fileinfo.fdateLastWrite.day;
               tc.tm_hour = fileinfo.ftimeLastWrite.hours;
               tc.tm_min = fileinfo.ftimeLastWrite.minutes;
               tc.tm_sec = fileinfo.ftimeLastWrite.twosecs * 2;

               tc2.tm_year = fdate.year + 80;
               tc2.tm_mon = fdate.month - 1;
               tc2.tm_mday = fdate.day;
               tc2.tm_hour = ftime.hours;
               tc2.tm_min = ftime.minutes;
               tc2.tm_sec = ftime.twosecs * 2;

               if ((tt = mktime(&tc)) == -1 || (tt2 = mktime(&tc2)) == -1 || tt > tt2) {

                   if (file[1] == ':')
                       i = confirm("File \"%s\" has a newer modification time. Overwrite?", file);  // prompt for overwrite
                   else {
                       if (installdir[strlen(installdir)-1] == '\\')
                           i = confirm("File \"%s%s\" has a newer modification time. Overwrite?", installdir, file);  // prompt for overwrite
                       else
                           i = confirm("File \"%s\\%s\" has a newer modification time. Overwrite?", installdir, file);  // prompt for overwrite
                   }
                   if (i == 1) {
                       overwriteall = 1;
                       i = 0;
                   }
                   if (i == 3)
                       f_err = ERR_USER;
                   if (i)
                       return -1;
               }
           }
           fileinfo.attrFile = FILE_NORMAL;
           DosSetPathInfo(file, FIL_STANDARD, (PVOID)&fileinfo, sizeof(FILESTATUS3), 0);
#endif
         if (!f_ovrall) {
             if (installdir[strlen(installdir)-1] == '\\')
                 i = confirm("Overwrite file \"%s%s\"?", installdir, file);  // prompt for overwrite
             else
                 i = confirm("Overwrite file \"%s\\%s\"?", installdir, file);  // prompt for overwrite
             f_ovrall = (i == 1);
             if (i == 3)
                 f_err = ERR_USER;
         }
         if (i && !f_ovrall || ovr_delete(file))
            return (-1);            // delete?
      }
      if ((han = open(file, O_WRONLY | O_TRUNC | O_CREAT | O_BINARY,
                            S_IREAD | S_IWRITE | S_IEXEC | S_IDELETE |
                            S_IRGRP | S_IWGRP  | S_IROTH | S_IWOTH )) < 0)
         error("Could not create destination file \"%s\".", file);
      else
      {
          if(logfile)
          {
              if(!no_update)
              {
                  if(strlen(file) > 1 && file[1] == ':')
                      fprintf(logfile, "<FileInst>,%s\r\n", file);
                  else
                  {
                      if(installdir[strlen(installdir)-1] == '\\')
                          fprintf(logfile, "<FileInst>,%s%s\r\n", installdir, file);
                      else
                          fprintf(logfile, "<FileInst>,%s\\%s\r\n", installdir, file);
                  }
              }
          }
      }

      return (han);
   }
}

