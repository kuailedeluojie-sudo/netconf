/**
 * \file config.h
 * \author Radek Krejci <rkrejci@cesnet.cz>
 * \brief libnetconf2 various configuration settings.
 *
 * Copyright (c) 2015 - 2017 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef NC_CONFIG_H_
#define NC_CONFIG_H_

/*
 * Mark all objects as hidden and export only objects explicitly marked to be part of the public API or
 * those marked as mock objects for testing purpose
 */
#define API __attribute__((visibility("default")))
#define MOCK __attribute__((visibility("default")))

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

/*
 * Support for atomic variables
 */
/* #undef HAVE_STDATOMIC */
#ifdef HAVE_STDATOMIC
# include <stdatomic.h>

# define ATOMIC_UINT32_T atomic_uint_fast32_t
# define ATOMIC_INC(x) atomic_fetch_add(x, 1)
#else
# define ATOMIC_UINT32_T uint32_t
# define ATOMIC_INC(x) __sync_add_and_fetch(x, 1)
#endif

/*
 * support for pthread_mutex_timedlock
 */
#define HAVE_PTHREAD_MUTEX_TIMEDLOCK

/*
 * Support for getpeereid
 */
/* #undef HAVE_GETPEEREID */

/*
 * Support for shadow file manipulation
 */
/* #undef HAVE_SHADOW */

/*
 * Support for crypt.h
 */
/* #undef HAVE_CRYPT */

/*
 * Location of installed basic YANG modules on the system
 */
#define NC_YANG_DIR "/home/mo/share/netconf2/lib/libnetconf2/_install/share/yang/modules"

/*
 * Inactive read timeout
 */
#define NC_READ_INACT_TIMEOUT 20

/*
 * Active read timeout in seconds
 * (also used for internal <get-schema> RPC reply timeout)
 */
#define NC_READ_ACT_TIMEOUT 300

/*
 * pspoll structure queue size (also found in nc_server.h)
 */
#define NC_PS_QUEUE_SIZE 6

/* Microseconds after which tasks are repeated until the full timeout elapses.
 * A millisecond (1000) should be divisible by this number without remain.
 */
#define NC_TIMEOUT_STEP 100

/* Portability feature-check macros. */
#define HAVE_PTHREAD_RWLOCKATTR_SETKIND_NP

#endif /* NC_CONFIG_H_ */
