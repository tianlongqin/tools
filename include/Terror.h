#ifndef __TOOL_ERROR__19910612
#define __TOOL_ERROR__19910612

#define T_SUCCESS	0
#define T_EPERM		-1	/* Operation not permitted */
#define T_ENOENT	-2	/* No such file or directory */
#define T_ESRCH		-3	/* No such process */
#define T_EINTR		-4	/* Interrupted system call */
#define T_EIO		-5	/* I/O error */
#define T_ENXIO		-6	/* No such device or address */
#define T_E2BIG		-7	/* Argument list too long */
#define T_ENOEXEC	-8       /* Exec format error */
#define T_EBADF		-9	/* Bad file number */
#define T_ECHILD	-10	/* No child processes */
#define T_EAGAIN	-11	/* Try again */
#define T_ENOMEM	-12	/* Out of memory */
#define T_EACCES 	-13	/* Permission denied */
#define T_EFAULT 	-14	/* Bad address */
#define T_ENOTBLK 	-15	/* Block device required */
#define T_EBUSY 	-16	/* Device or resource busy */
#define T_EEXIST 	-17	/* File exists */
#define T_EXDEV 	-18	/* Cross-device link */
#define T_ENODEV 	-19	/* No such device */
#define T_ENOTDIR 	-20	/* Not a directory */
#define T_EISDIR 	-21	/* Is a directory */
#define T_EINVAL 	-22	/* Invalid argument */
#define T_ENFILE 	-23	/* File table overflow */
#define T_EMFILE	-24	/* Too many open files */
#define T_ENOTTY	-25	/* Not a typewriter */
#define T_ETXTBSY	-26	/* Text file busy */
#define T_EFBIG		-27	/* File too large */
#define T_ENOSPC	-28	/* No space left on device */
#define T_ESPIPE	-29	/* Illegal seek */
#define T_EROFS 	-30	/* Read-only file system */
#define T_EMLINK	-31	/* Too many links */
#define T_EPIPE 	-32	/* Broken pipe */
#define T_EDOM 		-33	/* Math argument out of domain of func */
#define T_ERANGE	-34	/* Math result not representable */

#endif
