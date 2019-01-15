//
// Created by Cangsu Lei on 2019-01-09.
//

#include "test.hpp"
#include <iostream>
#include <memory>
#include <map>
// class inherited test
class A {
public:
    int a;
    int func(int b) { return a + b; }
    int func2(double d) { return d + func(d); }
};

class B : public A {
public:
    double c;
    double func(double d) { return c + d + a; }
};

class C {
public:
    int a;
    int b;
    int c;
    int d;
};

int main() {
    
    std::map<int, int> mp;
    for (int i = 0; i < 100; i++) {
        mp[i];
    }
    std::cout << mp.size() << std::endl;
//    std::allocator<C> alloc;
//    C* ptr;
//    alloc.allocate(1, ptr);
//    new(&(ptr->a)) int(12);
//    ptr->a += 10;

//    A A_class;
//    A_class.a = 3;
//    std::cout << A_class.func2(5) << std::endl;
//    B B_class;
//    B_class.a = 5;
//    B_class.c = 6;
//    std::cout << B_class.func2(100) << std::endl;
}