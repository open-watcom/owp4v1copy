/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2009-2011 The Open Watcom Contributors. All Rights Reserved.
*
*  ========================================================================
*
* Description:  RDOS kernel-mode header file
*
****************************************************************************/

#ifndef _RDOSDEV_H
#define _RDOSDEV_H

#pragma pack( push, 1 )

#ifdef __cplusplus
extern "C" {
#endif

#include "rdk.h"

// special user-mode gates

#define UserGate_free_mem 0x9a 2 0 0 0 2 0
#define UserGate_create_thread 0x9a 28 0 0 0 2 0

// callback pragmas

typedef void __far (__rdos_swap_callback)(char level);

#pragma aux __rdos_swap_callback "*" \
                    parm caller [al] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_thread_callback)(void *);

#pragma aux __rdos_thread_callback "*" \
                    parm caller [gs ebx] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_timer_callback)(int sel, unsigned long expire_msb, unsigned long expire_lsb);

#pragma aux __rdos_timer_callback "*" \
                    parm caller [ecx] [edx] [eax] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_wait_callback)(int wait_obj);

#pragma aux __rdos_wait_callback "*" \
                    parm caller [es] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_hook_callback)();

#pragma aux __rdos_hook_callback "*" \
                    parm caller \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_hook_state_callback)(int thread, char *buf);

#pragma aux __rdos_hook_state_callback "*" \
                    parm caller [ebx] [es edi] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_irq_callback)();

#pragma aux __rdos_irq_callback "*" \
                    parm caller \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_handle_delete_callback)(int handle);

#pragma aux __rdos_handle_delete_callback "*" \
                    parm caller [ebx] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_net_prot_callback)(int size, short int packet_type, void *ads, int selector);

#pragma aux __rdos_net_prot_callback "*" \
                    parm caller [ecx] [dx] [ds esi] [es] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_net_preview_callback)();

#pragma aux __rdos_net_preview_callback "*" \
                    parm caller \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef char* __far (__rdos_net_receive_callback)(int size, char *buf);

#pragma aux __rdos_net_receive_callback "*" \
                    parm caller [ecx] [es edi] \
                    value struct routine [es edi] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_net_remove_callback)(int size);

#pragma aux __rdos_net_remove_callback "*" \
                    parm caller [ecx] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef char* __far (__rdos_net_get_buf_callback)(int size);

#pragma aux __rdos_net_get_buf_callback "*" \
                    parm caller [ecx] \
                    value struct routine [es edi] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_net_send_callback)(int size, short int packet_type, void *dest_ads, int buf_sel);

#pragma aux __rdos_net_send_callback "*" \
                    parm caller [ecx] [dx] [ds esi] [es] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef char* __far (__rdos_net_address_callback)();

#pragma aux __rdos_net_address_callback "*" \
                    parm caller \
                    value struct routine [es edi] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_net_get_address_callback)(int buf_sel);

#pragma aux __rdos_net_get_address_callback "*" \
                    parm caller [es] \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_disc_assign_callback)();

#pragma aux __rdos_disc_assign_callback "*" \
                    parm caller \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_drive_assign_callback)(int disc_handle);

#pragma aux __rdos_drive_assign_callback "*" \
                    parm caller [ebx]  \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_drive_mount_callback)(int disc_handle);

#pragma aux __rdos_drive_mount_callback "*" \
                    parm caller [ebx]  \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_drive_erase_callback)(int disc_handle, int start_sector, int sector_count);

#pragma aux __rdos_drive_erase_callback "*" \
                    parm caller [ebx] [edx] [ecx]  \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

typedef void __far (__rdos_disc_change_callback)(int disc_sel);

#pragma aux __rdos_disc_change_callback "*" \
                    parm caller [fs]  \
                    value struct routine [eax] \
                    modify [eax ebx ecx edx esi edi]

// structures

struct TKernelSection
{
    long value;
    short int list;
};

struct TWaitHeader
{
    __rdos_wait_callback *init_proc;
    __rdos_wait_callback *abort_proc;
    __rdos_wait_callback *clear_proc;
    __rdos_wait_callback *idle_proc;
};

struct THandleHeader
{
    short int sign;
    short int handle;
};

struct TNetDriverTable
{
    __rdos_net_preview_callback *preview_proc;
    __rdos_net_receive_callback *receive_proc;
    __rdos_net_remove_callback *remove_proc;
    __rdos_net_get_buf_callback *get_buf_proc;
    __rdos_net_send_callback *send_proc;
    __rdos_net_address_callback *address_proc;
    __rdos_net_get_address_callback *get_address_proc;
};

struct TDiscSystemHeader
{
    __rdos_disc_assign_callback *disc_assign_proc;
    __rdos_drive_assign_callback *drive_assign1_proc;
    __rdos_drive_assign_callback *drive_assign2_proc;
    __rdos_drive_mount_callback *mount_proc;
    __rdos_drive_erase_callback *erase_proc;
};

// function definitions

int RdosIsValidOsGate(int gate);

void RdosReturnOk();
void RdosReturnFail();

void *RdosSelectorToPointer(int sel);
void *RdosSelectorOffsetToPointer(int sel, long offset);
int RdosPointerToSelector(void *ptr);
int RdosPointerToOffset(void *ptr);

int RdosAllocateGdt();
void RdosFreeGdt(int sel);

int RdosAllocateLdt();
void RdosFreeLdt(int sel);

int RdosGetSelectorBaseSize(int sel, long *base, long *limit);
void RdosCreateDataSelector16(int sel, long base, long limit);
void RdosCreateDataSelector32(int sel, long base, long limit);
void RdosCreateCodeSelector16(int sel, long base, long limit);
void RdosCreateCodeSelector32(int sel, long base, long limit);
void RdosCreateConformSelector16(int sel, long base, long limit);
void RdosCreateConformSelector32(int sel, long base, long limit);
void RdosCreateCallGateSelector(int sel, void __far (*dest)(), int count);
void RdosCreateIntGateSelector(int intnum, int dpl, void __far (*dest)());
void RdosCreateTrapGateSelector(int intnum, int dpl, void __far (*dest)());

long RdosGetPhysicalPage(long linear);
void RdosSetPhysicalPage(long linear, long page);

long RdosGetThreadPhysicalPage(int thread, long linear);
void RdosSetThreadPhysicalPage(int thread, long linear, long page);

int RdosAllocateBigGlobalSelector(long size);
int RdosAllocateSmallGlobalSelector(long size);

int RdosAllocateBigLocalSelector(long size);
int RdosAllocateSmallLocalSelector(long size);
int RdosAllocateSmallKernelSelector(long size);

void *RdosAllocateBigGlobalMem(long size);
void *RdosAllocateSmallGlobalMem(long size);

void *RdosAllocateBigLocalMem(long size);
void *RdosAllocateSmallLocalMem(long size);
void *RdosAllocateSmallKernelMem(long size);

void RdosFreeMem(int sel);

long RdosAllocateBigGlobalLinear(long size);
long RdosAllocateSmallGlobalLinear(long size);
long RdosAllocateLocalLinear(long size);
long RdosAllocateDebugLocalLinear(long size);
long RdosAllocateVmLinear(long size);

int RdosReserveLocalLinear(long linear, long size);

void RdosFreeLinear(long linear, long size);

long RdosUsedSmallGlobalMem();
long RdosUsedBigGlobalMem();

void *RdosAllocateFixedSystemMem(int sel, long size);
void *RdosAllocateFixedProcessMem(int sel, long size);

long RdosAllocatePhysical();
long RdosAllocateMultiplePhysical(int pages);
void RdosFreePhysical(long ads);

void RdosRegisterSwapProc(__rdos_swap_callback *callb_proc);

void RdosStartTimer(    int sel_id, 
                        unsigned long expire_msb, 
                        unsigned long expire_lsb,
                        __rdos_timer_callback *callb_proc,
                        int callb_sel);

void RdosStopTimer(     int sel_id);

long RdosGetApicId();
int RdosGetProcessor();
int RdosGetProcessorNum(int num);
void RdosSendNmi(int processor);
void RdosSendInt(int processor, int int_num);

void RdosClearSignal();
void RdosSignal(int thread);
void RdosWaitForSignal();
void RdosWaitForSignalWithTimeout(long msb, long lsb);

int RdosAddWait(int space_needed, int wait_handle, struct TWaitHeader *wait_table);
void RdosSignalWait(int wait_obj);

void RdosInitKernelSection(struct TKernelSection *section);
void RdosEnterKernelSection(struct TKernelSection *section);
void RdosLeaveKernelSection(struct TKernelSection *section);

void RdosLockScheduler();
void RdosUnlockScheduler();

void RdosCreateKernelThread(
            int prio, 
            int stack, 
            __rdos_thread_callback *startup, 
            const char *name,
            void *parm);

void RdosCreateKernelProcess(
            int prio, 
            int stack, 
            __rdos_thread_callback *startup, 
            const char *name,
            void *parm);

void RdosHookInitTasking(__rdos_hook_callback *callb_proc);
void RdosHookCreateProcess(__rdos_hook_callback *callb_proc);
void RdosHookTerminateProcess(__rdos_hook_callback *callb_proc);
void RdosHookCreateThread(__rdos_hook_callback *callb_proc);
void RdosHookTerminateThread(__rdos_hook_callback *callb_proc);

void RdosHookOpenApp(__rdos_hook_callback *callb_proc);
void RdosHookCloseApp(__rdos_hook_callback *callb_proc);

void RdosHookEnableFocus(__rdos_hook_callback *callb_proc);
void RdosHookLostFocus(__rdos_hook_callback *callb_proc);
void RdosHookGotFocus(__rdos_hook_callback *callb_proc);

void RdosHookState(__rdos_hook_state_callback *callb_proc);

void RdosSendEoi(int irq);
int RdosIsIrqFree(int irq);

void RdosRequestPrivateIrqHandler(int irq, __rdos_irq_callback *irq_proc);
void RdosReleasePrivateIrqHandler(int irq);
void RdosRequestSharedIrqHandler(int irq, __rdos_irq_callback *irq_proc);

void RdosSetupIrqDetect();
int RdosPollIrqDetect();

struct THandleHeader *RdosAllocateHandle(short int signature, int size);
void RdosFreeHandle(struct THandleHeader *handle_data);
struct THandleHeader *RdosDerefHandle(short int signature, int handle);
void RdosRegisterHandle(short int signature, __rdos_handle_delete_callback *delete_proc);

int RdosLockSysEnv();
void RdosUnlockSysEnv();
int RdosLockProcEnv();
void RdosUnlockProcEnv();

int RdosGetFocusThread();
char RdosGetThreadFocusKey(int thread);
long RdosAllocateFocusLinear(int size);
void RdosAllocateFixedFocusMem(int size, int local_sel, int focus_sel);

void RdosRegisterNetClass(char class_id, int ads_size, void *broadcast_ads);
int RdosRegisterNetProtocol(int ads_size, short int packet_type, void *my_ads, __rdos_net_prot_callback *packet_callb);
int RdosRegisterNetDriver(char class_id, int max_size, struct TNetDriverTable *table, const char *name); 

void RdosHookInitDisc(struct TDiscSystemHeader *disc_table);
int RdosInstallDisc(int disc_handle, int read_ahead, int *disc_nr);
void RdosRegisterDiscChange(__rdos_disc_change_callback *callb_proc);
void RdosStartDisc(int disc_sel);
void RdosStopDisc(int disc_sel);
void RdosSetDiscParam(  int disc_sel, 
                        int bytes_per_sector, 
                        int sectors_per_unit,
                        int units,
                        int bios_sectors_per_cyl,
                        int bios_heads); 

void RdosWaitForDiscRequest(int disc_sel);
long RdosGetDiscRequest(int disc_sel);
long RdosPollDiscRequest(int disc_sel);
void RdosDiscRequestCompleted(int disc_sel, long disc_handle);
long RdosNewDiscRequest(int disc_sel, int sector, int unit);
long RdosLockDiscRequest(int disc_sel, int sector, int unit);
void RdosModifyDiscRequest(long disc_handle);
void RdosUnlockDiscRequest(long disc_handle);
int RdosGetDiscRequestArray(int disc, int max_entries, long **req_array);

void RdosOpenDrive(int drive, int disc_nr, int start_sector, int sector_count);
void RdosCloseDrive(int drive);
void RdosFlushDrive(int drive);
int RdosGetDriveParam(int drive, int *read_ahead, int *sector_per_unit, int *units);
int RdosLockSector(int drive, int sector, void **data);
void RdosUnlockSector(int handle);
void RdosModifySector(int handle);
void RdosFlushSector(int handle);
int RdosNewSector(int drive, int sector, void **data);
int RdosReqSector(int drive, int sector, void *data);
int RdosDefineSector(int drive, int sector, void *data);
void RdosWaitForSector(int handle);

int RdosCreateDiscSeq(int max_entries);
void RdosModifySeqSector(int seq_handle, int handle);
void RdosPerformDiscSeq(int seq_handle);

void RdosEraseSectors(int drive, int start_sector, int sector_count);
void RdosResetDrive(int drive);

/* 32-bit compact memory model (device-drivers) */

// check carry flag, and set eax=0 if set and eax=1 if clear
#define CarryToBool 0x73 4 0x33 0xC0 0xEB 5 0xB8 1 0 0 0

// check carry flag, and set ebx=0 if set and ebx=bx if clear
#define ValidateHandle 0x73 2 0x33 0xDB 0xF 0xB7 0xDB

// check carry flag, and set eax=0 if set
#define ValidateEax 0x73 2 0x33 0xC0

// check carry flag, and set ecx=0 if set
#define ValidateEcx 0x73 2 0x33 0xC9

// check carry flag, and set edx=0 if set
#define ValidateEdx 0x73 2 0x33 0xD2

// check carry flag, and set esi=0 if set
#define ValidateEsi 0x73 2 0x33 0xF6

// check carry flag, and set edi=0 if set
#define ValidateEdi 0x73 2 0x33 0xFF

#pragma aux RdosIsValidOsGate = \
    OsGate_is_valid_osgate  \
    CarryToBool \
    parm [eax] \
    value [eax];

#pragma aux RdosReturnOk = \
    "clc" ;

#pragma aux RdosReturnFail = \
    "stc" ;

#pragma aux RdosSelectorToPointer = \
    "mov dx,bx" \
    "xor eax,eax" \    
    parm [ebx] \
    value [dx eax];

#pragma aux RdosSelectorOffsetToPointer = \
    "mov dx,bx" \
    parm [ebx] [eax] \
    value [dx eax];

#pragma aux RdosPointerToSelector = \
    "movzx ebx,dx" \
    parm [edx eax] \
    value [ebx];

#pragma aux RdosPointerToOffset = \
    parm [edx eax] \
    value [eax];

#pragma aux RdosAllocateGdt = \
    OsGate_allocate_gdt  \
    "movzx ebx,bx" \
    value [ebx];

#pragma aux RdosFreeGdt = \
    OsGate_free_gdt  \
    parm [ebx];

#pragma aux RdosAllocateLdt = \
    OsGate_allocate_ldt  \
    "movzx ebx,bx" \
    value [ebx];

#pragma aux RdosFreeLdt = \
    OsGate_free_ldt  \
    parm [ebx];

#pragma aux RdosGetSelectorBaseSize = \
    OsGate_get_selector_base_size  \
    CarryToBool \
    "mov fs:[esi],edx" \
    "mov es:[edi],ecx" \
    parm [ebx] [fs esi] [es edi] \
    value [eax];

#pragma aux RdosCreateDataSelector16 = \
    OsGate_create_data_sel16  \
    parm [ebx] [edx] [ecx];

#pragma aux RdosCreateDataSelector32 = \
    OsGate_create_data_sel32  \
    parm [ebx] [edx] [ecx];

#pragma aux RdosCreateCodeSelector16 = \
    OsGate_create_code_sel16  \
    parm [ebx] [edx] [ecx];

#pragma aux RdosCreateCodeSelector32 = \
    OsGate_create_code_sel32  \
    parm [ebx] [edx] [ecx];

#pragma aux RdosCreateConformSelector16 = \
    OsGate_create_conform_sel16  \
    parm [ebx] [edx] [ecx];

#pragma aux RdosCreateConformSelector32 = \
    OsGate_create_conform_sel32  \
    parm [ebx] [edx] [ecx];

#pragma aux RdosCreateCallGateSelector = \
    "push ds" \
    "push ax" \
    "mov ax,cs" \
    "mov ds,ax" \
    OsGate_create_call_gate_sel32  \
    "pop ax" \
    "pop ds" \
    parm [ebx] [esi] [ecx];

#pragma aux RdosCreateCallGateSelector = \
    "push ds" \
    "push ax" \
    "mov ax,cs" \
    "mov ds,ax" \
    OsGate_create_call_gate_sel32  \
    "pop ax" \
    "pop ds" \
    parm [eax] [ebx] [esi];

#pragma aux RdosCreateTrapGateSelector = \
    "push ds" \
    "push ax" \
    "mov ax,cs" \
    "mov ds,ax" \
    OsGate_create_call_gate_sel32  \
    "pop ax" \
    "pop ds" \
    parm [eax] [ebx] [esi];

#pragma aux RdosGetPhysicalPage = \
    OsGate_get_physical_page  \
    parm [edx] \
    value [eax];

#pragma aux RdosSetPhysicalPage = \
    OsGate_set_physical_page  \
    parm [edx] [eax];

#pragma aux RdosGetThreadPhysicalPage = \
    OsGate_get_thread_physical_page  \
    parm [ebx] [edx] \
    value [eax];

#pragma aux RdosSetThreadPhysicalPage = \
    OsGate_set_thread_physical_page  \
    parm [ebx] [edx] [eax];

#pragma aux RdosAllocateBigGlobalSelector = \
    "push es" \
    OsGate_allocate_global_mem  \
    "mov ebx,es" \
    "pop es" \
    parm [eax]  \
    value [ebx];

#pragma aux RdosAllocateSmallGlobalSelector = \
    "push es" \
    OsGate_allocate_small_global_mem  \
    "mov ebx,es" \
    "pop es" \
    parm [eax]  \
    value [ebx];

#pragma aux RdosAllocateBigLocalSelector = \
    "push es" \
    OsGate_allocate_big_mem  \
    "mov ebx,es" \
    "pop es" \
    parm [eax]  \
    value [ebx];

#pragma aux RdosAllocateSmallLocalSelector = \
    "push es" \
    OsGate_allocate_small_mem  \
    "mov ebx,es" \
    "pop es" \
    parm [eax]  \
    value [ebx];

#pragma aux RdosAllocateSmallKernelSelector = \
    "push es" \
    OsGate_allocate_small_kernel_mem  \
    "mov ebx,es" \
    "pop es" \
    parm [eax]  \
    value [ebx];

#pragma aux RdosAllocateBigGlobalMem = \
    "push es" \
    OsGate_allocate_global_mem  \
    "mov dx,es" \
    "xor eax,eax" \
    "pop es" \
    parm [eax]  \
    value [dx eax];

#pragma aux RdosAllocateSmallGlobalMem = \
    "push es" \
    OsGate_allocate_small_global_mem  \
    "mov dx,es" \
    "xor eax,eax" \
    "pop es" \
    parm [eax]  \
    value [dx eax];

#pragma aux RdosAllocateBigLocalMem = \
    "push es" \
    OsGate_allocate_big_mem  \
    "mov dx,es" \
    "xor eax,eax" \
    "pop es" \
    parm [eax]  \
    value [dx eax];

#pragma aux RdosAllocateSmallLocalMem = \
    "push es" \
    OsGate_allocate_small_mem  \
    "mov dx,es" \
    "xor eax,eax" \
    "pop es" \
    parm [eax]  \
    value [dx eax];

#pragma aux RdosAllocateSmallKernelMem = \
    "push es" \
    OsGate_allocate_small_kernel_mem  \
    "mov dx,es" \
    "xor eax,eax" \
    "pop es" \
    parm [eax]  \
    value [dx eax];

#pragma aux RdosFreeMem = \
    "push es" \
    "mov es,bx" \
    UserGate_free_mem  \
    "pop es" \
    parm [ebx];

#pragma aux RdosAllocateBigGlobalLinear = \
    OsGate_allocate_big_linear  \
    parm [eax]  \
    value [edx];

#pragma aux RdosAllocateSmallGlobalLinear = \
    OsGate_allocate_small_linear  \
    parm [eax]  \
    value [edx];

#pragma aux RdosAllocateLocalLinear = \
    OsGate_allocate_local_linear  \
    parm [eax]  \
    value [edx];

#pragma aux RdosAllocateDebugLocalLinear = \
    OsGate_allocate_debug_local_linear  \
    parm [eax]  \
    value [edx];

#pragma aux RdosAllocateVmLinear = \
    OsGate_allocate_vm_linear  \
    parm [eax]  \
    value [edx];

#pragma aux RdosReserveLocalLinear = \
    OsGate_reserve_local_linear  \
    CarryToBool \
    parm [edx] [eax] \
    value [eax];

#pragma aux RdosFreeLinear = \
    OsGate_free_linear  \
    parm [edx] [ecx];

#pragma aux RdosUsedBigGlobalMem = \
    OsGate_used_big_linear  \
    value [eax];

#pragma aux RdosUsedSmallGlobalMem = \
    OsGate_used_small_linear  \
    value [eax];

#pragma aux RdosAllocateFixedSystemMem = \
    "push es" \
    OsGate_allocate_fixed_system_mem  \
    "mov dx,bx" \
    "xor eax,eax" \
    "pop es" \
    parm [ebx] [eax]  \
    value [dx eax];

#pragma aux RdosAllocateFixedProcessMem = \
    "push es" \
    OsGate_allocate_fixed_process_mem  \
    "mov dx,bx" \
    "xor eax,eax" \
    "pop es" \
    parm [ebx] [eax]  \
    value [dx eax];

#pragma aux RdosAllocatePhysical = \
    OsGate_allocate_physical  \
    value [eax];

#pragma aux RdosAllocateMultiplePhysical = \
    OsGate_allocate_physical  \
    parm [ecx] \
    value [eax];

#pragma aux RdosFreePhysical = \
    OsGate_allocate_physical  \
    parm [eax];

#pragma aux RdosRegisterSwapProc = \
    OsGate_register_swap_proc  \
    parm [es edi];

#pragma aux RdosStartTimer = \
    OsGate_start_timer  \
    parm [ebx] [edx] [eax] [es edi] [ecx];

#pragma aux RdosStopTimer = \
    OsGate_stop_timer  \
    parm [ebx];

#pragma aux RdosGetApicId = \
    OsGate_get_apic_id  \
    value [edx];

#pragma aux RdosGetProcessor = \
    "push fs" \
    OsGate_get_processor  \
    "mov eax,fs" \
    "pop fs" \
    value [eax];

#pragma aux RdosGetProcessorNum = \
    "push fs" \
    OsGate_get_processor_num  \
    "mov eax,fs" \
    "pop fs" \
    parm [eax] \
    value [eax];

#pragma aux RdosSendNmi = \
    "push fs" \
    "mov fs,bx" \
    OsGate_send_nmi  \
    "pop fs" \
    parm [ebx];

#pragma aux RdosSendInt = \
    "push fs" \
    "mov fs,bx" \
    OsGate_send_int  \
    "pop fs" \
    parm [ebx] [eax];

#pragma aux RdosLockScheduler = \
    OsGate_lock_task; 

#pragma aux RdosUnlockScheduler = \
    OsGate_unlock_task; 

#pragma aux RdosClearSignal = \
    OsGate_clear_signal; 

#pragma aux RdosSignal = \
    OsGate_signal \
    parm [ebx]; 

#pragma aux RdosWaitForSignal = \
    OsGate_wait_for_signal; 

#pragma aux RdosWaitForSignalWithTimeout = \
    OsGate_wait_for_signal_timeout \
    parm [edx] [eax]; 

#pragma aux RdosAddWait = \
    "push es" \
    OsGate_add_wait \
    "mov eax,es" \
    "pop es" \
    parm [eax] [ebx] [es edi] \
    value [eax];

#pragma aux RdosSignalWait = \
    "push es" \
    "mov es,eax" \
    OsGate_signal_wait \
    "pop es" \
    parm [eax];

#pragma aux RdosInitKernelSection = \
    "mov dword ptr es:[edi],0" \
    "mov word ptr es:[edi+4],0" \
    parm [es edi]; 

#pragma aux RdosEnterKernelSection = \
    " lock sub dword ptr es:[edi],1" \
    " jc short enter_done" \
    " push ds" \
    " push esi" \
    " mov esi,es" \
    " mov ds,esi" \
    " mov esi,edi" \
    " add esi,4" \
    OsGate_enter_section \
    " pop esi" \
    " pop ds" \
    "enter_done: " \
    parm [es edi]; 

#pragma aux RdosLeaveKernelSection = \
    " lock add dword ptr es:[edi],1" \
    " jc short leave_done" \
    " push ds" \
    " push esi" \
    " mov esi,es" \
    " mov ds,esi" \
    " mov esi,edi" \
    " add esi,4" \
    OsGate_leave_section \
    " pop esi" \
    " pop ds" \
    "leave_done: " \
    parm [es edi]; 

#pragma aux RdosCreateKernelThread = \
    "push ds" \
    "mov edx,fs" \
    "mov ds,edx" \
    "movzx eax,al" \
    UserGate_create_thread \
    "pop ds" \
    parm [eax] [ecx] [fs esi] [es edi] [gs ebx] \
    modify [edx];

#pragma aux RdosCreateKernelProcess = \
    "push ds" \
    "mov edx,fs" \
    "mov ds,edx" \
    "movzx eax,al" \
    OsGate_create_process \
    "pop ds" \
    parm [eax] [ecx] [fs esi] [es edi] [gs ebx] \
    modify [edx];

#pragma aux RdosHookInitTasking = \
    OsGate_hook_init_tasking \
    parm [es edi];

#pragma aux RdosHookCreateProcess = \
    OsGate_hook_create_process \
    parm [es edi];

#pragma aux RdosHookTerminateProcess = \
    OsGate_hook_terminate_process \
    parm [es edi];

#pragma aux RdosHookCreateThread = \
    OsGate_hook_create_thread \
    parm [es edi];

#pragma aux RdosHookTerminateThread = \
    OsGate_hook_terminate_thread \
    parm [es edi];

#pragma aux RdosHookOpenApp = \
    OsGate_hook_open_app \
    parm [es edi];

#pragma aux RdosHookCloseApp = \
    OsGate_hook_close_app \
    parm [es edi];

#pragma aux RdosHookEnableFocus = \
    OsGate_hook_enable_focus \
    parm [es edi];

#pragma aux RdosHookGotFocus = \
    OsGate_hook_got_focus \
    parm [es edi];

#pragma aux RdosHookLostFocus = \
    OsGate_hook_lost_focus \
    parm [es edi];

#pragma aux RdosHookState = \
    OsGate_hook_state \
    parm [es edi];

#pragma aux RdosSendEoi = \
    OsGate_send_eoi \
    parm [eax];

#pragma aux RdosIsIrqFree = \
    OsGate_is_irq_free \
    CarryToBool \    
    parm [eax] \
    value [eax];

#pragma aux RdosRequestPrivateIrqHandler = \
    OsGate_request_private_irq_handler \
    parm [eax] [es edi];

#pragma aux RdosReleasePrivateIrqHandler = \
    OsGate_release_private_irq_handler \
    parm [eax];

#pragma aux RdosRequestSharedIrqHandler = \
    OsGate_request_shared_irq_handler \
    parm [eax] [es edi];

#pragma aux RdosSetupIrqDetect = \
    OsGate_setup_irq_detect;

#pragma aux RdosPollIrqDetect = \
    OsGate_setup_irq_detect \
    value [eax];

#pragma aux RdosAllocateHandle = \
    "push ds" \
    OsGate_allocate_handle \
    "mov dx,ds" \
    "pop ds" \
    parm [ax] [ecx] \
    value [dx ebx];

#pragma aux RdosFreeHandle = \
    "push ds" \
    "mov ds,dx" \
    OsGate_free_handle \
    "pop ds" \
    parm [dx ebx];

#pragma aux RdosDerefHandle = \
    "push ds" \
    OsGate_allocate_handle \
    "mov dx,ds" \
    "pop ds" \
    parm [ax] [ebx] \
    value [dx ebx];

#pragma aux RdosRegisterHandle = \
    OsGate_register_handle \
    parm [ax] [es edi];

#pragma aux RdosLockSysEnv = \
    OsGate_lock_sys_env \
    value [ebx];

#pragma aux RdosUnlockSysEnv = \
    OsGate_unlock_sys_env;

#pragma aux RdosLockProcEnv = \
    OsGate_lock_proc_env \
    value [ebx];

#pragma aux RdosUnlockProcEnv = \
    OsGate_unlock_proc_env;

#pragma aux RdosGetFocusThread = \
    OsGate_get_focus_thread \
    "movzx eax,ax" \
    value [eax];

#pragma aux RdosGetThreadFocusKey = \
    OsGate_get_thread_focus_key \
    parm [ebx] \
    value [al];

#pragma aux RdosAllocateFocusLinear = \
    OsGate_allocate_focus_linear \
    parm [eax] \
    value [edx];

#pragma aux RdosAllocateFixedFocusMem = \
    "push es" \
    OsGate_allocate_fixed_focus_mem \
    "pop es" \
    parm [eax] [ebx] [edx];

#pragma aux RdosRegisterNetClass = \
    "push ds" \
    "mov ds,dx" \
    OsGate_register_net_class \
    "pop ds" \
    parm [al] [ecx] [dx esi];

#pragma aux RdosRegisterNetProtocol = \
    "push ds" \
    "mov ds,bx" \
    OsGate_register_net_protocol \
    "pop ds" \
    parm [ecx] [dx] [bx esi] [es edi] \
    value [ebx];

#pragma aux RdosRegisterNetDriver = \
    "push ds" \
    "mov ds,dx" \
    OsGate_register_net_protocol \
    "pop ds" \
    parm [al] [ecx] [dx esi] [es edi] \
    value [ebx];

#pragma aux RdosHookInitDisc = \
    OsGate_hook_init_disc \
    parm [es edi];

#pragma aux RdosInstallDisc = \
    OsGate_install_disc \
    "movzx eax,al" \
    "mov es:[edi],eax" \
    "movzx ebx,bx" \
    parm [ebx] [ecx] [es edi] \
    value [ebx] \
    modify [eax];

#pragma aux RdosRegisterDiscChange = \
    OsGate_register_disc_change \
    parm [es edi];

#pragma aux RdosStartDisc = \
    OsGate_start_disc \
    parm [ebx];

#pragma aux RdosStopDisc = \
    OsGate_stop_disc \
    parm [ebx];

#pragma aux RdosSetDiscParam = \
    OsGate_set_disc_param \
    parm [ebx] [ecx] [eax] [edx] [esi] [edi];

#pragma aux RdosWaitForDiscRequest = \
    OsGate_wait_for_disc_request \
    parm [ebx];

#pragma aux RdosGetDiscRequest = \
    OsGate_get_disc_request \
    parm [ebx] \
    value [edi];

#pragma aux RdosPollDiscRequest = \
    OsGate_poll_disc_request \
    parm [ebx] \
    value [edi];

#pragma aux RdosDiscRequestCompleted = \
    OsGate_disc_request_completed \
    parm [ebx] [edi];

#pragma aux RdosNewDiscRequest = \
    OsGate_new_disc_request \
    parm [ebx] [eax] [edx] \
    value [edi];

#pragma aux RdosLockDiscRequest = \
    OsGate_lock_disc_request \
    parm [ebx] [eax] [edx] \
    value [edi];

#pragma aux RdosModifyDiscRequest = \
    OsGate_modify_disc_request \
    parm [edi];

#pragma aux RdosUnlockDiscRequest = \
    OsGate_unlock_disc_request \
    parm [edi];

#pragma aux RdosGetDiscRequestArray = \
    OsGate_get_disc_request_array \
    "mov es:[edi],esi" \
    "mov esi,0x20" \
    "mov es:[edi+4],esi" \
    parm [ebx] [ecx] [es edi] \
    value [ecx] \
    modify [esi];

#pragma aux RdosOpenDrive = \
    "mov ah,bl" \
    OsGate_open_drive \
    parm [eax] [ebx] [edx] [ecx] \
    modify [eax];

#pragma aux RdosCloseDrive = \
    OsGate_close_drive \
    parm [eax];

#pragma aux RdosFlushDrive = \
    OsGate_flush_drive \
    parm [eax];

#pragma aux RdosGetDriveParam = \
    "push edi" \
    "push esi" \
    OsGate_flush_drive \
    "mov gs:[edx],eax" \
    "movzx eax,si" \
    "pop esi" \
    "mov fs:[esi],eax" \
    "movzx eax,di" \
    "pop edi" \
    "mov es:[edi],eax" \
    parm [eax] [gs edx] [fs esi] [es edi] \
    value [ecx] \
    modify [eax];

#pragma aux RdosLockSector = \
    OsGate_lock_sector \
    "mov es:[edi],esi" \
    "mov esi,0x20" \
    "mov es:[edi+4],esi" \
    parm [eax] [edx] [es edi] \
    value [ebx] \
    modify [esi];

#pragma aux RdosUnlockSector = \
    OsGate_unlock_sector \
    parm [ebx];

#pragma aux RdosModifySector = \
    OsGate_modify_sector \
    parm [ebx];

#pragma aux RdosFlushSector = \
    OsGate_flush_sector \
    parm [ebx];

#pragma aux RdosNewSector = \
    OsGate_new_sector \
    "mov es:[edi],esi" \
    "mov esi,0x20" \
    "mov es:[edi+4],esi" \
    parm [eax] [edx] [es edi] \
    value [ebx] \
    modify [esi];

#pragma aux RdosReqSector = \
    OsGate_req_sector \
    parm [eax] [edx] [es esi] \
    value [ebx];

#pragma aux RdosDefineSector = \
    OsGate_define_sector \
    parm [eax] [edx] [es esi] \
    value [ebx];

#pragma aux RdosWaitForSector = \
    OsGate_wait_for_sector \
    parm [edx];

#pragma aux RdosCreateDiscSeq = \
    OsGate_create_disc_seq \
    "movzx eax,ax" \
    parm [ecx] \
    value [eax];

#pragma aux RdosModifySeqSector = \
    OsGate_modify_seq_sector \
    parm [eax] [ebx];

#pragma aux RdosPerformDiscSeq = \
    OsGate_perform_disc_seq \
    parm [eax];

#pragma aux RdosEraseSectors = \
    OsGate_erase_sectors \
    parm [eax] [edx] [ecx];

#pragma aux RdosResetDrive = \
    OsGate_reset_drive \
    parm [eax];

#ifdef __cplusplus
}
#endif

#pragma pack( pop )

#endif
