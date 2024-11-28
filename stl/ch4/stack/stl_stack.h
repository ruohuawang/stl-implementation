#ifndef STL_STACK_H
#define STL_STACK_H

#include "../../base/stl_deque.h"
#include <cstddef> // ptrdiff_t
#include <algorithm> // max
#include <iostream>

template <class T, class Sequence = deque<T>>
class stack
{
    template <class U, class Seq>
    friend bool operator==(const stack<U, Seq>& x, const stack<U, Seq>& y);

    template <class U, class Seq>
    friend bool operator<(const stack<U, Seq>& x, const stack<U, Seq>& y);
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c;
public:
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_back(); }
};

template <class T, class Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y)
{
    return x.c == y.c;
}

template <class T, class Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y)
{
    return x.c < y.c;
}

#endif // STL_STACK_H