#ifndef _NSPAPI_H
#define _NSPAPI_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define NS_ALL		0

#define NS_SAP		1
#define NS_NDS		2
#define NS_PEER_BROWSE	3 

#define NS_TCPIP_LOCAL	10
#define NS_TCPIP_HOSTS	11
#define NS_DNS		12
#define NS_NETBT	13 
#define NS_WINS		14

#define NS_NBP		20

#define NS_MS		30
#define NS_STDA		31
#define NS_NTDS		32

#define NS_X500		40
#define NS_NIS		41
#define NS_NISPLUS	42

#define NS_WRQ		50

#define SERVICE_REGISTER	1
#define SERVICE_DEREGISTER	2
#define SERVICE_FLUSH		3
#define SERVICE_FLAG_HARD	0x00000002
    
#ifndef __BLOB_T_DEFINED /* also in wtypes.h */
#define __BLOB_T_DEFINED
typedef struct _BLOB {
	ULONG   cbSize;
	BYTE    *pBlobData;
} BLOB,*PBLOB,*LPBLOB;
#endif
typedef struct _SOCKET_ADDRESS {
	LPSOCKADDR lpSockaddr;
	INT iSockaddrLength;
} SOCKET_ADDRESS,*PSOCKET_ADDRESS,*LPSOCKET_ADDRESS;
typedef struct _CSADDR_INFO {
	SOCKET_ADDRESS LocalAddr;
	SOCKET_ADDRESS RemoteAddr;
	INT iSocketType;
	INT iProtocol;
} CSADDR_INFO,*PCSADDR_INFO,*LPCSADDR_INFO;
typedef struct _SERVICE_ADDRESS {
	DWORD dwAddressType;
	DWORD dwAddressFlags;
	DWORD dwAddressLength;
	DWORD dwPrincipalLength;
	BYTE *lpAddress;
	BYTE *lpPrincipal;
} SERVICE_ADDRESS;
typedef struct _SERVICE_ADDRESSES {
	DWORD dwAddressCount;
	SERVICE_ADDRESS Addresses[1];
} SERVICE_ADDRESSES, *PSERVICE_ADDRESSES, *LPSERVICE_ADDRESSES;
typedef struct _SERVICE_INFO {
	LPGUID lpServiceType;
	LPTSTR lpServiceName;
	LPTSTR lpComment;
	LPTSTR lpLocale;
	DWORD dwDisplayHint;
	DWORD dwVersion;
	DWORD dwTime;
	LPTSTR lpMachineName;
	LPSERVICE_ADDRESSES lpServiceAddress;
	BLOB ServiceSpecificInfo;
} SERVICE_INFO, *LPSERVICE_INFO;

typedef void *LPSERVICE_ASYNC_INFO;
INT WINAPI SetServiceA(DWORD,DWORD,DWORD,LPSERVICE_INFO,LPSERVICE_ASYNC_INFO,LPDWORD);
INT WINAPI SetServiceW(DWORD,DWORD,DWORD,LPSERVICE_INFO,LPSERVICE_ASYNC_INFO,LPDWORD);
INT WINAPI GetAddressByNameA(DWORD,LPGUID,LPTSTR,LPINT,DWORD,LPSERVICE_ASYNC_INFO,LPVOID,LPDWORD,LPTSTR,LPDWORD);
INT WINAPI GetAddressByNameW(DWORD,LPGUID,LPTSTR,LPINT,DWORD,LPSERVICE_ASYNC_INFO,LPVOID,LPDWORD,LPTSTR,LPDWORD);
#ifdef UNICODE
#define SetService SetServiceW
#define GetAddressByName GetAddressByNameW
#else
#define SetService SetServiceA
#define GetAddressByName GetAddressByNameA
#endif

#ifdef __cplusplus
}
#endif
#endif /* _NSPAPI_H */
