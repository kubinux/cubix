#include <mm/slab.h>


struct kmem_cache
{
    char const *name;
};


struct kmem_cache *kmem_cache_create(const char *name, size_t size,
                                     size_t alignment,
                                     void (*ctor)(void *, size_t),
                                     void (*dtor)(void *, size_t));


void kmem_cache_destroy(struct kmem_cache *cache);


void *kmem_cache_alloc(struct kmem_cache *cache, uint32_t flags);


void kmem_cache_free(struct kmem_cache *cache, void *object);

