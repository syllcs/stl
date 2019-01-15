//
// Created by Cangsu Lei on 2019-01-07.
//

#ifndef STL2_QUEUE_HPP
#define STL2_QUEUE_HPP
#include "deque.hpp"

template<class T, class Sequence = deque<T>>
class queue {
    friend bool operator== (const queue& x, const queue& y);
    friend bool operator< (const queue& x, const queue& y);
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
    size_type  size() const { return c.size(); }
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference  back() const { return c.back(); }
    void push(const value_type& value) { c.push_back(value); }
    void pop() { c.pop_front(); }
};

template<class T, class Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return x.c == y.c;
}

template<class T, class Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return x.c < y.c;
}

#endif //STL2_QUEUE_HPP
