#include <c89/abpi.h>
#if (ABPI_PLATFORM_WINDOWS_NT == 1)
    #include <profileapi.h>
#elif (ABPI_PLATFORM_GNU_LINUX == 1) /* ABPI_PLATFORM_WINDOWS_NT == 1 */
    #include <time.h>
#endif /* ABPI_PLATFORM_GNU_LINUX == 1 */


#if !defined(CEN_INCLUDED_TIME_H)
#define CEN_INCLUDED_TIME_H

double dtruntime(void);

#endif /* !CEN_INCLUDED_TIME_H */


#if defined(CEN_IMPLEMENT) && !defined(CEN_IMPLEMENTED_TIME_H)
    #define CEN_IMPLEMENTED_TIME_H

    double
    dtruntime(void)
    {
        #if (ABPI_PLATFORM_WINDOWS_NT == 1)
            LARGE_INTEGER frequency, counter;
            QueryPerformanceFrequency(&frequency);
            QueryPerformanceCounter(&counter);
            return (double) counter.QuadPart / (double) frequency.QuadPart;
        #elif (ABPI_PLATFORM_GNU_LINUX == 1) /* ABPI_PLATFORM_WINDOWS_NT == 1 */
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            return (double) ts.tv_sec + (double) ts.tv_nsec * 1e-9;
        #endif /* ABPI_PLATFORM_GNU_LINUX == 1 */
    }
#endif /* CEN_IMPLEMENT && !CEN_IMPLEMENTED_TIME_H */