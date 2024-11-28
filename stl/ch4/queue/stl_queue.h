//STL_QUEUE.H
#ifndef STL_QUEUE_H
#define STL_QUEUE_H

#include "stl_deque.h"
#include <cstddef> // ptrdiff_t
#include <iostream>

template <class T, class Sequence = deque<T>>
class queue
{
    template <class U, class Seq>
    friend bool operator==(const queue<U, Seq>& x, const queue<U, Seq>& y);

    template <class U, class Seq>
    friend bool operator<(const queue<U, Seq>& x, const queue<U, Seq>& y);
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
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_front(); }
};

template <class T, class Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y)
{
    return x.c == y.c;
}

template <class T, class Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y)
{
    return x.c < y.c;
}

#endif // STL_QUEUE_H