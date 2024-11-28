// filename: test.cpp
#include "deque.h"
#include <iostream>
#include <typeinfo>
template <typename T>
void print_deque(const deque<T>& dq) {
    std::cout << "Deque size: " << dq.size() << 
                 ", map size: " << dq.get_map_size() << ", elements: ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main(){
    deque<int> dq;
    dq.push_back(1);
    dq.push_back(3);
    dq.push_back(5);
    dq.push_back(7);
    dq.push_back(9);
    std::cout<<"Printing Deque nodes: "<<std::endl;
    dq.get_nodes_address();
    std::cout<<"Printing Deque items: "<<std::endl;
    dq.get_item_address();
    //print_deque(dq);
    dq.get_index(3);
    size_t cnt = 0;
    for(auto it = dq.begin(); it != dq.end(); ++it, ++cnt){
        std::cout<<"Element no. "<<cnt<<std::endl;
        std::cout<<"Address: "<<&*it<<std::endl;
    }
    auto it = dq.begin();
    auto it0 = it;
    it++;
    it++;
    //difference_type span = it - it0;
    std::cout<<"Iterator no.3 - Iterator no.1 has span: "
    <<it-it0<<std::endl;
    // std::cout<<"Span type: "
    // <<typeid(it-it0).name()<<std::endl;
    std::cout<<"Before erase: "<<std::endl;
    print_deque(dq);
    dq.erase(it);
    std::cout<<"After erase: "<<std::endl;
    print_deque(dq);
    dq.clear();
}