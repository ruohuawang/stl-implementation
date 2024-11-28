#include <iostream>
#include "stl_iterator.h"   // 包含自定义的 stl_iterator 头文件
#include "stl_construct.h"  // 包含自定义的 stl_construct 头文件
#include "type_traits.h"    // 包含自定义的 type_traits 头文件

// 简单的 vector 类
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

// 测试 construct 函数
void test_construct() {
    int* p = static_cast<int*>(::operator new(sizeof(int)));
    construct(p, 42);
    std::cout << "Constructed value: " << *p << std::endl;
    destroy(p);
    ::operator delete(p);
}

// 测试 destroy 函数
void test_destroy() {
    int* p = new int(42);
    destroy(p);
    std::cout << "Destroyed value." << std::endl;
    delete p;
}

// 测试 destroy 函数遍历 SimpleVector
void test_destroy_vector() {
    SimpleVector<int> vec(5);
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = i + 1;
    }
    destroy(vec.begin(), vec.end());
    std::cout << "Destroyed vector elements." << std::endl;
}

int main() {
    test_construct();
    test_destroy();
    test_destroy_vector();
    return 0;
}