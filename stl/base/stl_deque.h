//filename: deque.h
#ifndef STL_DEQUE_H
#define STL_DEQUE_H
#include "stl_iterator.h"
#include "stl_alloc.h"
#include "stl_construct.h"
#include "stl_uninitialized.h"
#include <cstddef>//ptrdiff_t
#include <algorithm>//max
#include <iostream>
inline size_t __deque_buf_size(size_t n, size_t sz) {
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template <class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator{    
    typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef random_access_iterator_tag iterator_category;
    typedef T** map_pointer;
    typedef __deque_iterator self;//an instantiated template class

    static size_type buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }
//if not inline, __deque_buf_size must be defined before buffer_size
    //members
    T* cur;
    T* first;
    T* last;
    T** node;
    __deque_iterator(T* x, T** y)
        : cur(x), first(*y), last(*y + buffer_size()), node(y) {}
    __deque_iterator(): cur(0), first(0), last(0), node(0) {}
    __deque_iterator(const iterator& x)
        : cur(x.cur), first(x.first), last(x.last), node(x.node) {}
    //template <class U> __deque_iterator(const __deque_iterator<T, U, Ptr, BufSiz>& x)
    //    : cur(x.cur), first(x.first), last(x.last), node(x.node) {}
    
    self& operator=(const self& x) {
        if (this != &x) {
            cur = x.cur;
            first = x.first;
            last = x.last;
            node = x.node;
        }
        return *this;
    }

    void set_node(map_pointer new_node) {
        //assert(new_node >= map && new_node < map + map_size);
        node = new_node;
        first = *new_node;
        last = first + difference_type(buffer_size());
    }
    //operators
    reference operator*() const {return *cur;}
    pointer operator->() const {return &(operator*());}
    difference_type operator-(const self& x) const {
        return difference_type(buffer_size()) * (node - x.node - 1)+(cur - first)+(x.last - x.cur);

    } 
    self& operator++() {
        ++cur;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--() {
        --cur;
        if (cur == first) {
            set_node(node - 1);
            cur = last;
        }
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }    

    //+=, -= return a revised object.
    //+, - return a new object.
    self& operator+=(difference_type n) {
        difference_type offset = n + (cur - first);
        if (offset >= 0 && offset < difference_type(buffer_size()))
            cur += n;
        else {
            difference_type node_offset = 
                offset > 0 ? 
                    offset / difference_type(buffer_size())  
                    : -difference_type((-offset - 1) / buffer_size()) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * difference_type(buffer_size()));
        }
        return *this;
    }
    self operator+(difference_type n) const {
        self tmp = *this;
        return tmp += n;
    }
    self& operator-=(difference_type n) {
        return *this += -n;
    }
    self& operator-(difference_type n) const {
        self tmp = *this;
        return tmp -= n;
    }

    //random access
    reference operator[](difference_type n) const {
        return *(*this + n);
    }
    bool operator==(const self& x) const {return cur == x.cur;}
    //cur is the most accurate location measure. 
    //As long as cur is fixed, all of other items are fixed.
    bool operator!=(const self& x) const {return !(*this == x);}
    bool operator<(const self& x) const {
        return (node == x.node) ? (cur < x.cur) : (node < x.node);
    }//so that we can use cur<=finish semantics in a for-loop
};

template <class T, class Ref, class Ptr, size_t BufSiz>
inline __deque_iterator<T, Ref, Ptr, BufSiz>
operator+(ptrdiff_t __n, const __deque_iterator<T,Ref,Ptr,BufSiz>& __x)
{
  return __x + __n;
}

template <class T, class Ref, class Ptr, size_t BufSiz>
inline random_access_iterator_tag
iterator_category(const __deque_iterator<T,Ref,Ptr,BufSiz>&)
{
  return random_access_iterator_tag();
}

template <class T, class Ref, class Ptr, size_t BufSiz>
inline T* value_type(const __deque_iterator<T,Ref,Ptr,BufSiz>&) { return 0; }
template <class T, class Ref, class Ptr, size_t BufSiz>
inline ptrdiff_t* distance_type(const __deque_iterator<T,Ref,Ptr,BufSiz>&) { return 0; }

template <class T, class Alloc = alloc, size_t BufSiz = 0>
class deque{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef Alloc allocator_type; 
    typedef __deque_iterator<T,T&,T*,BufSiz>             iterator;
    typedef __deque_iterator<T,const T&,const T*,BufSiz> const_iterator;
    typedef T** map_pointer;
    map_pointer map;//T**
    size_type map_size;  
    iterator start;//T*
    iterator finish;//T*
    //iterator memebers: cur, first, last, node

public: 
    iterator begin() { return start; }
    iterator end() { return finish; }
    const_iterator begin() const { return start; }
    const_iterator end() const { return finish; }
    static size_type buffer_size() { return __deque_buf_size(BufSiz, sizeof(value_type)); }

    reference operator[](size_type n) {return start[difference_type(n)];}
    const_reference operator[](size_type n) const {return start[difference_type(n)];}
    reference front() {return *start;}
    reference back() {
        iterator tmp = finish;
        --tmp;
        return *tmp;
    }
    const_reference front() const {return *start;}
    const_reference back() const {
        const_iterator tmp = finish;
        --tmp;
        return *tmp;
    }
    size_type size() const {return finish - start;}
    size_type max_size() const {return size_type(-1);}
    bool empty() const {return finish == start;}
    size_type get_map_size() const { return map_size; }

protected:
    typedef simple_alloc<value_type, Alloc> data_allocator;
    typedef simple_alloc<pointer, Alloc> map_allocator;
    static size_type initial_map_size() { return 8; } 
    pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
    void deallocate_node(pointer p) { data_allocator::deallocate(p, buffer_size()); }
    void __initialize(size_t __n, const T& __value);
    void __initialize();

    inline void fill_initialize(int n, const value_type& value){
        create_map_and_nodes(n);
        map_pointer mcur;
        __STL_TRY { /*try*/
            for (mcur = start.node; mcur < finish.node; ++mcur)
                uninitialized_fill_n(*mcur, buffer_size(), value);
            uninitialized_fill_n(finish.first, finish.cur - finish.first, value);
        }
        catch(...){
            /*commit or rollback semantics*/
        destroy(start, finish);
        // release data_allocator 
        // for (mcur = start.node; mcur < finish.node; ++mcur) {
        //     data_allocator::deallocate(*mcur, buffer_size());
        // }
        // no need for releasing map_allocator
        throw;
        }
    }
public: 
    inline void create_map_and_nodes(size_type num_elements){
        std::cout << "create_map_and_nodes is called" << std::endl;
        size_type num_nodes = num_elements / buffer_size() + 1;
        map_size = std::max(initial_map_size(), num_nodes + 2);
        map = map_allocator::allocate(map_size);
        std::cout << "allocated map size by create_map_and_nodes: " << map_size << std::endl;
        map_pointer nstart = map + (map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;
        map_pointer cur;
        __STL_TRY { 
            for (cur = nstart; cur <= nfinish; ++cur)
                *cur = allocate_node();
        }
        catch(...){
            destroy(start, finish);
            map_allocator::deallocate(map, map_size);
            throw;
        }
        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.first + num_elements % buffer_size();
    }
public:    
    //constructors and destructors
    deque(int n, const value_type& value): map(0), map_size(0), start(), finish() {
         fill_initialize(n, value);
    }
    explicit deque() 
        : map(0), map_size(0), start(), finish() 
        {
            create_map_and_nodes(0);
            std::cout<<"constructor deque() is called."<<std::endl;
            std::cout<<"map address: "<<&*map<<std::endl;            
        }
    deque(const deque& __x) 
        : map(0), map_size(0), start(), finish()  
        { uninitialized_copy(__x.begin(), __x.end(), start); }
    explicit deque(size_type __n) 
        : map(0), map_size(0), start(), finish() 
        {   
            create_map_and_nodes(__n);
            //std::cout<<"constructor __deque_base(n) is called."<<std::endl;    
        }

    deque(const value_type* __first, const value_type* __last,
        const allocator_type& __a = allocator_type()) 
        : map(0), map_size(0), start(), finish()  
        { uninitialized_copy(__first, __last, start); }
    deque(const_iterator __first, const_iterator __last,
        const allocator_type& __a = allocator_type()) 
        : map(0), map_size(0), start(), finish()  
        { uninitialized_copy(__first, __last, start); }
    ~deque() { 
        destroy(start, finish); 
        // for (map_pointer cur = start.node; cur <= finish.node; ++cur)
        //     data_allocator::deallocate(*cur, buffer_size());
        // data_allocator::deallocate(finish.first, finish.cur - finish.first);
        // map_allocator::deallocate(map, map_size);
    }

    void push_back(const value_type& t){
        if (finish.cur != finish.last - 1) {
            construct(finish.cur, t);
            ++finish.cur;
        } else
            push_back_aux(t);
    }

    deque& operator= (const deque& __x) {
        const size_type __len = size();
        if (&__x != this) {
        if (__len >= __x.size())
            erase(copy(__x.begin(), __x.end(), start), finish);
        else {
            const_iterator __mid = __x.begin() + difference_type(__len);
            std::copy(__x.begin(), __mid, start);
            insert(finish, __mid, __x.end());
        }
        }
        return *this;
    }        

    void swap(deque& __x) {
        std::swap(start, __x.start);
        std::swap(finish, __x.finish);
        std::swap(map, __x.map);
        std::swap(map_size, __x.map_size);
    }

    void fill_assign(size_type __n, const T& __val) {
        if (__n > size()) {
        fill(begin(), end(), __val);
        insert(end(), __n - size(), __val);
        }
        else {
        erase(begin() + __n, end());
        fill(begin(), end(), __val);
        }
    }

    void assign(size_type __n, const T& __val) {
        fill_assign(__n, __val);
    }
    inline void push_back_aux(const value_type& t){
        value_type t_copy = t;
        reserve_map_at_back();
        *(finish.node + 1) = allocate_node();
        try {
            construct(finish.cur, t_copy);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
        catch(...){
            /*commit or rollback semantics*/
            finish.set_node(finish.node - 1);
            finish.cur = finish.last;
            deallocate_node(*(finish.node + 1));
        };
    }
    void push_front(const value_type& t) {
        if (start.cur != start.first) {
            construct(start.cur - 1, t);
            --start.cur;
        } else
            push_front_aux(t);
    }
    void push_front_aux(const value_type& t) {
        value_type t_copy = t;
        reserve_map_at_front();
        *(start.node - 1) = allocate_node();
        try {
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur, t_copy);
        }
        catch(...){
            /*commit or rollback semantics*/
            start.set_node(start.node + 1);
            start.cur = start.first;
            deallocate_node(*(start.node - 1));
            throw;
        }
    }
    inline void reserve_map_at_front(size_type nodes_to_add = 1){
        if (nodes_to_add + 1 > map_size - (finish.node - map))
            reallocate_map(nodes_to_add, false);
    }
    inline void reserve_map_at_back(size_type nodes_to_add = 1){
        if (static_cast<ptrdiff_t>(nodes_to_add) > (start.node - map)) 
            reallocate_map(nodes_to_add, true);
    }
    inline void reallocate_map(size_type nodes_to_add, bool add_at_front){
        size_type old_num_nodes = finish.node - start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;
        map_pointer new_nstart;
        if (map_size > 2 * new_num_nodes) {
            new_nstart = map + (map_size - new_num_nodes) / 2 
                             + (add_at_front ? nodes_to_add : 0);
            if(new_nstart < start.node) 
                std::copy(start.node, finish.node + 1, new_nstart);
            else
                std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);            
        }
        else {
            size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_nstart = new_map + (new_map_size - new_num_nodes) / 2 
                             + (add_at_front ? nodes_to_add : 0);
            std::copy(start.node, finish.node + 1, new_nstart);
            map_allocator::deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        }
        start.set_node(new_nstart);
        finish.set_node(new_nstart + old_num_nodes - 1);
    }

    void pop_back() {
        if(finish.cur != finish.first) {
            --finish.cur;
            destroy(finish.cur);
        }
        else 
            pop_back_aux();
    }
    inline void pop_back_aux() {
        destroy(finish.cur);
        deallocate_node(finish.first);
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
    }
    void pop_front() {
        if (start.cur != start.last - 1) {
            destroy(start.cur);
            ++start.cur;
        }
        else
            pop_front_aux();
    }
    inline void pop_front_aux() {
        destroy(start.cur);
        deallocate_node(start.first);
        start.set_node(start.node + 1);
        start.cur = start.first;
    }
    void clear() {
        for (map_pointer cur = start.node; cur < finish.node; ++cur){
            destroy(*cur, *cur + buffer_size());
            data_allocator::deallocate(*cur, buffer_size());
        }
        if (start.node != finish.node) {
            destroy(start.cur, finish.last);
            destroy(finish.first, finish.cur);
            data_allocator::deallocate(finish.first, buffer_size());
        }
        else 
            destroy(start.cur, start.last);
        finish = start;        
    }

    void __print_iterator(const iterator& it) {
    std::cout << "cur: " << it.cur << std::endl;
    std::cout << "first: " << it.first << std::endl;
    std::cout << "last: " << it.last << std::endl;
    std::cout << "node: " << it.node << std::endl;
    }

    void print_start() {
    std::cout << "Start iterator is now: " << std::endl;
    __print_iterator(start);
    }
    iterator erase(iterator pos){
        iterator next = pos;
        ++next;
        std::cout << "iterator pos: " << std::endl;
        __print_iterator(pos);
        std::cout << "iterator start: " << std::endl;
        __print_iterator(start);        
        difference_type index = pos - start;
        std::cout << "index = pos - start: " << index << std::endl;
        difference_type size_half = difference_type(size()>>1);
        std::cout << "size_half: " << size_half << std::endl;
        if(index < size_half) {
            std::cout<<"call copy_backward()"<< std::endl;
            std::copy_backward(start, pos, next);
            pop_front();
        }
        else {
            std::cout<<"call copy()"<<std::endl;
            std::copy(next, finish, pos);
            pop_back();
        }
        return start + index;
    }
    iterator erase(iterator first, iterator last) {
        if (first == start && last == finish) {
            clear();
            return finish;
        }
        else {
            difference_type n = last - first;
            difference_type elems_before = first - start;
            if (elems_before < (size() - n) / 2) {
                std::copy_backward(start, first, last);
                iterator new_start = start + n;
                destroy(start, new_start);
                for (map_pointer cur = new_start.node + 1; cur <= finish.node; ++cur)
                    data_allocator::deallocate(*cur, buffer_size());
                start = new_start;
            }
            else {
                std::copy(last, finish, first);
                iterator new_finish = finish - n;
                destroy(new_finish.cur, finish.cur);
                for (map_pointer cur = finish.node + 1; cur <=  new_finish.node; ++cur)
                    data_allocator::deallocate(*cur, buffer_size());
                finish = new_finish;
            }
            return start + elems_before;
        }
    }
    iterator insert(iterator position, const value_type& x) {
        if (position.cur == start.cur) {
            push_front(x);
            return start;
        }
        else if (position.cur == finish.cur) {
            push_back(x);
            iterator tmp = finish;
            --tmp;
            return tmp;
        }
        else {
            return insert_aux(position, x);
        }
    }
    inline iterator insert_aux(iterator pos, const value_type& x) {
        difference_type index = pos - start;
        value_type x_copy = x;
        if (static_cast<size_type>(index) < (size() / 2)) {
            push_front(front());
            iterator front1 = start;
            ++front1;
            iterator front2 = front1;
            ++front2;
            pos = start + index;
            iterator pos1 = pos;
            ++pos1;
            std::copy(front2, pos1, front1);
        }
        else {
            push_back(back());
            iterator back1 = finish;
            --back1;
            iterator back2 = back1;
            --back2;
            pos = start + index;
            std::copy_backward(pos, back2, back1);
        }
        *pos = x_copy;
        return pos;
    }

    size_t get_index(const T& x) const {
        size_t cnt = 0;
        bool flg = 0;
        for (auto it = this->begin(); it != this->end(); ++it) {
            std::cout << "Item at index: " << cnt << std::endl;
            std::cout << "Address: " << &*it << std::endl;
            std::cout << "Content: " << *it << std::endl;
            if (*it == x) {
                std::cout << "Found x at index: " << cnt << std::endl;
                flg = 1;
            }
            ++cnt;
        }
        if (flg) return cnt;
        std::cout << "x not found" << std::endl;
        return size() + 1; 
    }

        void get_nodes_address() const {
        for (T** n = start.node; n <= finish.node; ++n) {
            std::cout << "Node address: " << *n << std::endl;
        }
    }

    void get_item_address() const {
        for (auto it = begin(); it != end(); ++it) {
            std::cout << "Element address: " << &*it
                      << " Belongs to node: " << *it.node << std::endl;
        }
    }
};


template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::__initialize(size_t __n, const T& __value) {
    std::cout << "__initialize(n, val) is called" << std::endl;
    create_map_and_nodes(__n);
    map_pointer __cur;
    __STL_TRY {
    for (__cur = start.node; __cur < finish.node; ++__cur)
        uninitialized_fill(*__cur, *__cur + buffer_size(), __value);
    uninitialized_fill(finish.first, finish.cur, __value);
    }
    __STL_UNWIND(destroy(start, iterator(*__cur, __cur)));
}

template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::__initialize() {
    std::cout << "__initialize() is called" << std::endl;
    create_map_and_nodes(0);
}


namespace std {
    template <class T, class Ref, class Ptr, size_t BufSiz>
    struct iterator_traits<__deque_iterator<T, Ref, Ptr, BufSiz>> {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef Ptr pointer;
        typedef Ref reference;
    };
}
#endif