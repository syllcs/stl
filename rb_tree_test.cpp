//
// Created by Cangsu Lei on 2019-01-08.
//

#include "rb_tree.hpp"
#include "functional.hpp"
#include <iostream>

int main() {
    rb_tree<int, int, identity<int>, std::less<int>> itree;
    std::cout << itree.size() << std::endl;
    itree.insert_unique(10);
    itree.insert_unique(7);
    itree.insert_unique(8);
    itree.insert_unique(15);
    itree.insert_unique(5);
    itree.insert_unique(6);
    itree.insert_unique(11);
    itree.insert_unique(13);
    itree.insert_unique(12);
    std::cout << itree.size() << std::endl;
    auto ite1 = itree.begin();
    auto ite2 = itree.end();
    for (; ite1 != ite2; ++ite1) {
        std::cout << *ite1 << " ";
    }
    std::cout << std::endl;

    auto ite3 = --itree.end();
    auto ite4 = itree.begin();
    for (; ite3 != ite4; --ite3) {
        std::cout << *ite3 << " ";
    }
    std::cout << *(ite3) << std::endl;

}