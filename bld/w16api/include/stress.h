#ifndef	__STRESS_H
#define	__STRESS_H

#ifndef	__WINDOWS_H
#include <windows.h>
#endif

#ifdef	__cplusplus
extern	"C" {
#endif

#pragma pack(push,1)

#define	EDS_WIN	1
#define	EDS_CUR	2
#define	EDS_TEMP	3

BOOL	WINAPI	AllocMem(DWORD);
void	WINAPI	FreeAllMem(void);
int	WINAPI	AllocFileHandles(int);
void	WINAPI	UnAllocFileHandles(void);
int	WINAPI	GetFreeFileHandles(void);
int	WINAPI	AllocDiskSpace(long,UINT);
void	WINAPI	UnAllocDiskSpace(UINT);
BOOL	WINAPI	AllocUserMem(UINT);
void	WINAPI	FreeAllUserMem(void);
BOOL	WINAPI	AllocGDIMem(UINT);
void	WINAPI	FreeAllGDIMem(void);

#pragma pack(pop)

#ifdef	__cplusplus
}
#endif

#endif
