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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <errno.h>

#if defined( __QNX__ )
    // QNX errno is magically multithread aware
    #define _RWD_errno      errno
#elif defined( __NETWARE__ )
    // What does NETWARE do?
  #if defined( _NETWARE_LIBC )
    #undef errno
    #define _RWD_errno      (*___errno())       /* get LibC errno */
  #else
    #define _RWD_errno      errno
  #endif
#elif defined( __SW_BM ) && !defined( __RDOSDEV__ )
    #undef errno
    #define _RWD_errno      (__THREADDATAPTR->__errnoP)
#else
    #define _RWD_errno      errno
#endif


#if !defined( __UNIX__ ) && !defined( __NETWARE__ )
  #if defined( __SW_BM ) && !defined( __RDOSDEV__ )
    #undef _doserrno
    #define _RWD_doserrno   (__THREADDATAPTR->__doserrnoP)
  #else
    #define _RWD_doserrno   _doserrno
  #endif
#endif
