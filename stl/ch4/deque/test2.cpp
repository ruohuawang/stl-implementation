#include "deque.h"
#include <iostream>

int main() {
    // 创建一个deque并插入一些元素
    //deque<int> dq(3,1); // 创建一个包含5个元素的deque，值为默认构造的int（0）
    // for (size_t i = 0; i < dq.size(); ++i) {
    //     dq[i] = i + 1; // 设置值为1, 2, 3, 4, 5
    // }
    deque<int> dq;
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    // std::cout << "Start node address: " << &*dq.begin() << std::endl;
    // std::cout << "Finish node address: " << &*dq.end() << std::endl;
    
    // 输出原始deque
    std::cout << "Original deque: ";
    for (deque<int>::iterator it = dq.begin(); it != dq.end(); ++it) {
        std::cout << "Iterator node address: " << &*it << std::endl;
        std::cout << "Iterator node content: " << *it << std::endl;
    }
    std::cout << std::endl;

    deque<int>::iterator pos = dq.begin(); 
    pos++;
    std::cout << "Iterator at position 2 address: "<< &*pos << std::endl;
    std::cout << "Iterator at position 2 content: "<< *pos << std::endl;
    dq.erase(pos);

    // 输出删除后的deque
    std::cout << "After erase(pos): ";
    for (deque<int>::iterator it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 测试删除结尾的元素
    dq.erase(--dq.end());
    std::cout << "After erase(--end()): ";
    for (deque<int>::iterator it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}