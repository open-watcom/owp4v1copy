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
* Description:  Viper INI file access routines.
*
****************************************************************************/


#if defined( __OS2__ )
    #include <stdlib.h>
    #include <string.h>

    #define INCL_WINSHELLDATA
    #include <os2.h>

    /* On OS/2, closing a profile is potentially expensive operation
     * (buffer flushes and whatnot). Therefore we cache the ini handle
     * to minimize the profile open/close operations.
     */
    char ini_dir[_MAX_PATH] = "";
    char ini_file[_MAX_PATH] = "";
    HINI ini_handle = NULLHANDLE;

    static HINI getIniFile(const char *dir, const char *filename)
    {
        char    buff[_MAX_PATH];
        char    *p;

        if( strcmp( ini_dir, dir ) || strcmp( ini_file, filename ) ) {
            strcpy( ini_dir, dir );
            strcpy( ini_file, filename );

            dir = getenv( "USER_INI" );
            strcpy( buff, dir );
            for( p=buff + strlen(buff) - 1; p >= buff; p-- ) {
                if( *p == '\\' ) {
                    *p = 0;
                    break;
                }
            }
            strcat( buff, "\\" );
            strcat( buff, filename );

            if( ini_handle != NULLHANDLE )
                PrfCloseProfile( ini_handle );

            ini_handle = PrfOpenProfile( NULL, (PSZ)buff );
        }
        return ini_handle;
    }

    int MyGetProfileString( const char *dir, const char *filename,
                            const char *section, const char *key,
                            const char *def, char *buffer, int len )
    {
        HINI    hini;
        int     rc;

        hini = getIniFile( dir, filename );

        rc = PrfQueryProfileString( hini, (PSZ)section,
                                    (PSZ)key, (PSZ)def,
                                    buffer, len );
        return( rc );
    }

    int MyWriteProfileString( const char *dir, const char *filename,
                              const char *section, const char *key,
                              const char *string )
    {
        HINI hini;
        int     rc;

        hini = getIniFile( dir, filename );

        rc = PrfWriteProfileString( hini, (PSZ)section,
                                    (PSZ)key, (PSZ)string );
        return( rc );
    }

    int MyCloseCurrentProfile( void )
    {
        int rc;

        rc = PrfCloseProfile( ini_handle );
        ini_handle = NULLHANDLE;
        return rc;
    }
#elif defined( __WINDOWS__ ) || defined( __NT__ )
    #include <windows.h>
    int MyGetProfileString( const char *dir, const char *filename,
                            const char *section, const char *key,
                            const char *def, char *buffer, int len )
    {
        dir =dir; // ignored in this model
        return(GetPrivateProfileString(section,key,def,buffer,len,filename));
    }
    int MyWriteProfileString( const char *dir, const char *filename,
                              const char *section, const char *key,
                              const char *string )
    {
        dir =dir; // ignored in this model
        return(WritePrivateProfileString(section,key,string,filename));
    }
    int MyCloseCurrentProfile( void )
    {
        return 0;
    }
#else
    #error UNSUPPORTED OS
#endif
