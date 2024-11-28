//defalloc.h
#ifndef DEFALLOC_H
#define DEFALLOC_H
#include <new>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include </usr/include/c++/11/parallel/algobase.h>

namespace defalloc{
    using namespace std;
template <class T> inline T* allocate(ptrdiff_t size, T*){
    set_new_handler(0);
    T* tmp = (T*)(::operator new((size_t)(size*sizeof(T))));
    if(tmp==0)
        std::cerr<<"out of memory"<<std::endl;
    return tmp;
}

template <class T> inline void deallocate(T* buffer){
    ::operator delete(buffer);
}

template <class T> class allocator{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    pointer allocate(size_type n, const void* hint=0){
        return defalloc::allocate((difference_type)n, (pointer)0);
    }

    void deallocate(pointer p, size_type n){
        defalloc::deallocate(p);
    }

    pointer address(reference x) const{
        return (pointer)&x;
    }

    const_pointer address(const_reference x) const{
        return (const_pointer)&x;
    }

    size_type max_size() const{
        return size_type(UINT_MAX/sizeof(T));
    }

    void construct(pointer p, const T& value){
        new(p) T(value);
    }
};

template <> class allocator<void>{
public:
    typedef void* pointer;
};
}
#endif