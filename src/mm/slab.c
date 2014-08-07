#include <mm/slab.h>
#include <mm/page.h>
#include <lib/string.h>
#include <lib/align.h>
#include <stddef.h>


struct kmem_cache
{
    char name[32];
    size_t size;
    size_t alignment;
    size_t object_size;
    size_t pages_per_slab;
};


struct kmem_cache root;


static void setup_cache(struct kmem_cache *cache, const char *name,
                        size_t size, size_t alignment)
{
    strncpy(cache->name, name, sizeof(cache->name) / sizeof(*cache->name) - 1);
    cache->size = size;
    cache->alignment = alignment;
    if (size < sizeof(void *))
    {
        size = sizeof(void *);
    }
    cache->object_size = ALIGN(size, alignment);
    cache->pages_per_slab = 2; // TODO
}


void kmem_cache_init(void)
{
    setup_cache(&root, "root", sizeof(struct kmem_cache),
                sizeof(struct kmem_cache));
}


struct kmem_cache *kmem_cache_create(const char *name, size_t size,
                                     size_t alignment)
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

