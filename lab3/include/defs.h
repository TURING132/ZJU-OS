#pragma once

#ifndef NULL
#define NULL ((void *)0)
#endif

#define __always_inline inline __attribute__((always_inline))
#define __noinline             __attribute__((noinline))
#define __noreturn             __attribute__((noreturn))

/* Represents true-or-false values */
typedef int bool;

/* Explicitly-sized versions of integer types */
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

/* Add fast types */
typedef signed char int_fast8_t;
typedef short int_fast16_t;
typedef long int_fast32_t;
typedef long long int_fast64_t;

typedef unsigned char uint_fast8_t;
typedef unsigned short uint_fast16_t;
typedef unsigned long uint_fast32_t;
typedef unsigned long long uint_fast64_t;

#define INT8_MIN (-0x7f - 1)
#define INT16_MIN (-0x7fff - 1)
#define INT32_MIN (-0x7fffffff - 1)
#define INT64_MIN (-0x7fffffffffffffff - 1)

#define INT8_MAX (0x7f)
#define INT16_MAX (0x7fff)
#define INT32_MAX (0x7fffffff)
#define INT64_MAX (0x7fffffffffffffff)

#define UINT8_MAX (0xff)
#define UINT16_MAX (0xffff)
#define UINT32_MAX (0xffffffff)
#define UINT64_MAX (0xffffffffffffffff)

/* *
 * Pointers and addresses are 32 bits long.
 * We use pointer types to represent addresses,
 * uintptr_t to represent the numerical values of addresses.
 * */
typedef int64_t intptr_t;
typedef uint64_t uintptr_t;

/* size_t is used for memory object sizes */
typedef uintptr_t size_t;

/* used for page numbers */
typedef size_t ppn_t;

struct sbiret {
  uint64_t error;
  uint64_t value;
};

/* *
 * Rounding operations (efficient when n is a power of 2)
 * Round down to the nearest multiple of n
 * */
#define ROUNDDOWN(a, n)           \
  ({                              \
    size_t __a = (size_t)(a);     \
    (typeof(a))(__a - __a % (n)); \
  })

/* Round up to the nearest multiple of n */
#define ROUNDUP(a, n)                                   \
  ({                                                    \
    size_t __n = (size_t)(n);                           \
    (typeof(a))(ROUNDDOWN((size_t)(a) + __n - 1, __n)); \
  })
