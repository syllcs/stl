//
// Created by Cangsu Lei on 2019-01-08.
//

#ifndef STL2_RB_TREE_IMPL_HPP
#define STL2_RB_TREE_IMPL_HPP

void _rb_tree_rotate_right(_rb_tree_node_base* x, _rb_tree_node_base*& root);
void _rb_tree_rotate_left(_rb_tree_node_base* x, _rb_tree_node_base*& root);

void _rb_tree_iterator_base::increment() {
    if (node->right != 0) {
        node = node->right;
        while (node->left != 0)
            node = node->left;
    } else {
        base_ptr y = node->parent;
        while (node == y->right) {
            node = y;
            y = y->parent;
        }
        if (node->right != y)
            node = y;
    }
}

void _rb_tree_iterator_base::decrement() {
    if (node->color == _rb_tree_red && node->parent->parent == node)
        node = node->right;
    else if (node->left != 0) {
        base_ptr y = node->left;
        while (y->right != 0)
            y = y->right;
        node = y;
    } else {
        base_ptr y = node->parent;
        while (node == y->left) {
            node = y;
            y = y->parent;
        }
        node = y;
    }
}

template<class Key, class Value, class KeyOfValue, class Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::link_type rb_tree<Key, Value, KeyOfValue, Compare>::create_node(const_reference x) {
    link_type tmp = get_node();
    rb_tree_node_allocator.construct(&tmp->value_field, x);
    return tmp;
}

template<class Key, class Value, class KeyOfValue, class Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::link_type rb_tree<Key, Value, KeyOfValue, Compare>::clone_node(rb_tree::link_type x) {
    link_type tmp = create_node(x->value_field);
    tmp->color = x->color;
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
}

template<class Key, class Value, class KeyOfValue, class Compare>
void rb_tree<Key, Value, KeyOfValue, Compare>::destroy_node(rb_tree::link_type p) {
    rb_tree_node_allocator.destroy(&p->value_field);
    put_node(p);
}

template<class Key, class Value, class KeyOfValue, class Compare>
void rb_tree<Key, Value, KeyOfValue, Compare>::init() {
    header = get_node();
    color(header) = _rb_tree_red;
    root() = 0;
    leftmost() = header;
    rightmost() = header;
}

template<class Key, class Value, class KeyOfValue, class Compare>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator, bool> rb_tree<Key, Value, KeyOfValue, Compare>::insert_unique(const_reference v) {
    link_type y = header;
    link_type x = root();
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x));
        x = comp ? left(x) : right(x);
    }

    iterator j = iterator(y);
    if (comp) {
        auto tmp = begin();
        if (j == tmp)
            return pair<iterator, bool>(_insert(x, y, v), true);
        else
            --j;
    }
    if (key_compare(key((link_type)j.node), KeyOfValue()(v)))
        return pair<iterator, bool>(_insert(x, y, v), true);
    return pair<iterator, bool>(j, false);
}

template<class Key, class Value, class KeyOfValue, class Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator rb_tree<Key, Value, KeyOfValue, Compare>::insert_equal(const_reference v) {
    link_type y = header;
    link_type x = root();
    while (x != 0) {
        y = x;
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }
    return _insert(x, y, v);
}

template<class Key, class Value, class KeyOfValue, class Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
rb_tree<Key, Value, KeyOfValue, Compare>::_insert(rb_tree::base_ptr x_, rb_tree::base_ptr y_, const_reference v) {
    link_type x = (link_type) x_;
    link_type y = (link_type) y_;
    link_type z;
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

template<class Key, class Value, class KeyOfValue, class Compare>
inline void
rb_tree<Key, Value, KeyOfValue, Compare>::_rb_tree_rebalance(_rb_tree_node_base *x, _rb_tree_node_base*& root) {
    x->color = _rb_tree_red;
    while (x != root && x->parent->color == _rb_tree_red) {
        if (x->parent == x->parent->parent->left) {
            _rb_tree_node_base* y = x->parent->parent->right;
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
            _rb_tree_node_base* y = x->parent->parent->left;
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


inline void _rb_tree_rotate_right(_rb_tree_node_base *x, _rb_tree_node_base *&root) {
    _rb_tree_node_base* y = x->left;
    x->left = y->right;
    if (y->right != 0)
        y->right->parent = x;
    y->parent = x->parent;
    if (x == root)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

inline void _rb_tree_rotate_left(_rb_tree_node_base *x, _rb_tree_node_base *&root) {
    _rb_tree_node_base* y = x->right;
    x->right = y->left;
    if (y->left != 0)
        y->left->parent = x;
    y->parent = x->parent;
    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}


#endif //STL2_RB_TREE_IMPL_HPP
