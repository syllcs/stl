//
// Created by Cangsu Lei on 2019-01-07.
//

#include <iostream>
#include "queue.hpp"
#include <cstdlib>

int main() {
    queue<int> q;
    for (int i = 0; i < 100; ++i) {
        q.push(rand());
    }
    while (!q.empty()) {
        std::cout << q.front() << std::endl;
        q.pop();
    }
}