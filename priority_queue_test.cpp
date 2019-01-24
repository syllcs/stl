//
// Created by Cangsu Lei on 2019-01-08.
//

#include "priority_queue.hpp"
#include <iostream>
#include <cstdlib>

int main() {
    priority_queue<int> pq;
    for (int i = 0; i < 100; ++i) {
        pq.push(rand() % 200);
    }
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
}