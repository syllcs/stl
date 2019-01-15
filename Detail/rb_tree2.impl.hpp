//
// Created by Cangsu Lei on 2019-01-08.
//

#ifndef STL2_RB_TREE2_IMPL_HPP
#define STL2_RB_TREE2_IMPL_HPP

#include "../rb_tree2.hpp"

template <class Value>
void _rb_tree_rotate_right(_rb_tree_node<Value> x, _rb_tree_node<Value> y);

template <class Value>
void _rb_tree_rotate_left(_rb_tree_node<Value> x, _rb_tree_node<Value> y);

template<class Value>
typename _rb_tree_iterator<Value>::self& _rb_tree_iterator<Value>::operator++() {
    if (node->right != 0) {
        node = node->right;
        while (node->left != 0)
            node = node->left;
    } else {
        node_ptr y = node->parent;
        while (node == y->right) {
            node = y;
            y = y->parent;
        }
        if (node->right != y) {
            node = y;
        }
    }
    return *this;
}

template<class Value>
typename _rb_tree_iterator<Value>::self& _rb_tree_iterator<Value>::operator--() {
    if (node->color == _rb_tree_red && node->parent->parent == node)
        node = node->right;
    else if (node->left != 0) {
        node_ptr y = node->left;
        while (y->right != 0)
            y = y->right;
        node = y;
    } else {
        node_ptr y = node->parent;
        while (node == y->left) {
            node = y;
            y = y->parent;
        }
        node = y;
    }
    return *this;
}

template<class Value>
typename _rb_tree_iterator<Value>::self _rb_tree_iterator<Value>::operator++(int) {
    self tmp = *this;
    operator++();
    return tmp;
}

template<class Value>
typename _rb_tree_iterator<Value>::self _rb_tree_iterator<Value>::operator--(int) {
    self tmp = *this;
    operator--();
    return tmp;
}

template<class Value, class Compare>
typename _rb_tree<Value, Compare>::link_type _rb_tree<Value, Compare>::create_node(const_reference x) {
    node_ptr tmp = get_node();
    construct(&tmp->value_field, x);
    return tmp;
}

template<class Value, class Compare>
typename _rb_tree<Value, Compare>::link_type _rb_tree<Value, Compare>::clone_node(
        _rb_tree<Value, Compare>::link_type x) {
    link_type tmp = create_node(x->value_field);
    tmp->color = x->color;
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
}

template<class Value, class Compare>
void _rb_tree<Value, Compare>::destroy_node(_rb_tree::link_type p) {
    destroy(p);
    put_node(p);
}

template<class Value, class Compare>
void _rb_tree<Value, Compare>::_rb_tree_rebalance(_rb_tree::node_ptr x, _rb_tree::node_ptr &root) {
    x->color = _rb_tree_red;
    while (x != root && x->parent->color == _rb_tree_red) {
        if (x->parent == x->parent->parent->left) {
            rb_tree_node* y = x->parent->parent->right;
            if (y && y->color == _rb_tree_red) {
                x->parent->color = _rb_tree_black;
                y->color = _rb_tree_black;
                x->parent->parent->color = _rb_tree_red;
                x = x->parent->parent;
            } else {
                if (x == x->parent->right) {
                    x = x->parent;
                    _rb_tree_rotate_left(x, root);
                }
                x->parent->color = _rb_tree_black;
                x->parent->parent->color = _rb_tree_red;
                _rb_tree_rotate_right(x->parent->parent, root);
            }
        } else {
            rb_tree_node* y = x->parent->parent->left;
            if (y && y->color == _rb_tree_red) {
                x->parent->color = _rb_tree_black;
                y->color = _rb_tree_black;
                x->parent->parent->color = _rb_tree_red;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    _rb_tree_rotate_right(x, root);
                }
                x->parent->color = _rb_tree_black;
                x->parent->parent->color = _rb_tree_red;
                _rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }
    root->color = _rb_tree_black;
}

template<class Value, class Compare>
void _rb_tree<Value, Compare>::init() {
    header = get_node();
    color(header) = _rb_tree_red;
    root() = 0;
    leftmost() = 0;
    rightmost() = 0;
}

template<class Value, class Compare>
typename _rb_tree<Value, Compare>::iterator _rb_tree<Value, Compare>::_insert(_rb_tree::node_ptr x, _rb_tree::node_ptr y, const_reference v) {
    node_ptr z;
    if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
        z = create_node(v);
        left(y) = z;
        if (y == header) {
            root() = z;
            rightmost() = z;
        } else if (y == leftmost()){
            leftmost() = z;
        }
    } else {
        z = create_node(v);
        right(y) = z;
        if (y == rightmost())
            rightmost() = z;
    }
    parent(z) = y;
    left(z) = 0;
    right(z) = 0;
    _rb_tree_rebalance(z, header->parent);
    ++node_count;
    return iterator(z);
}


#endif //STL2_RB_TREE2_IMPL_HPP
