//
// Created by Cangsu Lei on 2019-01-11.
//

#ifndef STL2_MAP_HPP
#define STL2_MAP_HPP

#include "rb_tree.hpp"
#include "utility.hpp"

template<class Key, class T, class Compare = std::less<Key>>
class map {
public:
    typedef Key key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef Compare key_compare;
    typedef pair<const Key, T> value_type;

    class value_compare {
        friend class map<Key, T, Compare>;
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) { }
    public:
        bool operator()(const value_type& x, const value_type& y) const {
            return comp(x.first, y.first);
        }
    };

private:
    typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare> tree_type;

    tree_type t;

public:
    typedef typename tree_type::pointer pointer;
    typedef typename tree_type::const_pointer const_pointer;
    typedef typename tree_type::reference reference;
    typedef typename tree_type::const_reference const_reference;
    typedef typename tree_type::iterator iterator;
    typedef typename tree_type::const_iterator const_iterator;

public:
    map() : t(Compare()) { }
    explicit map(const Compare& comp) : t(comp) { }
    template<class InputIterator>
    map(InputIterator first, InputIterator last, Compare);
    map(const map& x) : t(x.t) { }
    map& operator=(const map& other) { t = other.t; return *this; }

    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return value_compare(t.key_comp()); }

    iterator begin() { return t.begin(); }
    iterator end() { return t.end(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    T& operator[](const key_type& k) {
        return (*((insert(value_type(k, T()))).first)).second;
    }
    void swap(map& x);
    pair<iterator, bool> insert(const value_type& x) { return t.insert_unique(x); }
    iterator insert(iterator position, const value_type& x);
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last);
    
    void erase(iterator position);
    size_type erase(const key_type& position);
    
    void erase(iterator first, iterator last);
    void clear();
    iterator find(const key_type& x);
    size_type count(const key_type& x) const;
    iterator lower_bound(const key_type& x);
    iterator upper_bound(const key_type& x);
    
    pair<iterator, iterator> equal_range(const key_type& x);
    
    friend bool operator==(const map&, const map&);
    friend bool operator<(const map&, const map&);
};

template<class Key, class T, class Compare>
inline bool operator==(const map<Key, T, Compare>& x, const map<Key, T, Compare>& y) {
    return x.t == y.t;
}

template<class Key, class T, class Compare>
inline bool operator<(const map<Key, T, Compare>& x, const map<Key, T, Compare>& y) {
    return x.t < y.t;
}
#endif //STL2_MAP_HPP
