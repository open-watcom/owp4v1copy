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
* Description:  Implementation of Floating-point environment for C99.
*
****************************************************************************/

#include "variety.h"
#include <fenv.h>

#pragma disable_message(200,202);

/***************************************************************************/

#ifdef __386__
_WCRTLINK const fenv_t __fenv_h_default_environment = {
    0x037f,   /*CW*/
    0,
    0,        /*SW*/
    0,
    0xffff,   /*TAG*/
    0,
    0,0,0,0,0,0,
};
#else
_WCRTLINK const fenv_t __fenv_h_default_environment = {
    0x037f,   /*CW*/
    0,        /*SW*/
    0xffff,   /*TAG*/
    0,0,0,0,
};
#endif

/****************************************************************************
* exception handling functions
****************************************************************************/

/*
The feclearexcept function clears the supported floating-point exceptions
represented by its argument.
*/
_WCRTLINK void feclearexcept(int excepts)
{
    fenv_t env;

    __asm fnstenv env;
    env.status_word &= ~excepts;
    __asm fldenv env;
}

/*
The fegetexceptflag function stores an implementation-defined 
representation of the states of the floating-point status flags
indicated by the argument excepts in the object pointed to by
the argument flagp.
*/
_WCRTLINK void fegetexceptflag(fexcept_t *flagp, int excepts)
{
    unsigned short status;

    __asm fnstsw [status];
    *flagp = excepts & status & FE_ALL_EXCEPT;
}

/*
The feraiseexcept function raises the supported floating-point
exceptions represented by its argument. The order in which
these floating-point exceptions are raised is unspecified, except
as stated in F.7.6. Whether the feraiseexcept function additionally
raises the inexact floating-point exception whenever it raises the
overflow or underflow floating-point exception is implementation-defined.
*/
_WCRTLINK void feraiseexcept(int excepts)
{
    fenv_t env;
    __asm fnstenv env
    env.status_word &= ~FE_ALL_EXCEPT;
    env.status_word |= excepts;
    __asm fldenv env;
    __asm fwait;
}

/*
_feenableexcept is an OW extension to unmask the given fpu exception
*/
_WCRTLINK void __feenableexcept(int excepts)
{
    unsigned short status;

    __asm   fnstcw  [status]
    status &= ~FE_ALL_EXCEPT;
    status |= excepts ^ FE_ALL_EXCEPT;
    __asm   fldcw   [status];
}

/*
_fedisableexcept is an OW extension to mask the given fpu exception
*/
_WCRTLINK void __fedisableexcept(int excepts)
{
    unsigned short status;

    __asm   fnstcw  [status]
    status &= ~FE_ALL_EXCEPT;
    status |= excepts;
    __asm   fldcw [status];
}

/*
The fesetexceptflag function sets the floating-point status flags
indicated by the argument excepts to the states stored in the object
pointed to by flagp. The value of *flagp shall have been set by a
previous call to fegetexceptflag whose second argument represented
at least those floating-point exceptions represented by the argument
excepts. This function does not raise floating-point exceptions,
but only sets the state of the flags.
*/
_WCRTLINK void fesetexceptflag(const fexcept_t *flagp, int excepts)
{
    fenv_t env;

    __asm fnstenv env;
    env.status_word &= ~FE_ALL_EXCEPT;
    env.status_word |= excepts;
    __asm fldenv env;
}

/*
The fetestexcept function determines which of a specified
subset of the floatingpoint exception flags are currently set.
The excepts argument specifies the floatingpoint status flags
to be queried.
Returns: the value of the bitwise OR of the floating-point
exception macros corresponding to the currently set floating-point
exceptions included in excepts.
*/
_WCRTLINK int fetestexcept(int excepts)
{
    fexcept_t curr;
    fegetexceptflag(&curr, excepts);
    return (int)curr;
}

/****************************************************************************
* rounding mode functions
****************************************************************************/

/*
The fegetround function gets the current rounding direction.
*/
_WCRTLINK int fegetround(void)
{
    unsigned short status;
    __asm fnstcw [status];
    return (int)(status & 0x0C00);
}

/*
The fesetround function sets the current rounding direction.
*/
_WCRTLINK int fesetround(int round)
{

    switch (round) {
        case FE_TONEAREST:
        case FE_DOWNWARD:
        case FE_TOWARDZERO:
        case FE_UPWARD:
            {
            unsigned short status;
            __asm fnstcw [status];
            status &= ~0x0C00;
            status |= (unsigned short)round;
            __asm fldcw [status];
            }
        return 0;
    }
    return 1;
}

/****************************************************************************
* FPU environment functions
****************************************************************************/

/*
The fegetenv function stores the current floating-point environment
in the object pointed to by envp.
*/
_WCRTLINK void fegetenv(fenv_t *envp)
{
    __asm fnstenv envp;
}

/*
The feholdexcept function saves the current floating-point environment
in the object pointed to by envp, clears the floating-point status
flags, and then installs a non-stop (continue on floating-point exceptions)
mode, if available, for all floating-point exceptions.
Returns: zero if and only if non-stop floating-point exception handling
was successfully installed.
*/
_WCRTLINK int feholdexcept(fenv_t *envp)
{
    unsigned short status;

    __asm fnstenv envp;
    status = envp->control_word | FE_ALL_EXCEPT;
    __asm fldcw [status];    
    return 0;
}

/*
The fesetenv function establishes the floating-point environment represented by the
object pointed to by envp. The argument envp shall point to an object set by a call to
fegetenv or feholdexcept, or equal a floating-point environment macro. Note that
fesetenv merely installs the state of the floating-point status flags represented through
its argument, and does not raise these floating-point exceptions.
*/
_WCRTLINK void fesetenv(const fenv_t *envp)
{
    __asm fldenv envp
}

/*
The feupdateenv function saves the currently raised floating-point exceptions in its
automatic storage, installs the floating-point environment represented by the object
pointed to by envp, and then raises the saved floating-point exceptions. The argument
envp shall point to an object set by a call to feholdexcept or fegetenv, or equal a
floating-point environment macro.
*/
_WCRTLINK void feupdateenv(const fenv_t *envp)
{
    unsigned short status;
    __asm fnstsw [status];
    fesetenv(envp);
    feraiseexcept(status & FE_ALL_EXCEPT);
}
