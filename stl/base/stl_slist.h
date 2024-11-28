//STL_SLIST.H
#ifndef STL_SLIST_H
#define STL_SLIST_H
#include "stl_iterator.h"
#include "stl_alloc.h"
#include "stl_construct.h"
#include <cstddef> 

struct __slist_node_base
{
    __slist_node_base* next;
};

template <class T>
struct __slist_node : public __slist_node_base
{
    T data;
};

inline __slist_node_base* __slist_make_link(__slist_node_base* prev_node,
                                          __slist_node_base* new_node)
{
    new_node->next = prev_node->next;
    prev_node->next = new_node;
    return new_node;
}

inline size_t __slist_size(__slist_node_base* node)
{
    size_t result = 0;
    for ( ; node != 0; node = node->next)
        ++result;
    return result;
}
struct __slist_iterator_base
{
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    //typedef random_access_iterator_tag iterator_category;    
    __slist_node_base* node;
    __slist_iterator_base(__slist_node_base* x) : node(x) {}
    void incr() { node = node->next; }
    bool operator==(const __slist_iterator_base& x) const {
        return node == x.node;
    }
    bool operator!=(const __slist_iterator_base& x) const {
        return node != x.node;
    }    
};

template <class T, class Ref, class Ptr>
struct __slist_iterator: public __slist_iterator_base {
    typedef __slist_iterator<T, T&, T*> iterator;
    typedef __slist_iterator<T, const T&, const T*> const_iterator;
    typedef __slist_iterator<T, Ref, Ptr> self;

    typedef __slist_node<T> list_node;
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;

    __slist_iterator(list_node* x) : __slist_iterator_base(x) {}
    __slist_iterator() : __slist_iterator_base(0) {}
    __slist_iterator(const_iterator &x) : __slist_iterator_base(x.node) {}
    reference operator*() const { return ((list_node*) node)->data; }
    pointer operator->() const { return &(operator*()); }
    self & operator++()
    {
        incr();
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        incr();
        return tmp;
    }
};

template <class T, class Alloc = alloc>
class slist
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef forward_iterator_tag  iterator_category;
    typedef __slist_iterator<T, T&, T*> iterator;
    typedef __slist_iterator<T, const T&, const T*> const_iterator;

private:
    typedef __slist_node<T> list_node;
    typedef __slist_node_base node_base;
    typedef __slist_node_base list_node_base;
    typedef __slist_iterator_base iterator_base;
    typedef simple_alloc<list_node, alloc> list_node_allocator;

    static list_node* create_node(const value_type& x) {
        list_node* node = list_node_allocator::allocate();
        try{
            construct(&node->data, x);
            node->next = 0;
        }
        catch(...) {
            list_node_allocator::deallocate(node, 1);
            throw;
        }    
        return node;
    }

    static void destroy_node(list_node* node) {
        destroy(&node->data);
        list_node_allocator::deallocate(node, 1);
    }
private:
    list_node_base head;
    void clear() {
        while (head.next != 0) {
            list_node* to_delete = (list_node*) head.next;
            head.next = to_delete->next;
            destroy_node(to_delete);
        }
    }
public:
    slist() {head.next = 0;}
    ~slist() {clear();}
public:
    iterator begin() {return iterator((list_node*)head.next);}
    iterator end() {return iterator(0);}
    size_type size() const {return __slist_size(head.next);}
    bool empty() const {return head.next == 0;}
    void swap(slist &L){
        list_node_base* tmp = head.next;
        head.next = L.head.next;
        L.head.next = tmp;
    }
public:
    reference front() {return ((list_node*)head.next)->data;}
    void push_front(const value_type&x) {
        __slist_make_link(&head, create_node(x));
    }
    void pop_front(){
        list_node* node = (list_node*) head.next;
        head.next = node->next;
        destroy_node(node);
    }
    iterator insert(iterator pos, const T& x) {
        if (pos == begin()) {
            push_front(x);
            return begin();
        }
        
        list_node* prev = (list_node*) head.next;
        while (prev != nullptr && prev->next != pos.node) {
            prev = (list_node*) prev->next;
        }
        
        if (prev == nullptr) {
            // Insert at the end
            if (empty()) {
                push_front(x);
                return begin();
            } else {
                list_node* curr = (list_node*) head.next;
                while (curr->next != nullptr) {
                    curr = (list_node*) curr->next;
                }
                __slist_make_link(curr, create_node(x));
                return iterator((list_node*) curr->next);
            }
        } else {
            // Insert before pos
            __slist_make_link(prev, create_node(x));
            return iterator((list_node*) prev->next);
        }
    }

    iterator erase(iterator pos) {
        // If the list is empty or pos is not in the list, do nothing
        if (empty() || pos == end()) {
            return end();
        }
        
        // If pos is the first node
        if (pos.node == head.next) {
            pop_front();
            return begin();
        }
        
        // Traverse the list to find the node before pos
        list_node* prev = (list_node*) head.next;
        while (prev != nullptr && prev->next != pos.node) {
            prev = (list_node*) prev->next;
        }
        
        // If pos was not found
        if (prev == nullptr) {
            return end();
        }
        
        // Remove the node
        list_node* to_delete = (list_node*) prev->next;
        prev->next = to_delete->next;
        destroy_node(to_delete);
        return iterator((list_node*) prev->next);
    }
    iterator insert_after(iterator pos, const T& x) {
        if (pos == begin()) {
            push_front(x);
            return begin();
        }
        if (pos == end()) {
            if (empty()) {
                push_front(x);
                return begin();
            }
            list_node* curr = (list_node*) head.next;
            while (curr->next != nullptr) {
                curr = curr->next;
            }
            __slist_make_link(curr, create_node(x));
            return iterator(curr->next);
        }
        list_node* prev = (list_node*) head.next;
        while (prev != nullptr && prev->next != pos.node) {
            prev = prev->next;
        }
        if (prev == nullptr) {
            return end();
        }
        __slist_make_link(prev, create_node(x));
        return iterator(prev->next);
    }
    iterator erase_after(iterator pos) {
        if (pos.node == 0) {
            return end();
        }
        list_node* to_delete = (list_node*) pos.node->next;
        if (to_delete == 0) {
            return end();
        }
        pos.node->next = to_delete->next;
        destroy_node(to_delete);
        return iterator(pos.node->next);
    }    
    void splice(iterator pos, slist& other_list) {
        if (!other_list.empty()) {
            pos.node->next = other_list.head.next;
            other_list.head.next = 0;
        }
    } 
    void splice(iterator pos, slist& other_list, iterator it) {
        if (it == other_list.end()) {
            return;
        }
        if (&other_list == this) {
            return;
        }
        list_node* to_move = (list_node*) it.node;
        list_node* prev = 0;
        for (list_node* curr = (list_node*) other_list.head.next; curr != to_move; curr = curr->next) {
            prev = curr;
        }
        if (prev == 0) {
            other_list.head.next = to_move->next;
        } else {
            prev->next = to_move->next;
        }
        to_move->next = pos.node->next;
        pos.node->next = to_move;
    }   
    void remove(const T& x) {
        list_node* prev = &head;
        for (list_node* curr = (list_node*) head.next; curr != 0;) {
            if (curr->data == x) {
                prev->next = curr->next;
                destroy_node(curr);
                curr = prev->next;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    }
    void unique() {
        list_node* curr = (list_node*) head.next;
        while (curr != 0 && curr->next != 0) {
            if (curr->data == curr->next->data) {
                list_node* to_delete = curr->next;
                curr->next = to_delete->next;
                destroy_node(to_delete);
            } else {
                curr = curr->next;
            }
        }
    } 
    void merge(slist& other) {
        list_node* curr = (list_node*) head.next;
        list_node* other_curr = (list_node*) other.head.next;
        while (other_curr != 0) {
            if (curr == 0 || curr->data > other_curr->data) {
                list_node* to_move = other_curr;
                other_curr = other_curr->next;
                to_move->next = curr;
                head.next = to_move;
                curr = to_move;
            } else {
                curr = curr->next;
            }
        }
        other.head.next = 0;
    }   
    void sort() {
        head.next = __merge_sort(head.next);
    }

    list_node* __merge_sort(list_node* node) {
        if (node == 0 || node->next == 0) {
            return node;
        }
        list_node* mid = __split(node);
        list_node* left = __merge_sort(node);
        list_node* right = __merge_sort(mid);
        return __merge(left, right);
    }

    list_node* __split(list_node* node) {
        list_node* fast = node->next;
        list_node* slow = node;
        while (fast != 0 && fast->next != 0) {
            fast = fast->next->next;
            slow = slow->next;
        }
        list_node* mid = slow->next;
        slow->next = 0;
        return mid;
    }

    list_node* __merge(list_node* left, list_node* right) {
        if (left == 0) return right;
        if (right == 0) return left;
        if (left->data < right->data) {
            left->next = __merge(left->next, right);
            return left;
        } else {
            right->next = __merge(left, right->next);
            return right;
        }
    }      
    void assign(size_type n, const T& x) {
        clear();
        for (size_type i = 0; i < n; ++i) {
            push_front(x);
        }
    }    
    friend bool operator==(const slist& lhs, const slist& rhs) {
        list_node* lhs_curr = (list_node*) lhs.head.next;
        list_node* rhs_curr = (list_node*) rhs.head.next;
        while (lhs_curr != 0 && rhs_curr != 0) {
            if (lhs_curr->data != rhs_curr->data) {
                return false;
            }
            lhs_curr = lhs_curr->next;
            rhs_curr = rhs_curr->next;
        }
        return lhs_curr == rhs_curr;
    }

    friend bool operator<(const slist& lhs, const slist& rhs) {
        list_node* lhs_curr = (list_node*) lhs.head.next;
        list_node* rhs_curr = (list_node*) rhs.head.next;
        while (lhs_curr != 0 && rhs_curr != 0) {
            if (lhs_curr->data < rhs_curr->data) {
                return true;
            } else if (lhs_curr->data > rhs_curr->data) {
                return false;
            }
            lhs_curr = lhs_curr->next;
            rhs_curr = rhs_curr->next;
        }
        return lhs.size() < rhs.size();
    }
    void reverse() {
        head.next = __reverse(head.next);
    }

    list_node* __reverse(list_node* node) {
        list_node* prev = 0;
        list_node* curr = node;
        while (curr != 0) {
            list_node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }
};

namespace std {
    template <class T, class Ref, class Ptr>
    struct iterator_traits<__slist_iterator<T, Ref, Ptr>> {
        typedef forward_iterator_tag  iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef Ptr pointer;
        typedef Ref reference;
    };
}

#endif