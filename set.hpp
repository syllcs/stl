//
// Created by Cangsu Lei on 2019-01-10.
//

#ifndef STL2_SET_HPP
#define STL2_SET_HPP

#include "rb_tree.hpp"
#include "functional.hpp"
template<class Key, class Compare = std::less<Key>>
class set {
public:
    typedef Key key_type;
    typedef Key value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    typedef rb_tree<key_type, value_type, identity<key_type>, Compare> tree_type;
    tree_type t;
public:
    typedef typename tree_type::const_iterator const_iterator;
    typedef typename tree_type::const_iterator iterator;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef typename tree_type::const_pointer pointer;
    typedef typename tree_type::const_pointer const_pointer;
public:
    set() : t(Compare()) { }
    explicit set(const Compare& comp) : t(comp) { }
    template<class InputIt>
    set(InputIt first, InputIt last, const Compare& comp = Compare()) { t.insert_unique(first, last); }
    //~set() { t.~rb_tree(); }
    set(const set& x) : t(x.t) {}

    set& operator=(const set& x) { t = x.t; return *this; }
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return t.key_comp(); }
    iterator begin() const { return t.begin(); }
    iterator end() const { return t.end(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    //void swap(set& x) { t.swap(x.t); }

    typedef pair<iterator, bool> pair_iterator_bool;
    pair_iterator_bool insert(const value_type& x) {
        pair<typename tree_type::iterator, bool> p = t.insert_unique(x);
        return pair<iterator, bool>(p.first, p.second);
    }

    void erase(iterator position);
    void clear();

    iterator find(const key_type& x) const;
    size_type count(const key_type& x) const;
    iterator lower_bound(const key_type& x) const;
    iterator upper_bound(const key_type& x) const;
    pair<iterator, iterator> equal_range(const key_type& x) const;

    friend bool operator==(const set&, const set&);
    friend bool operator<(const set&, const set&);
};

template<class Key, class Compare>
inline bool operator==(const set<Key, Compare>& x, const set<Key, Compare>& y) {
    return x.t == y.t;
}

template<class Key, class Compare>
inline bool operator<(const set<Key, Compare>& x, const set<Key, Compare>& y) {
    return x.t < y.t;
}

#endif //STL2_SET_HPP
