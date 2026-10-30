#pragma peephole off
#pragma pool_data off
typedef struct FragmentInfo {
    void *exception_table;
    void *toc;
    int active;
} FragmentInfo;
static FragmentInfo fragmentinfo[1];
void __unregister_fragment(int fragment)
{
    if (fragment >= 0 && fragment < 1) {
        fragmentinfo[fragment].exception_table = 0;
        fragmentinfo[fragment].toc = 0;
        fragmentinfo[fragment].active = 0;
    }
}
int __register_fragment(void *exception_table, void *toc)
{
    int fragment;
    FragmentInfo *entry;
    for (fragment = 0, entry = fragmentinfo; fragment < 1; fragment++, entry++) {
        if (!entry->active) {
            entry->exception_table = exception_table;
            entry->toc = toc;
            entry->active = 1;
            return fragment;
        }
    }
    return -1;
}
