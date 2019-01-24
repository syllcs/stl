//
// Created by Cangsu Lei on 2019-01-06.
//

#include "deque.hpp"
#include <iostream>

int main() {
    deque<int> dq(12, 13);
    dq.push_back(100);
    auto it = dq.begin();
    for (; it != dq.end(); ++it) {
        std::cout << *it << std::endl;
    }
}