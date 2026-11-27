#if !defined(LDBG_AVOID_CONFIG)
    #if !defined(LDBG_BUFFER_SIZE)
        #define LDBG_BUFFER_SIZE 1024
    #endif

    #if !defined(LDBG_HELP_LEVEL)
        #define LDBG_HELP_LEVEL 2
    #endif /* !LDBG_HELP_LEVEL */

    #if !defined(LDBG)
        #define LDBG ABPI_TRANSLATION_UNIT
    #endif /* !LDBG */

    #if !defined(LDBG_SYMBOLS_DB)
        #define LDBG_SYMBOLS_DB 0
    #endif /* LDBG_SYMBOLS_DB */
#endif /* !LDBG_AVOID_CONFIG */

#if (LDBG_HELP_LEVEL != 0) && (LDBG_HELP_LEVEL != 1) && (LDBG_HELP_LEVEL != 2)
    #error "c89/ldbg.h: unrecognized debug help level, available: 0, 1, 2"
#endif /* unknown ldbg help level */

#if (LDBG_SYMBOLS_DB != 0) && (LDBG_SYMBOLS_DB != 1)
    #error "c89/ldbg.h: unrecognized debug symbol mode, available: 0, 1"
#endif /* unknown debug symbol mode */


#include <c89/abpi.h>
#include <c89/vmss.h>

#if (ABPI_PLATFORM_WINDOWS_NT == 1)
    #define WIN32_MEAN_AND_LEAN
    #include <windows.h>
#elif (ABPI_PLATFORM_GNU_LINUX == 1) /* ABPI_PLATFORM_WINDOWS_NT == 1 */
    #include <signal.h>
    #include <unistd.h>
#endif /* ABPI_PLATFORM_GNU_LINUX == 1 */

#if (ABPI_PLATFORM_ANDROID == 1)
    #include <android/log.h>
#elif (ABPI_PLATFORM_GNU_LINUX == 1) /* ABPI_PLATFORM_ANDROID == 1 */
    #include <syslog.h>
#endif /* ABPI_PLATFORM_GNU_LINUX == 1 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#if !defined(CEN_INCLUDED_LDBG_H)
    #define LDBG_CRASHFAULT_BEGIN ((const char*) &_cdv1_asf_logout)
    #define LDBG_INFO 1
    #define LDBG_ERROR 2
    #define LDBG_DEBUG 3
    #define LDBG_CRASHFAULT_FINAL ((const char*) &_cdv1_crashfault_handler)

    #define TRACE89_FLAGS_FILTERED_BIT 0
    #define TRACE89_FLAGS_FILTERED ((uint8_t) 1u << TRACE89_FLAGS_FILTERED_BIT)
    #define TRACE89_FLAGS_PRINTED_BIT 1
    #define TRACE89_FLAGS_PRINTED ((uint8_t) 1u << TRACE89_FLAGS_PRINTED_BIT)

    typedef struct _cdv1_trace89_base
    {
        const char *func;
        const char *unit;
        uint8_t flags;
        size_t line;

    } _cdv1_trace89_base;

    #define DEBUG89_FLAGS_EXTENDED_BIT 0
    #define DEBUG89_FLAGS_EXTENDED ((uint8_t) 1u << DEBUG89_FLAGS_EXTENDED_BIT)
    #define DEBUG89_FLAGS_FILTERED_BIT 1
    #define DEBUG89_FLAGS_FILTERED ((uint8_t) 1u << DEBUG89_FLAGS_FILTERED_BIT)
    #define DEBUG89_FLAGS_OUTGROUP_BIT 2
    #define DEBUG89_FLAGS_OUTGROUP ((uint8_t) 1u << DEBUG89_FLAGS_OUTGROUP_BIT)
    #define DEBUG89_FLAGS_NOBUFFER_BIT 3
    #define DEBUG89_FLAGS_NOBUFFER ((uint8_t) 1u << DEBUG89_FLAGS_NOBUFFER_BIT)
    #define DEBUG89_FLAGS_DBG_SYM_DUMMY_BIT 4
    #define DEBUG89_FLAGS_DBG_SYM_DUMMY ((uint8_t) 1u << DEBUG89_FLAGS_DBG_SYM_DUMMY_BIT)

    typedef struct _cdv1_debug89_mini
    {
        uint8_t flags;
        intptr_t error;
        char *message;

    } _cdv1_debug89_mini;

    typedef struct _cdv1_debug89_full
    {
        uint8_t flags;
        intptr_t error;
        char *volatile message;
        _cdv1_trace89_base *stack;
        _cdv1_trace89_base *volatile cursor;
        void (*log)(void*, const char*);
        void *out;
        double start;

    } _cdv1_debug89_full;

    typedef struct _cdv1_debugger_sym
    {
        const char *spec_word;
        const char *function;

    } _cdv1_debugger_sym;


    void _cdv1_load(void *suite, _cdv1_trace89_base *entries, char *message, double (*timer)(void));
    const char* _cdv1_message_of(uint8_t warning, const char *format, va_list *args);
    const char* _cdv1_header_of(_cdv1_trace89_base *entry, uint8_t newline, double fixed_time);
    void _cdv1_platform_log(int level, const char *format, ...);
    void _cdv1_crashfault_default(uint64_t crashfault);
    void _cdv1_crashfault_asafelog(const char *text);
    int _cdv1_inform_log(const char *format, ...);
    int _cdv1_warn_log(const char *format, ...);
    char* _cdv1_fix(char *buffer, const char *text);


    extern abpi_thread void *_cdv1_module_debug;
    extern uint8_t _cdv1_dummy_mask;
    extern void (*_cdv1_asf_logout)(const char *message);
    extern void (*_cdv1_crashfault_handler)(uint64_t crashfault);
    extern double (*_cdv1_execution_timer)(void);
    extern _cdv1_debugger_sym _cdv1_debugger_funcsym;

    #if (ABPI_PLATFORM_WINDOWS_NT == 1)
        extern LONG (*_cdv1_old_exception_handler)(LPEXCEPTION_POINTERS info);
        LONG _cdv1_exception_handler(LPEXCEPTION_POINTERS info);
    #elif (ABPI_PLATFORM_GNU_LINUX == 1) /* ABI_PLATFORM_WINDOWS_NT == 1 */
        extern void (*_cdv1_old_signal_handler)(int signal, siginfo_t *info, void *ctx);
        void _cdv1_signal_handler(int signal, siginfo_t *info, void *ctx);
        extern struct sigaction _cdv1_old_SIGSEGV;
        extern struct sigaction _cdv1_old_SIGFPE;
        extern struct sigaction _cdv1_old_SIGBUS;
        extern struct sigaction _cdv1_old_SIGILL;
        extern struct sigaction _cdv1_tmp_action;
    #else /* ABPI_PLATFORM_GNU_LINUX == 1 */
        /* #error "c89/ldbg.h: if you need, implement here crash internals for your platform support" */
    #endif /* a block, where you can extend crashfault handler functionality for your platform */
#endif /* CEN_INCLUDED_LDBG_H */


#undef _cdv1_dbsym_begin
#undef _cdv1_dbsym_final

#if (LDBG_SYMBOLS_DB == 1)
    #define _cdv1_dbsym_begin \
        const char *_cdv1_dbsym_temp1 = _cdv1_debugger_funcsym.spec_word = "LDBG.H FUNCTION BEGIN"; \
        const char *_cdv1_dbsym_temp2 = _cdv1_debugger_funcsym.function = ABPI_FUNCTION_UNIT

    #define _cdv1_dbsym_final \
        _cdv1_debugger_funcsym.spec_word = "LDBG.H FUNCTION FINAL"; \
        _cdv1_debugger_funcsym.function = ABPI_FUNCTION_UNIT
#elif (LDBG_SYMBOLS_DB == 0) /* LDBG_SYMBOLS_DB == 1 */
    #define _cdv1_dbsym_begin uint8_t _cdv1_dbsym_dead_code
    #define _cdv1_dbsym_final (void) _cdv1_dbsym_dead_code
#endif /* LDBG_SYMBOLS_DB == 0 */

#undef trace89
#undef debug89
#undef dbgsuite
#undef dbgload
#undef dbgfnjmp
#undef dbgfnret
#undef dbgfninf
#undef dbgfnwrn
#undef dbgfnmsk
#undef dbgfnsav
#undef dbgdef
#undef dbglog
#undef dbgmsk
#undef dbgmskdecl
#undef dbgmskimpl
#undef dbgmskoptn
#undef dbginstall

#if (LDBG_HELP_LEVEL == 0)
    #define trace89 _cdv1_trace89_base
    #define debug89 _cdv1_debug89_mini
    #define dbgsuite ((debug89*) _cdv1_module_debug)

    #define dbgload(suite, entries, message, timer) \
        _cdv1_dbsym_begin; \
        _cdv1_load((void*) suite, NULL, NULL, NULL); \
        _cdv1_dbsym_final

    #define dbgfnjmp() _cdv1_dbsym_begin
    #define dbgfnret() _cdv1_dbsym_final
    #define dbgfninf (void) sizeof _cdv1_inform_empty
    #define dbgfnwrn (void) sizeof _cdv1_warn_empty
    #define dbgfnmsk(mask, option) ((void) 0)
    #define dbgmsk(mask) _cdv1_dummy_mask
    #define dbgdef(log, out) _cdv1_mini_define((void (*)(void*, const char*)) log, (void*) out)
    #define dbglog (void) _cdv1_log_empty
    #define dbgfnsav() ((void) 0)

    #if !defined(_cdv1_outputs_empty_defined)
        #define _cdv1_outputs_empty_defined

        static int
        _cdv1_inform_empty(const char *text, ...) { (void) text; return 0; }

        static int
        _cdv1_warn_empty(const char *text, ...) { (void) text; return 0; }
    #endif /* !_cdv1_outputs_empty_defined */

    #if !defined(_cdv1_log_empty_defined)
        #define _cdv1_log_empty_defined

        static int
        _cdv1_log_empty(int level, const char *format, ...) { (void) level, (void) format; return 0; }
    #endif /* !_cdv1_log_empty_defined */

    #if !defined(_cdv1_build_mini_defined)
        #define _cdv1_build_mini_defined

        abpi_inline static debug89
        _cdv1_mini_define(void (*log)(void*, const char*), void *out)
        {
            debug89 build_struct;
            build_struct.flags = 0;
            build_struct.error = 0;
            build_struct.message = NULL;
            (void) log, (void) out;

            return build_struct;
        }
    #endif /* _cdv1_build_mini_defined */
#elif (LDBG_HELP_LEVEL == 1) /* LDBG_HELP_LEVEL == 0 */
    #define trace89 _cdv1_trace89_base
    #define debug89 _cdv1_debug89_full
    #define dbgsuite ((debug89*) _cdv1_module_debug)

    #define dbgload(suite, entries, message, timer) \
        _cdv1_dbsym_begin; \
        debug89 *_cdv1_lfn_dbp = (debug89*) suite; \
        uint8_t _cdv1_lfn_full = suite != NULL && entries != NULL && \
                                 ((((debug89*) suite)->flags & DEBUG89_FLAGS_EXTENDED) != 0); \
        uint8_t _cdv1_lfn_override = (_cdv1_lfn_full == 1) ? entries != _cdv1_lfn_dbp->stack : 0; \
        trace89 *_cdv1_lfn_first = (_cdv1_lfn_full == 1) ? _cdv1_lfn_dbp->cursor : NULL; \
        _cdv1_load((void*) suite, entries, message, timer); \
        if (_cdv1_lfn_full == 1 && _cdv1_lfn_override == 1) { \
            if (_cdv1_lfn_first != NULL) { \
                *_cdv1_lfn_dbp->stack = *_cdv1_lfn_first; \
                _cdv1_lfn_dbp->stack->flags &= ~TRACE89_FLAGS_PRINTED; \
            } else { \
                (_cdv1_lfn_dbp->cursor)->flags = 0; \
                (_cdv1_lfn_dbp->cursor)->unit = LDBG; \
                (_cdv1_lfn_dbp->cursor)->func = ABPI_FUNCTION_UNIT; \
                (_cdv1_lfn_dbp->cursor)->line = __LINE__; \
            } \
        } \
        _cdv1_dbsym_final

    #define dbgfnjmp() \
        _cdv1_dbsym_begin; \
        uint8_t _cdv1_lfn_dbpret_used; \
        debug89 *_cdv1_lfn_dbp = (debug89*) dbgsuite; \
        uint8_t _cdv1_lfn_full = _cdv1_lfn_dbp != NULL && \
                                 ((_cdv1_lfn_dbp->flags & DEBUG89_FLAGS_EXTENDED) != 0) && \
                                _cdv1_lfn_dbp->stack != NULL; \
        if (_cdv1_lfn_full == 1) { \
            (++_cdv1_lfn_dbp->cursor)->flags = 0; \
            (_cdv1_lfn_dbp->cursor)->unit = LDBG; \
            (_cdv1_lfn_dbp->cursor)->func = ABPI_FUNCTION_UNIT; \
            (_cdv1_lfn_dbp->cursor)->line = __LINE__; \
        }

    #define dbgfnret() \
        (void) _cdv1_lfn_dbpret_used; \
        if (_cdv1_lfn_full == 1) { \
            _cdv1_lfn_dbp->cursor--; \
        } \
        _cdv1_dbsym_final;

    #define dbgfnsav() ((_cdv1_lfn_full == 1) ? _cdv1_lfn_dbp->cursor->line = __LINE__ : 0)
    #define dbgdef(log, out) _cdv1_define((void (*)(void*, const char*)) log, (void*) out)
    #define dbgfninf dbgfnsav(), (void) sizeof _cdv1_inform_empty
    #define dbgfnwrn dbgfnsav(), (void) sizeof _cdv1_warn_empty
    #define dbgfnmsk(mask, option) ((void) 0)
    #define dbgmsk(mask) *_ldbg_get_##mask()
    #define dbglog (dbgsuite != NULL) ? ((dbgsuite->message != NULL) ? \
                    /* (void) printf("dbgsuite->message is not NULL, so putting LDBG address into message buffer\n"), */ \
                    (void*) (*((uintptr_t*) malignto(1, dbgsuite->message, malignof(uintptr_t))) = (uintptr_t) LDBG) : \
                    /* (void) printf("dbgsuite->message is NULL, so overwriting NULL message buffer to LDBG address\n"), */ \
                    (void*) (dbgsuite->message = LDBG)) : 0, _cdv1_platform_log

    #if !defined(_cdv1_outputs_empty_defined)
        #define _cdv1_outputs_empty_defined

        static int
        _cdv1_inform_empty(const char *text, ...) { (void) text; return 0; }

        static int
        _cdv1_warn_empty(const char *text, ...) { (void) text; return 0; }
    #endif /* !_cdv1_outputs_empty_defined */

    #if !defined(_cdv1_build_full_defined)
        #define _cdv1_build_full_defined

        abpi_inline static debug89
        _cdv1_define(void (*log)(void*, const char*), void *out)
        {
            debug89 build_struct;
            build_struct.flags = 0 | DEBUG89_FLAGS_EXTENDED;
            build_struct.error = 0;
            build_struct.cursor = NULL;
            build_struct.stack = NULL;
            build_struct.message = NULL;
            build_struct.log = log;
            build_struct.out = out;
            build_struct.start = 0.0;

            return build_struct;
        }
    #endif /* _cdv1_build_full_defined */
#elif (LDBG_HELP_LEVEL == 2) /* LDBG_HELP_LEVEL == 1 */
    #define trace89 _cdv1_trace89_base
    #define debug89 _cdv1_debug89_full
    #define dbgsuite ((debug89*) _cdv1_module_debug)

    #define dbgload(suite, entries, message, timer) \
        _cdv1_dbsym_begin; \
        debug89 *_cdv1_lfn_dbp = (debug89*) suite; \
        uint8_t _cdv1_lfn_full = suite != NULL && entries != NULL && \
                                 ((((debug89*) suite)->flags & DEBUG89_FLAGS_EXTENDED) != 0); \
        uint8_t _cdv1_lfn_override = (_cdv1_lfn_full == 1) ? entries != _cdv1_lfn_dbp->stack : 0; \
        trace89 *_cdv1_lfn_first = (_cdv1_lfn_full == 1) ? _cdv1_lfn_dbp->cursor : NULL; \
        _cdv1_load((void*) suite, entries, message, timer); \
        if (_cdv1_lfn_full == 1 && _cdv1_lfn_override == 1) { \
            if (_cdv1_lfn_first != NULL) { \
                *_cdv1_lfn_dbp->stack = *_cdv1_lfn_first; \
                _cdv1_lfn_dbp->stack->flags &= ~TRACE89_FLAGS_PRINTED; \
                _cdv1_lfn_dbp->log(_cdv1_lfn_dbp->out, "\n"); \
            } else { \
                (_cdv1_lfn_dbp->stack)->flags = 0; \
                (_cdv1_lfn_dbp->stack)->unit = LDBG; \
                (_cdv1_lfn_dbp->stack)->func = ABPI_FUNCTION_UNIT; \
                (_cdv1_lfn_dbp->stack)->line = __LINE__; \
            } \
        } \
        _cdv1_dbsym_final

    #define dbgfnjmp() \
        _cdv1_dbsym_begin; \
        uint8_t _cdv1_lfn_dbpret_used; \
        debug89 *_cdv1_lfn_dbp = (debug89*) dbgsuite; \
        uint8_t _cdv1_lfn_full = _cdv1_lfn_dbp != NULL && \
                                 ((_cdv1_lfn_dbp->flags & DEBUG89_FLAGS_EXTENDED) != 0) && \
                                _cdv1_lfn_dbp->stack != NULL; \
        if (_cdv1_lfn_full == 1) { \
            (++_cdv1_lfn_dbp->cursor)->flags = \
                0 | (((_cdv1_lfn_dbp->flags & DEBUG89_FLAGS_FILTERED) != 0) << TRACE89_FLAGS_FILTERED_BIT); \
            (_cdv1_lfn_dbp->cursor)->unit = LDBG; \
            (_cdv1_lfn_dbp->cursor)->func = ABPI_FUNCTION_UNIT; \
            (_cdv1_lfn_dbp->cursor)->line = __LINE__; \
            _cdv1_lfn_dbp->flags &= ~DEBUG89_FLAGS_FILTERED; \
        }

    #define dbgfnret() \
        (void) _cdv1_lfn_dbpret_used; \
        if (_cdv1_lfn_full == 1) { \
            if (_cdv1_lfn_dbp->cursor == _cdv1_lfn_dbp->stack + 1 && \
                (_cdv1_lfn_dbp->cursor->flags & TRACE89_FLAGS_PRINTED) != 0) { \
                _cdv1_lfn_dbp->flags |= DEBUG89_FLAGS_OUTGROUP; \
            }; \
            _cdv1_lfn_dbp->flags &= ~DEBUG89_FLAGS_FILTERED; \
            _cdv1_lfn_dbp->flags |= ((_cdv1_lfn_dbp->cursor->flags & TRACE89_FLAGS_FILTERED) != 0) \
                                    << DEBUG89_FLAGS_FILTERED_BIT; \
            _cdv1_lfn_dbp->cursor--; \
        } \
        _cdv1_dbsym_final;

    #define dbgfnsav() ((_cdv1_lfn_full == 1) ? _cdv1_lfn_dbp->cursor->line = __LINE__ : 0)
    #define dbgdef(log, out) _cdv1_define((void (*)(void*, const char*)) log, (void*) out)
    #define dbgfninf dbgfnsav(), _cdv1_inform_log
    #define dbgfnwrn dbgfnsav(), _cdv1_warn_log
    #define dbgmsk(mask) *_ldbg_get_##mask()
    #define dbglog (void) sizeof _cdv1_log_empty
    #define dbgfnmsk(mask, option) \
        do { \
            if (_cdv1_lfn_full == 1) { \
                uint8_t RESET_MASK = 0; \
                uint64_t mask##_EVERYTHING = ((uint64_t) ~0u << 32) | ~0u; \
                uint64_t _cdv1_test = mask##_##option; \
                (void) RESET_MASK, (void) mask##_EVERYTHING; \
                if (strcmp(#mask, "RESET") == 0 && strcmp(#option, "MASK") == 0) { \
                    _cdv1_lfn_dbp->flags &= ~DEBUG89_FLAGS_FILTERED; \
                } else { \
                    _cdv1_lfn_dbp->flags |= ((*_ldbg_get_##mask() & _cdv1_test) == 0) << DEBUG89_FLAGS_FILTERED_BIT; \
                } \
            } \
        } while(0)

    #if !defined(_cdv1_log_empty_defined)
        #define _cdv1_log_empty_defined

        static int
        _cdv1_log_empty(int level, const char *text, ...) { (void) level, (void) text; return 0; }
    #endif /* !_cdv1_log_empty_defined */

    #if !defined(_cdv1_build_full_defined)
        #define _cdv1_build_full_defined

        abpi_inline static debug89
        _cdv1_define(void (*log)(void*, const char*), void *out)
        {
            debug89 build_struct;
            build_struct.flags = 0 | DEBUG89_FLAGS_EXTENDED;
            build_struct.error = 0;
            build_struct.cursor = NULL;
            build_struct.stack = NULL;
            build_struct.message = NULL;
            build_struct.log = log;
            build_struct.out = out;
            build_struct.start = 0.0;

            return build_struct;
        }
    #endif /* _cdv1_build_full_defined */
#endif /* LDBG_HELP_LEVEL == 2 */

#define dbgmskdecl(abi, mask) \
    abi uint64_t* _ldbg_get_##mask(void);

#define dbgmskoptn(mask, option, bit) \
    static const uint64_t mask##_##option = (uint64_t)1 << bit;

#define dbgmskimpl(mask) \
    static uint64_t _cdv1_containerof_##mask = ~(uint64_t)0; \
    uint64_t* _ldbg_get_##mask(void) { \
        return &_cdv1_containerof_##mask; \
    }

#if (LDBG_HELP_LEVEL == 0)
    #define dbginstall(handler, async_safe_log) ((void) 0)
#elif (LDBG_HELP_LEVEL == 1) || (LDBG_HELP_LEVEL == 2) /* LDBG_HELP_LEVEL == 0 */
    #if (ABPI_PLATFORM_GNU_LINUX == 1)
        #define _cdv1_sigaction(signal) \
            if (sigaction(signal, &_cdv1_tmp_action, &_cdv1_old_##signal) == -1) { \
                dbgsuite->error = signal; \
            }

        #define dbginstall(handler, async_safe_log) \
            _cdv1_crashfault_handler = (void (*)(uint64_t)) handler; \
            sigemptyset(&_cdv1_tmp_action.sa_mask); \
            _cdv1_tmp_action.sa_sigaction = &_cdv1_signal_handler; \
            _cdv1_tmp_action.sa_flags = SA_SIGINFO; \
            _cdv1_sigaction(SIGSEGV); \
            _cdv1_sigaction(SIGBUS); \
            _cdv1_sigaction(SIGFPE); \
            _cdv1_sigaction(SIGILL); \
            _cdv1_asf_logout = (async_safe_log == NULL) ? &_cdv1_crashfault_asafelog : async_safe_log;
    #elif (ABPI_PLATFORM_WINDOWS_NT == 1) /* ABPI_PLATFORM_GNU_LINUX == 1 */
        #define dbginstall(handler, async_safe_log) \
            SetErrorMode(SEM_FAILCRITICALERRORS); \
            _cdv1_crashfault_handler = (void (*)(uint64_t)) handler; \
            _cdv1_old_exception_handler = SetUnhandledExceptionFilter(&_cdv1_exception_handler); \
            _cdv1_asf_logout = (async_safe_log == NULL) ? &_cdv1_crashfault_asafelog : async_safe_log;
    #else /* ABPI_PLATFORM_WINDOWS_NT == 1 */
        #error "c89/ldbg.h: please, implement my dbginstall with the support of your platform"
    #endif /* not supported platform yet */
#endif /* LDBG_HELP_LEVEL == 1 || LDBG_HELP_LEVEL == 2 */


#if !defined(CEN_INCLUDED_LDBG_H)
    #define CEN_INCLUDED_LDBG_H

    dbgmskdecl(extern, RESET)
#endif /* !CEN_INCLUDED_LDBG_H */


#if defined(CEN_IMPLEMENT) && !defined(CEN_IMPLEMENTED_LDBG_H)
    #define CEN_IMPLEMENTED_LDBG_H

    abpi_thread void *_cdv1_module_debug = NULL;
    void (*_cdv1_asf_logout)(const char*) = NULL;
    void (*_cdv1_crashfault_handler)(uint64_t) = NULL;
    double (*_cdv1_execution_timer)(void) = NULL;
    uint8_t _cdv1_dummy_mask;

    #if (LDBG_SYMBOLS_DB == 1)
        _cdv1_debugger_sym _cdv1_debugger_funcsym;
    #endif /* LDBG_SYMBOLS_DB == 1 */


    int
    _cdv1_inform_log(const char *format, ...)
    {
        #if (LDBG_HELP_LEVEL == 2)
            if (dbgsuite == NULL || (dbgsuite->flags & DEBUG89_FLAGS_FILTERED) != 0 || \
                (dbgsuite->flags & DEBUG89_FLAGS_EXTENDED) == 0) {
                return 0;
            } else {
                const char *msg = NULL;
                va_list args;

                va_start(args, format);
                msg = _cdv1_message_of(0, format, &args);
                dbgsuite->log(dbgsuite->out, msg);
                va_end(args);
	    }
        #else /* LDBG_HELP_LEVEL == 2 */
            (void) format;
        #endif /* LDBG_HELP_LEVEL != 2 */

        return 0;
    }

    int
    _cdv1_warn_log(const char *format, ...)
    {
        #if (LDBG_HELP_LEVEL == 2)
            if (dbgsuite == NULL || (dbgsuite->flags & DEBUG89_FLAGS_FILTERED) != 0 || \
                (dbgsuite->flags & DEBUG89_FLAGS_EXTENDED) == 0) {
                return 0;
            } else {
                const char *msg = NULL;
                va_list args;

                va_start(args, format);
                msg = _cdv1_message_of(1, format, &args);
                dbgsuite->log(dbgsuite->out, msg);
                va_end(args);
	    }
        #else /* LDBG_HELP_LEVEL == 2 */
            (void) format;
        #endif /* LDBG_HELP_LEVEL != 2 */

        return 0;
    }

    void
    _cdv1_load(void *suite, _cdv1_trace89_base *entries, char *message, double (*timer)(void))
    {
        if (suite == NULL) {
            return;
        }

        #if defined(_cdv1_log_empty_defined)
            (void) _cdv1_log_empty;
        #endif /* _cdv1_log_empty_defined */

        #if defined(_cdv1_outputs_empty_defined)
            (void) _cdv1_warn_empty, (void) _cdv1_inform_empty;
        #endif /* _cdv1_outputs_empty_defined */

        _cdv1_module_debug = (debug89*) suite;
        _cdv1_execution_timer = timer;

        dbgsuite->message = message;

        if (message != NULL) {
            dbgsuite->flags &= ~DEBUG89_FLAGS_NOBUFFER;
        } else {
            dbgsuite->flags |= DEBUG89_FLAGS_NOBUFFER;
        }

        #if (LDBG_HELP_LEVEL != 0)
            if ((dbgsuite->flags & DEBUG89_FLAGS_EXTENDED) != 0) {
                if (entries != NULL && entries != dbgsuite->stack) {
                    dbgsuite->stack = (trace89*) entries;
                    dbgsuite->cursor = (trace89*) entries;

                    if (timer != NULL) {
                        dbgsuite->start = timer();
                    }
                }
            }
        #else /* LDBG_HELP_LEVEL != 0 */
            (void) entries;
        #endif /* LDBG_HELP_LEVEL == 0 */

        #if (LDBG_SYMBOLS_DB == 1)
            if (_cdv1_debugger_funcsym.function[0]) {
                int dummybit = _cdv1_debugger_funcsym.function[1] * 2 > 27;
                dbgsuite->flags |= dummybit << DEBUG89_FLAGS_DBG_SYM_DUMMY_BIT;
        #endif /* LDBG_SYMBOLS_DB == 1 */
    }

    char*
    _cdv1_fix(char *buffer, const char *text)
    {
        #if (LDBG_HELP_LEVEL != 0)
            char *writer = NULL;
            char *last_ln = NULL;
            char *ptr, sym;

            writer = buffer;

            for (ptr = (char*) text, sym = *text; sym != '\0'; ++ptr, sym = *ptr) {
                if (sym == '\n') {
                    *writer = '\t', last_ln = (last_ln == NULL) ? writer : last_ln, writer++;
                } else {
                    *writer++ = sym, last_ln = NULL;
                }
            }

            if (last_ln == NULL) {
                *writer++ = '\n', *writer = '\0';
            } else {
                /* last_ln points to writer's last \t, so overwrite it */
                *last_ln++ = '\n', *last_ln = '\0';
            }

            return buffer;
        #else /* LDBG_HELP_LEVEL != 0 */
            (void) buffer, (void) text;

            return NULL;
        #endif /* LDBG_HELP_LEVEL == 0 */
    }

    #if (ABPI_PLATFORM_WINDOWS_NT == 1)
        LONG (*_cdv1_old_exception_handler)(LPEXCEPTION_POINTERS) = NULL;

        LONG
        _cdv1_exception_handler(LPEXCEPTION_POINTERS info)
        {
            #if (LDBG_HELP_LEVEL != 0)
                if (_cdv1_crashfault_handler != NULL) {
                    _cdv1_crashfault_handler(info->ExceptionRecord->ExceptionCode);
                } else {
                    _cdv1_crashfault_default(info->ExceptionRecord->ExceptionCode);
                }

                if (_cdv1_old_exception_handler != NULL) {
                    return _cdv1_old_exception_handler(info);
                }

                return EXCEPTION_EXECUTE_HANDLER;
            #else /* LDBG_HELP_LEVEL != 0 */
                (void) info;

                return 0;
            #endif /* LDBG_HELP_LEVEL == 0 */
        }
    #elif (ABPI_PLATFORM_GNU_LINUX == 1) /* ABPI_PLATFORM_WINDOWS_NT == 1 */
        struct sigaction _cdv1_old_SIGSEGV;
        struct sigaction _cdv1_old_SIGFPE;
        struct sigaction _cdv1_old_SIGBUS;
        struct sigaction _cdv1_old_SIGILL;
        struct sigaction _cdv1_tmp_action;
        void (*_cdv1_old_signal_handler)(int, siginfo_t*, void*) = NULL;

        void
        _cdv1_signal_handler(int signal, siginfo_t *info, void *ctx)
        {
            #if (LDBG_HELP_LEVEL != 0)
                struct sigaction *old_sa = NULL;

                if (_cdv1_crashfault_handler != NULL) {
                    _cdv1_crashfault_handler(signal);
                } else {
                    _cdv1_crashfault_default(signal);
                }

                switch (signal) {
                    case SIGSEGV: old_sa = &_cdv1_old_SIGSEGV; break;
                    case SIGFPE: old_sa = &_cdv1_old_SIGFPE; break;
                    case SIGBUS: old_sa = &_cdv1_old_SIGBUS; break;
                    case SIGILL: old_sa = &_cdv1_old_SIGILL; break;
                }

                if (old_sa != NULL && old_sa->sa_handler != SIG_DFL && old_sa->sa_handler != SIG_IGN) {
                    if ((old_sa->sa_flags & SA_SIGINFO) != 0) {
                        old_sa->sa_sigaction(signal, info, ctx);
                    } else {
                        old_sa->sa_handler(signal);
                    }
                }

                _exit(signal);
            #else /* LDBG_HELP_LEVEL != 0 */
                (void) signal, (void) info, (void) ctx;
            #endif /* LDBG_HELP_LEVEL == 0 */
        }
    #else /* ABPI_PLATFORM_GNU_LINUX == 1 */
        #error "c89/ldbg.h: please, implement my crashfault registration with the support of your platform"
    #endif /* a block, where you can extend crashfault registration for your platform */

    uint64_t*
    _ldbg_get_RESET(void)
    {
        return NULL;
    }

    void
    _cdv1_crashfault_default(uint64_t crashfault)
    {
        #if (LDBG_HELP_LEVEL != 0)
            const char *exception_name = NULL;
            trace89 *scanner = dbgsuite->stack;
            double crashfault_time = 0.0;
            uint8_t id_instead_name = 0;
            int32_t depth = 0, i = 0;
            size_t parent_line = 0;
            char buffer[64];

            if (_cdv1_asf_logout == NULL) {
                return;
            }

            if (dbgsuite == NULL || (dbgsuite->flags & DEBUG89_FLAGS_EXTENDED) == 0) {
                _cdv1_asf_logout(LDBG_CRASHFAULT_BEGIN);
                _cdv1_asf_logout("COULD NOT PRINT TRACE SNAPSHOT DUE TO DEBUG RESOURCES ARE NULL/ NOT EXTENDED\n");
                _cdv1_asf_logout(LDBG_CRASHFAULT_FINAL);

                return;
            }

            if (_cdv1_execution_timer != NULL) {
                crashfault_time = _cdv1_execution_timer() - dbgsuite->start;
            }

            _cdv1_asf_logout(LDBG_CRASHFAULT_BEGIN);
            _cdv1_asf_logout("\nMODULE TRACE SNAPSHOT (STACK & LAST SAVINGS)\n");

            while (scanner != dbgsuite->cursor + 1) {
                _cdv1_asf_logout(" ");

                for (i = 0; i < depth; ++i) {
                    _cdv1_asf_logout(" ");
                }

                if (depth++ != 0) {
                    _cdv1_asf_logout("\\_ ");
                }

                if (scanner == dbgsuite->stack) {
                    parent_line = 0;
                } else {
                    parent_line = (scanner - 1)->line;
                }

                _cdv1_asf_logout(scanner->unit);
                _cdv1_asf_logout("/");
                _cdv1_asf_logout(scanner->func);
                _cdv1_asf_logout(".............");
                sprintf(buffer, "%" FMTSIZE "+", parent_line);
                _cdv1_asf_logout(buffer);
                _cdv1_asf_logout("\n");
                scanner++;
            }
            for (i = 0; i < depth; ++i) {
                _cdv1_asf_logout(" ");
            }

            _cdv1_asf_logout(" \t <");

            #if (ABPI_PLATFORM_WINDOWS_NT == 1)
                if (crashfault == EXCEPTION_ACCESS_VIOLATION) {
                    exception_name = "EXCEPTION_ACCESS_VIOLATION";
                } else if (crashfault == EXCEPTION_EXECUTE_FAULT) {
                    exception_name = "EXCEPTION_EXECUTE_FAULT";
                } else if (crashfault == EXCEPTION_ILLEGAL_INSTRUCTION) {
                    exception_name = "EXCEPTION_ILLEGAL_INSTRUCTION";
                } else if (crashfault == EXCEPTION_INT_DIVIDE_BY_ZERO) {
                    exception_name = "EXCEPTION_INT_DIVIDE_BY_ZERO";
                } else if (crashfault == EXCEPTION_FLT_DENORMAL_OPERAND) {
                    exception_name = "EXCEPTION_FLT_DENORMAL_OPERAND";
                } else if (crashfault == EXCEPTION_FLT_OVERFLOW) {
                    exception_name = "EXCEPTION_FLT_OVERFLOW";
                } else if (crashfault == EXCEPTION_FLT_UNDERFLOW) {
                    exception_name = "EXCEPTION_FLT_UNDERFLOW";
                } else if (crashfault == EXCEPTION_DATATYPE_MISALIGNMENT) {
                    exception_name = "EXCEPTION_DATATYPE_MISALIGNMENT";
                } else if (crashfault == EXCEPTION_IN_PAGE_ERROR) {
                    exception_name = "EXCEPTION_IN_PAGE_ERROR";
                } else if (crashfault == EXCEPTION_FLT_INVALID_OPERATION) {
                    exception_name = "EXCEPTION_FLT_INVALID_OPERATION";
                } else if (crashfault == EXCEPTION_BREAKPOINT) {
                    exception_name = "EXCEPTION_BREAKPOINT";
                } else if (crashfault == EXCEPTION_FLT_INEXACT_RESULT) {
                    exception_name = "EXCEPTION_FLT_INEXACT_RESULT";
                } else if (crashfault == EXCEPTION_GUARD_PAGE) {
                    exception_name = "EXCEPTION_GUARD_PAGE";
                } else {
                    id_instead_name = 1;
                }
            #elif (ABPI_PLATFORM_GNU_LINUX == 1) /* ABPI_PLATFORM_WINDOWS_NT == 1 */
                if (crashfault == SIGSEGV) {
                    exception_name = "SEGMENTATION VIOLATION[SIGSEGV]";
                } else if (crashfault == SIGFPE) {
                    exception_name = "FLOATING-POINT EXCEPTION[SIGFPE]";
                } else if (crashfault == SIGBUS) {
                    exception_name = "MACHINE DATA BUS[SIGBUS]";
                } else if (crashfault == SIGILL) {
                    exception_name = "ILLEGAL INSTUCTION[SIGILL]";
                } else {
                    id_instead_name = 1;
                }
            #else /* ABPI_PLATFORM_GNU_LINUX == 1 */
                #error "c89/ldbg.h: please, implement my default handler with the support of your platform"
            #endif /* not supported platform yet */

            if (id_instead_name == 0) {
                _cdv1_asf_logout(exception_name);
            } else {
                sprintf(buffer, "EXCEPTION[0x%08X", (uint32_t) (crashfault >> 32));
                _cdv1_asf_logout(buffer);
                sprintf(buffer, "%08X]", (uint32_t) crashfault);
                _cdv1_asf_logout(buffer);
            }

            _cdv1_asf_logout(">");
            _cdv1_asf_logout(".............");
            sprintf(buffer, "%" FMTSIZE "+", (scanner - 1)->line);
            _cdv1_asf_logout(buffer);
            _cdv1_asf_logout("\n\nREPORT THIS TO YOUR SOFTWARE DISTRIBUTOR\n");
            _cdv1_asf_logout("COMLILED FOR PLATFORM ");
            sprintf(buffer, "%u BIT: ", ABPI_ARCHITECTURE);
            _cdv1_asf_logout(buffer);
            _cdv1_asf_logout(ABPI_PLATFORM_STRING "\n");
            
            if (_cdv1_execution_timer != NULL) {
                _cdv1_asf_logout("THIS MODULE RAN FOR ");
                sprintf(buffer, "%.3f", crashfault_time);
                _cdv1_asf_logout(buffer);
                _cdv1_asf_logout(" SECONDS\n");
            }

            _cdv1_asf_logout(LDBG_CRASHFAULT_FINAL);
        #else /* LDBG_HELP_LEVEL != 0 */
            (void) crashfault;
        #endif /* LDBG_HELP_LEVEL == 0 */
    }

    void
    _cdv1_crashfault_asafelog(const char *text)
    {
        #if (ABPI_PLATFORM_WINDOWS_NT == 1)
            DWORD written, length;
            const char *symbol = text;

            if (text == LDBG_CRASHFAULT_BEGIN || text == LDBG_CRASHFAULT_FINAL) {
                return;
            }

            for (length = 0; *symbol++ != '\0'; ++length);
            WriteConsole(GetStdHandle(STD_ERROR_HANDLE), text, length, &written, (LPVOID*) &symbol);
        #elif (ABPI_PLATFORM_GNU_LINUX == 1) /* ABPI_PLATFORM_WINDOWS_NT == 1 */
            const char *symbol = text;
            size_t length;

            if (text == LDBG_CRASHFAULT_BEGIN || text == LDBG_CRASHFAULT_FINAL) {
                return;
            }

            for (length = 0; *symbol++ != '\0'; ++length);
            write(2, text, length);
        #else /* ABPI_PLATFORM_GNU_LINUX == 1 */
            #error "c89/ldbg.h: please, implement my default asf-log with the support of your platform"
        #endif /* not supported platform yet */
    }

    const char*
    _cdv1_header_of(trace89 *entry, uint8_t newline, double fixed_time)
    {
        #if (LDBG_HELP_LEVEL != 0)
            int32_t depth = entry - dbgsuite->stack;
            char *writer = dbgsuite->message;
            size_t parent_line = (depth == 0) ? 0 : (entry - 1)->line;
            int32_t i = 0;

            if (newline == 1) {
                *writer++ = '\n';
            }

            if (_cdv1_execution_timer != NULL) {
                writer += sprintf(writer, "%11.3f ", fixed_time);
            }

            if (depth != 0) {
                *writer++ = ' ';
            }

            for (i = 1; i < depth; ++i) {
                writer += sprintf(writer, "|  ");
            }

            if (depth != 0) {
                writer += sprintf(writer, "\\_ ");
            }

            writer += sprintf(writer, "%s/%s.............%" FMTSIZE "+:\n",
                entry->unit, entry->func, parent_line
            );

            *writer = '\0';

            return dbgsuite->message;
        #else /* LDBG_HELP_LEVEL != 0 */
            (void) entry, (void) newline, (void) fixed_time;

            return "";
        #endif /* LDBG_HELP_LEVEL == 0 */
    }

    const char*
    _cdv1_message_of(uint8_t warning, const char *format, va_list *args)
    {
        #if (LDBG_HELP_LEVEL != 0)
            uint8_t outgroup = (dbgsuite->flags & DEBUG89_FLAGS_OUTGROUP) != 0;
            trace89 *entry = dbgsuite->cursor;
            trace89 *stack = dbgsuite->stack;
            char *writer = dbgsuite->message;
            uint8_t newline_required = 0;
            int32_t depth = entry - stack;
            double fixed_time = 0.0;
            trace89 *prev = entry;
            int32_t i = 0;

            if (dbgsuite->message == NULL) {
                return "";
            }

            if (_cdv1_execution_timer != NULL) {
                fixed_time = _cdv1_execution_timer() - dbgsuite->start;
            }

            while (prev != stack && ((prev - 1)->flags & TRACE89_FLAGS_PRINTED) == 0) {
                prev--;
            }

            newline_required = (prev == stack + 1 && (prev->flags & TRACE89_FLAGS_PRINTED) == 0);
            if ((dbgsuite->flags & DEBUG89_FLAGS_OUTGROUP) != 0 && newline_required == 1) {
                dbgsuite->flags &= ~DEBUG89_FLAGS_OUTGROUP;
                outgroup = 0;
            }

            while (prev != entry + 1) {
                if ((prev->flags & TRACE89_FLAGS_PRINTED) == 0) {
                    dbgsuite->log(dbgsuite->out, _cdv1_header_of(prev, newline_required, fixed_time));
                    prev->flags |= TRACE89_FLAGS_PRINTED, newline_required = 0;
                }

                prev++;
            }

            writer = dbgsuite->message;

            if (outgroup == 1) {
                *writer++ = '\n';
            }

            if (_cdv1_execution_timer != NULL) {
                writer += sprintf(writer, "%11.3f ", fixed_time);
            }

            *writer++ = ' ';

            for (i = 0; i < depth; ++i) {
                writer += sprintf(writer, "|  ");
            }

            writer += sprintf(writer, "| %s", (!warning) ? "[?] " : "[!] ");
            writer += vsprintf(writer, format, *args);
            _cdv1_fix(dbgsuite->message + outgroup, dbgsuite->message + outgroup);

            dbgsuite->flags &= ~DEBUG89_FLAGS_OUTGROUP;

            return dbgsuite->message;
        #else /* LDBG_HELP_LEVEL != 0 */
            (void) warning, (void) format, (void) args;
            return "";
        #endif /* LDBG_HELP_LEVEL == 0 */
    }

    void
    _cdv1_platform_log(int level, const char *format, ...)
    {
        #if (ABPI_PLATFORM_ANDROID == 1)
            uint8_t nosuite = dbgsuite == NULL;
            uint8_t nobuffer = nosuite == 0 && (dbgsuite->flags & DEBUG89_FLAGS_NOBUFFER) != 0;
            const char *title = "LDBG.H";
            int priority = 1;
            va_list args;

            if (format == NULL || *format == '\0') {
                return;
            }

            if (level == 1) {
                priority = ANDROID_LOG_INFO;
            } else if (level == 2) {
                priority = ANDROID_LOG_ERROR;
            } else if (level == 3) {
                priority = ANDROID_LOG_DEBUG;
            }

            va_start(args, format);
            if (nosuite == 0) {
                if (nobuffer == 0) {
                    title = (const char*) (*(uintptr_t*) malignto(1, dbgsuite->message, malignof(uintptr_t)));
                    format = _cdv1_fix(dbgsuite->message, format);
                } else {
                    title = (const char*) dbgsuite->message;
                }
            }
            __android_log_vprint(priority, title, format, args);
            va_end(args);
            if (nosuite == 0 && nobuffer == 1) {
                dbgsuite->message = NULL;
            }
        #elif (ABPI_PLATFORM_WINDOWS_NT == 1) /* ABPI_PLATFORM_ANDROID == 1 */
            uint8_t nosuite = dbgsuite == NULL;
            uint8_t nobuffer = nosuite == 0 && ((dbgsuite->flags & DEBUG89_FLAGS_NOBUFFER) != 0);
            char *source = (nosuite == 0) ? dbgsuite->message : NULL;
            const char *title = "LDBG.H";
            uint8_t tmp_allocated = 0;
            char *writer = source;
            va_list args;

            if (format == NULL || *format == '\0') {
                return;
            }

            va_start(args, format);
            if (nosuite == 0) {
                if (nobuffer == 0) {
                    title = (const char*) (*(uintptr_t*) malignto(1, dbgsuite->message, malignof(uintptr_t)));
                } else {
                    title = (const char*) dbgsuite->message;
                }
            }
            if (level == 1) {
                fprintf(stdout, "%s: ", title);
                vfprintf(stdout, format, args);
                if (*(format + strlen(format) - 1) != '\n') {
                    fprintf(stdout, "\n");
                }
            } else if (level == 2) {
                fprintf(stderr, "%s: ", title);
                vfprintf(stderr, format, args);
                if (*(format + strlen(format) - 1) != '\n') {
                    fprintf(stderr, "\n");
                }
            } else if (level == 3) {
                if (nosuite == 1 || nobuffer == 1) {
                    /* i am using malloc here because it is Windows */
                    /* on Embedded you would implement your safe logic */
                    char *tmp_buffer = malloc(LDBG_BUFFER_SIZE);
                    writer = source = tmp_buffer;
                    tmp_allocated = 1;
                }

                if (writer != NULL) {
                    writer += sprintf(writer, "%s", title), *writer++ = ' ';
                    writer += vsprintf(writer, format, args);
                    if (*(writer - 1) != '\n') {
                        *writer++ = '\n', *writer = '\0';
                    }
                    source = _cdv1_fix(source, source);
                    OutputDebugString(source);
                }

                if (tmp_allocated == 1 && source != NULL) {
                    free(source);
                }
            }
            va_end(args);
            if (nosuite == 0 && nobuffer == 1) {
                dbgsuite->message = NULL;
            }
        #elif (ABPI_PLATFORM_GNU_LINUX == 1) /* ABPI_PLATFORM_WINDOWS_NT == 1 */
            uint8_t nosuite = dbgsuite == NULL;
            uint8_t nobuffer = nosuite == 0 && (dbgsuite->flags & DEBUG89_FLAGS_NOBUFFER) != 0;
            char *source = (nosuite == 0) ? dbgsuite->message : NULL;
            static uint8_t daemon_opened = 0;
            const char *title = "LDBG.H";
            uint8_t tmp_allocated = 0;
            char *writer = source;
            va_list args;

            if (format == NULL || *format == '\0') {
                return;
            }

            va_start(args, format);
            if (nosuite == 0) {
                if (nobuffer == 0) {
                    title = (const char*) (*(uintptr_t*) malignto(1, dbgsuite->message, malignof(uintptr_t)));
                } else {
                    title = (const char*) dbgsuite->message;
                }
            }
            if (level == 1) {
                fprintf(stdout, "%s: ", title);
                vfprintf(stdout, format, args);
                if (*(format + strlen(format) - 1) != '\n') {
                    fprintf(stdout, "\n");
                }
            } else if (level == 2) {
                fprintf(stderr, "%s: ", title);
                vfprintf(stderr, format, args);
                if (*(format + strlen(format) - 1) != '\n') {
                    fprintf(stderr, "\n");
                }
            } else if (level == 3) {
                if (daemon_opened == 0) {
                    openlog("LDBG.H", LOG_PID | LOG_NOWAIT, LOG_USER);
                    daemon_opened = 1;
                }

                if (nosuite == 1 || nobuffer == 1) {
                    /* i am using malloc here because it is Linux (not RPi) */
                    /* on Embedded/others you would implement your safe logic */
                    char *tmp_buffer = malloc(LDBG_BUFFER_SIZE);
                    writer = source = tmp_buffer;
                    tmp_allocated = 1;
                }

                if (writer != NULL) {
                    writer += sprintf(writer, "%s", title), *writer++ = ' ';
                    writer += vsprintf(writer, format, args);
                    if (*(writer - 1) != '\n') {
                        *writer++ = '\n', *writer++ = '\0';
                    }
                    source = _cdv1_fix(source, source);
                    syslog(LOG_DEBUG, "%s", source);
                }

                if (tmp_allocated == 1 && source != NULL) {
                    free(source);
                }
            }
            va_end(args);
            if (nosuite == 0 && nobuffer == 1) {
                dbgsuite->message = NULL;
            }
        #else /* ABPI_PLATFORM_GNU_LINUX == 1 */
            #error "c89/ldbg.h: please, implement my dbglog with the support of your platform"
        #endif /* not supported platform log yet */
    }
#endif /* CEN_IMPLEMENT && !CEN_IMPLEMENTED_LDBG_H */


#undef LDBG_AVOID_CONFIG


/*
    To/do:
            1) Replace <stdio.h>'s not async-signal safe functions
*/
