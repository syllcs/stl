//
// Created by Cangsu Lei on 2019-01-03.
//

#ifndef STL2_LIST_HPP
#define STL2_LIST_HPP

#include <memory>
#include <iterator>
#include <limits>

template<class T>
class list;

template<class T>
struct _list_node {
    typedef _list_node<T>* pointer;
    T data;
    pointer next;
    pointer prev;
    explicit _list_node(const T& d) : data(d), next(0), prev(0) { }
    _list_node() : next(0), prev(0) { }
};

template<class T>
class _list_iterator {
    friend class list<T>;
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

private:
    typedef _list_node<T> node_type;
    typedef _list_node<T>* link_type;


public:
    link_type node;
    _list_iterator() : node(0) { };
    _list_iterator(link_type n) : node(n) { }
    _list_iterator(const _list_iterator& it) : node(it.node) { }
    bool operator==(const _list_iterator& it) const { return node == it.node; }
    bool operator!=(const _list_iterator& it) const { return node != it.node; }
    reference operator*() const { return (*node).data; }
    pointer operator->() const { return &(operator*()); }
    _list_iterator& operator++();
    _list_iterator& operator--();
    _list_iterator operator++(int);
    _list_iterator operator--(int);
};


template<class T>
class list {
public:
    typedef T                                                           value_type;
    typedef std::allocator<_list_node<T>>                               allocator_type;
    typedef std::size_t                                                 size_type;
    typedef std::ptrdiff_t                                              difference_type;
    typedef value_type&                                                 reference;
    typedef const value_type&                                           const_reference;
    typedef typename std::allocator_traits<allocator_type>::pointer          pointer;
    typedef typename std::allocator_traits<allocator_type>::const_pointer    const_pointer;

    typedef _list_iterator<T>                                           iterator;
    typedef const _list_iterator<T>                                     const_iterator;
    typedef std::reverse_iterator<iterator>                             reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                       const_reverse_iterator;

private:
    typedef _list_node<T> list_node;
    typedef list_node* link_type;
    link_type node;
    allocator_type alloc;

public:
    list();
    ~list();
    list& operator=(const list& other);
    list& operator=(list&& other);
    void assign(size_type count, const_reference value);
    template<class InputIt>
    void assign(InputIt first, InputIt last);
    allocator_type get_allocator() const { return alloc; }

    reference front() { return *begin(); }
    const_reference  front() const { return *cbegin(); }
    reference back() { return *(--end()); }
    const_reference  back() const { return *(--cend()); }

    iterator begin() { return iterator((*node).next); }
    const_iterator cbegin() { return (*node).next; }
    iterator end() { return iterator(node); }
    const_iterator cend() { return node; }
    reverse_iterator rbegin() { return reverse_iterator(begin()); }
    const_reverse_iterator crbegin() { return const_reference (cbegin()); };
    reverse_iterator rend() { return reverse_iterator(end()); }
    const_reverse_iterator crend() { return const_reverse_iterator(cend()); }

    bool empty() { return (*node).next == node; }
    size_type size () const;
    size_type max_size() const { return std::numeric_limits<size_type>::max(); }

    void clear();
    iterator insert(iterator pos, const_reference value);
    //iterator insert(iterator pos, T&& value);
    iterator insert(iterator pos, size_type count, const_reference value);
    template<class InputIt>
    void insert(iterator pos, InputIt first, InputIt last);
    //TODO: emplace
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_back(const_reference value);
    //void push_back(T&& value);
    void pop_back();
    void push_front(const_reference value);
    //void push_front(T&& value);
    void pop_front();
    void resize(size_type count);
    void resize(size_type count, const_reference value);
    void swap(list& other);

    void merge(list& other);
    // void merge(list&& other);
    template<class Compare>
    void merge(list& other, Compare comp);
    // template<class Compare>
    // void merge(list&& other, Compare comp);
    void splice(const_iterator pos, list& other);
    //void splice(const_iterator pos, list&& other);
    void splice(const_iterator pos, list& other, const_iterator it);
    //void splice(const_iterator pos, list&& other, const_iterator it);
    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);
    //void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last);
    void remove(const_reference value);
    template<class UnaryPredicate>
    void remove_if(UnaryPredicate p);
    void reverse();
    void unique();
    template<class BinaryPredicate>
    void unique(BinaryPredicate p);
    void sort();
    template<class Compare>
    void sort(Compare comp);

//    bool operator==(list& other);
//    bool operator!=(list& other);
//    bool operator<(list& other);
//    bool operator<=(list& other);
//    bool operator>(list& other);
//    bool operator>=(list& other);
private:
    void transfer(iterator pos, iterator first, iterator last);
    void destroy_node(const_iterator it) { destroy_node(it.node); }
    void destroyAndDeallocateAll();
    void destroyTail();

    link_type get_node() { return alloc.allocate(1); }
    void put_node(link_type n) { return alloc.deallocate(n, 1); }
    link_type create_node(const_reference val) {
        link_type n = get_node();
        alloc.construct(n, val);
        return n;
    }
    void destroy_node(link_type n) {
        alloc.destroy(n);
        put_node(n);
    }
    void empty_initialize();
};
#include "Detail/list.impl.hpp"

#endif //STL2_LIST_HPP
