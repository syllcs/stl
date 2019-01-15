//
// Created by Cangsu Lei on 2019-01-08.
//

#ifndef STL2_RB_TREE_HPP
#define STL2_RB_TREE_HPP
#include <iterator>
#include "functional.hpp"
#include "utility.hpp"

typedef bool _rb_tree_color_type;
const _rb_tree_color_type _rb_tree_red = false;
const _rb_tree_color_type _rb_tree_black = true;

struct _rb_tree_node_base {
    typedef _rb_tree_color_type  color_type;
    typedef _rb_tree_node_base* base_ptr;
    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;
    static base_ptr minimum(base_ptr x) {
        while (x->left != 0) x = x->left;
        return x;
    }

    static  base_ptr maximum(base_ptr x) {
        while (x->right != 0) x = x->right;
        return x;
    }
};


template<class Value>
struct _rb_tree_node : public _rb_tree_node_base {
    typedef _rb_tree_node<Value>* link_type;
    Value value_field;
};

struct _rb_tree_iterator_base {
    typedef _rb_tree_node_base::base_ptr  base_ptr;
    //typedef bidirectional_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;
    base_ptr node;
    void increment();
    void decrement();
};

template<class Value>
struct _rb_tree_iterator : public _rb_tree_iterator_base {
    typedef Value value_type;
    typedef Value& reference;
    typedef Value* pointer;
    typedef _rb_tree_iterator<Value> iterator;
    // TODO: change const
    typedef _rb_tree_iterator<Value> const_iterator;
    typedef _rb_tree_iterator<Value> self;
    typedef _rb_tree_node<Value>* link_type;

    _rb_tree_iterator() = default;
    _rb_tree_iterator(link_type x) { node = x; }
    _rb_tree_iterator(const iterator& it) { node = it.node; }

    reference operator*() const { return ((link_type&)node)->value_field; }
    pointer operator->() const { return &(operator*()); }

    self& operator++() { increment(); return *this; }
    self operator++(int) {
        self tmp = *this;
        increment();
        return tmp;
    }
    self& operator--() { decrement(); return *this; }
    self operator--(int) {
        self tmp = *this;
        decrement();
        return tmp;
    }
    bool operator==(self& other) {
        return node == other.node;
    }
    bool operator!=(self& other) {
        return node != other.node;
    }
};

template<class Key, class Value, class KeyOfValue, class Compare>
class rb_tree {
private:
    typedef void* void_pointer;
    typedef _rb_tree_node_base* base_ptr;
    typedef _rb_tree_node<Value> rb_tree_node;
    typedef std::allocator<rb_tree_node> rb_tree_node_allocator_type;
    typedef _rb_tree_color_type color_type;

public:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
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

    link_type& root() const { return (link_type &)(header->parent); }
    link_type& leftmost() const { return (link_type&)(header->left); }
    link_type& rightmost() const { return (link_type&)(header->right); }

    static link_type& left(link_type x) { return (link_type&)(x->left); }
    static link_type& right(link_type x) { return (link_type&)(x->right); }
    static link_type& parent(link_type x) { return (link_type&)(x->parent); }
    static reference  value(link_type x) { return x->value_field; }

    static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
    static color_type& color(link_type x) { return (color_type&)(x->color); }

    static link_type minimum(link_type x) { return (link_type&)(_rb_tree_node_base::minimum(x)); }
    static link_type maximum(link_type x) { return (link_type&)(_rb_tree_node_base::maximum(x)); }

public:
    typedef _rb_tree_iterator<value_type> iterator;
    // TODO: change to const
    typedef _rb_tree_iterator<value_type> const_iterator;

private:
    void _rb_tree_rebalance(_rb_tree_node_base* x, _rb_tree_node_base*& root);
    iterator _insert(base_ptr x, base_ptr y, const_reference v);
    link_type _copy(link_type x, link_type p);
    void _erase(link_type x);
    void init();

public:
    rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) { init(); }
    ~rb_tree() {
        //clear();
        put_node(header);
    }

    rb_tree& operator=(const rb_tree& x);

    Compare key_comp() const { return key_compare; }
    iterator begin() const { return leftmost(); }
    iterator end() const{ return header; }
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }

public:
    pair<iterator, bool> insert_unique(const_reference v);
    iterator insert_equal(const_reference v);


};


#include "Detail/rb_tree.impl.hpp"
#endif //STL2_RB_TREE_HPP
