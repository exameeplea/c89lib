<b>LDBG.H: Lightweight Debugging</b> <br>
<br>
You are about to transform source texts into controllable debug contents. To do that, you are putting ```dbgfnjmp()``` and ```dbgfnret()``` 
at the beggining/ending of your functions to form the shadow call stack. You are also interested in to make subprograms self-documented,
so you putting ```dbgfninf("Created toolbox window <%p> with %d widgets\n", window, widgets_amount)``` or 
```dbgfnwrn("Could not initialize IUP subsystem. Exiting..\n")``` to output their information/warning. Filtering feature you probably
was ever needed in will allow you to mute irrelevant outputs flexibly in run-time by just one change. To make it real, you assign
every function to a combination of logical meanings in many different masks with ```dbgfnmsk(VMSS_DEBUG_TYPE, ALLOCATIONS)``` and 
*dbgfnmsk(VMSS_DEBUG_STRATEGY, STOCK)*. In any part of program you settuping ```dbgmsk(VMSS_DEBUG_TYPE) &= ~VMSS_DEBUG_TYPE_ALLOCATIONS```
just to mute all further outputs about allocations in all strategies. Being a library's developer you would like to provide library's LDBG.H
filtering masks. So you putting ```dbgmskdecl(LIB_API, VMSS_DEBUG_TYPE)``` and *dbgmskdecl(LIB_API, VMSS_DEBUG_STRATEGY)* in the header
to declare public masks VMSS_DEBUG_TYPE and VMSS_DEBUG_STRATEGY respectively. While typing the ```dbgmskoptn(VMSS_DEBUG_TYPE, ALLOCATIONS, 0)```
you create an option, the logical meaning of allocations for mask VMSS_DEBUG_TYPE at first bit client developer can use in mute manipulations. To make
it real and shareable, you have to put ```dbgmskimpl(VMSS_DEBUG_TYPE)``` in one translation unit for LDBG.H's mask fetchers. Now you need to
prepare your program for debugging. Somewhere you define the shadow stack source ```trace89 callstack[16]``` for example with 16 call depth max.
After, you define the message format buffer *char msgbuf[128]* for 128 and less long outputs. Now you have to define debug resources and
specify the logging function to use: ```debug89 utils = dbgdef(&fprintf, stdout)```. Finally, you load these debug resources into module's thread by
using ```dbgload(utils, callstack, msgbuf, NULL)```. The last arg is a timer function LDBG.H would use to profile the outputs in millisecond precision.
Thus, your thread is now accessible to execute further debugging commands. As you know, to configure the LDBG.H of others .DLL/.SO modules, you have to
call their exported something like *mylib_configure_ldbg(void \*suite)* function and pass ```dbgsuite```, a pointer of thread's loaded debug resources.
In this function they have to load it into a module's thread context by *dbgload*, carefully preserving the *dbgsuite->stack* if *dbgsuite->flags* 
have got ```DEBUG89_FLAGS_EXTENDED``` version, and the *dbgsuite->message* buffer. Of course you can override it if you will. Now, when you manually
instrumented your source texts it would seem you became a LDBG.H cursed hostage. But it is not, because you can fully control the usage by
specifying ```LDBG_HELP_LEVEL```. In level 0 of help, there is no shadow call stack leading, no I/O, no filtering and no usage of provided buffers (so,
the compiler would carve them out as useless), but a minimal version of resources with *dbgsuite->error* available, passable to modules compiled
with different help level without any crashfaults. In level 1 of help, there is a shadow call stack leading with base logging into platform channels via
```dbglog(level, "Hello, World")```, where level is: ```LDBG_INFO``` (Windows/Linux: stdout, Android: logcat[info]), ```LDBG_ERROR``` (Windows/Linux
stderr, Android: logcat[error]), ```LDBG_DEBUG``` (Windows: debugger, Linux: syslog, Android: logcat[debug]), all function outputs are replaced with
lightest ```dbgfnsav()``` macroinstruction, which saves latest checkpoint (line number) in call entry. In level 2 of help, there is pretty tree-like
printing of call stack in run-time, filterable by masks with deferred parent headers printing (even muted), but without base logging. You control output's
prefix by specifying ```LDBG``` with preffered string literal, default is translation unit filename. You also can enable assembly guaranteed labels for
function borders in extended disassembler viewer by specifying ```LDBG_SYMBOLS_DB``` with 1. Now, to make it all worthy you setting up an unexpected
crashfault handler for a proccess via ```dbginstall(NULL, NULL)```. First argument is a function called for fully exception handling. Second argument is
a function for async-safe designed default contents logging in desired way (in dialog, in custom file, with additional data for example). Last function
receive unique pointers ```LDBG_CRASHFAULT_BEGIN``` before default contents will be sent and ```LDBG_CRASHFAULT_FINAL``` after contents are sent. In
other time, there a strings of default content sent. **Congratulations, you have learned this library fully! Feel free to leave an error/idea reports.** <br>
<br>
For commerical (and not) module developers, there an utility capable enough to help protecting the work against reverse-engineers. Firstly, it can patch
read only section of executable format where are string literals of human-readable function names are stored with an untilligent jumble of symbols exactly
same length long (to avoid ruining offsets) with forming mapped *symbols.ldb* (or another called) file for real function names. Secondly, it can modify
source texts proceed after preprocessor and replace function names with sequencial ID stored in mapped *symbols.ldb* file for real function names (it could
save memory a lot, for example, 100'000'000 functions will take ~ 381 MB metadata instead of ~ 1.9 GB, but for small projects the difference is not big). And
thirdly, it could be a static analyzer for missing *dbgfnjmp()*/*dbgfnret()* pairs with auto-fix or developer notifying. **Work in progress, please wait.**
