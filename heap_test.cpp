//
// Created by Cangsu Lei on 2019-01-07.
//


#include "algorithm.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
void func(std::vector<int>& v) {
    for (auto i : v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
int main() {
    std::vector<int> v;
    for (int i = 0; i < 100; i++) {
        v.push_back(rand());
    }
    func(v);

    ::make_heap(v.begin(), v.end());
    func(v);
    ::push_heap(v.begin(), v.end());
    func(v);
    ::pop_heap(v.begin(), v.end());
    func(v);
}
