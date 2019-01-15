//
// Created by Cangsu Lei on 2019-01-08.
//

#ifndef STL2_PRIORITY_QUEUE_HPP
#define STL2_PRIORITY_QUEUE_HPP

#include <vector>
#include "algorithm.hpp"

template<class T, class Sequence = std::vector<T>, class Compare = std::less<typename Sequence::value_type>>
class priority_queue {
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type sizs_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
    typedef std::size_t size_type;
    
private:
    Sequence c;
    Compare comp;
public:
    priority_queue() : c() { }
    explicit priority_queue(const Compare& x) : c(), comp(x) { }
    template<class InputIterator>
    priority_queue(InputIterator first, InputIterator last) : c(first, last) { make_heap(c.begin(), c.end(), comp); }
    template<class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const Compare& x) : c(first, last), comp(x) { make_heap(c.begin(), c.end(), comp); }
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    const_reference top() const { return c.front(); }
    void push(const value_type& x) {
        c.push_back(x);
        push_heap(c.begin(), c.end(), comp);
    }
    void pop() {
        pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }
};

#endif //STL2_PRIORITY_QUEUE_HPP
