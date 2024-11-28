#include "stl_alloc.h"
#include <iostream>

int main() {
    // 使用 alloc 分配内存
    int* p1 = static_cast<int*>(alloc::allocate(sizeof(int)));
    *p1 = 42;
    std::cout << "Allocated int: " << *p1 << std::endl;

    // 使用 alloc 分配更大的内存块
    int* p2 = static_cast<int*>(alloc::allocate(10 * sizeof(int)));
    for (int i = 0; i < 10; ++i) {
        p2[i] = i;
    }
    std::cout << "Allocated array: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << p2[i] << " ";
    }
    std::cout << std::endl;

    // 使用 alloc 重新分配内存
    int* p3 = static_cast<int*>(alloc::reallocate(p2, 10 * sizeof(int), 20 * sizeof(int)));
    if (p3 != nullptr) {
        for (int i = 10; i < 20; ++i) {
            p3[i] = i;
        }
        std::cout << "Reallocated array: ";
        for (int i = 0; i < 20; ++i) {
            std::cout << p3[i] << " ";
        }
        std::cout << std::endl;
    }

    // 释放内存
    alloc::deallocate(p1, sizeof(int));
    alloc::deallocate(p3, 20 * sizeof(int));

    std::cout << "Memory deallocated successfully." << std::endl;

    return 0;
}