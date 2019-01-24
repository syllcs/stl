#include <iostream>
#include <algorithm>
#include <iterator>
#include "vector.hpp"

using std::cout;
using std::endl;
int main() {
    vector<int> a;
    for (int i = 0; i < 10000; i++) {
        a.push_back(i);
    }
    a.resize(10, 0);
    cout << a.size() << endl;
    for (int i = 0; i < 10; i++) {
        cout << a[i] << endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}