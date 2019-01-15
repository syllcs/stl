//
// Created by Cangsu Lei on 2019-01-07.
//

#ifndef STL2_STACK_HPP
#define STL2_STACK_HPP
#include "deque.hpp"
#include <iterator>
#include <memory>
#include <algorithm>
template<class T, class Sequence = deque<T>>
class stack {
    friend bool operator==(const stack&, const stack&);
    friend bool operator<(const stack&, const stack&);
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type sizs_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
    typedef std::size_t size_type;
private:
    Sequence c;

public:
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void push(const value_type& value) { c.push_back(value); }
    void pop() { c.pop_back(); }
};

template<class T, class Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return x.c == y.c;
}

template<class T, class Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return x.c < y.c;
}
#endif //STL2_STACK_HPP
