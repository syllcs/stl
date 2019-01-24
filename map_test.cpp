//
// Created by Cangsu Lei on 2019-01-11.
//

#include "map.hpp"
#include <iostream>
#include <cstdlib>

using std::cout; using std::endl;

int main() {
    map<int, double> mp;
    for (int i = 0; i < 100; ++i) {
        mp[rand()%1000] = rand() / 10.0;
    }
    auto it = mp.begin();
    auto it2 = mp.end();
    for (; it != it2; ++it) {
        cout << it->first << " " << it->second << endl;
    }
}
