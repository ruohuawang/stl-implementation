#include "vector.h"
#include <iostream>

int main() {
    // Test default constructor
    vector<int> v1;
    std::cout << "v1 size: " << v1.size() << ", capacity: " << v1.capacity() << std::endl;

    // Test constructor with size and value
    vector<int> v2(5, 10);
    std::cout << "v2 size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
    for (size_t i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << " ";
    }
    std::cout << std::endl;

    // Test push_back
    v2.push_back(20);
    std::cout << "After push_back(20), v2 size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
    for (size_t i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << " ";
    }
    std::cout << std::endl;

    // Test pop_back
    v2.pop_back();
    std::cout << "After pop_back, v2 size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
    for (size_t i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << " ";
    }
    std::cout << std::endl;

    // Test erase
    v2.erase(v2.begin() + 2);
    std::cout << "After erase at index 2, v2 size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
    for (size_t i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << " ";
    }
    std::cout << std::endl;

    // Test resize
    v2.resize(10, 30);
    std::cout << "After resize to 10 with value 30, v2 size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;
    for (size_t i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << " ";
    }
    std::cout << std::endl;

    // Test clear
    v2.clear();
    std::cout << "After clear, v2 size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;

    return 0;
}