//
// Created by Cangsu Lei on 2019-01-03.
//

#ifndef STL2_DEQUE_HPP
#define STL2_DEQUE_HPP

#include <iterator>
#include <memory>
#include <algorithm>

inline size_t _deque_buf_size(size_t n, size_t sz) {
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template<class T, size_t BufSiz>
struct _deque_iterator {
    typedef _deque_iterator<T, BufSiz> iterator;
    typedef _deque_iterator<const T, BufSiz> const_iterator;
    static size_t buffer_size() { return _deque_buf_size(BufSiz, sizeof(T)); }

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T** map_pointer;

    typedef _deque_iterator self;
    T* curr;
    T* first;
    T* last;
    map_pointer node;
    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + difference_type(buffer_size());
    }
    reference operator*() const { return *curr; }
    pointer operator->() const { return &operator*(); }
    difference_type operator-(const self& x) const {
        return difference_type (buffer_size()) * (node - x.node - 1) + (curr - first) + (x.last - x.curr);
    }

    self& operator++() {
        ++curr;
        if (curr == last) {
            set_node(node+1);
            curr = first;
        }
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--() {
        if (curr == first) {
            set_node(node-1);
            curr = last;
        }
        --curr;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

    self& operator+=(difference_type n) {
        difference_type offset = n + (curr - first);
        if (offset >= 0 && offset < difference_type(buffer_size())) {
            curr += n;
        } else {
            difference_type  node_offset = offset > 0 ? offset / difference_type (buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
            set_node(node + node_offset);
            curr = first + (offset - node_offset * difference_type(buffer_size()));
        }
        return *this;
    }

    self operator+(difference_type n) const {
        self tmp = *this;
        return tmp += n;
    }

    self& operator-=(difference_type n) {
        return *this += -n;
    }

    self operator-(difference_type n) const {
        self tmp = *this;
        return tmp -= n;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator==(const self& x) const { return curr == x.curr; }
    bool operator!=(const self& x) const { return !(*this == x); }
    bool operator<(const self& x) const {
        return (node == x.node) ? (curr < x.curr) : (node < x.node);
    }
};


template<class T, size_t BufSiz = 0>
class deque {
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef std::size_t size_type;
private:
    typedef pointer* map_pointer;
    typedef _deque_iterator<T, BufSiz> iterator;

    map_pointer map;
    size_type map_size;

    iterator start;
    iterator finish;

    typedef std::allocator<value_type> data_allocator_type;
    typedef std::allocator<pointer> map_allocator_type;
    data_allocator_type data_allocator;
    map_allocator_type map_allocator;
public:
    iterator begin() { return start; }
    iterator end() { return finish; }

    reference operator[](size_type n) { return start[difference_type(n)]; }
    reference front() { return *start; }
    reference back() {
        iterator tmp = finish;
        --tmp;
        return *tmp;
    }

    size_type size() const { return finish - start; }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return start == finish; }
    deque(int n, const value_type& value);
    deque() : deque(0, value_type()) { }
    void push_back(const value_type& value);
    void push_front(const value_type& value);
    void pop_back();
    void pop_front();
    void clear();
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    iterator insert(iterator pos, const value_type& value);

private:
    void fill_initialize(size_type n, const value_type& value);
    void create_map_and_nodes(size_type num_elements);
    size_type initial_map_size();
    pointer allocate_node();
    void deallocate_node(pointer mp);
    void push_back_aux(const value_type& value);
    void reverse_map_at_back(size_type nodes_to_add = 1);
    void push_front_aux(const value_type& value);
    void reverse_map_at_front(size_type nodes_to_add = 1);
    void reallocate_map(size_type nodes_to_add, bool add_at_front);
    void pop_back_aux();
    void pop_front_aux();
    iterator insert_aux(iterator pos, const value_type& value);
};

#include "Detail/deque.impl.hpp"
#endif //STL2_DEQUE_HPP
