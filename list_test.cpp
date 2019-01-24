//
// Created by Cangsu Lei on 2019-01-05.
//

#include "list.hpp"
#include <cstdlib>
#include <iostream>

int main() {
    list<int> l;
    for (int i = 0; i < 100; i++) {
        l.push_back(rand() % 100);
    }
    for (auto it = l.begin(); it != l.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    l.sort();
    std::cout << "test" << std::endl;
    for (auto it = l.begin(); it != l.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}