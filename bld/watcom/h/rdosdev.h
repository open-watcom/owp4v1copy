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

void RdosHookState(__rdos_hook_state_callback *callb_proc);
 
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
    OsGate_allocate_debug_vm_linear  \
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

#pragma aux RdosHookState = \
    OsGate_hook_state \
    parm [es edi];

#ifdef __cplusplus
}
#endif

#pragma pack( pop )

#endif
