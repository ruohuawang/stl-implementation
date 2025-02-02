//STL_HEAP.H
#ifndef STL_HEAP_H
#define STL_HEAP_H
#include "stl_vector.h"
#include "stl_iterator.h"


// Declare helper functions first
template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*);

template <class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*);

template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value);

template <class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*);

template <class RandomAccessIterator, class Distance, class T>
inline void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value);

template <class RandomAccessIterator, class T, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*);

// push_heap
template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    //typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
    //typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    //Distance holeIndex = last - first - 1;
    //T value = *(last - 1);
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}

template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
    __push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
}

template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + parent) < value)
    {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

// pop_heap
template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    //typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
    //typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    __pop_heap_aux(first, last, value_type(first));
}

template <class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template <class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, 
                       T value, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <class RandomAccessIterator, class Distance, class T>
inline void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len)
    {
        if (*(first + secondChild) < *(first + secondChild - 1))
            --secondChild;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * secondChild + 2;
    }
    if (secondChild == len)
    {
        *(first + holeIndex) = *(first + secondChild - 1);
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value);
}

template <class RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    while (last - first > 1)
        pop_heap(first, last--);
}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    //typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    //typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
    __make_heap(first, last, value_type(first), distance_type(first));
}

template <class RandomAccessIterator, class T, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*)
{
    if (last - first < 2)
        return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    while (true)
    {
        __adjust_heap(first, parent, len, T(*(first + parent)));
        if (parent == 0)
            return;
        --parent;
    }
}

#endif