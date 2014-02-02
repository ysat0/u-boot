#ifndef __ASM_SH_STRING_H
#define __ASM_SH_STRING_H

/*
 * Copyright (C) 1999 Niibe Yutaka
 * But consider these trivial functions to be public domain.
 *
 * from linux kernel code.
 */

#ifdef __KERNEL__               /* only set these up for kernel code */

#else                           /* KERNEL */

/*
 * let user libraries deal with these,
 * IMHO the kernel has no place defining these functions for user apps
 */

#define __HAVE_ARCH_STRCPY      1
#define __HAVE_ARCH_STRNCPY     1
#define __HAVE_ARCH_STRCAT      1
#define __HAVE_ARCH_STRNCAT     1
#define __HAVE_ARCH_STRCMP      1
#define __HAVE_ARCH_STRNCMP     1
#define __HAVE_ARCH_STRNICMP    1
#define __HAVE_ARCH_STRCHR      1
#define __HAVE_ARCH_STRRCHR     1
#define __HAVE_ARCH_STRSTR      1
#define __HAVE_ARCH_STRLEN      1
#define __HAVE_ARCH_STRNLEN     1
#define __HAVE_ARCH_MEMSET      1
#define __HAVE_ARCH_MEMCPY      1
#define __HAVE_ARCH_MEMMOVE     1
#define __HAVE_ARCH_MEMSCAN     1
#define __HAVE_ARCH_MEMCMP      1
#define __HAVE_ARCH_MEMCHR      1
#define __HAVE_ARCH_STRTOK      1

#endif /* KERNEL */
#endif /* __ASM_SH_STRING_H */
