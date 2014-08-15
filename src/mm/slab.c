#include <mm/mm.h>
#include <mm/paging.h>
#include <mm/slab.h>
#include <mm/page.h>
#include <mm/phys_allocator.h>
#include <lib/string.h>
#include <lib/unused.h>
#include <lib/printf.h>
#include <lib/assert.h>
#include <lib/align.h>
#include <lib/list.h>
#include <stddef.h>


declare_list_head(page_list, struct page, list);


struct kmem_cache
{
    char name[32];
    size_t size;
    size_t alignment;
    size_t object_size;
    size_t num_objects;
    size_t pages_per_slab;
    struct page_list partial_slabs;
};


static struct kmem_cache root;


size_t get_wasted_mem_percent(size_t object_size, size_t num_pages)
{
    size_t slab_size = num_pages * PAGE_SIZE;
    size_t wasted_mem = slab_size % object_size;

    size_t val = (wasted_mem * 1000000) / slab_size;
    if (val % 10000 >= 5000)
    {
        val += 10000;
    }
    val /= 10000;
    return val;
}


static size_t get_pages_per_slab(size_t object_size)
{
    size_t num_pages = ALIGN(object_size, PAGE_SIZE) / PAGE_SIZE;
    size_t wasted_mem_pct = get_wasted_mem_percent(object_size, num_pages);
    while (wasted_mem_pct > 10 && num_pages < 16)
    {
        ++num_pages;
        wasted_mem_pct = get_wasted_mem_percent(object_size, num_pages);
    }
    return num_pages;
}


static void kmem_cache_ctor(struct kmem_cache *cache, const char *name,
                            size_t size, size_t alignment)
{
    strncpy(cache->name, name, sizeof(cache->name) / sizeof(*cache->name) - 1);
    cache->size = size;
    if (alignment < sizeof(void *))
    {
        alignment = sizeof(void *);
    }
    cache->alignment = alignment;
    cache->object_size = ALIGN(size, alignment);
    cache->pages_per_slab = get_pages_per_slab(cache->object_size);
    cache->num_objects =
        cache->pages_per_slab * PAGE_SIZE / cache->object_size;
    list_init_head(&cache->partial_slabs);
}


static void kmem_cache_dtor(struct kmem_cache *cache UNUSED)
{
    // TODO: check if all objects have been freed
}


void kmem_cache_init(void)
{
    kmem_cache_ctor(&root, "root", sizeof(struct kmem_cache),
                    sizeof(struct kmem_cache));
}


struct kmem_cache *kmem_cache_create(const char *name, size_t size,
                                     size_t alignment)
{
    struct kmem_cache *new_cache = kmem_cache_alloc(&root, 0);
    kmem_cache_ctor(new_cache, name, size, alignment);
    return new_cache;
}


void kmem_cache_destroy(struct kmem_cache *cache)
{
    kmem_cache_dtor(cache);
    kmem_cache_free(&root, cache);
}


static void add_free_object(struct page *page, void *object)
{
    *(void **)object = page->free_objects;
    page->free_objects = object;
    --page->num_allocated;
}


static void *get_free_object(struct page *page)
{
    void *object = page->free_objects;
    page->free_objects = *(void **)object;
    ++page->num_allocated;
    return object;
}


static void init_slab(struct page *page, struct kmem_cache *cache)
{
    page->flags = PG_HEAD;
    page->slab_cache = cache;
    page->num_allocated = cache->num_objects;
    page->free_objects = NULL;
    void *object = (void *)mm_page_to_virt(page);
    for (size_t i = 0; i < cache->num_objects;
         ++i, object += cache->object_size)
    {
        add_free_object(page, object);
    }
    for (size_t idx = 1; idx < cache->pages_per_slab; ++idx)
    {
        page[idx].flags = PG_CONT;
        page[idx].head_page = page;
    }
}


static struct page *make_new_slab(struct kmem_cache *cache)
{
    struct page *page = alloc_pages(cache->pages_per_slab);
    if (page)
    {
        init_slab(page, cache);
    }
    return page;
}


void *kmem_cache_alloc(struct kmem_cache *cache, uint32_t flags UNUSED)
{
    struct page *page = list_first(&cache->partial_slabs);
    if (!page)
    {
        page = make_new_slab(cache);
        if (!page)
        {
            return NULL;
        }
        list_append(&cache->partial_slabs, page);
    }
    void *result = get_free_object(page);
    if (page->num_allocated == cache->num_objects)
    {
        list_remove(&cache->partial_slabs, page);
    }
    return result;
}


static inline struct page *get_head_page(struct page *page)
{
    if (page->flags & PG_CONT)
    {
        page = page->head_page;
    }
    ASSERT_MSG(page->flags & PG_HEAD, "invalid slab page flags");
    return page;
}


void kmem_cache_free(struct kmem_cache *cache, void *object)
{
    struct page *page = mm_page_from_virt((uintptr_t)object);
    page = get_head_page(page);
    ASSERT_MSG(cache == page->slab_cache,
               "attemp to free object from another cache");
    add_free_object(page, object);
    if (page->num_allocated == 0)
    {
        list_remove(&cache->partial_slabs, page);
        free_pages(page, cache->pages_per_slab);
    }
    else if (page->num_allocated == cache->num_objects - 1)
    {
        list_append(&cache->partial_slabs, page);
    }
}


void kmem_cache_print_info(struct kmem_cache *cache)
{
    printf("Object size: %lu\n", cache->object_size);
    printf("Num pages:   %lu\n", cache->pages_per_slab);
    printf("Wasted mem:  %lu\n",
           get_wasted_mem_percent(cache->object_size, cache->pages_per_slab));
}

