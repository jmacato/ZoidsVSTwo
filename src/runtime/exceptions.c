#pragma peephole off
extern void __unregister_fragment(int fragment);
extern int __register_fragment(void *exception_table, void *toc);
extern void *GetR2__Fv(void);
extern unsigned char _eti_init_info[];
static int fragmentID = -2;
void __fini_cpp_exceptions(void)
{
    if (fragmentID != -2) {
        __unregister_fragment(fragmentID);
        fragmentID = -2;
    }
}
void __init_cpp_exceptions(void)
{
    if (fragmentID == -2)
        fragmentID = __register_fragment(_eti_init_info, GetR2__Fv());
}
