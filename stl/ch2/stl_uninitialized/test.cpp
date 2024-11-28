#include "stl_uninitialized.h"
#include <iostream>

// SimpleVector class definition
template <typename T>
class SimpleVector {
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    SimpleVector(size_t size = 0) : size_(size), capacity_(size) {
        data_ = size ? new T[size] : nullptr;
    }

    ~SimpleVector() {
        delete[] data_;
    }

    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    iterator begin() {
        return data_;
    }

    const_iterator begin() const {
        return data_;
    }

    iterator end() {
        return data_ + size_;
    }

    const_iterator end() const {
        return data_ + size_;
    }

    size_t size() const {
        return size_;
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

// Helper function to print a range of elements
template <class Iterator>
void print_range(Iterator first, Iterator last) {
    for (; first != last; ++first) {
        std::cout << *first << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Test uninitialized_fill_n
    SimpleVector<int> vec1(5);
    uninitialized_fill_n(vec1.begin(), 3, 42);
    std::cout << "uninitialized_fill_n result: ";
    print_range(vec1.begin(), vec1.end());

    // Test uninitialized_fill
    SimpleVector<int> vec2(5);
    uninitialized_fill(vec2.begin(), vec2.end(), 99);
    std::cout << "uninitialized_fill result: ";
    print_range(vec2.begin(), vec2.end());

    // Test uninitialized_copy
    SimpleVector<int> vec3(5);
    for (size_t i = 0; i < vec3.size(); ++i) {
        vec3[i] = i + 1;
    }

    SimpleVector<int> vec4(5);
    uninitialized_copy(vec3.begin(), vec3.end(), vec4.begin());
    std::cout << "uninitialized_copy result: ";
    print_range(vec4.begin(), vec4.end());

    return 0;
}