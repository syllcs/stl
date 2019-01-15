//
// Created by Cangsu Lei on 2019-01-08.
//

#ifndef STL2_RB_TREE2_HPP
#define STL2_RB_TREE2_HPP

#include <cstddef>
#include <memory>
#include "memory.hpp"

typedef bool _rb_tree_color_type;
const _rb_tree_color_type _rb_tree_red = false;
const _rb_tree_color_type _rb_tree_black = true;

template <class Value, class Compare>
class rb_tree;

template<class Value>
struct _rb_tree_node {
    typedef _rb_tree_color_type color_type;
    typedef _rb_tree_node* node_ptr;
    typedef _rb_tree_node<Value>* link_type;
    typedef Value value_type;

    color_type color;
    node_ptr parent;
    node_ptr left;
    node_ptr right;
    value_type value_field;
    static node_ptr minimum(node_ptr x) {
        while (x->left != 0) x = x->left;
        return x;
    }

    static node_ptr maximum(node_ptr x) {
        while (x->right != 0) x = x->right;
        return x;
    }

};

template<class Value>
class _rb_tree_iterator {
    template<class V, class Compare> friend class rb_tree;
public:
    typedef typename _rb_tree_node<Value>::node_ptr node_ptr;
    typedef std::ptrdiff_t difference_type;
    typedef Value value_type;
    typedef Value& reference;
    typedef Value* pointer;
    typedef _rb_tree_iterator<Value> iterator;
    // TODO: change const
    typedef _rb_tree_iterator<Value> const_iterator;
    typedef _rb_tree_iterator<Value> self;
    typedef _rb_tree_node<Value>* link_type;

    node_ptr node;

public:
    _rb_tree_iterator() = default;
    _rb_tree_iterator(link_type x) { node = x; }
    _rb_tree_iterator(const iterator& it) { node = it.node; }

    reference operator*() const { return node->value_field; }
    pointer operator->() const { return &(operator*()); }
    self& operator++();
    self operator++(int);
    self& operator--();
    self operator--(int);
    bool operator==(self& other) { return node == other.node; }
    bool operator!=(self& other) { return node != other.node; }

};

template <class Value, class Compare>
class _rb_tree {
private:
    typedef void* void_pointer;
    typedef _rb_tree_node<Value>* node_ptr;
    typedef _rb_tree_node<Value> rb_tree_node;
    typedef std::allocator<rb_tree_node> rb_tree_node_allocator_type;
    typedef typename rb_tree_node::color_type color_type;
public:
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* cont_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef rb_tree_node* link_type;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;

private:
    rb_tree_node_allocator_type rb_tree_node_allocator;
    link_type get_node() { return rb_tree_node_allocator.allocate(1); }
    void put_node(link_type p) { rb_tree_node_allocator.deallocate(p, 1); }
    link_type create_node(const_reference x);
    link_type clone_node(link_type x);
    void destroy_node(link_type p);

private:
    size_type node_count;
    link_type header;
    Compare key_compare;

    link_type& root() const { return header->parent; }
    link_type& leftmost() const { return header->left; }
    link_type& rightmost() const { return header->right; }

    static link_type& left(link_type x) { return x->left; }
    static link_type& right(link_type x) { return x->right; }
    static link_type& parent(link_type x) { return x->parent; }
    static reference  value(link_type x) { return x->value_field; }

    static color_type& color(link_type x) { return (color_type&)(x->color); }

    static link_type minimum(link_type x) { return (link_type&)(rb_tree_node::minimum(x)); }
    static link_type maximum(link_type x) { return (link_type&)(rb_tree_node::maximum(x)); }

public:
    typedef _rb_tree_iterator<value_type> iterator;

private:
    void _rb_tree_rebalance(node_ptr x, node_ptr& root);
    iterator _insert(node_ptr x, node_ptr y, const_reference v);
    node_ptr _copy(node_ptr x, node_ptr y);
    void _erase(node_ptr x);
    void init();

public:
    _rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) { init(); }
    ~_rb_tree() { clear(); put_node(header); }

    _rb_tree& operator=(const _rb_tree& x);
    void clear();
    Compare key_comp() const { return key_compare; }
    iterator begin() { return leftmost(); }
    iterator end() { return header; }
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }

public:
    std::pair<iterator, bool> insert_unique(const_reference v);
    iterator insert_equal(const_reference v);
};



//
//template<class Key>
//struct _rb_tree_node_base {
//    typedef _rb_tree_color_type color_type;
//    typedef _rb_tree_node_base *base_ptr;
//    typedef Key key_type;
//    //typedef _rb_tree_node_base<key_type>* link_type;
//    color_type color;
//    base_ptr parent;
//    base_ptr left;
//    base_ptr right;
//    key_type key;
//
//    static base_ptr minimum(base_ptr x) {
//        while (x->left != 0) x = x->left;
//        return x;
//    }
//
//    static base_ptr maximum(base_ptr x) {
//        while (x->right != 0) x = x->right;
//        return x;
//    }
//};
//
//template<class Key>
//struct _rb_tree_iterator_base {
//    typedef typename _rb_tree_node_base<Key>::base_ptr base_ptr;
//    typedef std::ptrdiff_t difference_type;
//    typedef Key key_type;
//    typedef Key &key_reference;
//    typedef Key *key_pointer;
//    typedef _rb_tree_iterator_base<key_type> iterator;
//    typedef _rb_tree_iterator_base<key_type> self;
//    typedef _rb_tree_node_base<key_type> *link_type;
//    //typedef bidirectional_iterator_tag iterator_category;
//    base_ptr node;
//
//    _rb_tree_iterator_base() = default;
//
//    _rb_tree_iterator_base(link_type x) { node = x; }
//
//    _rb_tree_iterator_base(const iterator &it) { node = it.node; }
//
//    key_reference operator*();
//
//    key_pointer operator->();
//
//    self &operator++();
//
//    self operator++(int);
//
//    self &operator--();
//
//    self operator--(int);
//
//    bool operator==(self &other);
//
//    bool operator!=(self &other);
//};
//
//template<class Key, class Compare>
//class _rb_tree_base {
//private:
//    typedef void* void_pointer;
//    typedef _rb_tree_node_base<Key>* base_ptr;
//    typedef _rb_tree_node_base<Key> rb_tree_node_base;
//
//    typedef _rb_tree_color_type  color_type;
//
//public:
//    typedef Key key_type;
//    typedef key_type* key_pointer;
//    typedef const key_pointer const_key_pointer;
//    typedef key_type& key_reference;
//    typedef const key_type& const_key_reference;
//    typedef _rb_tree_node_base<key_type>* link_type;
//    typedef std::size_t size_type;
//    typedef std::ptrdiff_t difference_type;
//
//private:
//    link_type get_node();
//    void put_node(link_type p);
//    link_type create_node(const_key_reference x);
//    link_type clone_node(link_type x);
//    void destroy_node(link_type p);
//
//private:
//    size_type node_count;
//    link_type header;
//    Compare key_compare;
//
//    link_type& root() const { return header->parent; }
//    link_type& leftmost() const { return header->left; }
//    link_type& rightmost() const { return header->right; }
//
//    static link_type& left(link_type x) { return x->left; }
//    static link_type& right(link_type x) { return x->right; }
//    static link_type& parent(link_type x) { return x->parent; }
//    static key_reference key(link_type x) { return x->key; }
//    static color_type& color(link_type x) { return (color_type&)(x->color); }
//
//    static link_type minimum(link_type x) { return _rb_tree_node_base<key_type>::minimum(x); }
//    static link_type maximum(link_type x) { return _rb_tree_node_base<key_type>::maximum(x); }
//
//public:
//    typedef _rb_tree_iterator_base<key_type> iterator;
//
//private:
//    void _rb_tree_rebalance(_rb_tree_node_base<key_type>* x, _rb_tree_node_base<key_type>*& root);
//    iterator _insert(base_ptr x, base_ptr y, const_key_reference k);
//    link_type _copy(link_type x, link_type p);
//    void _erase(link_type x);
//    void init();
//
//public:
//    _rb_tree_base(const Compare& comp = Compare()) : node_count(0), key_compare(comp) { init(); }
//    ~_rb_tree_base();
//
//    _rb_tree_base& operator=(const _rb_tree_base& x);
//
//    Compare key_comp() const { return key_compare; }
//    iterator begin() { return leftmost(); }
//    iterator end() { return header; }
//    bool empty() const { return node_count == 0; }
//    size_type size() const { return node_count; }
//    size_type max_size() const { return size_type(-1); }
//    void clear();
//
//    std::pair<iterator, bool> insert_unique(const_key_reference k);
//    iterator insert_equal(const_key_reference k);
//};
//
//
//
//
//}
#endif //STL2_RB_TREE2_HPP
