// filename: test.cpp
#include "deque.h"
#include <iostream>
#include <cstddef>
#include <algorithm> 
template <typename T>
void print_deque(const deque<T>& dq) {
    std::cout << "Deque size: " << dq.size() << 
                 ", map size: " << dq.get_map_size() << ", elements: ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template <typename T, typename Ref, typename Ptr, size_t BufSiz>
void print_iterator(const __deque_iterator<T, Ref, Ptr, BufSiz>& it) {
    std::cout << "cur: " << it.cur << std::endl;
    std::cout << "first: " << it.first << std::endl;
    std::cout << "last: " << it.last << std::endl;
    std::cout << "node: " << it.node << std::endl;
}

template <class T, class Ref, class Ptr, size_t BufSiz>
void print_iterator_diff(const __deque_iterator<T, Ref, Ptr, BufSiz>& it1,
                         const __deque_iterator<T, Ref, Ptr, BufSiz>& it2) 
{
    size_t buffer_size = __deque_iterator<T, int&, T*, 0>::buffer_size();
    std::cout << "difference_type(buffer_size()) * (node - x.node - 1): "<<std::endl;
    std::cout << ptrdiff_t(buffer_size) * (it1.node - it2.node - 1) << std::endl;
    std::cout << " (cur - first) + (x.last - x.cur): "<<std::endl;
    std::cout <<  (it1.cur - it1.first) + (it2.last - it2.cur) << std::endl;
    std::cout << " (cur - x.cur): "<<std::endl;
    std::cout <<  (it1.cur - it2.cur) << std::endl;
    std::cout << " (x.last - first): "<<std::endl;
    std::cout <<  (it2.last - it1.first) << std::endl;
    std::cout << " index = pos - start: "<<std::endl;
    std::cout <<  ptrdiff_t(buffer_size) * (it1.node - it2.node - 1) +
                (it1.cur - it1.first) + (it2.last - it2.cur)
        << std::endl;   
    std::cout << " index = pos - start(using operator - ): "<<std::endl;
    std::cout <<  ptrdiff_t(it1-it2) << std::endl; 
}
int main() {
    // // Test default constructor
    // std::cout<<"Buffer size: "
    //          <<__deque_iterator<int, int&, int*, 0>::buffer_size()<<std::endl;
    // deque<int> dq1;
    // std::cout << "After default constructor:" << std::endl;
    // print_deque(dq1);

    // // Test push_back
    // dq1.push_back(1);
    // std::cout << "After push_back:" << std::endl;
    // print_deque(dq1);
    // dq1.print_start();

    // // Test push_front
    // dq1.push_front(0);
    // std::cout << "After push_front:" << std::endl;
    // print_deque(dq1);
    // dq1.print_start();

    // // Test pop_back
    // dq1.push_back(1);
    // dq1.pop_back();
    // std::cout << "After pop_back:" << std::endl;
    // print_deque(dq1);  
    // dq1.print_start();

    // // Test pop_front
    // dq1.push_front(0);
    // dq1.pop_front();
    // std::cout << "After pop_front:" << std::endl;
    // print_deque(dq1);
    // dq1.print_start();

    // // Test insert
    // dq1.push_back(3);
    // auto it = dq1.begin();
    // ++it;
    // std::cout << "Deque now has elements: " << std::endl;
    // print_deque(dq1);
    // dq1.print_start();
    // std::cout << "Element at position no.2: " << *it << std::endl;
    // dq1.insert(it, 10);
    // dq1.print_start();
    // std::cout << "After insert:" << std::endl;
    // print_deque(dq1);  

    // // Test erase
    // deque<int> dq3;
    // dq3.push_back(1);
    // dq1.print_start();
    // dq3.push_back(2);
    // dq1.print_start();
    // auto it2 = dq3.begin();
    // auto it0 = it2;
    // ++it2;
    // std::cout<<"Iterator no.2: "<<std::endl;
    // print_iterator(it2);
    // std::cout<<"Iterator no.1: "<<std::endl;
    // print_iterator(it0);
    // dq1.print_start();

    // std::cout << "Before erase:" << std::endl;
    // print_deque(dq1);
    // dq1.erase(it2);
    // std::cout << "After erase:" << std::endl;
    // print_deque(dq1);
    // dq1.print_start();
    // // Test clear
    // dq1.push_back(1);
    // dq1.clear();
    // std::cout << "After clear:" << std::endl;
    // print_deque(dq1);
//test---------------------------part2-----------------------------------
    // deque<int> dq(5,10);
    // dq.push_back(1);
    // dq.push_back(2);
    // dq.push_back(3);
    // std::cout<<"Deque is now: "<<std::endl;
    // print_deque(dq);
    // auto it = dq.begin();
    // auto it0 = it;
    // it++;
    // dq.print_start();
    // std::cout<<"Iterator at start: "<<std::endl;
    // print_iterator(it0);
    
    // dq.erase(it);
    // std::cout<<"After erase: "<<std::endl;
    // print_deque(dq);

    // dq.insert(it0, 5);
    // std::cout<<"After insert: "<<std::endl;
    // dq.print_start();
    // print_deque(dq);

    // dq.erase(it0);
    // std::cout<<"After erase: "<<std::endl;
    // print_deque(dq);
    using namespace std;
    deque<int> ideq(20, 9);
    cout << "size=" << ideq.size() << endl;

    for (deque<int>::size_type i = 0; i < ideq.size(); ++i)
        cout << ideq[i] << ' ';
    cout << endl;

    for (deque<int>::size_type i = 0; i < 3; ++i)
        ideq.push_back(i);
    
    for (deque<int>::size_type i = 0; i < ideq.size(); ++i)
        cout << ideq[i] << ' ';
    cout << endl;
    cout << "size=" << ideq.size() << endl;

    ideq.push_back(3);
    for (deque<int>::size_type i = 0; i < ideq.size(); ++i)
        cout << ideq[i] << ' ';
    cout << endl;
    cout << "size=" << ideq.size() << endl;

    ideq.push_front(99);
    for (deque<int>::size_type i = 0; i < ideq.size(); ++i)
        cout << ideq[i] << ' ';
    cout << endl;
    cout << "size=" << ideq.size() << endl;

    ideq.push_front(98);
    ideq.push_front(97);
    for (deque<int>::size_type i = 0; i < ideq.size(); ++i)
        cout << ideq[i] << ' ';
    cout << endl;
    cout << "size=" << ideq.size() << endl;

    deque<int, allocator<int>>::iterator itr;
    itr = std::find(ideq.begin(), ideq.end(), 99);
    cout << *itr << endl;
    cout << *(itr.cur) << endl;
    return 0;
}