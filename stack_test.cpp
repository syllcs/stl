//
// Created by Cangsu Lei on 2019-01-07.
//

#include "stack.hpp"
#include <iostream>
#include <cstdlib>

int main() {
    stack<int> stk;
    for (int i = 0; i < 100; i++) {
        stk.push(rand());
    }
    while (!stk.empty()) {
        std::cout << stk.top() << std::endl;
        stk.pop();
    }
}
