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
* Description: This file contains helper function/macros to facilitate
*              sanity checks in the other OWSTL test programs. The
*              facilities defined here are facilities of interest to
*              all programs in the test suite.
*
*              This file is intended to be #included into the other test
*              files (it contains macros).
****************************************************************************/

#if defined(__WATCOMC__)

  #include <malloc.h>

  #define INSANE(x) ( !( x )._Sane( ) )

  bool heap_ok( const char *msg )
  {
    bool rc = true;
    if( _heapchk( ) != _HEAPOK ) {
      std::cout << "!!! HEAP CONSISTENCY FAILURE: " << msg << "\n";
      rc = false;
    }
    return( rc );
  }

  int heap_count( )
  {
    int used_size = 0;
    struct _heapinfo info;

    info._pentry = NULL;
    while( _heapwalk( &info ) != _HEAPEND ) {
      if( info._useflag == _USEDENTRY ) {
        used_size += info._size;
      }
    }
    return( used_size );
  }

#else

  #define INSANE(x) false

  bool heap_ok( const char * )
  {
    return( true );
  }

  int heap_count( )
  {
    return( 0 );
  }

#endif
