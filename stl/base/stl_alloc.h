//stl_alloc.h
#ifndef STL_ALLOC_H
#define STL_ALLOC_H

#include <iostream>
#include <cstdlib>
#include <new>

#ifndef __USE_MALLOC
#define __USE_MALLOC 0  // 默认不使用 malloc_alloc
#endif

#ifndef __NODE_ALLOCATOR_THREADS
#define __NODE_ALLOCATOR_THREADS 1  // 默认使用线程安全的分配器
#endif

#if 0
#include <new>
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#define __THROW_BAD_ALLOC do { std::cerr << "out of memory" << std::endl; exit(1); } while(0)
#endif

// malloc-based allocator
// tier-one allocator

template <int inst> class __malloc_alloc_template
{
    private:
    //private member functions
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    //this is a member variable that needs to be initialized by the user
    static void (*__malloc_alloc_oom_handler)();//a static void pointer that points to a function
   

    public:
    static void *allocate(size_t n)
    {
        void *result = malloc(n);
        if (0 == result) result = oom_malloc(n);
        return result;
    }

    static void deallocate(void *p, size_t /*n*/)
    {
        free(p);
    }
    //'allocate' warps around malloc(), and 'deallocate' wraps around free()
    static void *reallocate(void *p, size_t /*old_sz*/, size_t new_sz)
    {
        void *result = realloc(p, new_sz);
        if (0 == result) result = oom_realloc(p, new_sz);
        return result;
    }

    static void (*set_malloc_handler(void (*f)()))()
    {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return (old);
    }

};

template <int inst> void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template <int inst>
void *__malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    void (*my_malloc_handler)();
    void *result;
    for (;;)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) __THROW_BAD_ALLOC;
        (*my_malloc_handler)();
        result = malloc(n);
        if (result) return (result);
    }
}    

template <int inst>
void *__malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
{
    void (*my_malloc_handler)();
    void *result;
    for (;;)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) __THROW_BAD_ALLOC;
        (*my_malloc_handler)();
        result = realloc(p, n);
        if (result) return (result);
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;

enum {__ALIGN = 8};
enum {__MAX_BYTES = 128};
enum {__NFREELISTS = __MAX_BYTES / __ALIGN};
template <bool threads, int inst> class __default_alloc_template
{
    private:
        static size_t ROUND_UP(size_t __bytes)
        { return (__bytes + __ALIGN - 1) & ~(__ALIGN - 1); }
        //rounds up to the next multiple of __ALIGN using bitwise AND

        union obj {
            union obj* free_list_link;
            char client_data[1];
        };

        static obj* volatile __NFreelist[__NFREELISTS];//use 'volatile' for thread safety

        static size_t FREELIST_INDEX(size_t bytes)
        {
            return (((bytes) + __ALIGN - 1)/__ALIGN - 1);
        }

        static void* refill(size_t n);
        static char* chunk_alloc(size_t size, int& n_objs);
        static char* start_free;
        static char* end_free;
        static size_t heap_size;

    public:
        static void* allocate(size_t n);
        static void deallocate(void* p, size_t n);
        static void* reallocate(void* p, size_t old_sz, size_t new_sz);
};

//initializing start_free, end_free and heap_size to 0, and a all-zero freelist
template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;
template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;
template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template <bool threads, int inst> 
typename __default_alloc_template<threads, inst>::obj* volatile
__default_alloc_template<threads, inst>::__NFreelist[__NFREELISTS] = {0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0};


template <bool threads, int inst> void *
__default_alloc_template<threads, inst>::allocate(size_t n) 
{
    obj* volatile * my_free_list;
    obj* result;
    if (n > (size_t)__MAX_BYTES) return (malloc_alloc::allocate(n));
    my_free_list = __NFreelist + FREELIST_INDEX(n);
    result = *my_free_list;
    if (result == 0)
    {
        void* r = refill(ROUND_UP(n));
        return r;
    }
    *my_free_list = result->free_list_link;
    return result;
}

template <bool threads, int inst> void
__default_alloc_template<threads, inst>::deallocate(void* p, size_t n)
{
    obj* q = (obj*)p;
    obj* volatile * my_free_list;

    if (n > (size_t)__MAX_BYTES)
    {
        malloc_alloc::deallocate(p, n);
        return;
    }
    my_free_list = __NFreelist + FREELIST_INDEX(n);
    q->free_list_link = *my_free_list;
    *my_free_list = q;
}

template <bool threads, int inst> 
void* __default_alloc_template<threads, inst>::refill(size_t n)
{
    int nobjs = 20;
    char* chunk = chunk_alloc(n, nobjs);
    obj* volatile * my_free_list;
    obj* current_obj;
    obj* next_obj;
    int i;

    if (1 == nobjs) return chunk;
    my_free_list = __NFreelist + FREELIST_INDEX(n);
    current_obj = (obj*)chunk;
    *my_free_list = next_obj = (obj*)(chunk + n);
    for (i= 1; ; ++i)
    {
        current_obj->free_list_link = next_obj;
        current_obj = next_obj;
        if (nobjs - 1 == i)
        {
            current_obj->free_list_link = 0;
            break;
        }
        else next_obj = (obj*)((char*)next_obj + n);
    }
    return chunk;
}
//fetch space from memory pool and construct a free list
template <bool threads, int inst> char*
__default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& n_objs)
{
    char* result;
    size_t total_bytes = size * n_objs;
    size_t bytes_left = end_free - start_free;
    if (bytes_left >= total_bytes)//case1: enough space in the pool
    {
        result = start_free;
        start_free += total_bytes;
        return result;//exit of case1
    }
    else if (bytes_left >= size)//case2: not enough space but enough for >=1 chuncks
    {
        n_objs = bytes_left/size;
        total_bytes = n_objs*size;
        result = start_free;
        start_free += total_bytes;
        return result;//exit of case2
    }
    else//case3: not enough space and not enough for >=1 chuncks
    {
        size_t bytes_to_get = 2*total_bytes + ROUND_UP(heap_size >> 4);//heurisitcally get more space than we need
        if (bytes_left > 0)//there is something in the pool. bytes_left = end_free - start_free
        {
            //put the rest back in the free list
            obj* volatile * my_free_list = __NFreelist + FREELIST_INDEX(bytes_left);
            ((obj*)start_free)->free_list_link = *my_free_list;
            *my_free_list = (obj*)start_free;
        
        }
        //get more memory from the memory pool
        start_free = (char*)malloc(bytes_to_get);
        if (0 == start_free)//if malloc fails, search a free list to provide the space
        {
            int i;
            obj* volatile * my_free_list;
            obj* p;
            for (i=size; i <= __MAX_BYTES; i += __ALIGN)//__MAX_BYTES = 128, __ALIGN = 8
            {
                my_free_list = __NFreelist + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p)//found a chunk.
                //we have found a none-empty, big enough chunk
                {
                    *my_free_list = p->free_list_link;
                    start_free = (char*)p;
                    end_free = start_free + bytes_left;//set end and start to the new chunk
                    return chunk_alloc(size, n_objs);
                }
            }
            //no chunk found
            end_free = 0;
            start_free = (char*)malloc_alloc::allocate(bytes_to_get);//use the first tier allocator
        }
        //get more memory from the memory pool
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, n_objs);//(recursive) exit of case3
    }
};


// Default allocator type definition
#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc;
#endif

template <class T, class Alloc> class simple_alloc{
    public:
        static T* allocate(size_t n)
        {
            return 0 == n ? 0 : (T*)Alloc::allocate(n*sizeof(T));
        }
        static T* allocate()
        {
            return (T*)Alloc::allocate(sizeof(T));
        }
        static void deallocate(T* p, size_t n)
        {
            if (0 != n) Alloc::deallocate(p, n*sizeof(T));
        }
};

#endif // STL_ALLOC_H