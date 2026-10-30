typedef struct DestructorNode {
    struct DestructorNode *next;
    void (*destructor)(void *, int);
    void *object;
} DestructorNode;
DestructorNode *__global_destructor_chain;
void __destroy_global_chain(void)
{
    DestructorNode *node;
    while ((node = __global_destructor_chain) != 0) {
        __global_destructor_chain = node->next;
        node->destructor(node->object, -1);
    }
}
