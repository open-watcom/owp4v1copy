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
*  Description: Implementation of the uuid.lib library.
*
**************************************************************************/

#include <guiddef.h>

/* unknwn.h */
EXTERN_C const IID  IID_IUnknown = 
    { 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IClassFactory =
    { 0x00000001, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

/* objidl.h */
EXTERN_C const IID  IID_IMarshal =
    { 0x00000003, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IMarshal2 =
    { 0x000001CF, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IMalloc =
    { 0x00000002, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IMallocSpy =
    { 0x0000001D, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IStdMarshalInfo =
    { 0x00000018, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IExternalConnection =
    { 0x00000019, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IMultiQI =
    { 0x00000020, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IInternalUnknown =
    { 0x00000021, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumUnknown =
    { 0x00000100, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IBindCtx =
    { 0x0000000E, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumMoniker =
    { 0x00000102, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IRunnableObject =
    { 0x00000126, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IRunningObjectTable =
    { 0x00000010, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IPersist =
    { 0x0000010C, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IPersistStream =
    { 0x00000109, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IMoniker =
    { 0x0000000F, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IROTData =
    { 0xF29F6BC0, 0x5021, 0x11CE, { 0xAA, 0x15, 0x00, 0x00, 0x69, 0x01, 0x29, 0x3F } };
EXTERN_C const IID  IID_IEnumString =
    { 0x00000101, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ISequentialStream =
    { 0x0C733A30, 0x2A1C, 0x11CE, { 0xAD, 0xE5, 0x00, 0xAA, 0x00, 0x44, 0x77, 0x3D } };
EXTERN_C const IID  IID_IStream =
    { 0x0000000C, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumSTATSTG =
    { 0x0000000D, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IStorage =
    { 0x0000000B, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IPersistFile =
    { 0x0000010B, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IPersistStorage =
    { 0x0000010A, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ILockBytes =
    { 0x0000000A, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumFORMATETC =
    { 0x00000103, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumSTATDATA =
    { 0x00000105, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IRootStorage =
    { 0x00000012, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IAdviseSink =
    { 0x0000010F, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IAdviseSink2 =
    { 0x00000125, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IDataObject =
    { 0x0000010E, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IDataAdviseHolder =
    { 0x00000110, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IMessageFilter =
    { 0x00000016, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IRpcChannelBuffer =
    { 0xD5F56B60, 0x593B, 0x101A, { 0xB5, 0x69, 0x08, 0x00, 0x2B, 0x2D, 0xBF, 0x7A } };
EXTERN_C const IID  IID_IRpcChannelBuffer2 =
    { 0x594F31D0, 0x7F19, 0x11D0, { 0xB1, 0x94, 0x00, 0xA0, 0xC9, 0x0D, 0xC8, 0xBF } };
EXTERN_C const IID  IID_IAsyncRpcChannelBuffer =
    { 0xA5029FB6, 0x3C34, 0x11D1, { 0x9C, 0x99, 0x00, 0xC0, 0x4F, 0xB9, 0x98, 0xAA } };
EXTERN_C const IID  IID_IRpcChannelBuffer3 =
    { 0x25B15600, 0x0115, 0x11D0, { 0xBF, 0x0D, 0x00, 0xAA, 0x00, 0xB8, 0xDF, 0xD2 } };
EXTERN_C const IID  IID_IRpcSyntaxNegotiate =
    { 0x58A08519, 0x24C8, 0x4935, { 0xB4, 0x82, 0x3F, 0xD8, 0x23, 0x33, 0x3A, 0x4F } };
EXTERN_C const IID  IID_IRpcProxyBuffer =
    { 0xD5F56A34, 0x593B, 0x101A, { 0xB5, 0x69, 0x08, 0x00, 0x2B, 0x2D, 0xBF, 0x7A } };
EXTERN_C const IID  IID_IRpcStubBuffer =
    { 0xD5F56AFC, 0x593B, 0x101A, { 0xB5, 0x69, 0x08, 0x00, 0x2B, 0x2D, 0xBF, 0x7A } };
EXTERN_C const IID  IID_IPSFactoryBuffer =
    { 0xD5F569D0, 0x593B, 0x101A, { 0xB5, 0x69, 0x08, 0x00, 0x2B, 0x2D, 0xBF, 0x7A } };
EXTERN_C const IID  IID_IChannelHook =
    { 0x1008C4A0, 0x7613, 0x11CF, { 0x9A, 0xF1, 0x00, 0x20, 0xAF, 0x6E, 0x72, 0xF4 } };
EXTERN_C const IID  IID_IClientSecurity =
    { 0x0000013D, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IServerSecurity =
    { 0x0000013E, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IClassActivator =
    { 0x00000140, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IRpcOptions =
    { 0x00000144, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IFillLockBytes =
    { 0x99CAF010, 0x415E, 0x11CF, { 0x88, 0x14, 0x00, 0xAA, 0x00, 0xB5, 0x69, 0xF5 } };
EXTERN_C const IID  IID_IProgressNotify =
    { 0xA9D758A0, 0x4617, 0x11CF, { 0x95, 0xFC, 0x00, 0xAA, 0x00, 0x68, 0x0D, 0xB4 } };
EXTERN_C const IID  IID_ILayoutStorage =
    { 0x0E6D4D90, 0x6738, 0x11CF, { 0x96, 0x08, 0x00, 0xAA, 0x00, 0x68, 0x0D, 0xB4 } };
EXTERN_C const IID  IID_IBlockingLock =
    { 0x30F3D47A, 0x6447, 0x11D1, { 0x8E, 0x3C, 0x00, 0xC0, 0x4F, 0xB9, 0x38, 0x6D } };
EXTERN_C const IID  IID_ITimeAndNoticeControl =
    { 0xBC0BF6AE, 0x8878, 0x11D1, { 0x83, 0xE9, 0x00, 0xC0, 0x4F, 0xC2, 0xC6, 0xD4 } };
EXTERN_C const IID  IID_IOplockStorage =
    { 0x8D19C834, 0x8879, 0x11D1, { 0x83, 0xE9, 0x00, 0xC0, 0x4F, 0xC2, 0xC6, 0xD4 } };
EXTERN_C const IID  IID_ISurrogate =
    { 0x00000022, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IGlobalInterfaceTable =
    { 0x00000146, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IDirectWriterLock =
    { 0x0E6D4D92, 0x6738, 0x11CF, { 0x96, 0x08, 0x00, 0xAA, 0x00, 0x68, 0x0D, 0xB4 } };
EXTERN_C const IID  IID_ISynchronize =
    { 0x00000030, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ISynchronizeHandle =
    { 0x00000031, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ISynchronizeEvent =
    { 0x00000032, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ISynchronizeContainer =
    { 0x00000033, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ISynchronizeMutex =
    { 0x00000025, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ICancelMethodCalls =
    { 0x00000029, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IAsyncManager =
    { 0x0000002A, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ICallFactory =
    { 0x1C733A30, 0x2A1C, 0x11CE, { 0xAD, 0xE5, 0x00, 0xAA, 0x00, 0x44, 0x77, 0x3D } };
EXTERN_C const IID  IID_IRpcHelper =
    { 0x00000149, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IReleaseMarshalBuffers =
    { 0xEB0CB9E8, 0x7996, 0x11D2, { 0x87, 0x2E, 0x00, 0x00, 0xF8, 0x08, 0x08, 0x59 } };
EXTERN_C const IID  IID_IWaitMultiple =
    { 0x0000002B, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IUrlMon =
    { 0x00000026, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IForegroundTransfer =
    { 0x00000145, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IAddrTrackingControl =
    { 0x00000147, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IAddrExclusionControl =
    { 0x00000148, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IPipeByte =
    { 0xDB2F3ACA, 0x2F86, 0x11D1, { 0x8E, 0x04, 0x00, 0xC0, 0x4F, 0xB9, 0x98, 0x9A } };
EXTERN_C const IID  IID_IPipeLong =
    { 0xDB2F3ACC, 0x2F86, 0x11D1, { 0x8E, 0x04, 0x00, 0xC0, 0x4F, 0xB9, 0x98, 0x9A } };
EXTERN_C const IID  IID_IPipeDouble =
    { 0xDB2F3ACE, 0x2F86, 0x11D1, { 0x8E, 0x04, 0x00, 0xC0, 0x4F, 0xB9, 0x98, 0x9A } };
EXTERN_C const IID  IID_IThumbnailExtractor =
    { 0x969DC708, 0x5C76, 0x11D1, { 0x8D, 0x86, 0x00, 0x00, 0xF8, 0x04, 0xB0, 0x57 } };
EXTERN_C const IID  IID_IDummyHICONIncluder =
    { 0x947990DE, 0xCC28, 0x11D2, { 0xA0, 0xF7, 0x00, 0x80, 0x5F, 0x85, 0x8F, 0xB1 } };
EXTERN_C const IID  IID_IProcessLock =
    { 0x000001D5, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ISurrogateService =
    { 0x000001D4, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IComThreadingInfo =
    { 0x000001CE, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IProcessInitControl =
    { 0x72380D55, 0x8D2B, 0x43A3, { 0x85, 0x13, 0x2B, 0x6E, 0xF3, 0x14, 0x34, 0xE9 } };
EXTERN_C const IID  IID_IInitializeSpy =
    { 0x00000034, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

/* oleidl.h */
EXTERN_C const IID  IID_IOleAdviseHolder =
    { 0x00000111, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleCache =
    { 0x0000011E, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleCache2 =
    { 0x00000128, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleCacheControl =
    { 0x00000129, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IParseDisplayName =
    { 0x0000011A, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleContainer =
    { 0x0000011B, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleClientSite =
    { 0x00000118, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleObject =
    { 0x00000112, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleWindow =
    { 0x00000114, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleLink =
    { 0x0000011D, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleItemContainer =
    { 0x0000011C, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleInPlaceUIWindow =
    { 0x00000115, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleInPlaceActiveObject =
    { 0x00000117, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleInPlaceFrame =
    { 0x00000116, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleInPlaceObject =
    { 0x00000113, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IOleInPlaceSite =
    { 0x00000119, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IContinue =
    { 0x0000012A, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IViewObject =
    { 0x0000010D, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IViewObject2 =
    { 0x00000127, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IDropSource =
    { 0x00000121, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IDropTarget =
    { 0x00000122, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IDropSourceNotify =
    { 0x0000012B, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumOLEVERB =
    { 0x00000104, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

/* servprov.h */
EXTERN_C const IID  IID_IServiceProvider =
    { 0x6D5140C1, 0x7436, 0x11CE, { 0x80, 0x34, 0x00, 0xAA, 0x00, 0x60, 0x09, 0xFA } };

/* oaidl.h */
EXTERN_C const IID  IID_ICreateTypeInfo =
    { 0x00020405, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ICreateTypeInfo2 =
    { 0x0002040E, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ICreateTypeLib =
    { 0x00020406, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ICreateTypeLib2 =
    { 0x0002040F, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IDispatch =
    { 0x00020400, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumVARIANT =
    { 0x00020404, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ITypeComp =
    { 0x00020403, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ITypeInfo =
    { 0x00020401, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ITypeInfo2 =
    { 0x00020412, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ITypeLib =
    { 0x00020402, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ITypeLib2 =
    { 0x00020411, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ITypeChangeEvents =
    { 0x00020410, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IErrorInfo =
    { 0x1CF2B120, 0x547D, 0x101B, { 0x8E, 0x65, 0x08, 0x00, 0x2B, 0x2B, 0xD1, 0x19 } };
EXTERN_C const IID  IID_ICreateErrorInfo =
    { 0x22F03340, 0x547D, 0x101B, { 0x8E, 0x65, 0x08, 0x00, 0x2B, 0x2B, 0xD1, 0x19 } };
EXTERN_C const IID  IID_ISupportErrorInfo =
    { 0xDF0B3D60, 0x548F, 0x101B, { 0x8E, 0x65, 0x08, 0x00, 0x2B, 0x2B, 0xD1, 0x19 } };
EXTERN_C const IID  IID_ITypeFactory =
    { 0x0000002E, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ITypeMarshal =
    { 0x0000002D, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IRecordInfo =
    { 0x0000002F, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IErrorLog =
    { 0x3127CA40, 0x446E, 0x11CE, { 0x81, 0x35, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
EXTERN_C const IID  IID_IPropertyBag =
    { 0x55272A00, 0x42CB, 0x11CE, { 0x81, 0x35, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };

/* msxml.h */
EXTERN_C const IID      LIBID_MSXML =
    { 0xD63E0CE2, 0xA0A2, 0x11D0, { 0x9C, 0x02, 0x00, 0xC0, 0x4F, 0xC9, 0x9C, 0x8E } };
EXTERN_C const IID      IID_IXMLDOMNode =
    { 0x2933BF80, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMDocument =
    { 0x2933BF81, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMNodeList =
    { 0x2933BF82, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMNamedNodeMap =
    { 0x2933BF83, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMDocumentFragment =
    { 0x3EFAA413, 0x272F, 0x11D2, { 0x83, 0x6F, 0x00, 0x00, 0xF8, 0x7A, 0x77, 0x82 } };
EXTERN_C const IID      IID_IXMLDOMCharacterData =
    { 0x2933BF84, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMAttribute =
    { 0x2933BF85, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMElement =
    { 0x2933BF86, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMText =
    { 0x2933BF87, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMComment =
    { 0x2933BF88, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMProcessingInstruction =
    { 0x2933BF89, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMCDATASection =
    { 0x2933BF8A, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMDocumentType =
    { 0x2933BF8B, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMNotation =
    { 0x2933BF8C, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMEntity =
    { 0x2933BF8D, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMEntityReference =
    { 0x2933BF8E, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLDOMImplementation =
    { 0x2933BF8F, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXTLRuntime =
    { 0x3EFAA425, 0x272F, 0x11D2, { 0x83, 0x6F, 0x00, 0x00, 0xF8, 0x7A, 0x77, 0x82 } };
EXTERN_C const IID      IID_IXMLDOMParseError =
    { 0x3EFAA426, 0x272F, 0x11D2, { 0x83, 0x6F, 0x00, 0x00, 0xF8, 0x7A, 0x77, 0x82 } };
EXTERN_C const IID      DIID_XMLDOMDocumentEvents =
    { 0x3EFAA427, 0x272F, 0x11D2, { 0x83, 0x6F, 0x00, 0x00, 0xF8, 0x7A, 0x77, 0x82 } };
EXTERN_C const CLSID    CLSID_DOMDocument =
    { 0x2933BF90, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const CLSID    CLSID_DOMFreeThreadedDocument =
    { 0x2933BF91, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } };
EXTERN_C const IID      IID_IXMLHttpRequest =
    { 0xED8C108D, 0x4349, 0x11D2, { 0x91, 0xA4, 0x00, 0xC0, 0x4F, 0x79, 0x69, 0xE8 } };
EXTERN_C const CLSID    CLSID_XMLHTTPRequest =
    { 0xED8C108E, 0x4349, 0x11D2, { 0x91, 0xA4, 0x00, 0xC0, 0x4F, 0x79, 0x69, 0xE8 } };
EXTERN_C const IID      IID_IXMLDSOControl =
    { 0x310AFA62, 0x0575, 0x11D2, { 0x9C, 0xA9, 0x00, 0x60, 0xB0, 0xEC, 0x3D, 0x39 } };
EXTERN_C const CLSID    CLSID_XMLDSOControl =
    { 0x550DDA30, 0x0541, 0x11D2, { 0x9C, 0xA9, 0x00, 0x60, 0xB0, 0xEC, 0x3D, 0x39 } };
EXTERN_C const IID      IID_IXMLElementCollection =
    { 0x65725580, 0x9B5D, 0x11D0, { 0x9B, 0xFE, 0x00, 0xC0, 0x4F, 0xC9, 0x9C, 0x8E } };
EXTERN_C const IID      IID_IXMLDocument =
    { 0xF52E2B61, 0x18A1, 0x11D1, { 0xB1, 0x05, 0x00, 0x80, 0x5F, 0x49, 0x91, 0x6B } };
EXTERN_C const IID      IID_IXMLDocument2 =
    { 0x2B8DE2FE, 0x8D2D, 0x11D1, { 0xB2, 0xFC, 0x00, 0xC0, 0x4F, 0xD9, 0x15, 0xA9 } };
EXTERN_C const IID      IID_IXMLElement =
    { 0x3F7F31AC, 0xE15F, 0x11D0, { 0x9C, 0x25, 0x00, 0xC0, 0x4F, 0xC9, 0x9C, 0x8E } };
EXTERN_C const IID      IID_IXMLElement2 =
    { 0x2B8DE2FF, 0x8D2D, 0x11D1, { 0xB2, 0xFC, 0x00, 0xC0, 0x4F, 0xD9, 0x15, 0xA9 } };
EXTERN_C const IID      IID_IXMLAttribute =
    { 0xD4D4A0FC, 0x3B73, 0x11D1, { 0xB2, 0xB4, 0x00, 0xC0, 0x4F, 0xB9, 0x25, 0x96 } };
EXTERN_C const IID      IID_IXMLError =
    { 0x948C5AD3, 0xC58D, 0x11D0, { 0x9C, 0x0B, 0x00, 0xC0, 0x4F, 0xC9, 0x9C, 0x8E } };
EXTERN_C const CLSID    CLSID_XMLDocument =
    { 0xCFC399AF, 0xD876, 0x11D0, { 0x9C, 0x10, 0x00, 0xC0, 0x4F, 0xC9, 0x9C, 0x8E } };

/* urlmon.h */
EXTERN_C const IID  IID_IPersistMoniker =
    { 0x79EAC9C9, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IMonikerProp =
    { 0xA5CA5F7F, 0x1847, 0x4D87, { 0x9C, 0x5B, 0x91, 0x85, 0x09, 0xF7, 0x51, 0x1D } };
EXTERN_C const IID  IID_IBindProtocol =
    { 0x79EAC9CD, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IBinding =
    { 0x79EAC9C0, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IBindStatusCallback =
    { 0x79EAC9C1, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IAuthenticate =
    { 0x79EAC9D0, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IHttpNegotiate =
    { 0x79EAC9D2, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IHttpNegotiate2 =
    { 0x4F9F9FCB, 0xE0F4, 0x48EB, { 0xB7, 0xAB, 0xFA, 0x2E, 0xA9, 0x36, 0x5C, 0xB4 } };
EXTERN_C const IID  IID_IWinInetFileStream =
    { 0xF134C4B7, 0xB1F8, 0x4E75, { 0xB8, 0x86, 0x64, 0xB9, 0x09, 0x43, 0xBE, 0xCB } };
EXTERN_C const IID  IID_IWindowForBindingUI =
    { 0x79EAC9D5, 0xBAFA, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_ICodeInstall =
    { 0x79EAC9D1, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IUri =
    { 0xA39EE748, 0x6A27, 0x4817, { 0xA6, 0xF2, 0x13, 0x91, 0x4B, 0xEF, 0x58, 0x90 } };
EXTERN_C const IID  IID_IUriContainer =
    { 0xA158A630, 0xED6F, 0x45FB, { 0xB9, 0x87, 0xF6, 0x86, 0x76, 0xF5, 0x77, 0x52 } };
EXTERN_C const IID  IID_IUriBuilder =
    { 0x4221B2E1, 0x8955, 0x46C0, { 0xBD, 0x5B, 0xDE, 0x98, 0x97, 0x56, 0x5D, 0xE7 } };
EXTERN_C const IID  IID_IUriBuilderFactory =
    { 0xE982CE48, 0x0B96, 0x440C, { 0xBC, 0x37, 0x0C, 0x86, 0x9B, 0x27, 0xA2, 0x9E } };
EXTERN_C const IID  IID_IWinInetInfo =
    { 0x79EAC9D6, 0xBAFA, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IHttpSecurity =
    { 0x79EAC9D7, 0xBAFA, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IWinInetHttpInfo =
    { 0x79EAC9D8, 0xBAFA, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IWinInetCacheHints =
    { 0xDD1EC3B3, 0x8391, 0x4FDB, { 0xA9, 0xE6, 0x34, 0x7C, 0x3C, 0xAA, 0xA7, 0xDD } };
EXTERN_C const IID  IID_IWinInetCacheHints2 =
    { 0x7857AEAC, 0xD31F, 0x49BF, { 0x88, 0x4E, 0xDD, 0x46, 0xDF, 0x36, 0x78, 0x0A } };
EXTERN_C const IID  IID_IBindHost =
    { 0xFC4801A1, 0x2BA9, 0x11CF, { 0xA2, 0x29, 0x00, 0xAA, 0x00, 0x3D, 0x73, 0x52 } };
EXTERN_C const IID  IID_IInternet =
    { 0x79EAC9E0, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetBindInfo =
    { 0x79EAC9E1, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetProtocolRoot =
    { 0x79EAC9E3, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetProtocol =
    { 0x79EAC9E4, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetProtocolEx =
    { 0xC7A98E66, 0x1010, 0x492C, { 0xA1, 0xC8, 0xC8, 0x09, 0xE1, 0xF7, 0x59, 0x05 } };
EXTERN_C const IID  IID_IInternetProtocolSink =
    { 0x79EAC9E5, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetProtocolSinkStackable =
    { 0x79EAC9F0, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetSession =
    { 0x79EAC9E7, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetThreadSwitch =
    { 0x79EAC9E8, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetPriority =
    { 0x79EAC9EB, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetProtocolInfo =
    { 0x79EAC9EC, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetSecurityMgrSite =
    { 0x79EAC9ED, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetSecurityManager =
    { 0x79EAC9EE, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetSecurityManagerEx =
    { 0xF163EDF1, 0xCC7C, 0x4F0D, { 0x9A, 0x94, 0x34, 0x22, 0x26, 0x25, 0xC3, 0x93 } };
EXTERN_C const IID  IID_IInternetSecurityManagerEx2 =
    { 0xF1E50292, 0xA795, 0x4117, { 0x8E, 0x09, 0x2B, 0x56, 0x0A, 0x72, 0xAC, 0x60 } };
EXTERN_C const IID  IID_IZoneIdentifier =
    { 0xCD45F185, 0x1B21, 0x48E2, { 0x96, 0x7B, 0xEA, 0xD7, 0x43, 0xA8, 0x91, 0x4E } };
EXTERN_C const IID  IID_IInternetHostSecurityManager =
    { 0x3AF280B6, 0xCB3F, 0x11D0, { 0x89, 0x1E, 0x00, 0xC0, 0x4F, 0xB6, 0xBF, 0xC4 } };
EXTERN_C const IID  IID_IInternetZoneManager =
    { 0x79EAC9EF, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
EXTERN_C const IID  IID_IInternetZoneManagerEx =
    { 0xA4C23339, 0x8E06, 0x431E, { 0x9B, 0xF4, 0x7E, 0x71, 0x1C, 0x08, 0x56, 0x48 } };
EXTERN_C const IID  IID_IInternetZoneManagerEx2 =
    { 0xEDC17559, 0xDD5D, 0x4846, { 0x8E, 0xEF, 0x8B, 0xEC, 0xBA, 0x5A, 0x4A, 0xBF } };
EXTERN_C const IID  IID_ISoftDistExt =
    { 0xB15B8DC1, 0xC7E1, 0x11D0, { 0x86, 0x80, 0x00, 0xAA, 0x00, 0xBD, 0xCB, 0x71 } };
EXTERN_C const IID  IID_ICatalogFileInfo =
    { 0x711C7600, 0x6B48, 0x11D1, { 0xB4, 0x03, 0x00, 0xAA, 0x00, 0xB9, 0x2A, 0xF1 } };
EXTERN_C const IID  IID_IDataFilter =
    { 0x69D14C80, 0xC18E, 0x11D0, { 0xA9, 0xCE, 0x00, 0x60, 0x97, 0x94, 0x23, 0x11 } };
EXTERN_C const IID  IID_IEncodingFilterFactory =
    { 0x70BDDE00, 0xC18E, 0x11D0, { 0xA9, 0xCE, 0x00, 0x60, 0x97, 0x94, 0x23, 0x11 } };
EXTERN_C const IID  IID_IWrappedProtocol =
    { 0x53C84785, 0x8425, 0x4DC5, { 0x97, 0x1B, 0xE5, 0x8D, 0x9C, 0x19, 0xF9, 0xB6 } };

/* propidl.h */
EXTERN_C const IID  IID_IPropertyStorage =
    { 0x00000138, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IPropertySetStorage =
    { 0x0000013A, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumSTATPROPSTG =
    { 0x00000139, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumSTATPROPSETSTG =
    { 0x0000013B, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

/* commdlg.h */
EXTERN_C const IID  IID_IPrintDialogCallback =
    { 0x5852A2C3, 0x6530, 0x11D1, { 0xB6, 0xA3, 0x00, 0x00, 0xF8, 0x75, 0x7B, 0xF9 } };
EXTERN_C const IID  IID_IPrintDialogServices =
    { 0x509AAEDA, 0x5639, 0x11D1, { 0xB6, 0xA1, 0x00, 0x00, 0xF8, 0x75, 0x7B, 0xF9 } };

/* aclui.h */
EXTERN_C const IID  IID_ISecurityInformation =
    { 0x965FC360, 0x16FF, 0x11D0, { 0x91, 0xCB, 0x00, 0xAA, 0x00, 0xBB, 0xB7, 0x23 } };
EXTERN_C const IID  IID_ISecurityInformation2 =
    { 0xC3CCFDB4, 0x6F88, 0x11D2, { 0xA3, 0xCE, 0x00, 0xC0, 0x4F, 0xB1, 0x78, 0x2A } };
EXTERN_C const IID  IID_IEffectivePermission =
    { 0x3853DC76, 0x9F35, 0x407C, { 0x88, 0xA1, 0xD1, 0x93, 0x44, 0x36, 0x5F, 0xBC } };
EXTERN_C const IID  IID_ISecurityObjectTypeInfo =
    { 0xFC3066EB, 0x79EF, 0x444B, { 0x91, 0x11, 0xD1, 0x8A, 0x75, 0xEB, 0xF2, 0xFA } };
EXTERN_C const IID  IID_ISecurityInformation3 =
    { 0xE2CDC9CC, 0x31BD, 0x4F8F, { 0x8C, 0x8B, 0xB6, 0x41, 0xAF, 0x51, 0x6A, 0x1A } };

/* comcat.h */
EXTERN_C const IID  IID_IEnumGUID =
    { 0x0002E000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_IEnumCATEGORYINFO =
    { 0x0002E011, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ICatRegister =
    { 0x0002E012, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID  IID_ICatInformation =
    { 0x0002E013, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

/* ocidl.h */
EXTERN_C const IID  IID_IEnumConnections =
    { 0xB196B287, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IConnectionPoint =
    { 0xB196B286, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IEnumConnectionPoints =
    { 0xB196B285, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IConnectionPointContainer =
    { 0xB196B284, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IClassFactory2 =
    { 0xB196B28F, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IProvideClassInfo =
    { 0xB196B283, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IProvideClassInfo2 =
    { 0xA6BC3AC0, 0xDBAA, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
EXTERN_C const IID  IID_IProvideMultipleClassInfo =
    { 0xA7ABA9C1, 0x8983, 0x11CF, { 0x8F, 0x20, 0x00, 0x80, 0x5F, 0x2C, 0xD0, 0x64 } };
EXTERN_C const IID  IID_IOleControl =
    { 0xB196B288, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IOleControlSite =
    { 0xB196B289, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IPropertyPage =
    { 0xB196B28D, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IPropertyPage2 =
    { 0x01E44665, 0x24AC, 0x101B, { 0x84, 0xED, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
EXTERN_C const IID  IID_IPropertyPageSite =
    { 0xB196B28C, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IPropertyNotifySink =
    { 0x9BFBBC02, 0xEFF1, 0x101A, { 0x84, 0xED, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_ISpecifyPropertyPages =
    { 0xB196B28B, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const IID  IID_IPersistMemory =
    { 0xBD1AE5E0, 0xA6AE, 0x11CE, { 0xBD, 0x37, 0x50, 0x42, 0x00, 0xC1, 0x00, 0x00 } };
EXTERN_C const IID  IID_IPersistStreamInit =
    { 0x7FD52380, 0x4E07, 0x101B, { 0xAE, 0x2D, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
EXTERN_C const IID  IID_IPersistPropertyBag =
    { 0x37D84F60, 0x42CB, 0x11CE, { 0x81, 0x35, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
EXTERN_C const IID  IID_ISimpleFrameSite =
    { 0x742B0E01, 0x14E6, 0x101B, { 0x91, 0x4E, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const IID  IID_IFont =
    { 0xBEF6E002, 0xA874, 0x101A, { 0x8B, 0xBA, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const IID  IID_IPicture =
    { 0x7BF80980, 0xBF32, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const IID  IID_IPicture2 =
    { 0xF5185DD8, 0x2012, 0x4B0B, { 0xAA, 0xD9, 0xF0, 0x52, 0xC6, 0xBD, 0x48, 0x2B } };
EXTERN_C const IID  IID_IFontEventsDisp =
    { 0x4EF6100A, 0xAF88, 0x11D0, { 0x98, 0x46, 0x00, 0xC0, 0x4F, 0xC2, 0x99, 0x93 } };
EXTERN_C const IID  IID_IFontDisp =
    { 0xBEF6E003, 0xA874, 0x101A, { 0x8B, 0xBA, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const IID  IID_IPictureDisp =
    { 0x7BF80981, 0xBF32, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const IID  IID_IOleInPlaceObjectWindowless =
    { 0x1C2056CC, 0x5EF4, 0x101B, { 0x8B, 0xC8, 0x00, 0xAA, 0x00, 0x3E, 0x3B, 0x29 } };
EXTERN_C const IID  IID_IOleInPlaceSiteEx =
    { 0x9C2CAD80, 0x3424, 0x11CF, { 0xB6, 0x70, 0x00, 0xAA, 0x00, 0x4C, 0xD6, 0xD8 } };
EXTERN_C const IID  IID_IOleInPlaceSiteWindowless =
    { 0x922EADA0, 0x3424, 0x11CF, { 0xB6, 0x70, 0x00, 0xAA, 0x00, 0x4C, 0xD6, 0xD8 } };
EXTERN_C const IID  IID_IViewObjectEx =
    { 0x3AF24292, 0x0C96, 0x11CE, { 0xA0, 0xCF, 0x00, 0xAA, 0x00, 0x60, 0x0A, 0xB8 } };
EXTERN_C const IID  IID_IOleUndoUnit =
    { 0x894AD3B0, 0xEF97, 0x11CE, { 0x9B, 0xC9, 0x00, 0xAA, 0x00, 0x60, 0x8E, 0x01 } };
EXTERN_C const IID  IID_IOleParentUndoUnit =
    { 0xA1FAF330, 0xEF97, 0x11CE, { 0x9B, 0xC9, 0x00, 0xAA, 0x00, 0x60, 0x8E, 0x01 } };
EXTERN_C const IID  IID_IEnumOleUndoUnits =
    { 0xB3E7C340, 0xEF97, 0x11CE, { 0x9B, 0xC9, 0x00, 0xAA, 0x00, 0x60, 0x8E, 0x01 } };
EXTERN_C const IID  IID_IOleUndoManager =
    { 0xD001F200, 0xEF97, 0x11CE, { 0x9B, 0xC9, 0x00, 0xAA, 0x00, 0x60, 0x8E, 0x01 } };
EXTERN_C const IID  IID_IPointerInactive =
    { 0x44980BA0, 0x35AA, 0x11CF, { 0xB6, 0x71, 0x00, 0xAA, 0x00, 0x4C, 0xD6, 0xD8 } };
EXTERN_C const IID  IID_IObjectWithSite =
    { 0xFC4801A3, 0x2BA9, 0x11CF, { 0xA2, 0x29, 0x00, 0xAA, 0x00, 0x3D, 0x73, 0x52 } };
EXTERN_C const IID  IID_IPerPropertyBrowsing =
    { 0x376BD3AA, 0x3845, 0x101B, { 0x84, 0xED, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
EXTERN_C const IID  IID_IPropertyBag2 =
    { 0x22F55882, 0x280B, 0x11D0, { 0xA8, 0xA9, 0x00, 0xA0, 0xC9, 0x0C, 0x20, 0x04 } };
EXTERN_C const IID  IID_IPersistPropertyBag2 =
    { 0x22F55881, 0x280B, 0x11D0, { 0xA8, 0xA9, 0x00, 0xA0, 0xC9, 0x0C, 0x20, 0x04 } };
EXTERN_C const IID  IID_IAdviseSinkEx =
    { 0x3AF24290, 0x0C96, 0x11CE, { 0xA0, 0xCF, 0x00, 0xAA, 0x00, 0x60, 0x0A, 0xB8 } };
EXTERN_C const IID  IID_IQuickActivate =
    { 0xCF51ED10, 0x62FE, 0x11CF, { 0xBF, 0x86, 0x00, 0xA0, 0xC9, 0x03, 0x48, 0x36 } };

/* docobj.h */
EXTERN_C const IID  IID_IOleDocument =
    { 0xB722BCC5, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };
EXTERN_C const IID  IID_IOleDocumentSite =
    { 0xB722BCC7, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };
EXTERN_C const IID  IID_IOleDocumentView =
    { 0xB722BCC6, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };
EXTERN_C const IID  IID_IEnumOleDocumentViews =
    { 0xB722BCC8, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };
EXTERN_C const IID  IID_IContinueCallback =
    { 0xB722BCCA, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };
EXTERN_C const IID  IID_IPrint =
    { 0xB722BCC9, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };
EXTERN_C const IID  IID_IOleCommandTarget =
    { 0xB722BCCB, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };
EXTERN_C const IID  IID_IZoomEvents =
    { 0x41B68150, 0x904C, 0x4E17, { 0xA0, 0xBA, 0xA4, 0x38, 0x18, 0x2E, 0x35, 0x9D } };
EXTERN_C const IID  IID_IProtectFocus =
    { 0xD81F90A3, 0x8156, 0x44F7, { 0xAD, 0x28, 0x5A, 0xBB, 0x87, 0x00, 0x32, 0x74 } };
EXTERN_C const IID  IID_IProtectedModeMenuServices =
    { 0x73C105EE, 0x9DFF, 0x4A07, { 0xB8, 0x3C, 0x7E, 0xFF, 0x29, 0x0C, 0x26, 0x6E } };

/* olectl.h */
EXTERN_C const IID      IID_IPropertyFrame =
    { 0xB196B28A, 0xBAB4, 0x101A, { 0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };
EXTERN_C const CLSID    CLSID_CFontPropPage =
    { 0x0BE35200, 0x8F91, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
EXTERN_C const CLSID    CLSID_CColorPropPage =
    { 0x0BE35201, 0x8F91, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
EXTERN_C const CLSID    CLSID_CPicturePropPage =
    { 0x0BE35202, 0x8F91, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
EXTERN_C const CLSID    CLSID_PersistPropset =
    { 0xFB8F0821, 0x0164, 0x101B, { 0x84, 0xED, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
EXTERN_C const CLSID    CLSID_ConvertVBX =
    { 0xFB8F0822, 0x0164, 0x101B, { 0x84, 0xED, 0x08, 0x00, 0x2B, 0x2E, 0xC7, 0x13 } };
EXTERN_C const CLSID    CLSID_StdFont =
    { 0x0BE35203, 0x8F91, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
EXTERN_C const CLSID    CLSID_StdPicture =
    { 0x0BE35204, 0x8F91, 0x11CE, { 0x9D, 0xE3, 0x00, 0xAA, 0x00, 0x4B, 0xB8, 0x51 } };
EXTERN_C const GUID     GUID_HIMETRIC =
    { 0x66504300, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_COLOR =
    { 0x66504301, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_XPOSPIXEL =
    { 0x66504302, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_YPOSPIXEL =
    { 0x66504303, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_XSIZEPIXEL =
    { 0x66504304, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_YSIZEPIXEL =
    { 0x66504305, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_XPOS =
    { 0x66504306, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_YPOS =
    { 0x66504307, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_XSIZE =
    { 0x66504308, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_YSIZE =
    { 0x66504309, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_TRISTATE =
    { 0x6650430A, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_OPTIONVALUEEXCLUSIVE =
    { 0x6650430B, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_CHECKVALUEEXCLUSIVE =
    { 0x6650430C, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_FONTNAME =
    { 0x6650430D, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_FONTSIZE =
    { 0x6650430E, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_FONTBOLD =
    { 0x6650430F, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_FONTITALIC =
    { 0x66504310, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_FONTUNDERSCORE =
    { 0x66504311, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_FONTSTRIKETHROUGH =
    { 0x66504312, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };
EXTERN_C const GUID     GUID_HANDLE =
    { 0x66504313, 0xBE0F, 0x101A, { 0x8B, 0xBB, 0x00, 0xAA, 0x00, 0x30, 0x0C, 0xAB } };

/* exdisp.h */
EXTERN_C const IID      LIBID_SHDocVw =
    { 0xEAB22AC0, 0x30C1, 0x11CF, { 0xA7, 0xEB, 0x00, 0x00, 0xC0, 0x5B, 0xAE, 0x0B } };
EXTERN_C const IID      IID_IWebBrowser =
    { 0xEAB22AC1, 0x30C1, 0x11CF, { 0xA7, 0xEB, 0x00, 0x00, 0xC0, 0x5B, 0xAE, 0x0B } };
EXTERN_C const IID      DIID_DWebBrowserEvents =
    { 0xEAB22AC2, 0x30C1, 0x11CF, { 0xA7, 0xEB, 0x00, 0x00, 0xC0, 0x5B, 0xAE, 0x0B } };
EXTERN_C const IID      IID_IWebBrowserApp =
    { 0x0002DF05, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID      IID_IWebBrowser2 =
    { 0xD30C1661, 0xCDAF, 0x11D0, { 0x8A, 0x3E, 0x00, 0xC0, 0x4F, 0xC9, 0xE2, 0x6E } };
EXTERN_C const IID      DIID_DWebBrowserEvents2 =
    { 0x34A715A0, 0x6587, 0x11D0, { 0x92, 0x4A, 0x00, 0x20, 0xAF, 0xC7, 0xAC, 0x4D } };
EXTERN_C const CLSID    CLSID_WebBrowser_V1 =
    { 0xEAB22AC3, 0x30C1, 0x11CF, { 0xA7, 0xEB, 0x00, 0x00, 0xC0, 0x5B, 0xAE, 0x0B } };
EXTERN_C const CLSID    CLSID_WebBrowser =
    { 0x8856F961, 0x340A, 0x11D0, { 0xA9, 0x6B, 0x00, 0xC0, 0x4F, 0xD7, 0x05, 0xA2 } };
EXTERN_C const CLSID    CLSID_InternetExplorer =
    { 0x0002DF01, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const CLSID    CLSID_ShellBrowserWindow =
    { 0xC08AFD90, 0xF2A1, 0x11D1, { 0x84, 0x55, 0x00, 0xA0, 0xC9, 0x1F, 0x38, 0x80 } };
EXTERN_C const IID      DIID_DShellWindowEvents =
    { 0xFE4106E0, 0x399A, 0x11D0, { 0xA4, 0x8C, 0x00, 0xA0, 0xC9, 0x0A, 0x8F, 0x39 } };
EXTERN_C const IID      IID_IShellWindows =
    { 0x85CB6900, 0x4D95, 0x11CF, { 0x96, 0x0C, 0x00, 0x80, 0xC7, 0xF4, 0xEE, 0x85 } };
EXTERN_C const CLSID    CLSID_ShellWindows =
    { 0x9BA05972, 0xF6A8, 0x11CF, { 0xA4, 0x42, 0x00, 0xA0, 0xC9, 0x0A, 0x8F, 0x39 } };
EXTERN_C const IID      IID_IShellUIHelper =
    { 0x729FE2F8, 0x1EA8, 0x11D1, { 0x8F, 0x85, 0x00, 0xC0, 0x4F, 0xC2, 0xFB, 0xE1 } };
EXTERN_C const CLSID    CLSID_ShellUIHelper =
    { 0x64AB4BB7, 0x111E, 0x11D1, { 0x8F, 0x79, 0x00, 0xC0, 0x4F, 0xC2, 0xFB, 0xE1 } };
EXTERN_C const IID      DIID__ShellFavoritesNameSpaceEvents =
    { 0x55136806, 0xB2DE, 0x11D1, { 0xB9, 0xF2, 0x00, 0xA0, 0xC9, 0x8B, 0xC5, 0x47 } };
EXTERN_C const IID      IID_IShellFavoritesNameSpace =
    { 0x55136804, 0xB2DE, 0x11D1, { 0xB9, 0xF2, 0x00, 0xA0, 0xC9, 0x8B, 0xC5, 0x47 } };
EXTERN_C const CLSID    CLSID_ShellFavoritesNameSpace =
    { 0x55136805, 0xB2DE, 0x11D1, { 0xB9, 0xF2, 0x00, 0xA0, 0xC9, 0x8B, 0xC5, 0x47 } };
EXTERN_C const IID      IID_IScriptErrorList =
    { 0xF3470F24, 0x15FD, 0x11D2, { 0xBB, 0x2E, 0x00, 0x80, 0x5F, 0xF7, 0xEF, 0xCA } };
EXTERN_C const CLSID    CLSID_CScriptErrorList =
    { 0xEFD01300, 0x160F, 0x11D2, { 0xBB, 0x2E, 0x00, 0x80, 0x5F, 0xF7, 0xEF, 0xCA } };
EXTERN_C const IID      IID_ISearch =
    { 0xBA9239A4, 0x3DD5, 0x11D2, { 0xBF, 0x8B, 0x00, 0xC0, 0x4F, 0xB9, 0x36, 0x61 } };
EXTERN_C const IID      IID_ISearches =
    { 0x47C922A2, 0x3DD5, 0x11D2, { 0xBF, 0x8B, 0x00, 0xC0, 0x4F, 0xB9, 0x36, 0x61 } };
EXTERN_C const IID      IID_ISearchAssistantOC =
    { 0x72423E8F, 0x8011, 0x11D2, { 0xBE, 0x79, 0x00, 0xA0, 0xC9, 0xA8, 0x3D, 0xA1 } };
EXTERN_C const IID      DIID__SearchAssistantEvents =
    { 0x1611FDDA, 0x445B, 0x11D2, { 0x85, 0xDE, 0x00, 0xC0, 0x4F, 0xA3, 0x5C, 0x89 } };
EXTERN_C const CLSID    CLSID_SearchAssistantOC =
    { 0xB45FF030, 0x4447, 0x11D2, { 0x85, 0xDE, 0x00, 0xC0, 0x4F, 0xA3, 0x5C, 0x89 } };

/* isguids.h */
EXTERN_C const CLSID    CLSID_InternetShortcut =
    { 0xFBF23B40, 0xE3F0, 0x101B, { 0x84, 0x88, 0x00, 0xAA, 0x00, 0x3E, 0x56, 0xF8 } };
EXTERN_C const IID      IID_IUniformResourceLocatorA =
    { 0xFBF23B80, 0xE3F0, 0x101B, { 0x84, 0x88, 0x00, 0xAA, 0x00, 0x3E, 0x56, 0xF8 } };
EXTERN_C const IID      IID_IUniformResourceLocatorW =
    { 0xCABB0DA0, 0xDA57, 0x11CF, { 0x99, 0x74, 0x00, 0x20, 0xAF, 0xD7, 0x97, 0x62 } };

/* mlang.h */
EXTERN_C const IID      LIBID_MultiLanguage =
    { 0x275C23E0, 0x3747, 0x11D0, { 0x9F, 0xEA, 0x00, 0xAA, 0x00, 0x3F, 0x86, 0x46 } };
EXTERN_C const IID      IID_IMLangStringBufW =
    { 0xD24ACD21, 0xBA72, 0x11D0, { 0xB1, 0x88, 0x00, 0xAA, 0x00, 0x38, 0xC9, 0x69 } };
EXTERN_C const IID      IID_IMLangStringBufA =
    { 0xD24ACD23, 0xBA72, 0x11D0, { 0xB1, 0x88, 0x00, 0xAA, 0x00, 0x38, 0xC9, 0x69 } };
EXTERN_C const IID      IID_IMLangString =
    { 0xC04D65CE, 0xB70D, 0x11D0, { 0xB1, 0x88, 0x00, 0xAA, 0x00, 0x38, 0xC9, 0x69 } };
EXTERN_C const IID      IID_IMLangStringWStr =
    { 0xC04D65D0, 0xB70D, 0x11D0, { 0xB1, 0x88, 0x00, 0xAA, 0x00, 0x38, 0xC9, 0x69 } };
EXTERN_C const IID      IID_IMLangStringAStr =
    { 0xC04D65D2, 0xB70D, 0x11D0, { 0xB1, 0x88, 0x00, 0xAA, 0x00, 0x38, 0xC9, 0x69 } };
EXTERN_C const CLSID    CLSID_CMLangString =
    { 0xC04D65CF, 0xB70D, 0x11D0, { 0xB1, 0x88, 0x00, 0xAA, 0x00, 0x38, 0xC9, 0x69 } };
EXTERN_C const IID      IID_IMLangLineBreakConsole =
    { 0xF5BE2EE1, 0xBFD7, 0x11D0, { 0xB1, 0x88, 0x00, 0xAA, 0x00, 0x38, 0xC9, 0x69 } };
EXTERN_C const IID      IID_IEnumCodePage =
    { 0x275C23E3, 0x3747, 0x11D0, { 0x9F, 0xEA, 0x00, 0xAA, 0x00, 0x3F, 0x86, 0x46 } };
EXTERN_C const IID      IID_IEnumRfc1766 =
    { 0x3DC39D1D, 0xC030, 0x11D0, { 0xB8, 0x1B, 0x00, 0xC0, 0x4F, 0xC9, 0xB3, 0x1F } };
EXTERN_C const IID      IID_IEnumScript =
    { 0xAE5F1430, 0x388B, 0x11D2, { 0x83, 0x80, 0x00, 0xC0, 0x4F, 0x8F, 0x5D, 0xA1 } };
EXTERN_C const IID      IID_IMLangConvertCharset =
    { 0xD66D6F98, 0xCDAA, 0x11D0, { 0xB8, 0x22, 0x00, 0xC0, 0x4F, 0xC9, 0xB3, 0x1F } };
EXTERN_C const CLSID    CLSID_CMLangConvertCharset =
    { 0xD66D6F99, 0xCDAA, 0x11D0, { 0xB8, 0x22, 0x00, 0xC0, 0x4F, 0xC9, 0xB3, 0x1F } };
EXTERN_C const IID      IID_IMultiLanguage =
    { 0x275C23E1, 0x3747, 0x11D0, { 0x9F, 0xEA, 0x00, 0xAA, 0x00, 0x3F, 0x86, 0x46 } };
EXTERN_C const IID      IID_IMultiLanguage2 =
    { 0xDCCFC164, 0x2B38, 0x11D2, { 0xB7, 0xEC, 0x00, 0xC0, 0x4F, 0x8F, 0x5D, 0x9A } };
EXTERN_C const IID      IID_IMLangCodePages =
    { 0x359F3443, 0xBD4A, 0x11D0, { 0xB1, 0x88, 0x00, 0xAA, 0x00, 0x38, 0xC9, 0x69 } };
EXTERN_C const IID      IID_IMLangFontLink =
    { 0x359F3441, 0xBD4A, 0x11D0, { 0xB1, 0x88, 0x00, 0xAA, 0x00, 0x38, 0xC9, 0x69 } };
EXTERN_C const IID      IID_IMLangFontLink2 =
    { 0xDCCFC162, 0x2B38, 0x11D2, { 0xB7, 0xEC, 0x00, 0xC0, 0x4F, 0x8F, 0x5D, 0x9A } };
EXTERN_C const IID      IID_IMultiLanguage3 =
    { 0x4E5868AB, 0xB157, 0x4623, { 0x9A, 0xCC, 0x6A, 0x1D, 0x9C, 0xAE, 0xBE, 0x04 } };
EXTERN_C const CLSID    CLSID_CMultiLanguage =
    { 0x275C23E2, 0x3747, 0x11D0, { 0x9F, 0xEA, 0x00, 0xAA, 0x00, 0x3F, 0x86, 0x46 } };

/* dimm.h */
EXTERN_C const IID      LIBID_ActiveIMM =
    { 0x4955DD30, 0xB159, 0x11D0, { 0x8F, 0xCF, 0x00, 0xAA, 0x00, 0x6B, 0xCC, 0x59 } };
EXTERN_C const IID      IID_IEnumRegisterWordA =
    { 0x08C03412, 0xF96B, 0x11D0, { 0xA4, 0x75, 0x00, 0xAA, 0x00, 0x6B, 0xCC, 0x59 } };
EXTERN_C const IID      IID_IEnumRegisterWordW =
    { 0x4955DD31, 0xB159, 0x11D0, { 0x8F, 0xCF, 0x00, 0xAA, 0x00, 0x6B, 0xCC, 0x59 } };
EXTERN_C const IID      IID_IEnumInputContext =
    { 0x09B5EAB0, 0xF997, 0x11D1, { 0x93, 0xD4, 0x00, 0x60, 0xB0, 0x67, 0xB8, 0x6E } };
EXTERN_C const IID      IID_IActiveIMMRegistrar =
    { 0xB3458082, 0xBD00, 0x11D1, { 0x93, 0x9B, 0x00, 0x60, 0xB0, 0x67, 0xB8, 0x6E } };
EXTERN_C const IID      IID_IActiveIMMMessagePumpOwner =
    { 0xB5CF2CFA, 0x8AEB, 0x11D1, { 0x93, 0x64, 0x00, 0x60, 0xB0, 0x67, 0xB8, 0x6E } };
EXTERN_C const IID      IID_IActiveIMMApp =
    { 0x08C0E040, 0x62D1, 0x11D1, { 0x93, 0x26, 0x00, 0x60, 0xB0, 0x67, 0xB8, 0x6E } };
EXTERN_C const IID      IID_IActiveIMMIME =
    { 0x08C03411, 0xF96B, 0x11D0, { 0xA4, 0x75, 0x00, 0xAA, 0x00, 0x6B, 0xCC, 0x59 } };
EXTERN_C const IID      IID_IActiveIME =
    { 0x6FE20962, 0xD077, 0x11D0, { 0x8F, 0xE7, 0x00, 0xAA, 0x00, 0x6B, 0xCC, 0x59 } };
EXTERN_C const IID      IID_IActiveIME2 =
    { 0xE1C4BF0E, 0x2D53, 0x11D2, { 0x93, 0xE1, 0x00, 0x60, 0xB0, 0x67, 0xB8, 0x6E } };
EXTERN_C const CLSID    CLSID_CActiveIMM =
    { 0x4955DD33, 0xB159, 0x11D0, { 0x8F, 0xCF, 0x00, 0xAA, 0x00, 0x6B, 0xCC, 0x59 } };

/* mshtml.h */
#define MSHTML_SUFFIX   0x98B5, 0x11CF, { 0xBB, 0x82, 0x00, 0xAA, 0x00, 0xBD, 0xCE, 0x0B }
EXTERN_C const IID      IID_IHTMLFiltersCollection =
    { 0x3050F3EE, MSHTML_SUFFIX };
EXTERN_C const IID      LIBID_MSHTML =
    { 0x3050F1C5, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyle =
    { 0x3050F25E, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyle2 =
    { 0x3050F4A2, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyle3 =
    { 0x3050F656, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyle4 =
    { 0x3050F816, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyle5 =
    { 0x3050F33A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLRuleStyle =
    { 0x3050F3CF, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLRuleStyle2 =
    { 0x3050F4AC, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLRuleStyle3 =
    { 0x3050F657, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLRuleStyle4 =
    { 0x3050F817, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLRuleStyle5 =
    { 0x3050F335, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLStyle =
    { 0x3050F55A, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLStyle =
    { 0x3050F285, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLRuleStyle =
    { 0x3050F55C, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLRuleStyle =
    { 0x3050F3D0, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLRenderStyle =
    { 0x3050F6AE, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLRenderStyle =
    { 0x3050F58B, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLRenderStyle =
    { 0x3050F6AA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLCurrentStyle =
    { 0x3050F3DB, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLCurrentStyle2 =
    { 0x3050F658, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLCurrentStyle3 =
    { 0x3050F818, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLCurrentStyle4 =
    { 0x3050F33B, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLCurrentStyle =
    { 0x3050F557, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLCurrentStyle =
    { 0x3050F3DC, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLRect =
    { 0x3050F4A3, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLRectCollection =
    { 0x3050F4A4, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDOMNode =
    { 0x3050F5DA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDOMNode2 =
    { 0x3050F80B, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDOMAttribute =
    { 0x3050F4B0, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDOMAttribute2 =
    { 0x3050F810, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDOMTextNode =
    { 0x3050F4B1, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDOMTextNode2 =
    { 0x3050F809, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDOMImplementation =
    { 0x3050F80D, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDOMAttribute =
    { 0x3050F564, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDOMAttribute =
    { 0x3050F4B2, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDOMTextNode =
    { 0x3050F565, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDOMTextNode =
    { 0x3050F4BA, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDOMImplementation =
    { 0x3050F58F, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDOMImplementation =
    { 0x3050F80E, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAttributeCollection =
    { 0x3050F4C3, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAttributeCollection2 =
    { 0x3050F80A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDOMChildrenCollection =
    { 0x3050F5AB, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDOMChildrenCollection =
    { 0x3050F56C, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLAttributeCollection =
    { 0x3050F4CC, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispDOMChildrenCollection =
    { 0x3050F577, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_DOMChildrenCollection =
    { 0x3050F5AA, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLElementEvents2 =
    { 0x3050F60F, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLElementEvents =
    { 0x3050F33C, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLElement =
    { 0x3050F1FF, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLElement2 =
    { 0x3050F434, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLElement3 =
    { 0x3050F673, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLElement4 =
    { 0x3050F80F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLElementRender =
    { 0x3050F669, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLUniqueName =
    { 0x3050F4D0, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDatabinding =
    { 0x3050F3F2, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLElementDefaults =
    { 0x3050F6C9, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDefaults =
    { 0x3050F58C, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDefaults =
    { 0x3050F6C8, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTCDefaultDispatch =
    { 0x3050F4FD, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTCPropertyBehavior =
    { 0x3050F5DF, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTCMethodBehavior =
    { 0x3050F631, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTCEventBehavior =
    { 0x3050F4FF, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTCAttachBehavior =
    { 0x3050F5F4, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTCAttachBehavior2 =
    { 0x3050F7EB, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTCDescBehavior =
    { 0x3050F5DC, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTCDefaultDispatch =
    { 0x3050F573, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTCDefaultDispatch =
    { 0x3050F4FC, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTCPropertyBehavior =
    { 0x3050F57F, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTCPropertyBehavior =
    { 0x3050F5DE, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTCMethodBehavior =
    { 0x3050F587, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTCMethodBehavior =
    { 0x3050F630, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTCEventBehavior =
    { 0x3050F574, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTCEventBehavior =
    { 0x3050F4FE, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTCAttachBehavior =
    { 0x3050F583, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTCAttachBehavior =
    { 0x3050F5F5, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTCDescBehavior =
    { 0x3050F57E, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTCDescBehavior =
    { 0x3050F5DD, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLUrnCollection =
    { 0x3050F5E2, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLUrnCollection =
    { 0x3050F580, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLGenericElement =
    { 0x3050F4B7, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLGenericElement =
    { 0x3050F563, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLGenericElement =
    { 0x3050F4B8, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyleSheetRule =
    { 0x3050F357, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyleSheetRulesCollection =
    { 0x3050F2E5, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLStyleSheetRule =
    { 0x3050F3CE, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLStyleSheetRulesCollection =
    { 0x3050F3CD, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyleSheetPage =
    { 0x3050F7EE, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyleSheetPagesCollection =
    { 0x3050F7F0, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLStyleSheetPage =
    { 0x3050F7EF, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLStyleSheetPagesCollection =
    { 0x3050F7F1, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyleSheet =
    { 0x3050F2E3, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyleSheet2 =
    { 0x3050F3D1, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLStyleSheet =
    { 0x3050F58D, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLStyleSheet =
    { 0x3050F2E4, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyleSheetsCollection =
    { 0x3050F37E, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLStyleSheetsCollection =
    { 0x3050F37F, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLLinkElementEvents2 =
    { 0x3050F61D, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLLinkElementEvents =
    { 0x3050F3CC, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLLinkElement =
    { 0x3050F205, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLLinkElement2 =
    { 0x3050F4E5, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLLinkElement3 =
    { 0x3050F81E, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLLinkElement =
    { 0x3050E524, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLLinkElement =
    { 0x3050E277, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTxtRange =
    { 0x3050F220, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTextRangeMetrics =
    { 0x3050F40B, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTextRangeMetrics2 =
    { 0x3050F4A6, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTxtRangeCollection =
    { 0x3050F7ED, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLFormElementEvents2 =
    { 0x3050F614, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLFormElementEvents =
    { 0x3050F364, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFormElement =
    { 0x3050F1F7, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFormElement2 =
    { 0x3050F4F6, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFormElement3 =
    { 0x3050F836, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLSubmitData =
    { 0x3050F645, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLFormElement =
    { 0x3050F510, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLFormElement =
    { 0x3050F251, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLControlElementEvents2 =
    { 0x3050F612, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLControlElementEvents =
    { 0x3050F4EA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLControlElement =
    { 0x3050F4E9, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTextElement =
    { 0x3050F218, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLTextElement =
    { 0x3050F537, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLTextElement =
    { 0x3050F26A, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLTextContainerEvents2 =
    { 0x3050F624, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLTextContainerEvents =
    { 0x1FF6AA72, 0x5842, 0x11CF, { 0xA7, 0x07, 0x00, 0xAA, 0x00, 0xC0, 0x09, 0x8D } };
EXTERN_C const IID      IID_IHTMLTextContainer =
    { 0x3050F230, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLControlRange =
    { 0x3050F29C, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLControlRange2 =
    { 0x3050F65E, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLImgEvents2 =
    { 0x3050F616, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLImgEvents =
    { 0x3050F25B, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLImgElement =
    { 0x3050F240, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLImgElement2 =
    { 0x3050F826, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLImageElementFactory =
    { 0x3050F38E, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLImg =
    { 0x3050F51C, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLImg =
    { 0x3050F241, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLImageElementFactory =
    { 0x3050F38F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLXMLHttpRequest =
    { 0x3051040A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLXMLHttpRequestFactory =
    { 0x3051040C, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLXMLHttpRequest =
    { 0x3050F496, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLXMLHttpRequest =
    { 0x3051040B, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLXMLHttpRequestFactory =
    { 0x3051040D, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLBodyElement =
    { 0x3050F1D8, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLBodyElement2 =
    { 0x3050F5C5, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLBody =
    { 0x3050F507, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLBody =
    { 0x3050F24A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFontElement =
    { 0x3050F1D9, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLFontElement =
    { 0x3050F512, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLFontElement =
    { 0x3050F27B, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLAnchorEvents2 =
    { 0x3050F610, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLAnchorEvents =
    { 0x3050F29D, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAnchorElement =
    { 0x3050F1DA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAnchorElement2 =
    { 0x3050F825, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLAnchorElement =
    { 0x3050F502, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLAnchorElement =
    { 0x3050F248, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLLabelEvents2 =
    { 0x3050F61C, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLLabelEvents =
    { 0x3050F329, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLLabelElement =
    { 0x3050F32A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLLabelElement2 =
    { 0x3050F832, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLLabelElement =
    { 0x3050F522, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLLabelElement =
    { 0x3050F32B, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLListElement =
    { 0x3050F20E, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLListElement2 =
    { 0x3050F822, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLListElement =
    { 0x3050F525, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLListElement =
    { 0x3050F272, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLUListElement =
    { 0x3050F1DD, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLUListElement =
    { 0x3050F538, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLUListElement =
    { 0x3050F269, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLOListElement =
    { 0x3050F1DE, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLOListElement =
    { 0x3050F52A, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLOListElement =
    { 0x3050F270, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLLIElement =
    { 0x3050F1E0, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLLIElement =
    { 0x3050F523, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLLIElement =
    { 0x3050F273, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLBlockElement =
    { 0x3050F208, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLBlockElement2 =
    { 0x3050F823, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLBlockElement =
    { 0x3050F506, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLBlockElement =
    { 0x3050F281, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDivElement =
    { 0x3050F200, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDivElement =
    { 0x3050F50C, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDivElement =
    { 0x3050F27E, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDDElement =
    { 0x3050F1F2, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDDElement =
    { 0x3050F50B, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDDElement =
    { 0x3050F27F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDTElement =
    { 0x3050F1F3, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDTElement =
    { 0x3050F50D, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDTElement =
    { 0x3050F27C, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLBRElement =
    { 0x3050F1F0, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLBRElement =
    { 0x3050F53A, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLBRElement =
    { 0x3050F280, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDListElement =
    { 0x3050F1F1, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDListElement =
    { 0x3050F53B, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDListElement =
    { 0x3050F27D, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLHRElement =
    { 0x3050F1F4, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLHRElement =
    { 0x3050F53D, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLHRElement =
    { 0x3050F252, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLParaElement =
    { 0x3050F1F5, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLParaElement =
    { 0x3050F52C, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLParaElement =
    { 0x3050F26F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLElementCollection =
    { 0x3050F21F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLElementCollection2 =
    { 0x3050F5EE, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLElementCollection3 =
    { 0x3050F835, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLElementCollection =
    { 0x3050F56B, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLElementCollection =
    { 0x3050F4CB, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLHeaderElement =
    { 0x3050F1F6, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLHeaderElement =
    { 0x3050F515, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLHeaderElement =
    { 0x3050F27A, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLSelectElementEvents2 =
    { 0x3050F622, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLSelectElementEvents =
    { 0x3050F302, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLSelectElementEx =
    { 0x3050F2D1, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLSelectElement =
    { 0x3050F244, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLSelectElement2 =
    { 0x3050F5ED, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLSelectElement4 =
    { 0x3050F838, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLSelectElement =
    { 0x3050F531, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLSelectElement =
    { 0x3050F245, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLWndSelectElement =
    { 0x3050F597, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLWndSelectElement =
    { 0x3050F2CF, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLSelectionObject =
    { 0x3050F25A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLSelectionObject2 =
    { 0x3050F7EC, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLOptionElement =
    { 0x3050F211, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLOptionElement3 =
    { 0x3050F820, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLOptionElementFactory =
    { 0x3050F38C, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLOptionElement =
    { 0x3050F52B, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLOptionElement =
    { 0x3050F24D, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLOptionElementFactory =
    { 0x3050F38D, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLWndOptionElement =
    { 0x3050F598, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLWndOptionElement =
    { 0x3050F2D0, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLButtonElementEvents2 =
    { 0x3050F617, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLButtonElementEvents =
    { 0x3050F2B3, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLInputTextElementEvents2 =
    { 0x3050F618, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLOptionButtonElementEvents2 =
    { 0x3050F619, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLInputFileElementEvents2 =
    { 0x3050F61A, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLInputImageEvents2 =
    { 0x3050F61B, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLInputTextElementEvents =
    { 0x3050F2A7, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLOptionButtonElementEvents =
    { 0x3050F2BD, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLInputFileElementEvents =
    { 0x3050F2AF, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLInputImageEvents =
    { 0x3050F2C3, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLInputElement =
    { 0x3050F5D2, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLInputElement2 =
    { 0x3050F821, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLInputButtonElement =
    { 0x3050F2B2, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLInputHiddenElement =
    { 0x3050F2A4, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLInputTextElement =
    { 0x3050F2A6, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLInputFileElement =
    { 0x3050F2AD, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLOptionButtonElement =
    { 0x3050F2BC, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLInputImage =
    { 0x3050F2C2, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLInputElement =
    { 0x3050F57D, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLInputElement =
    { 0x3050F5D8, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTextAreaElement =
    { 0x3050F2AA, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLTextAreaElement =
    { 0x3050F521, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLTextAreaElement =
    { 0x3050F2AC, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLRichtextElement =
    { 0x3050F54D, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLRichtextElement =
    { 0x3050F2DF, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLButtonElement =
    { 0x3050F2BB, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLButtonElement =
    { 0x3050F51F, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLButtonElement =
    { 0x3050F2C6, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLMarqueeElementEvents2 =
    { 0x3050F61F, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLMarqueeElementEvents =
    { 0x3050F2B8, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLMarqueeElement =
    { 0x3050F2B5, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLMarqueeElement =
    { 0x3050F527, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLMarqueeElement =
    { 0x3050F2B9, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLHtmlElement =
    { 0x3050F81C, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLHeadElement =
    { 0x3050F81D, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTitleElement =
    { 0x3050F322, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLMetaElement =
    { 0x3050F203, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLMetaElement2 =
    { 0x3050F81F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLBaseElement =
    { 0x3050F204, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLIsIndexElement =
    { 0x3050F206, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLIsIndexElement2 =
    { 0x3050F82F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLNextIdElement =
    { 0x3050F207, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLHtmlElement =
    { 0x3050F560, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLHtmlElement =
    { 0x3050F491, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLHeadElement =
    { 0x3050F561, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLHeadElement =
    { 0x3050F493, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLTitleElement =
    { 0x3050F516, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLTitleElement =
    { 0x3050F284, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLMetaElement =
    { 0x3050F517, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLMetaElement =
    { 0x3050F275, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLBaseElement =
    { 0x3050F518, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLBaseElement =
    { 0x3050F276, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLIsIndexElement =
    { 0x3050F519, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLIsIndexElement =
    { 0x3050F278, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLNextIdElement =
    { 0x3050F51A, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLNextIdElement =
    { 0x3050F279, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLBaseFontElement =
    { 0x3050F202, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLBaseFontElement =
    { 0x3050F504, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLBaseFontElement =
    { 0x3050F282, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLUnknownElement =
    { 0x3050F209, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLUnknownElement =
    { 0x3050F539, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLUnknownElement =
    { 0x3050F268, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IOmHistory =
    { 0xFECEAAA2, 0x8405, 0x11CF, { 0x8B, 0xA1, 0x00, 0xAA, 0x00, 0x47, 0x6D, 0xA6 } };
EXTERN_C const IID      IID_IHTMLMimeTypesCollection =
    { 0x3050F3FC, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLPluginsCollection =
    { 0x3050F3FD, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLOpsProfile =
    { 0x3050F401, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IOmNavigator =
    { 0xFECEAAA5, 0x8405, 0x11CF, { 0x8B, 0xA1, 0x00, 0xAA, 0x00, 0x47, 0x6D, 0xA6 } };
EXTERN_C const IID      IID_IHTMLLocation =
    { 0x163BB1E0, 0x6E00, 0x11CF, { 0x83, 0x7A, 0x48, 0xDC, 0x04, 0xC1, 0x00, 0x00 } };
EXTERN_C const CLSID    CLSID_HTMLHistory =
    { 0xFECEAAA3, 0x8405, 0x11CF, { 0x8B, 0xA1, 0x00, 0xAA, 0x00, 0x47, 0x6D, 0xA6 } };
EXTERN_C const CLSID    CLSID_CMimeTypes =
    { 0x3050F3FE, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_CPlugins =
    { 0x3050F3FF, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_COpsProfile =
    { 0x3050F402, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLNavigator =
    { 0xFECEAAA6, 0x8405, 0x11CF, { 0x8B, 0xA1, 0x00, 0xAA, 0x00, 0x47, 0x6D, 0xA6 } };
EXTERN_C const CLSID    CLSID_HTMLLocation =
    { 0x163BB1E1, 0x6E00, 0x11CF, { 0x83, 0x7A, 0x48, 0xDC, 0x04, 0xC1, 0x00, 0x00 } };
EXTERN_C const IID      IID_IHTMLBookmarkCollection =
    { 0x3050F4CE, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDataTransfer =
    { 0x3050F4B3, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLEventObj =
    { 0x3050F32D, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLEventObj2 =
    { 0x3050F48B, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLEventObj3 =
    { 0x3050F680, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLEventObj4 =
    { 0x3050F814, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispCEventObj =
    { 0x3050F558, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_CEventObj =
    { 0x3050F48A, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLWindowEvents2 =
    { 0x3050F625, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLWindowEvents =
    { 0x96A0A4E0, 0xD062, 0x11CF, { 0x94, 0xB6, 0x00, 0xAA, 0x00, 0x60, 0x27, 0x5C } };
EXTERN_C const IID      IID_IHTMLFramesCollection2 =
    { 0x332C4426, 0x26CB, 0x11D0, { 0xB4, 0x83, 0x00, 0xC0, 0x4F, 0xD9, 0x01, 0x19 } };
EXTERN_C const IID      IID_IHTMLScreen =
    { 0x3050F35C, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLScreen2 =
    { 0x3050F84A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLWindow2 =
    { 0x332C4427, 0x26CB, 0x11D0, { 0xB4, 0x83, 0x00, 0xC0, 0x4F, 0xD9, 0x01, 0x19 } };
EXTERN_C const IID      IID_IHTMLWindow3 =
    { 0x3050F4AE, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLWindow4 =
    { 0x3050F6CF, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLWindow5 =
    { 0x3051040E, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_FramesCollection =
    { 0x3050F7F6, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLScreen =
    { 0x3050F591, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLScreen =
    { 0x3050F35D, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLWindow2 =
    { 0x3050F55D, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLWindow2 =
    { 0xD48A6EC6, 0x6A4A, 0x11CF, { 0x94, 0xA7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };
EXTERN_C const IID      DIID_DispHTMLWindowProxy =
    { 0x3050F55E, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLWindowProxy =
    { 0x3050F391, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLDocumentEvents2 =
    { 0x3050F613, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLDocumentEvents =
    { 0x3050F260, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDocument =
    { 0x626FC520, 0xA41E, 0x11CF, { 0xA7, 0x31, 0x00, 0xA0, 0xC9, 0x08, 0x26, 0x37 } };
EXTERN_C const IID      IID_IHTMLDocument2 =
    { 0x332C4425, 0x26CB, 0x11D0, { 0xB4, 0x83, 0x00, 0xC0, 0x4F, 0xD9, 0x01, 0x19 } };
EXTERN_C const IID      IID_IHTMLDocument3 =
    { 0x3050F485, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDocument4 =
    { 0x3050F69A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDocument5 =
    { 0x3050F80C, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDocument =
    { 0x3050F55F, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDocument =
    { 0x25336920, 0x03F9, 0x11CF, { 0x8F, 0xD0, 0x00, 0xAA, 0x00, 0x68, 0x6F, 0x13 } };
EXTERN_C const IID      DIID_DWebBridgeEvents =
    { 0xA6D897FF, 0x0A95, 0x11D1, { 0xB0, 0xBA, 0x00, 0x60, 0x08, 0x16, 0x6E, 0x11 } };
EXTERN_C const IID      IID_IWebBridge =
    { 0xAE24FDAD, 0x03C6, 0x11D1, { 0x8B, 0x76, 0x00, 0x80, 0xC7, 0x44, 0xF3, 0x89 } };
EXTERN_C const IID      IID_IWBScriptControl =
    { 0xA5170870, 0x0CF8, 0x11D1, { 0x8B, 0x91, 0x00, 0x80, 0xC7, 0x44, 0xF3, 0x89 } };
EXTERN_C const CLSID    CLSID_Scriptlet =
    { 0xAE24FDAE, 0x03C6, 0x11D1, { 0x8B, 0x76, 0x00, 0x80, 0xC7, 0x44, 0xF3, 0x89 } };
EXTERN_C const IID      IID_IHTMLEmbedElement =
    { 0x3050F25F, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLEmbed =
    { 0x3050F52E, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLEmbed =
    { 0x3050F25D, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLMapEvents2 =
    { 0x3050F61E, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLMapEvents =
    { 0x3050F3BA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAreasCollection =
    { 0x3050F383, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAreasCollection2 =
    { 0x3050F5EC, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAreasCollection3 =
    { 0x3050F837, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLMapElement =
    { 0x3050F266, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLAreasCollection =
    { 0x3050F56A, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLAreasCollection =
    { 0x3050F4CA, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLMapElement =
    { 0x3050F526, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLMapElement =
    { 0x3050F271, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLAreaEvents2 =
    { 0x3050F611, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLAreaEvents =
    { 0x3050F366, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAreaElement =
    { 0x3050F265, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLAreaElement =
    { 0x3050F503, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLAreaElement =
    { 0x3050F283, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableCaption =
    { 0x3050F2EB, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLTableCaption =
    { 0x3050F508, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLTableCaption =
    { 0x3050F2EC, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLCommentElement =
    { 0x3050F20C, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLCommentElement2 =
    { 0x3050F813, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLCommentElement =
    { 0x3050F50A, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLCommentElement =
    { 0x3050F317, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLPhraseElement =
    { 0x3050F20A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLPhraseElement2 =
    { 0x3050F824, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLSpanElement =
    { 0x3050F3F3, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLPhraseElement =
    { 0x3050F52D, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLPhraseElement =
    { 0x3050F26E, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLSpanElement =
    { 0x3050F528, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLSpanElement =
    { 0x3050F3F5, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLTableEvents2 =
    { 0x3050F623, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLTableEvents =
    { 0x3050F407, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTable =
    { 0x3050F21E, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTable2 =
    { 0x3050F4AD, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTable3 =
    { 0x3050F829, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableCol =
    { 0x3050F23A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableCol2 =
    { 0x3050F82A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableSection =
    { 0x3050F23B, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableSection2 =
    { 0x3050F5C7, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableSection3 =
    { 0x3050F82B, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableRow =
    { 0x3050F23C, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableRow2 =
    { 0x3050F4A1, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableRow3 =
    { 0x3050F82C, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableRowMetrics =
    { 0x3050F413, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableCell =
    { 0x3050F23D, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLTableCell2 =
    { 0x3050F82D, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLTable =
    { 0x3050F532, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLTable =
    { 0x3050F26B, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLTableCol =
    { 0x3050F533, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLTableCol =
    { 0x3050F26C, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLTableSection =
    { 0x3050F534, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLTableSection =
    { 0x3050F2E9, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLTableRow =
    { 0x3050F535, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLTableRow =
    { 0x3050F26D, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLTableCell =
    { 0x3050F536, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLTableCell =
    { 0x3050F246, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLScriptEvents2 =
    { 0x3050F621, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLScriptEvents =
    { 0x3050F3E2, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLScriptElement =
    { 0x3050F28B, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLScriptElement2 =
    { 0x3050F828, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLScriptElement =
    { 0x3050F530, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLScriptElement =
    { 0x3050F28C, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLNoShowElement =
    { 0x3050F38A, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLNoShowElement =
    { 0x3050F528, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLNoShowElement =
    { 0x3050F38B, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLObjectElementEvents2 =
    { 0x3050F620, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLObjectElementEvents =
    { 0x3050F3C4, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLObjectElement =
    { 0x3050F24F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLObjectElement2 =
    { 0x3050F4CD, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLObjectElement3 =
    { 0x3050F827, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLParamElement =
    { 0x3050F83D, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLObjectElement =
    { 0x3050F529, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLObjectElement =
    { 0x3050F24E, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLParamElement =
    { 0x3050F590, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLParamElement =
    { 0x3050F83E, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLFrameSiteEvents2 =
    { 0x3050F7FF, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLFrameSiteEvents =
    { 0x3050F800, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFrameBase =
    { 0x3050F311, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFrameBase2 =
    { 0x3050F6DB, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFrameBase3 =
    { 0x3050F82E, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLFrameBase =
    { 0x3050F541, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLFrameBase =
    { 0x3050F312, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFrameElement =
    { 0x3050F313, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFrameElement2 =
    { 0x3050F7F5, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLFrameElement =
    { 0x3050F513, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLFrameElement =
    { 0x3050F314, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLIFrameElement =
    { 0x3050F315, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLIFrameElement2 =
    { 0x3050F4E6, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLIFrame =
    { 0x3050F51B, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLIFrame =
    { 0x3050F316, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDivPosition =
    { 0x3050F212, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFieldSetElement =
    { 0x3050F3E7, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFieldSetElement2 =
    { 0x3050F833, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLLegendElement =
    { 0x3050F3EA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLLegendElement2 =
    { 0x3050F834, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLDivPosition =
    { 0x3050F50F, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDivPosition =
    { 0x3050F249, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLFieldSetElement =
    { 0x3050F545, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLFieldSetElement =
    { 0x3050F3E8, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLLegendElement =
    { 0x3050F546, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLLegendElement =
    { 0x3050F3E9, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLSpanFlow =
    { 0x3050F3E5, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLSpanFlow =
    { 0x3050F544, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLSpanFlow =
    { 0x3050F3E6, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFrameSetElement =
    { 0x3050F319, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFrameSetElement2 =
    { 0x3050F5C6, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLFrameSetSite =
    { 0x3050F514, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLFrameSetSite =
    { 0x3050F31A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLBGsound =
    { 0x3050F369, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLBGsound =
    { 0x3050F53C, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLBGsound =
    { 0x3050F370, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFontNamesCollection =
    { 0x3050F376, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLFontSizesCollection =
    { 0x3050F377, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLOptionsHolder =
    { 0x3050F378, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLStyleElementEvents2 =
    { 0x3050F615, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLStyleElementEvents =
    { 0x3050F3CB, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyleElement =
    { 0x3050F375, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLStyleElement =
    { 0x3050F511, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLStyleElement =
    { 0x3050F37D, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLStyleFontFace =
    { 0x3050F3D5, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLStyleFontFace =
    { 0x3050F3D4, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ICSSFilterSite =
    { 0x3050F3ED, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ICSSFilter =
    { 0x3050F3EC, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ISecureUrlHost =
    { 0xC81984C4, 0x74C8, 0x11D2, { 0xBA, 0xA9, 0x00, 0xC0, 0x4F, 0xC2, 0x04, 0x0E } };
EXTERN_C const IID      IID_IMarkupServices =
    { 0x3050F4A0, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IMarkupServices2 =
    { 0x3050F682, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IMarkupContainer =
    { 0x3050F5F9, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IMarkupContainer2 =
    { 0x3050F648, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLChangePlayback =
    { 0x3050F6E0, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IMarkupPointer =
    { 0x3050F49F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IMarkupPointer2 =
    { 0x3050F675, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IMarkupTextFrags =
    { 0x3050F5FA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLChangeLog =
    { 0x3050F649, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLChangeSink =
    { 0x3050F64A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IXMLGenericParse =
    { 0xE4E23071, 0x4D07, 0x11D2, { 0xAE, 0x76, 0x00, 0x80, 0xC7, 0x3B, 0xC1, 0x99 } };
EXTERN_C const IID      IID_IHTMLEditHost =
    { 0x3050F6A0, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLEditHost2 =
    { 0x3050F848, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ISegment =
    { 0x3050F683, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ISegmentListIterator =
    { 0x3050F692, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ISegmentList =
    { 0x3050F605, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ISequenceNumber =
    { 0x3050F6C1, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IIMEServices =
    { 0x3050F6CA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLCaret =
    { 0x3050F604, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHighlightRenderingServices =
    { 0x3050F606, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ISelectionServicesListener =
    { 0x3050F699, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ISelectionServices =
    { 0x3050F684, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementSegment =
    { 0x3050F68F, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHighlightSegment =
    { 0x3050F690, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLEditDesigner =
    { 0x3050F662, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLEditServices =
    { 0x3050F663, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLEditServices2 =
    { 0x3050F812, MSHTML_SUFFIX };
EXTERN_C const IID      IID_ILineInfo =
    { 0x3050F7E2, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLComputedStyle =
    { 0x3050F6C3, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IDisplayPointer =
    { 0x3050F69E, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IDisplayServices =
    { 0x3050F69D, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHtmlDlgSafeHelper =
    { 0x3050F81A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IBlockFormats =
    { 0x3050F830, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IFontNames =
    { 0x3050F839, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HtmlDlgSafeHelper =
    { 0x3050F819, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_BlockFormats =
    { 0x3050F831, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_FontNames =
    { 0x3050F83A, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_HTMLNamespaceEvents =
    { 0x3050F6BD, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLNamespace =
    { 0x3050F6BB, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLNamespaceCollection =
    { 0x3050F6B8, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLNamespace =
    { 0x3050F6BC, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLNamespaceCollection =
    { 0x3050F6B9, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLPainter =
    { 0x3050F6A6, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLPainterEventInfo =
    { 0x3050F6DF, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLPainterOverlay =
    { 0x3050F7E3, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLPaintSite =
    { 0x3050F6A7, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLIPrintCollection =
    { 0x3050F6B5, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IEnumPrivacyRecords =
    { 0x3050F844, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IWPCBlockedUrls =
    { 0x30510413, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDialog =
    { 0x3050F216, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDialog2 =
    { 0x3050F5E0, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLDialog3 =
    { 0x3050F388, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLModelessInit =
    { 0x3050F5E4, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_ThreadDialogProcParam =
    { 0x3050F5EB, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLDialog =
    { 0x3050F28A, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLPopup =
    { 0x3050F666, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLPopup =
    { 0x3050F589, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLPopup =
    { 0x3050F667, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAppBehavior =
    { 0x3050F5CA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAppBehavior2 =
    { 0x3050F5C9, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHTMLAppBehavior3 =
    { 0x3050F5CD, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispHTMLAppBehavior =
    { 0x3050F57C, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLAppBehavior =
    { 0x3050F5CB, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_OldHTMLDocument =
    { 0xD48A6EC9, 0x6A4A, 0x11CF, { 0x94, 0xA7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };
EXTERN_C const CLSID    CLSID_OldHTMLFormElement =
    { 0x0D04D285, 0x6BEC, 0x11CF, { 0x8B, 0x97, 0x00, 0xAA, 0x00, 0x47, 0x6D, 0xA6 } };
EXTERN_C const IID      DIID_DispIHTMLInputButtonElement =
    { 0x3050F51E, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLInputButtonElement =
    { 0x3050F2B4, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispIHTMLInputTextElement =
    { 0x3050F520, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLInputTextElement =
    { 0x3050F2AB, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispIHTMLInputFileElement =
    { 0x3050F542, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLInputFileElement =
    { 0x3050F2AE, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispIHTMLOptionButtonElement =
    { 0x3050F509, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLOptionButtonElement =
    { 0x3050F2BE, MSHTML_SUFFIX };
EXTERN_C const IID      DIID_DispIHTMLInputImage =
    { 0x3050F51D, MSHTML_SUFFIX };
EXTERN_C const CLSID    CLSID_HTMLInputImage =
    { 0x3050F2C4, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorFactory =
    { 0x3050F429, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementNamespace =
    { 0x3050F671, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementNamespaceTable =
    { 0x3050F670, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementNamespaceFactory =
    { 0x3050F672, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementNamespaceFactory2 =
    { 0x3050F805, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementNamespaceFactoryCallback =
    { 0x3050F7FD, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehavior =
    { 0x3050F425, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorSite =
    { 0x3050F427, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorSiteOM =
    { 0x3050F489, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorSiteOM2 =
    { 0x3050F659, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorRender =
    { 0x3050F4AA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorSiteRender =
    { 0x3050F4A7, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorCategory =
    { 0x3050F4ED, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorSiteCategory =
    { 0x3050F4EE, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorSubmit =
    { 0x3050F646, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorFocus =
    { 0x3050F6B6, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorLayout =
    { 0x3050F6BA, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorLayout2 =
    { 0x3050F846, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorSiteLayout =
    { 0x3050F6B7, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IElementBehaviorSiteLayout2 =
    { 0x3050F847, MSHTML_SUFFIX };
EXTERN_C const IID      IID_IHostBehaviorInit =
    { 0x3050F842, MSHTML_SUFFIX };

