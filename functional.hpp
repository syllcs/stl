//
// Created by Cangsu Lei on 2019-01-10.
//

#ifndef STL2_FUNCTIONAL_HPP
#define STL2_FUNCTIONAL_HPP

template<class T>
struct identity {
    const T& operator()(const T& x) const { return x; }
};

template<class T>
struct select1st {
    const typename T::first_type& operator()(const T& x) const { return x.first; }
};
#endif //STL2_FUNCTIONAL_HPP
