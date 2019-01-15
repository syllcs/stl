//
// Created by Cangsu Lei on 2019-01-11.
//

#ifndef STL2_UTILITY_HPP
#define STL2_UTILITY_HPP

template <class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

#endif //STL2_UTILITY_HPP
