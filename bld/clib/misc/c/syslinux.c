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
* Description:  Module implementing linux system calls interface
*
****************************************************************************/

// TODO: I think this should all be split up into separate modules for
//       each of the functions implemented to ensure the runtime library
//       will static link as small as possible. KB.

#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/utsname.h>
#include "syslinux.h"

/* user-visible error numbers are in the range -1 - -124 */

#define __syscall_return(type, res)                     \
if ((u_long)(res) >= (u_long)(-125)) {                  \
    errno = -(res);                                     \
    res = (u_long)-1;                                   \
}                                                       \
return (type)(res);

#define __syscall_return_pointer(type, res)             \
if ((u_long)(res) >= (u_long)(-125)) {                  \
    errno = -(res);                                     \
    res = (u_long)0;                                    \
}                                                       \
return (type)(res);

u_long sys_brk(u_long brk)
{
    u_long newbrk;

    newbrk = sys_call1(SYS_brk,brk);
    if( newbrk < brk )
        return (u_long)-1;
    return newbrk;
}

long sys_exit(int error_code)
{
    return sys_call1(SYS_exit,error_code);
}

long sys_open(const char * filename, int flags, int mode)
{
    u_long res = sys_call3(SYS_open, (u_long)filename, flags, mode);
    __syscall_return(ssize_t,res);
}

int sys_getdents(u_int fd, struct dirent *dirp, u_int count)
{
    u_long res = sys_call3(SYS_getdents, fd, (u_long)dirp, count);
    __syscall_return(int,res);
}

long __socketcall(int call, u_long *args)
{
    u_long res = sys_call2(SYS_socketcall, call, (u_long)args);
    __syscall_return(long, res);
}

//
// It was passed into clib\direct\gtcwdlnx.c to implement full features
//
//_WCRTLINK char *getcwd( char *__buf, size_t __size )
//{
//    u_long res = sys_call2(SYS_getcwd, (u_long)__buf, __size);
//    __syscall_return_pointer(char *,res);
//}

_WCRTLINK int fcntl( int __fildes, int __cmd, ... )
{
    u_long      rest;
    va_list     args;
    u_long      res;

    va_start( args, __cmd );
    rest = va_arg( args, u_long );
    va_end( args );
    res = sys_call3(SYS_fcntl, (u_long)__fildes, (u_long)__cmd, rest);
    __syscall_return(int,res);
}

_WCRTLINK int ioctl( int __fd, unsigned long int __request, ... )
{
    u_long      argp;
    va_list     args;
    u_long      res;

    va_start( args, __request );
    argp = va_arg( args, u_long );
    va_end( args );
    res = sys_call3(SYS_ioctl, (u_long)__fd, (u_long)__request, argp);
    __syscall_return(int,res);
}

_WCRTLINK void (*signal(int signum, void (*sighandler)(int)))(int)
{
    u_long res = sys_call2(SYS_signal, (u_long)signum, (u_long)sighandler);
    __syscall_return(void (*)(int),res);
}

_WCRTLINK unsigned alarm( unsigned int __seconds )
{
    u_long res = sys_call1(SYS_alarm, __seconds);
    __syscall_return(unsigned,res);
}

_WCRTLINK int unlink( const char *filename )
{
    u_long res = sys_call1(SYS_unlink, (u_long)filename);
    __syscall_return(int,res);
}

_WCRTLINK time_t time( time_t *t )
{
    u_long res = sys_call1(SYS_time, (u_long)t);
    __syscall_return(int,res);
}

_WCRTLINK int gettimeofday( struct timeval *__tv, struct timezone *__tz )
{
    u_long res = sys_call2(SYS_gettimeofday, (u_long)__tv, (u_long)__tz);
    __syscall_return(int,res);
}

_WCRTLINK extern int settimeofday( const struct timeval *__tv, const struct timezone *__tz )
{
    u_long res = sys_call2(SYS_settimeofday, (u_long)__tv, (u_long)__tz);
    __syscall_return(int,res);
}

_WCRTLINK int access( const char *filename, int mode )
{
    u_long res = sys_call2(SYS_access, (u_long)filename, mode);
    __syscall_return(int,res);
}

_WCRTLINK int stat( const char *filename, struct stat * __buf )
{
    u_long res = sys_call2(SYS_stat, (u_long)filename, (u_long)__buf);
    __syscall_return(int,res);
}

_WCRTLINK int lstat( const char *filename, struct stat * __buf )
{
    u_long res = sys_call2(SYS_lstat, (u_long)filename, (u_long)__buf);
    __syscall_return(int,res);
}

_WCRTLINK int fstat( int __fildes, struct stat * __buf )
{
    u_long res = sys_call2(SYS_fstat, __fildes, (u_long)__buf);
    __syscall_return(int,res);
}

_WCRTLINK ssize_t read( int __fildes, void *__buf, size_t __len )
{
    u_long res = sys_call3(SYS_read, __fildes, (u_long)__buf, __len);
    __syscall_return(ssize_t,res);
}

_WCRTLINK ssize_t write( int __fildes, const void *__buf, size_t __len )
{
    u_long res = sys_call3(SYS_write, __fildes, (u_long)__buf, __len);
    __syscall_return(ssize_t,res);
}

_WCRTLINK off_t lseek( int __fildes, off_t __offset, int __whence )
{
    u_long res = sys_call3(SYS_lseek, __fildes, __offset, __whence);
    __syscall_return(off_t,res);
}

_WCRTLINK int _llseek( unsigned int __fildes, unsigned long __hi, unsigned long __lo, loff_t *__res, unsigned int __whence)
{
    u_long res = sys_call5(SYS__llseek, __fildes, __hi, __lo, (u_long)__res, __whence);
    __syscall_return(int,res);
}

_WCRTLINK int close( int __fildes )
{
    u_long res = sys_call1(SYS_close, __fildes);
    __syscall_return(int,res);
}

_WCRTLINK int fsync( int __fildes )
{
    u_long res = sys_call1(SYS_fsync, __fildes);
    __syscall_return(int,res);
}

_WCRTLINK int ioperm( unsigned long __from, unsigned long __num, int __turn_on )
{
    u_long res = sys_call3(SYS_ioperm, __from, __num, __turn_on);
    __syscall_return(int,res);
}

_WCRTLINK int iopl( int __level )
{
    u_long res = sys_call1(SYS_iopl, __level);
    __syscall_return(int,res);
}

_WCRTLINK int nice( int __val )
{
    u_long res = sys_call1(SYS_nice, __val);
    __syscall_return(int,res);
}

_WCRTLINK int select( int __width, fd_set * __readfds, fd_set * __writefds, fd_set * __exceptfds, struct timeval * __timeout )
{
    u_long res = sys_call5(SYS__newselect, __width, (u_long)__readfds, (u_long)__writefds, (u_long)__exceptfds, (u_long)__timeout);
    __syscall_return(int,res);
}

_WCRTLINK long ptrace( int request, int pid, void *addr, void *data )
{
    long    res,ret;

    /* Someone thought having ptrace() behave differently for the PEEK
     * requests was a clever idea. Instead of error code, ptrace()
     * returns the actual value and errno must be checked.
     */
    if( (request >= PTRACE_PEEKTEXT) && (request <= PTRACE_PEEKUSER) )
        *((long**)&data) = &ret;
    res = sys_call4( SYS_ptrace, request, pid, (u_long)addr, (u_long)data );
    if( res >= 0 ) {
        if( (request >= PTRACE_PEEKTEXT) && (request <= PTRACE_PEEKUSER) ) {
            return( ret );
        }
    }
    __syscall_return( long, res );
}

_WCRTLINK int readlink( const char *__path, char *__buf, size_t __bufsiz )
{
    u_long res = sys_call3(SYS_readlink, (u_long)__path, (u_long)__buf, __bufsiz);
    __syscall_return(int,res);
}

_WCRTLINK int link( const char *__path1, const char *__path2 )
{
    u_long res = sys_call2(SYS_link, (u_long)__path1, (u_long)__path2);
    __syscall_return(int,res);
}

_WCRTLINK int symlink( const char *__pname, const char *__slink )
{
    u_long res = sys_call2(SYS_symlink, (u_long)__pname, (u_long)__slink);
    __syscall_return(int,res);
}

_WCRTLINK int mknod( const char *__path, mode_t __mode, int __dev )
{
    u_long res = sys_call3(SYS_mknod, (u_long)__path, __mode, __dev);
    __syscall_return(int,res);
}


_WCRTLINK int chdir( const char *__path )
{
    u_long res = sys_call1(SYS_chdir, (u_long)__path);
    __syscall_return(int,res);
}

_WCRTLINK int rmdir( const char *__path )
{
    u_long res = sys_call1(SYS_rmdir, (u_long)__path);
    __syscall_return(int,res);
}

_WCRTLINK int mkdir( const char *__path, mode_t __mode )
{
    u_long res = sys_call2(SYS_mkdir, (u_long)__path, (u_long)__mode);
    __syscall_return(int,res);
}

_WCRTLINK int utime( const char *__path, const struct utimbuf * __times )
{
    u_long res = sys_call2(SYS_utime, (u_long)__path, (u_long)__times);
    __syscall_return(int,res);
}

_WCRTLINK pid_t fork( void )
{
    u_long res = sys_call0(SYS_fork);
    __syscall_return(pid_t,res);
}

_WCRTLINK int execve( const char *__path, const char *const __argv[],
                      const char *const __envp[] )
{
    u_long res = sys_call3(SYS_execve, (u_long)__path, (u_long)__argv,
                           (u_long)__envp);
    __syscall_return(int,res);
}

_WCRTLINK int sigaction(int __signum, const struct sigaction *__act,
                            struct sigaction *__oldact)
{
    /* given the sigaction layout we must use rt_sigaction
       this requires Linux kernel 2.2 or higher (probably not
       a big deal nowadays) */
    u_long res = sys_call4(SYS_rt_sigaction, __signum, (u_long)__act,
                           (u_long)__oldact, sizeof( sigset_t ) );
    __syscall_return(int,res);
}

_WCRTLINK int sigprocmask(int __how, const sigset_t *__set, sigset_t *__oldset)
{
    u_long res = sys_call3(SYS_sigprocmask, __how, (u_long)__set,
                           (u_long)__oldset);
    __syscall_return(int,res);
}

_WCRTLINK pid_t waitpid(pid_t __pid, __WAIT_STATUS __stat_loc, int __options)
{
    u_long res = sys_call3(SYS_waitpid, __pid, (u_long)__stat_loc,
                           (u_long)__options);
    __syscall_return(pid_t,res);
}

_WCRTLINK pid_t wait(__WAIT_STATUS __stat_loc)
{
    return waitpid( -1, __stat_loc, 0 );
}

_WCRTLINK int pipe( int __fildes[2] )
{
    u_long res = sys_call1(SYS_pipe, (u_long)__fildes);
    __syscall_return(int,res);
}

_WCRTLINK int rename( const char *__old, const char *__new )
{
    u_long res = sys_call2(SYS_rename, (u_long)__old, (u_long)__new);
    __syscall_return(int,res);
}

_WCRTLINK int truncate( const char *__path, off_t __length )
{
    u_long res = sys_call2(SYS_truncate, (u_long)__path, __length);
    __syscall_return(int,res);
}

_WCRTLINK int ftruncate( int __fd, off_t __length )
{
    u_long res = sys_call2(SYS_ftruncate, __fd, __length);
    __syscall_return(int,res);
}

_WCRTLINK int chmod( const char *__path, mode_t __mode )
{
    u_long res = sys_call2(SYS_chmod, (u_long)__path, __mode);
    __syscall_return(int,res);
}

_WCRTLINK int fchmod( int __fd, mode_t __mode )
{
    u_long res = sys_call2(SYS_fchmod, __fd, __mode);
    __syscall_return(int,res);
}

_WCRTLINK int chown( const char *__path, uid_t __owner, gid_t __group )
{
    u_long res = sys_call3(SYS_chown, (u_long)__path, __owner, __group);
    __syscall_return(int,res);
}

_WCRTLINK int fchown( int __fd, uid_t __owner, gid_t __group )
{
    u_long res = sys_call3(SYS_fchown, __fd, __owner, __group);
    __syscall_return(int,res);
}

_WCRTLINK mode_t umask( mode_t __cmask )
{
    u_long res = sys_call1(SYS_umask, __cmask);
    __syscall_return(mode_t,res);
}

_WCRTLINK pid_t getpid( void )
{
    u_long res = sys_call0(SYS_getpid);
    __syscall_return(pid_t,res);
}

_WCRTLINK pid_t getppid( void )
{
    u_long res = sys_call0(SYS_getppid);
    __syscall_return(pid_t,res);
}

_WCRTLINK int dup( int __oldfd )
{
    u_long res = sys_call1(SYS_dup, __oldfd);
    __syscall_return(int,res);
}

_WCRTLINK int dup2( int __oldfd, int __newfd )
{
    u_long res = sys_call2(SYS_dup2, __oldfd, __newfd);
    __syscall_return(int,res);
}

_WCRTLINK int kill( pid_t __pid, int __sig )
{
    u_long res = sys_call2(SYS_kill, __pid, __sig);
    __syscall_return(int,res);
}

_WCRTLINK int nanosleep( const struct timespec *__rqtp,
                         struct timespec *__rmtp )
{
    u_long res = sys_call2( SYS_nanosleep, (u_long)__rqtp, (u_long)__rmtp );
    __syscall_return(int,res);
}

_WCRTLINK clock_t times( struct tms *__buf )
{
    u_long res = sys_call1( SYS_times, (u_long)__buf );
    __syscall_return(clock_t,res);
}

_WCRTLINK pid_t  getpgrp( void )
{
    u_long res = sys_call0(SYS_getpgrp);
    __syscall_return(pid_t,res);
}

_WCRTLINK gid_t  getgid( void )
{
    u_long res = sys_call0(SYS_getgid);
    __syscall_return(gid_t,res);
}

_WCRTLINK gid_t  getegid( void )
{
    u_long res = sys_call0(SYS_getegid);
    __syscall_return(gid_t,res);
}

_WCRTLINK uid_t  getuid( void )
{
    u_long res = sys_call0(SYS_getuid);
    __syscall_return(uid_t,res);
}

_WCRTLINK uid_t  geteuid( void )
{
    u_long res = sys_call0(SYS_geteuid);
    __syscall_return(uid_t,res);
}

_WCRTLINK int    setegid( gid_t __newegroup )
{
    u_long res = sys_call2(SYS_setregid, -1, __newegroup);
    __syscall_return(int,res);
}

_WCRTLINK int    seteuid( uid_t __neweuserid )
{
    u_long res = sys_call2(SYS_setreuid, -1, __neweuserid);
    __syscall_return(int,res);
}

_WCRTLINK int    setgid( gid_t __newgroup )
{
    u_long res = sys_call1(SYS_setgid, __newgroup);
    __syscall_return(int,res);
}

_WCRTLINK int    setpgid( pid_t __pid, pid_t __pgroupid )
{
    u_long res = sys_call2(SYS_setpgid, __pid, __pgroupid);
    __syscall_return(int,res);
}

_WCRTLINK int    setregid(gid_t real, uid_t effective)
{
    u_long res = sys_call2(SYS_setregid, real, effective);
    __syscall_return(int,res);
}

_WCRTLINK int    setreuid(uid_t real, uid_t effective)
{
    u_long res = sys_call2(SYS_setreuid, real, effective);
    __syscall_return(int,res);
}

_WCRTLINK int    setuid( uid_t __newuserid )
{
    u_long res = sys_call1(SYS_setuid, __newuserid);
    __syscall_return(int,res);
}

_WCRTLINK void sync( void )
{
    (void) sys_call0(SYS_sync);
    return;
}

_WCRTLINK int    uname( struct utsname *__name )
{
    u_long res = sys_call1(SYS_uname, (u_long)__name);
    __syscall_return(int,res);
}

_WCRTLINK int mprotect( void *addr, size_t len, int prot )
{
    u_long res = sys_call3(SYS_mprotect, (u_long)addr, len, prot);
    __syscall_return(int,res);
}

_WCRTLINK int setsid( void )
{
    u_long res = sys_call0(SYS_setsid);
    __syscall_return(int,res);
}

_WCRTLINK int getitimer( int which, struct itimerval *value )
{
    u_long res = sys_call2( SYS_getitimer, which, (u_long)value );
    __syscall_return( int, res );
}

_WCRTLINK int setitimer( int which, const struct itimerval *value, struct itimerval *ovalue )
{
    u_long res = sys_call3( SYS_setitimer, which, (u_long)value, (u_long)ovalue );
    __syscall_return( int, res );
}
