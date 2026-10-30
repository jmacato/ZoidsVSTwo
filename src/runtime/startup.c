typedef void (*Constructor)(void);
extern Constructor _ctors[];
extern void PPCHalt(void);
extern void __init_cpp(void);
#pragma peephole off
void __init_user(void)
{
    __init_cpp();
}
void __init_cpp(void)
{
    Constructor *constructor;
    for (constructor = _ctors; *constructor; constructor++) {
        (*constructor)();
    }
}
void _ExitProcess(void)
{
    PPCHalt();
}
