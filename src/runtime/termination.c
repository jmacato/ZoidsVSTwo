#pragma pool_data off
typedef void (*ExitCallback)(void);
int __aborting = 0;
static ExitCallback atexit_funcs[64];
static int atexit_curr_func = 0;
static ExitCallback __atexit_funcs[64];
static int __atexit_curr_func = 0;
ExitCallback __stdio_exit = 0;
ExitCallback __console_exit = 0;
extern ExitCallback _dtors[];
extern void __destroy_global_chain(void);
extern void __kill_critical_regions(void);
extern void _ExitProcess(void);
void exit(int status)
{
    ExitCallback *destructor;
    if (!__aborting) {
        while (atexit_curr_func > 0)
            atexit_funcs[--atexit_curr_func]();
        __destroy_global_chain();
        for (destructor = _dtors; *destructor; destructor++)
            (*destructor)();
        if (__stdio_exit) {
            __stdio_exit();
            __stdio_exit = 0;
        }
    }
    while (__atexit_curr_func > 0)
        __atexit_funcs[--__atexit_curr_func]();
    __kill_critical_regions();
    if (__console_exit) {
        __console_exit();
        __console_exit = 0;
    }
    _ExitProcess();
}
