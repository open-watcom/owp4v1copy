/***************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2007 The Open Watcom Contributors. All Rights Reserved.
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
*  Description: Implementation of the amstrmid.lib library.
*
**************************************************************************/

#include <guiddef.h>

/* mmstream.h */
EXTERN_C const IID  IID_IMultiMediaStream =
    { 0xB502D1BC, 0x9A57, 0x11D0, { 0x8F, 0xDE, 0x00, 0xC0, 0x4F, 0xD9, 0x18, 0x9D } };
EXTERN_C const IID  IID_IMediaStream =
    { 0xB502D1BD, 0x9A57, 0x11D0, { 0x8F, 0xDE, 0x00, 0xC0, 0x4F, 0xD9, 0x18, 0x9D } };
EXTERN_C const IID  IID_IStreamSample =
    { 0xB502D1BE, 0x9A57, 0x11D0, { 0x8F, 0xDE, 0x00, 0xC0, 0x4F, 0xD9, 0x18, 0x9D } };

