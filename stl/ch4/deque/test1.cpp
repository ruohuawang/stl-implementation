//test1.cpp
#include "deque.h"
#include <iostream>
#include <list>
#include <cassert>

// 测试在列表头部插入元素
void test_insert_at_front() {
    std::list<int> myList;
    auto it = myList.insert(myList.begin(), 42);
    assert(myList.size() == 1);
    assert(*it == 42);
    assert(myList.front() == 42);
}

// 测试在列表尾部插入元素
void test_insert_at_back() {
    std::list<int> myList;
    auto it = myList.insert(myList.end(), 42);
    assert(myList.size() == 1);
    assert(*it == 42);
    assert(myList.back() == 42);
}

// 测试在列表中间插入元素
void test_insert_in_middle() {
    std::list<int> myList = {1, 2, 3, 4};
    auto it = myList.insert(std::next(myList.begin(), 2), 42);
    assert(myList.size() == 5);
    assert(*it == 42);
    //assert(std::next(myList.begin(), 2) != it); // 检查是否不是原来的迭代器
}

int main() {
    test_insert_at_front();
    test_insert_at_back();
    test_insert_in_middle();
    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}