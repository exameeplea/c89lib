#if !defined(CENLIB_INCLUDED_ABPI_H)
#define CENLIB_INCLUDED_ABPI_H

#undef _POSIX_C_SOURCE
#undef _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <stddef.h>
#include <stdarg.h>
#include <limits.h>


#if !defined(ABPI_ARCHITECTURE)
    #if defined(__x86_64__) || defined(__aarch64__)
        #define ABPI_ARCHITECTURE 64
    #elif defined(__i386__) || defined(__arm__) /* __x86_64__ || __aarch64__ */
        #define ABPI_ARCHITECTURE 32
    #else /* __i386__ || __arm__ */
        #error "c89lib/abpi.h: please, implement me with the support of your architecture"
    #endif /* unknown architecture */
#endif /* !ABPI_ARCHITECTURE */

#if defined(_WIN32) || defined(_WIN64)
    #define ABPI_PLATFORM_STRING "Windows NT"
    #define ABPI_PLATFORM_WINDOWS_NT 1
    #define ABPI_PLATFORM_GNU_LINUX 0
    #define ABPI_PLATFORM_ANDROID 0
#elif defined(__ANDROID__) /* _WIN32 || _WIN64 */
    #define ABPI_PLATFORM_STRING "Android"
    #define ABPI_PLATFORM_ANDROID 1
    #define ABPI_PLATFORM_GNU_LINUX 1
    #define ABPI_PLATFORM_WINDOWS_NT 0
#elif defined(__linux__) /* __ANDROID__ */
    #define ABPI_PLATFORM_STRING "GNU/Linux"
    #define ABPI_PLATFORM_GNU_LINUX 1
    #define ABPI_PLATFORM_WINDOWS_NT 0
    #define ABPI_PLATFORM_ANDROID 0
#else /* __linux__ */
    #error "c89lib/abpi.h: please, implement me with the support of your platform"
#endif /* unknown platform */

#if defined(__clang__)
    #define ABPI_CXX_COMPILER_CLANG 1
    #define ABPI_CXX_COMPILER_GCC 1
    #define ABPI_CXX_COMPILER_MSVC 0
#elif defined(__GNUC__) /* __clang__ */
    #define ABPI_CXX_COMPILER_GCC 1
    #define ABPI_CXX_COMPILER_CLANG 0
    #define ABPI_CXX_COMPILER_MSVC 0
#elif defined(_MSC_VER) /* __GNUC__ */
    #define ABPI_CXX_COMPILER_MSVC 1
    #define ABPI_CXX_COMPILER_GCC 0
    #define ABPI_CXX_COMPILER_CLANG 0
#else /* _MSC_VER */
    #error "c89lib/abpi.h: please, implement me with the support of your compiler"
#endif /* unknown compiler */


#define abpi_query(conditional, name) \
    typedef char _CDV0_QUERY##name[(conditional) ? 1 : -1];

#if (ABPI_CXX_COMPILER_MSVC == 1)
    #define ABPI_FUNCTION_UNIT __FUNCTION__
    #define ABPI_TRANSLATION_UNIT __FILE__

    #define abpi_export __declspec(dllexport)
    #define abpi_import __declspec(dllimport)
    #define abpi_thread __declspec(thread)
    #define abpi_final __declspec(noreturn)
    #define abpi_irrelv_of(reason) __declspec(deprecated(reason))
    #define abpi_text __forceinline
    #define abpi_inline __inline
    #define abpi_func __declspec(noinline)
    #define abpi_restrict __restrict
    #define abpi_extension

    #define abpi_cdecl __cdecl
    #define abpi_stdcall __stdcall
    #define abpi_fastcall __fastcall
    #define abpi_thiscall __thiscall
    #define abpi_vectorcall __vectorcall
#elif (ABPI_CXX_COMPILER_GCC == 1) /* ABPI_CXX_COMPILER_MSVC == 1 */
    #define ABPI_FUNCTION_UNIT __extension__ __FUNCTION__
    #if defined(__FILE_NAME__)
        #define ABPI_TRANSLATION_UNIT __FILE_NAME__
    #else /* !__FILE_NAME__ */
        #define ABPI_TRANSLATION_UNIT __FILE__
    #endif /* __FILE_NAME__ */

    #define abpi_export __attribute__((visibility("default")))
    #define abpi_import
    #define abpi_thread __thread
    #define abpi_final __attribute__((noreturn))
    #define abpi_irrelv_of(reason) __attribute__((deprecated(reason)))
    #define abpi_text __attribute__((always_inline))
    #define abpi_inline __inline__
    #define abpi_func __attribute__((noinline))
    #define abpi_restrict __restrict__
    #define abpi_extension __extension__

    #define abpi_cdecl __attribute__((cdecl))
    #define abpi_stdcall __attribute__((stdcall))
    #define abpi_fastcall __attribute__((fastcall))
    #define abpi_thiscall __attribute__((thiscall))
    #define abpi_vectorcall __attribute__((vectorcall))
#else /* ABPI_CXX_COMPILER_GCC == 1 */
    #error "c89lib/apbi.h: please, implement me with your macroinstructions"
#endif /* unspecified macroinstructions of compiler */


#if !defined(UINT32_MAX) && (__STDC_VERSION__ < 199901L)
    #if (CHAR_BIT != 8)
        #error "c89lib/abpi.h: your platform does not support 8 bits per byte"
    #endif /* byte == 8 bits */

    #if (UCHAR_MAX == 0xFFu)
        typedef unsigned char uint8_t;
        typedef signed char int8_t;
    #elif defined(ABPI_PROVIDE_TYPE_INT8) && defined(ABPI_PROVIDE_TYPE_UINT8) /* char == 1 byte */
        typedef ABPI_PROVIDE_TYPE_UINT8 uint8_t;
        typedef ABPI_PROVIDE_TYPE_INT8 int8_t;
    #else /* ABPI_PROVIDE_TYPE_UINT8 && ABPI_PROVIDE_TYPE_UINT8 */
        #error "c89lib/abpi.h: your platform does not support exactly 8 bits type"
    #endif /* not found 1 byte type */

    #if (USHRT_MAX == 0xFFFFu)
        typedef unsigned short uint16_t;
        typedef signed short int16_t;
    #elif (UINT_MAX == 0xFFFFu) /* USHRT_MAX == 2 bytes */
        typedef unsigned int uint16_t;
        typedef signed int int16_t;
    #elif (ULONG_MAX == 0xFFFFu) /* UINT_MAX == 2 bytes */
        typedef unsigned long uint16_t;
        typedef signed long int16_t;
    #elif defined(ABPI_PROVIDE_TYPE_INT16) && defined(ABPI_PROVIDE_TYPE_UINT16)
        typedef ABPI_PROVIDE_TYPE_UINT16 uint16_t;
        typedef ABPI_PROVIDE_TYPE_INT16 int16_t;
    #else /* ABPI_PROVIDE_TYPE_INT16 && ABPI_PROVIDE_TYPE_UINT16 */
        #error "c89lib/abpi.h: your platform does not support exactly 16 bits type"
    #endif /* not found 2 bytes type */

    #if (UINT_MAX == 0xFFFFFFFFu)
        typedef unsigned int uint32_t;
        typedef signed int int32_t;
    #elif (ULONG_MAX == 0xFFFFFFFFu) /* UINT_MAX == 4 bytes */
        typedef unsigned long uint32_t;
        typedef signed long int32_t;
    #elif (USHRT_MAX == 0xFFFFFFFFu) /* ULONG_MAX == 4 bytes */
        typedef unsigned short uint32_t;
        typedef signed short int32_t;
    #elif defined(ABPI_PROVIDE_TYPE_INT32) && defined(ABPI_PROVIDE_TYPE_UINT32) /* USHRT_MAX == 4 bytes */
        typedef ABPI_PROVIDE_TYPE_UINT32 uint32_t;
        typedef ABPI_PROVIDE_TYPE_INT32 int32_t;
    #else /* ABPI_PROVIDE_TYPE_INT32 && ABPI_PROVIDE_TYPE_UINT32 */
        #error "c89lib/abpi.h: your platform does not support exactly 32 bits type"
    #endif /* not found 4 bytes type */

    #if defined(ABPI_PROVIDE_TYPE_INT64) && defined(ABPI_PROVIDE_TYPE_UINT64)
        typedef ABPI_PROVIDE_TYPE_UINT64 uint64_t;
        typedef ABPI_PROVIDE_TYPE_INT64 int64_t;
    #elif (ABPI_CXX_COMPILER_GCC == 1)
        abpi_extension typedef unsigned long long uint64_t;
        abpi_extension typedef signed long long int64_t;
        #define _cdv0_long_long_used 1
    #endif /* long long and ull exists */
    #if !defined(_cdv0_long_long_used)
        #if (ABPI_CXX_COMPILER_MSVC == 1)
            typedef unsigned __int64 uint64_t;
            typedef __int64 int64_t;
        #else /* ABPI_CXX_COMPILER_MSVC */
            #error "c89lib/abpi.h: your platform does not support exactly 64 bits type"
        #endif /* not found 8 bytes type */
    #else /* long long was used already */
        #undef _cdv0_long_long_used
    #endif /* long long was used */
#elif (__STDC_VERSION__ >= 199901L) /* !stdint.h && __STDC_VERSION__ < C99 */
    #include <stdint.h>
#endif /* __STDC_VERSION__ >= C99 */

#if (__STDC_VERSION__ < 199901L)
    #define inline abpi_inline
#endif /* __STDC_VERSION__ > 199901L */

abpi_query((sizeof(uint8_t) * CHAR_BIT) == 8, abpi_u8_valid)
abpi_query((sizeof(uint16_t) * CHAR_BIT) == 16, abpi_u16_valid)
abpi_query((sizeof(uint32_t) * CHAR_BIT) == 32, abpi_u32_valid)
abpi_query((sizeof(uint64_t) * CHAR_BIT) == 64, abpi_u64_valid)

#if !defined(UINT32_MAX) && (__STDC_VERSION__ < 199901L)
    #if (ABPI_ARCHITECTURE == 8)
        typedef uint8_t uintptr_t;
        typedef int8_t intptr_t;
    #elif (ABPI_ARCHITECTURE == 16) /* ABPI_ARCHITECTURE == 8 */
        typedef uint16_t uintptr_t;
        typedef int16_t intptr_t;
    #elif (ABPI_ARCHITECTURE == 32) /* ABPI_ARCHITECTURE == 16 */
        typedef uint32_t uintptr_t;
        typedef int32_t intptr_t;
    #elif (ABPI_ARCHITECTURE == 64) /* ABPI_ARCHITECTURE == 32 */
        typedef uint64_t uintptr_t;
        typedef int64_t intptr_t;
    #endif /* ABPI_ARCHITECTURE == 64 */
#endif /* !stdint.h && __STDC_VERSION__ < C99 */

abpi_query((sizeof(uintptr_t) * CHAR_BIT) == ABPI_ARCHITECTURE, abpi_ptr_valid)

#endif /* CENLIB_INCLUDED_ABPI_H */