#include <c89/abpi.h>


#if !defined(CEN_INCLUDED_VMSS_H)
    #define CEN_INCLUDED_VMSS_H

    typedef enum eprivl89
    {
        PRIVL89_WRITE = 1 << 0,
        PRIVL89_READ = 1 << 1,
        PRIVL89_EXEC = 1 << 2,
        PRIVL89_NONE = 1 << 3

    } eprivl89;


    #define KB * (((uint64_t) 1) << 10)
    #define MB * (((uint64_t) 1) << 20)
    #define GB * (((uint64_t) 1) << 30)
    #define TB * (((uint64_t) 1) << 40)
    #define PG * _cdv2_platform_pagesize


    #define malignto(dir, ptr, val) \
        (((dir) == 0) ? (void*) (ptr) : (void*) \
        (((uintptr_t) (ptr) + (((val) - 1) * (dir))) & ~((val) - 1)))

    #define malignof(type) \
        (abpi_extension offsetof(struct {uint8_t _1; type _2;}, _2))

    #define malignis(ptr, val) \
        (((uintptr_t) (ptr) & ((val) - 1)) == 0)


    void* mvresrv(void *hint, size_t size);
    void* mvcmmit(void *addr, size_t size);
    int8_t mvprotc(void *addr, size_t size, eprivl89 access);
    int8_t mvrelea(void *reserved, size_t size);
    int8_t mvdmmit(void *commited, size_t size);
#endif /* CEN_INCLUDED_VMSS_H */
