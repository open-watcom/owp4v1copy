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
* Description:  Defines for Linux specific system calls. We maintain this
*               separately from the Linux kernel source code, so it may
*               need updating from time to time. We need to do this so that
*               the runtime library can be built on any platform without
*               needing the Linux kernel headers to be installed.
*
****************************************************************************/

#ifndef _SYSLINUX_H_INCLUDED
#define _SYSLINUX_H_INCLUDED

#ifndef __TYPES_H_INCLUDED
 #include <sys/types.h>
#endif

/*
 * Linux system call numbers
 */

#define SYS_exit                  1
#define SYS_fork                  2
#define SYS_read                  3
#define SYS_write                 4
#define SYS_open                  5
#define SYS_close                 6
#define SYS_waitpid               7
#define SYS_creat                 8
#define SYS_link                  9
#define SYS_unlink               10
#define SYS_execve               11
#define SYS_chdir                12
#define SYS_time                 13
#define SYS_mknod                14
#define SYS_chmod                15
#define SYS_lchown               16
#define SYS_break                17
#define SYS_oldstat              18
#define SYS_lseek                19
#define SYS_getpid               20
#define SYS_mount                21
#define SYS_umount               22
#define SYS_setuid               23
#define SYS_getuid               24
#define SYS_stime                25
#define SYS_ptrace               26
#define SYS_alarm                27
#define SYS_oldfstat             28
#define SYS_pause                29
#define SYS_utime                30
#define SYS_stty                 31
#define SYS_gtty                 32
#define SYS_access               33
#define SYS_nice                 34
#define SYS_ftime                35
#define SYS_sync                 36
#define SYS_kill                 37
#define SYS_rename               38
#define SYS_mkdir                39
#define SYS_rmdir                40
#define SYS_dup                  41
#define SYS_pipe                 42
#define SYS_times                43
#define SYS_prof                 44
#define SYS_brk                  45
#define SYS_setgid               46
#define SYS_getgid               47
#define SYS_signal               48
#define SYS_geteuid              49
#define SYS_getegid              50
#define SYS_acct                 51
#define SYS_umount2              52
#define SYS_lock                 53
#define SYS_ioctl                54
#define SYS_fcntl                55
#define SYS_mpx                  56
#define SYS_setpgid              57
#define SYS_ulimit               58
#define SYS_oldolduname          59
#define SYS_umask                60
#define SYS_chroot               61
#define SYS_ustat                62
#define SYS_dup2                 63
#define SYS_getppid              64
#define SYS_getpgrp              65
#define SYS_setsid               66
#define SYS_sigaction            67
#define SYS_sgetmask             68
#define SYS_ssetmask             69
#define SYS_setreuid             70
#define SYS_setregid             71
#define SYS_sigsuspend           72
#define SYS_sigpending           73
#define SYS_sethostname          74
#define SYS_setrlimit            75
#define SYS_getrlimit            76     /* Back compatible 2Gig limited rlimit */
#define SYS_getrusage            77
#define SYS_gettimeofday         78
#define SYS_settimeofday         79
#define SYS_getgroups            80
#define SYS_setgroups            81
#define SYS_select               82
#define SYS_symlink              83
#define SYS_oldlstat             84
#define SYS_readlink             85
#define SYS_uselib               86
#define SYS_swapon               87
#define SYS_reboot               88
#define SYS_readdir              89
#define SYS_mmap                 90
#define SYS_munmap               91
#define SYS_truncate             92
#define SYS_ftruncate            93
#define SYS_fchmod               94
#define SYS_fchown               95
#define SYS_getpriority          96
#define SYS_setpriority          97
#define SYS_profil               98
#define SYS_statfs               99
#define SYS_fstatfs             100
#define SYS_ioperm              101
#define SYS_socketcall          102
#define SYS_syslog              103
#define SYS_setitimer           104
#define SYS_getitimer           105
#define SYS_stat                106
#define SYS_lstat               107
#define SYS_fstat               108
#define SYS_olduname            109
#define SYS_iopl                110
#define SYS_vhangup             111
#define SYS_idle                112
#define SYS_vm86old             113
#define SYS_wait4               114
#define SYS_swapoff             115
#define SYS_sysinfo             116
#define SYS_ipc                 117
#define SYS_fsync               118
#define SYS_sigreturn           119
#define SYS_clone               120
#define SYS_setdomainname       121
#define SYS_uname               122
#define SYS_modify_ldt          123
#define SYS_adjtimex            124
#define SYS_mprotect            125
#define SYS_sigprocmask         126
#define SYS_create_module       127
#define SYS_init_module         128
#define SYS_delete_module       129
#define SYS_get_kernel_syms     130
#define SYS_quotactl            131
#define SYS_getpgid             132
#define SYS_fchdir              133
#define SYS_bdflush             134
#define SYS_sysfs               135
#define SYS_personality         136
#define SYS_afs_syscall         137 /* Syscall for Andrew File System */
#define SYS_setfsuid            138
#define SYS_setfsgid            139
#define SYS__llseek             140
#define SYS_getdents            141
#define SYS__newselect          142
#define SYS_flock               143
#define SYS_msync               144
#define SYS_readv               145
#define SYS_writev              146
#define SYS_getsid              147
#define SYS_fdatasync           148
#define SYS__sysctl             149
#define SYS_mlock               150
#define SYS_munlock             151
#define SYS_mlockall            152
#define SYS_munlockall          153
#define SYS_sched_setparam      154
#define SYS_sched_getparam      155
#define SYS_sched_setscheduler  156
#define SYS_sched_getscheduler  157
#define SYS_sched_yield         158
#define SYS_sched_get_priority_max      159
#define SYS_sched_get_priority_min      160
#define SYS_sched_rr_get_interval       161
#define SYS_nanosleep           162
#define SYS_mremap              163
#define SYS_setresuid           164
#define SYS_getresuid           165
#define SYS_vm86                166
#define SYS_query_module        167
#define SYS_poll                168
#define SYS_nfsservctl          169
#define SYS_setresgid           170
#define SYS_getresgid           171
#define SYS_prctl               172
#define SYS_rt_sigreturn        173
#define SYS_rt_sigaction        174
#define SYS_rt_sigprocmask      175
#define SYS_rt_sigpending       176
#define SYS_rt_sigtimedwait     177
#define SYS_rt_sigqueueinfo     178
#define SYS_rt_sigsuspend       179
#define SYS_pread               180
#define SYS_pwrite              181
#define SYS_chown               182
#define SYS_getcwd              183
#define SYS_capget              184
#define SYS_capset              185
#define SYS_sigaltstack         186
#define SYS_sendfile            187
#define SYS_getpmsg             188     /* some people actually want streams */
#define SYS_putpmsg             189     /* some people actually want streams */
#define SYS_vfork               190
#define SYS_ugetrlimit          191     /* SuS compliant getrlimit */
#define SYS_mmap2               192
#define SYS_truncate64          193
#define SYS_ftruncate64         194
#define SYS_stat64              195
#define SYS_lstat64             196
#define SYS_fstat64             197
#define SYS_lchown32            198
#define SYS_getuid32            199
#define SYS_getgid32            200
#define SYS_geteuid32           201
#define SYS_getegid32           202
#define SYS_setreuid32          203
#define SYS_setregid32          204
#define SYS_getgroups32         205
#define SYS_setgroups32         206
#define SYS_fchown32            207
#define SYS_setresuid32         208
#define SYS_getresuid32         209
#define SYS_setresgid32         210
#define SYS_getresgid32         211
#define SYS_chown32             212
#define SYS_setuid32            213
#define SYS_setgid32            214
#define SYS_setfsuid32          215
#define SYS_setfsgid32          216
#define SYS_pivot_root          217
#define SYS_mincore             218
#define SYS_madvise             219
#define SYS_madvise1            219     /* delete when C lib stub is removed */
#define SYS_getdents64          220
#define SYS_fcntl64             221
#define SYS_security            223     /* syscall for security modules */
#define SYS_gettid              224
#define SYS_readahead           225
#define SYS_setxattr            226
#define SYS_lsetxattr           227
#define SYS_fsetxattr           228
#define SYS_getxattr            229
#define SYS_lgetxattr           230
#define SYS_fgetxattr           231
#define SYS_listxattr           232
#define SYS_llistxattr          233
#define SYS_flistxattr          234
#define SYS_removexattr         235
#define SYS_lremovexattr        236
#define SYS_fremovexattr        237
#define SYS_tkill               238
#define SYS_sendfile64          239
#define SYS_futex               240
#define SYS_sched_setaffinity   241
#define SYS_sched_getaffinity   242
#define SYS_set_thread_area     243

/*
 * Inline assembler for calling Linux system calls
 */

u_long sys_call0(u_long func);
#pragma aux sys_call0 =                         \
    "int    0x80"                               \
    parm [eax]                                  \
    value [eax];

u_long sys_call1(u_long func,u_long r_ebx);
#pragma aux sys_call1 =                         \
    "int    0x80"                               \
    parm [eax] [ebx]                            \
    value [eax];

u_long sys_call2(u_long func,u_long r_ebx,u_long r_ecx);
#pragma aux sys_call2 =                         \
    "int    0x80"                               \
    parm [eax] [ebx] [ecx]                      \
    value [eax];

u_long sys_call3(u_long func,u_long r_ebx,u_long r_ecx,u_long r_edx);
#pragma aux sys_call3 =                         \
    "int    0x80"                               \
    parm [eax] [ebx] [ecx] [edx]                \
    value [eax];

u_long sys_call4(u_long func,u_long r_ebx,u_long r_ecx,u_long r_edx,u_long r_esi);
#pragma aux sys_call4 =                         \
    "int    0x80"                               \
    parm [eax] [ebx] [ecx] [edx] [esi]          \
    value [eax];

u_long sys_call5(u_long func,u_long r_ebx,u_long r_ecx,u_long r_edx,u_long r_esi,u_long r_edi);
#pragma aux sys_call5 =                         \
    "int    0x80"                               \
    parm [eax] [ebx] [ecx] [edx] [esi] [edi]    \
    value [eax];

/*
 * Prototypes for Linux system call functions
 */

u_long  sys_brk(u_long brk);
long    sys_open(const char * filename, int flags, int mode);
long    sys_exit(int error_code);
int     sys_getdents(u_int fd, struct dirent *dirp, u_int count);
long    sys_socketcall(int call, u_long *args);

#if 0
struct itimerval;
struct msghdr;
struct pollfd;
struct rlimit;
struct fd_set;
struct sigaction;
struct sockaddr;
struct statfs;
struct timespec;
struct timeval;
struct timezone;
struct tms;
struct utimbuf;

/* fs/exec.c */
long sys_uselib(const char * library);

/* fs/buffer.c */
long sys_sync(void);
long sys_fsync(u_int fd);
long sys_fdatasync(u_int fd);

/* fs/fcntl.c */
long sys_dup(u_int fildes);
long sys_dup2(u_int oldfd, u_int newfd);
long sys_fcntl(u_int fd, u_int cmd,u_long arg);
long sys_fcntl64(u_int fd, u_int cmd,u_long arg);

/* fs/ioctl.c */
long sys_ioctl(u_int fd, u_int cmd, void *);

/* fs/namei.c */
long sys_link(const char * oldname, const char * newname);
long sys_mkdir(const char * pathname, int mode);
long sys_mknod(const char * filename, int mode, dev_t dev);
long sys_rename(const char * oldname, const char * newname);
long sys_rmdir(const char * pathname);
long sys_symlink(const char * oldname, const char * newname);
long sys_unlink(const char * pathname);

/* fs/namespace.c */
long sys_umount(char * name, int flags);
long sys_oldumount(char * name);
long sys_mount(char * dev_name, char * dir_name,char * type, u_long flags, void * data);
long sys_pivot_root(const char *new_root,const char *put_old);

/* fs/open.c */
long sys_access(const char * filename, int mode);
long sys_chdir(const char * filename);
long sys_fchdir(u_int fd);
long sys_chroot(const char * filename);
long sys_chmod(const char * filename, mode_t mode);
long sys_fchmod(u_int fd, mode_t mode);
long sys_chown(const char * filename, uid_t user,gid_t group);
long sys_lchown(const char * filename, uid_t user,gid_t group);
long sys_fchown(u_int fd, uid_t user, gid_t group);
long sys_close(u_int fd);
long sys_creat(const char * pathname, int mode);
long sys_open(const char * filename, int flags, int mode);
long sys_statfs(const char * path, struct statfs * buf);
long sys_fstatfs(u_int fd, struct statfs * buf);
long sys_ftruncate(u_int fd, u_long length);
long sys_ftruncate64(u_int fd, loff_t length);
long sys_truncate(const char * path, u_long length);
long sys_truncate64(const char * path, loff_t length);
long sys_utime(char * filename, struct utimbuf * times);
long sys_utimes(char * filename, struct timeval * utimes);
long sys_vhangup(void);

/* fs/read_write.c */
off_t	sys_lseek(u_int fd, off_t offset,u_int origin);
long     sys_llseek(u_int fd, u_long offset_high,u_long offset_low, loff_t * result, u_int origin);
ssize_t  sys_read(u_int fd, char * buf, size_t count);
ssize_t  sys_readv(u_long fd,const struct iovec * vector, u_long count);
ssize_t  sys_pread(u_int fd, char * buf,size_t count, loff_t pos);
ssize_t  sys_pwrite(u_int fd, const char * buf,size_t count, loff_t pos);
ssize_t  sys_write(u_int fd, const char * buf,size_t count);
ssize_t  sys_writev(u_long fd,const struct iovec * vector, u_long count);

/* fs/readdir.c */
int old_readdir(u_int fd, void * dirent,u_int count);

/* fs/select.c */
long sys_poll(struct pollfd * ufds, u_int nfds,long timeout);
int sys_select(int, fd_set *, fd_set *, fd_set *,struct timeval *);

/* fs/stat.c */
long sys_readlink(const char * path, char * buf,int bufsiz);

/* fs/super.c */
long sys_sysfs(int option, u_long arg1,u_long arg2);

/* kernel/acct.c */
long sys_acct(const char *name);

/* kernel/exit.c */
long sys_exit(int error_code);
long sys_waitpid(pid_t pid, u_int *stat_addr,int options);

/* kernel/itimer.c */
long sys_getitimer(int which, struct itimerval *value);
long sys_setitimer(int which, struct itimerval *value,struct itimerval *ovalue);

/* kernel/sched.c */
long sys_nice(int increment);
long sys_sched_setscheduler(pid_t pid, int policy,struct sched_param *param);
long sys_sched_setparam(pid_t pid,struct sched_param *param);
long sys_sched_getscheduler(pid_t pid);
long sys_sched_getparam(pid_t pid,struct sched_param *param);
long sys_sched_yield(void);
long sys_sched_get_priority_max(int policy);
long sys_sched_get_priority_min(int policy);
long sys_sched_rr_get_interval(pid_t pid,struct timespec *interval);

/* kernel/signal.c */
long sys_kill(int pid, int sig);
long sys_rt_sigaction(int sig, const struct sigaction *act,struct sigaction *oact, size_t sigsetsize);
long sys_rt_sigpending(sigset_t *set, size_t sigsetsize);
long sys_rt_sigprocmask(int how, sigset_t *set,sigset_t *oset, size_t sigsetsize);
long sys_rt_sigtimedwait(const sigset_t *uthese,siginfo_t *uinfo, const struct timespec *uts,size_t sigsetsize);
long sys_sigaltstack(const stack_t *uss, stack_t *uoss);
long sys_sigpending(old_sigset_t *set);
long sys_sigprocmask(int how, old_sigset_t *set,old_sigset_t *oset);
int sys_sigsuspend(int history0, int history1,old_sigset_t mask);

/* kernel/sys.c */
long sys_gethostname(char *name, int len);
long sys_sethostname(char *name, int len);
long sys_setdomainname(char *name, int len);
long sys_getrlimit(u_int resource,struct rlimit *rlim);
long sys_setsid(void);
long sys_getsid(pid_t pid);
long sys_getpgid(pid_t pid);
long sys_setpgid(pid_t pid, pid_t pgid);
long sys_getgroups(int gidsetsize, gid_t *grouplist);
long sys_setgroups(int gidsetsize, gid_t *grouplist);
long sys_setpriority(int which, int who, int niceval);
long sys_getpriority(int which, int who);
long sys_reboot(int magic1, int magic2, u_int cmd,void * arg);
long sys_setgid(gid_t gid);
long sys_setuid(uid_t uid);
long sys_times(struct tms * tbuf);
long sys_umask(int mask);
long sys_prctl(int option, u_long arg2,u_long arg3, u_long arg4, u_long arg5);
long sys_setreuid(uid_t ruid, uid_t euid);
long sys_setregid(gid_t rgid, gid_t egid);

/* kernel/time.c */
long sys_gettimeofday(struct timeval *tv,struct timezone *tz);
long sys_settimeofday(struct timeval *tv,struct timezone *tz);
long sys_stime(int * tptr);
long sys_time(int * tloc);

/* kernel/timer.c */
u_long sys_alarm(u_int seconds);
long sys_getpid(void);
long sys_getppid(void);
long sys_getuid(void);
long sys_geteuid(void);
long sys_getgid(void);
long sys_getegid(void);
long sys_gettid(void);
long sys_nanosleep(struct timespec *rqtp,struct timespec *rmtp);

/* mm/filemap.c */
ssize_t sys_sendfile(int out_fd, int in_fd, off_t *offset,size_t count);
long sys_msync(u_long start, size_t len, int flags);
long sys_madvise(u_long start, size_t len,int behavior);
long sys_mincore(u_long start, size_t len,unsigned char * vec);

/* mm/mmap.c */
u_long sys_brk(u_long brk);
long sys_munmap(u_long addr, size_t len);

/* mm/mprotect.c */
long sys_mprotect(u_long start, size_t len,u_long prot);

/* net/socket.c */
long sys_socket(int family, int type, int protocol);
long sys_socketpair(int family, int type,int protocol, int usockvec[2]);
long sys_bind(int fd, struct sockaddr *umyaddr,int addrlen);
long sys_listen(int fd, int backlog);
long sys_accept(int fd, struct sockaddr *upeer_sockaddr,int *upeer_addrlen);
long sys_connect(int fd, struct sockaddr *uservaddr,int addrlen);
long sys_getsockname(int fd, struct sockaddr *usockaddr,int *usockaddr_len);
long sys_getpeername(int fd, struct sockaddr *usockaddr,int *usockaddr_len);
long sys_sendto(int fd, void * buff, size_t len,unsigned flags, struct sockaddr *addr,int addr_len);
long sys_send(int fd, void * buff, size_t len,unsigned flags);
long sys_recvfrom(int fd, void * ubuf, size_t size,unsigned flags, struct sockaddr *addr,int *addr_len);
long sys_setsockopt(int fd, int level, int optname,char *optval, int optlen);
long sys_getsockopt(int fd, int level, int optname,char *optval, int *optlen);
long sys_shutdown(int fd, int how);
long sys_sendmsg(int fd, struct msghdr *msg,unsigned flags);
long sys_recvmsg(int fd, struct msghdr *msg,u_int flags);
#endif

#endif /* _ASM_I386_UNISTD_H_ */

