//test.cpp
#include "list.h"
#include <iostream>
#include <cassert>

template <class T>
void print_list(const list<T>& lst) {
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void test_list_operations() {
    list<int> myList;

    // Test push_back and push_front
    std::cout << "Initial list: ";
    print_list(myList);

    myList.push_back(1);
    std::cout << "After push_back(1): ";
    print_list(myList);

    myList.push_front(2);
    std::cout << "After push_front(2): ";
    print_list(myList);

    myList.push_back(3);
    std::cout << "After push_back(3): ";
    print_list(myList);

    myList.push_front(4);
    std::cout << "After push_front(4): ";
    print_list(myList);

    // Test front and back
    std::cout << "Front: " << myList.front() << ", Back: " << myList.back() << std::endl;

    // Test size
    std::cout << "Size: " << myList.size() << std::endl;

    // Test iterator
    std::cout << "List contents: ";
    for (auto it = myList.begin(); it != myList.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Test erase
    auto it = myList.begin();
    ++it;
    myList.erase(it);
    std::cout << "After erase: ";
    print_list(myList);

    // Test pop_front and pop_back
    myList.pop_front();
    std::cout << "After pop_front: ";
    print_list(myList);

    myList.pop_back();
    std::cout << "After pop_back: ";
    print_list(myList);

    // Test clear
    myList.clear();
    std::cout << "After clear: ";
    print_list(myList);

    // Test sort
    myList.push_back(3);
    myList.push_back(1);
    myList.push_back(4);
    myList.push_back(2);
    std::cout << "Before sort: ";
    print_list(myList);

    myList.sort();
    std::cout << "After sort: ";
    print_list(myList);

    // Test unique
    myList.push_back(2);
    myList.push_back(2);
    std::cout << "Before unique: ";
    print_list(myList);

    myList.unique();
    std::cout << "After unique: ";
    print_list(myList);

    // Test remove
    myList.remove(2);
    std::cout << "After remove(2): ";
    print_list(myList);

    // Test merge
    list<int> anotherList;
    anotherList.push_back(0);
    anotherList.push_back(5);
    std::cout << "Another list: ";
    print_list(anotherList);

    myList.merge(anotherList);
    std::cout << "After merge: ";
    print_list(myList);

    // Test reverse
    myList.reverse();
    std::cout << "After reverse: ";
    print_list(myList);

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_list_operations();
    return 0;
}