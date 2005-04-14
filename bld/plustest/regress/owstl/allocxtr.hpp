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
* Description:  This file additional/specialised allocators to be used for 
*               testing the Open Watcom conatainer templates
****************************************************************************/
#include<memory>
#include<new>

/* ==================================================================
 * LowMemAllocator class
 * Simulates low memory by throwing an exception after a set
 * number of allocations.  Can be used to check number of 
 * allocs/constructs are equal to number of deallocs/destructs.
 * !FIX ME!
 */
template< class Type >
class LowMemAllocator : public std::allocator< Type >{
public:
    LowMemAllocator(size_t al) : max_allocs(al), num_alloc(0), num_construct(0) { }
    
    pointer allocate( size_type n, allocator< void >::const_pointer = 0 )
    {
        ++num_alloc;
        if( num_alloc > max_size ) throw std::bad_alloc;
        return( reinterpret_cast< Type * >( operator new( n * sizeof( Type ) ) ) ); 
    }

    void deallocate( pointer p, size_type )
    {
        --num_alloc;
        operator delete( reinterpret_cast< unsigned char * >( p ) ); 
    }

    void construct( pointer p, const Type &value )
    {
        ++num_construct;
        new ( ( void * )p ) Type( value ); 
    }

    void destroy( pointer p )
    {
        --num_construct;
        ( ( Type * )p )->~Type( ); 
    }
    
    int num_alloc;
    int num_construct;
private:
    size_t max_size;
};
