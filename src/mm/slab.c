#include <mm/slab.h>
#include <stddef.h>


struct kmem_cache
{
    char name[32];
    size_t size;
    size_t alignment;
    void (*ctor)(void *, size_t);
    void (*dtor)(void *, size_t);
    struct kmem_cache *next;
};


struct kmem_cache root;


struct kmem_cache *kmem_cache_create(const char *name, size_t size,
                                     size_t alignment,
                                     void (*ctor)(void *, size_t),
                                     void (*dtor)(void *, size_t))
{
    // TODO
    return NULL;
}


void kmem_cache_destroy(struct kmem_cache *cache)
{
}


void *kmem_cache_alloc(struct kmem_cache *cache, uint32_t flags)
{
    // TODO
    return NULL;
}


void kmem_cache_free(struct kmem_cache *cache, void *object)
{
    // TODO
}

