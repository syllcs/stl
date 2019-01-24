//
// Created by Cangsu Lei on 2019-01-10.
//

#include "set.hpp"
#include <iostream>
#include <cstdlib>

using std::cout; using std::endl;

int main() {
    set<int> st;
    for (int i = 0; i < 100; i++) {
        st.insert(rand() % 100);
    }
    cout << st.size() << endl;
    auto ed = st.end();
    for (auto it = st.begin(); it != ed; ++it) {
        cout << *it << " ";
    }
    cout << endl;
}
