//stl_iterator.h
#ifndef __STL_ITERATOR_H
#define __STL_ITERATOR_H
#include <cstddef>

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};



template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator
{
  typedef T value_type;
  typedef Distance difference_type;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef Category iterator_category;
};

template <class Iterator>
struct iterator_traits
{
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
  typedef typename Iterator::iterator_category iterator_category;
};

template <class T> struct iterator_traits<T*>
{
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef random_access_iterator_tag iterator_category;
};

template <class T> struct iterator_traits<const T*>
{
    typedef ptrdiff_t difference_type;
    typedef T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef random_access_iterator_tag iterator_category;
};

template <class Iterator> inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
  typedef typename iterator_traits<Iterator>::iterator_category category;
  return category();
};

template <class Iterator> inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
  return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
};

template <class Iterator> inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
  return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
};

template <class InputIterator> inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    ++first;
    ++n;
  }
  return n;
};

template <class RandomAccessIterator> inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
  return last - first;
};

template <class InputIterator> inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return __distance(first, last, category());
};

template <class BidirectionalIterator, class Distance> inline void
__advance(BidirectionalIterator& it, Distance n, input_iterator_tag)
{
  if (n >= 0)
    while (n--) ++it;
  else
    while (n++) --it;
};

template <class RandomAccessIterator, class Distance> inline void
__advance(RandomAccessIterator& it, Distance n, random_access_iterator_tag)
{
  it+=n;
};

template <class InputIterator, class Distance> inline void
advance(InputIterator& it, Distance n)
{
  typedef typename iterator_traits<InputIterator>::iterator_category category;
  __advance(it, n, category(it));
};

#endif //__STL_ITERATOR_H