//filename: list.h
#include "../../base/stl_iterator.h"
#include "../../base/stl_alloc.h"
#include "../../base/stl_construct.h"
#include <cstddef>
template <class T> struct __list_node {
    typedef void* void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

template <class T, class Ref, class Ptr> struct __list_iterator {
    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, Ref, Ptr> self;
    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef ptrdiff_t difference_type; 
    typedef __list_node<T>* link_type;
    link_type node;
    //constructor
    __list_iterator(link_type x): node(x) {}
    __list_iterator() {}
    __list_iterator(const iterator& other): node(other.node) {}//copy constructor
    __list_iterator& operator=(const iterator& other) {
        if (this != &other) {
            node = other.node;
        }
        return *this;
    }//copy assignment operator
    bool operator==(const self& x) const {return node == x.node;}
    bool operator!=(const self& x) const {return node != x.node;}
    reference operator*() const {return (*node).data;}
    pointer operator->() const {return &(operator*());}
    self& operator++(){
        node = (link_type)((*node).next);
        return *this;
    }
    self operator++(int){
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--(){
        node = (link_type)((*node).prev);
        return *this;
    }
    self operator--(int){
        self tmp = *this;
        --                                                                                                                                               *this;
        return tmp;
    }
};

template <class T, class Alloc=alloc>
class list {

protected:
    typedef __list_node<T> list_node;
    typedef simple_alloc<list_node, Alloc> list_node_allocator;
    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, const T&, const T*> const_iterator;
    typedef size_t size_type;
    typedef T& reference;
    typedef T* pointer;
public:
    typedef list_node* link_type;
protected:
    link_type node;
    link_type get_node() {return list_node_allocator::allocate();}//allocate a node
    void put_node(link_type p) {list_node_allocator::deallocate(p, 1);}//deallocate a node
    link_type create_node(const T& x) {
        link_type p = get_node();
        construct(&p->data, x);
        return p;
    }
    void destroy_node(link_type p) {destroy(&p->data); put_node(p);}
    void empty_initialize() {
        node = get_node();
        node->next = node;
        node->prev = node;
    }

    void transfer(iterator position, iterator first, iterator last){
        if(position != last) {
            (*(link_type((*last.node).prev))).next = position.node;
            (*(link_type((*first.node).prev))).next = last.node;
            (*(link_type((*position.node).prev))).next = first.node;
            link_type tmp = link_type((*position.node).prev);
            (*position.node).prev = (*last.node).prev;
            (*last.node).prev = (*first.node).prev;
            (*first.node).prev = tmp;
        }
    }

public:
    iterator begin() {return (link_type)((*node).next);}
    iterator end() {return node;}
    const_iterator begin() const {return (link_type)((*node).next);}
    const_iterator end() const {return node;}
    bool empty() const {return node->next == node;}
    size_type size() const {
        return distance(begin(), end());
    }

    reference front() {return *begin();}
    reference back() {return *(--end());}
    list(){empty_initialize();}//constructor

    //list operations

    //insert x before iterator position
    iterator insert(iterator position, const T& x) {
        link_type tmp = create_node(x);
        tmp->next = position.node;
        tmp->prev = position.node->prev;
        (link_type(position.node->prev))->next = tmp;
        position.node->prev = tmp;
        return tmp;
    }
    void push_front(const T& x) {insert(begin(), x);}
    void push_back(const T& x) {insert(end(), x);}
    iterator erase(iterator position) {
        link_type next_node = link_type(position.node->next);
        link_type prev_node = link_type(position.node->prev);
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(position.node);
        return iterator(next_node);
    }    
    
    void pop_front() {erase(begin());}
    void pop_back() {iterator tmp = end(); erase(--tmp);}
    void clear();
    void remove(const T&);
    void unique();
    void splice(iterator position, list& x) {
        if (!x.empty())
            transfer(position, x.begin(), x.end());
    }
    void splice(iterator position, iterator i) {
        iterator j = i;
        ++j;
        if (position == i || position == j) return;
        transfer(position, i, j);
    }
    void splice(iterator position, iterator first, iterator last) {
        if (first != last)
            transfer(position, first, last);
    }

    void merge(list&);
    void reverse();
    void sort();
    void swap(list& other) {
        std::swap(node, other.node);
    }
};

template <class T, class Alloc> void list<T, Alloc>::clear(){
    link_type cursor = (link_type) node->next;
    while(cursor != node){
        link_type tmp = cursor;
        cursor = (link_type) cursor->next;
        destroy_node(tmp);
    }
    node->next = node;
    node->prev = node;
}

template <class T, class Alloc> void list<T, Alloc>::remove(const T& value){
    iterator first = begin();
    iterator last = end();
    while (first != last)
    {
        iterator next = first;//node next functions as a cursor temporarily
        ++next;
        if (*first == value)
            erase(first);
        first = next; 
    }
}

template <class T, class Alloc> void list<T, Alloc>::unique(){
    iterator first = begin();
    iterator last = end();
    if (first == last) return;
    iterator next = first;
    while (++next != last)
    {
        if (*first == *next)
            erase(next);
        else
            first = next; 
        next = first;
    }
}

template <class T, class Alloc> void list<T, Alloc>::merge(list<T, Alloc>& x) {
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();
    //self and x must be sorted (increasing)
    while (first1 != last1 && first2 != last2) 
        if (*first2 < *first1) {
            iterator next = first2;
            transfer(first1, first2, ++next);
            first2 = next;
        }//if *first2 < *first1, insert *first2 before *first1, and move first2 to next node
        else
            ++first1;
        //if *first2 >= *first1, insert *first1 before *first2, and move first1 to next node
    if (first2 != last2) transfer(last1, first2, last2);
    //if x is not empty now, insert the remaining of x to the end of self
}

template <class T, class Alloc> void list<T, Alloc>::reverse() {
    //if the list is of size 0 or 1, do nothing
    if (node->next == node || link_type(node->next)->next == node) return;
    iterator first = begin();
    iterator last = end();
    ++first;
    while (first != last) {
        iterator old = first;
        ++first;
        transfer(begin(), old, first);
    }
}


template <class T, class Alloc> void list<T, Alloc>::sort() {
    //if the list is of size 0 or 1, do nothing
    if (node->next == node || link_type(node->next)->next == node) return;
    list<T, Alloc> carry;
    list<T, Alloc> counter[64];
    int fill = 0;
    while(!empty()) {
        carry.splice(carry.begin(), begin());
        int i = 0;
        while(i < fill && !counter[i].empty()) {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill) ++fill;
    }
    for (int i = 1; i < fill; ++i)
        counter[i].merge(counter[i-1]);
    swap(counter[fill-1]);
}
