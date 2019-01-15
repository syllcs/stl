//
// Created by Cangsu Lei on 2019-01-03.
//

#ifndef STL2_LIST_IMPL_HPP
#define STL2_LIST_IMPL_HPP


template<class T>
_list_iterator<T>& _list_iterator<T>::operator++() {
    node = node->next;
    return *this;
}

template<class T>
_list_iterator<T>& _list_iterator<T>::operator--() {
    node = node->prev;
    return *this;
}

template<class T>
_list_iterator<T> _list_iterator<T>::operator++(int) {
    _list_iterator tmp = *this;
    ++*this;
    return tmp;
}

template<class T>
_list_iterator<T> _list_iterator<T>::operator--(int) {
    _list_iterator tmp = *this;
    --*this;
    return tmp;
}

template<class T>
void list<T>::empty_initialize() {
    node = get_node();
    node->next = node;
    node->prev = node;
}
template<class T>
list<T>::list() {
    empty_initialize();
}

template<class T>
list<T>::~list() {
    destroyAndDeallocateAll();
}

template<class T>
list<T>& list<T>::operator=(const list& other) {
    if (this != &other) {
        clear();
        for (iterator it = other.head; it != other.tail; ++it) {
            this->push_back(*it);
        }
    }
    return *this;
}

template<class T>
void list<T>::destroyAndDeallocateAll() {
    clear();
    destroyTail();
}

template<class T>
void list<T>::destroyTail() {
    destroy_node(node);
}

template<class T>
list<T>& list<T>::operator=(list&& other) {
    node = other.node;
}

template<class T>
void list<T>::assign(size_type count, const_reference value) {
    destroyAndDeallocateAll();
    for (int i = 0; i < count; ++i) {
        this->push_back(value);
    }
}

template<class T>
template<class InputIt>
void list<T>::assign(InputIt first, InputIt last) {
    clear();
    for (InputIt it = first; it != last; ++it) {
        this->push_back(*it);
    }
}

template<class T>
typename list<T>::iterator list<T>::insert(iterator pos, const_reference value) {
    link_type n = create_node(value);
    n->next = pos.node;
    n->prev = pos.node->prev;
    pos.node->prev->next = n;
    pos.node->prev = n;
    return n;
}

template<class T>
typename list<T>::iterator list<T>::insert(const_iterator pos, size_type count, const_reference value) {
    for (int i = 0; i < count; ++i) {
        pos = insert(pos, value);
    }
    return pos;
}

template<class T>
template<class InputIt>
void list<T>::insert(iterator pos, InputIt first, InputIt last) {
    if (first == last) return;
    for (--last; last != first; --last) {
        pos = insert(pos, *last);
    }
    pos = insert(pos, *last);
}

template<class T>
void list<T>::push_back(const_reference value) {
    insert(end(), value);
}

template<class T>
void list<T>::push_front(const_reference value) {
    insert(begin(), value);
}

template<class T>
typename list<T>::iterator list<T>::erase(iterator pos) {
    link_type next = pos.node->next;
    link_type prev = pos.node->prev;
    prev->next = next;
    next->prev = prev;
    destroy_node(pos);
    return iterator(next);
}

template<class T>
void list<T>::pop_front() {
    erase(begin());
}

template<class T>
void list<T>::pop_back() {
    iterator tmp = end();
    erase(--tmp);
}

template<class T>
void list<T>::clear() {
    link_type curr = node->next;
    while (curr != node) {
        link_type tmp = curr;
        curr = curr->next;
        destroy_node(tmp);
    }
    node->next = node;
    node->prev = node;
}

template<class T>
void list<T>::remove(const_reference value) {
    iterator first = begin();
    iterator last = end();
    while (first != last) {
        iterator next = first;
        ++next;
        if (*first == value) erase(first);
        first = next;
    }
}

template<class T>
void list<T>::unique() {
    iterator first = begin();
    iterator last = end();
    if (first == last) return;
    iterator next = first;
    while (++next != last) {
        if (*first == *next)
            erase(next);
        else
            first = next;
        next = first;
    }
}

template<class T>
void list<T>::transfer(iterator pos, iterator first, iterator last) {
    if (pos != last) {
        last.node->prev->next = pos.node;
        first.node->prev->next = last.node;
        pos.node->prev->next = first.node;
        link_type tmp = pos.node->prev;
        pos.node->prev = last.node->prev;
        last.node->prev = first.node->prev;
        first.node->prev = tmp;
    }
}

template<class T>
void list<T>::splice(const_iterator pos, list& other) {
    if (!other.empty()) {
        transfer(pos, other.begin(), other.end());
    }
}

template<class T>
void list<T>::splice(list<T>::const_iterator pos, list &other, list<T>::const_iterator it) {
    iterator it2 = it;
    ++it2;
    if (pos == it || pos == it2) return;
    transfer(pos, it, it2);
}

template<class T>
void list<T>::splice(list<T>::const_iterator pos, list &other, list<T>::const_iterator first,
                     list<T>::const_iterator last) {
    if (first != last) {
        transfer(pos, first, last);
    }
}

template<class T>
void list<T>::merge(list& other) {
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = other.begin();
    iterator last2 = other.end();
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            iterator next = first2;
            transfer(first1, first2, ++next);
            first2 = next;
        } else {
            ++first1;
        }
    }
    if (first2 != last2) transfer(last1, first2, last2);
}

template<class T>
void list<T>::reverse() {
    if (node->next == node || node->next->next == node)
        return;
    iterator first = begin();
    ++first;
    while (first != end()) {
        iterator old = first;
        ++first;
        transfer(begin(), old, first);
    }
}

template<class T>
void list<T>::sort() {
    if (node->next == node || node->next->next == node)
        return;
    list<T> carry;
    list<T> counter[64];
    int fill = 0;
    while (!empty()) {
        carry.splice(carry.begin(), *this, begin());
        int i = 0;
        while (i < fill && !counter[i].empty()) {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill) ++fill;
    }
    for (int i = 1; i < fill; ++i) {
        counter[i].merge(counter[i-1]);
    }
    swap(counter[fill-1]);
}

template<class T>
typename list<T>::iterator list<T>::erase(const_iterator first, const_iterator last) {
    while (first != last) {
        const_iterator tmp = first;
        ++first;
        erase(tmp);
    }
}

template<class T>
typename list<T>::size_type list<T>::size() const {
    size_type sz = 0;
    for (iterator it = begin(); it != end(); ++it) {
        ++sz;
    }
    return sz;
}

template<class T>
void list<T>::resize(size_type count) {
    size_type sz = size();
    if (sz > count) {
        iterator it = begin();
        for (size_type i = 0; i < count && it != end(); ++i) {
            it++;
        }
        erase(it, end());
    } else if (sz < count){
        insert(end(), count - sz, value_type());
    }
}

template<class T>
void list<T>::resize(size_type count, const_reference value) {
    size_type sz = size();
    if (sz > count) {
        iterator it = begin();
        for (size_type i = 0; i < count && it != end(); ++i) {
            it++;
        }
        erase(it, end());
    } else if (sz < count){
        insert(end(), count - sz, value);
    }
}

template<class T>
void list<T>::swap(list& other) {
    link_type tmp = node;
    node = other.node;
    other.node = tmp;
}

template<class T>
template<class Compare>
void list<T>::merge(list& other, Compare comp) {
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = other.begin();
    iterator last2 = other.end();
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            iterator next = first2;
            transfer(first1, first2, ++next);
            first2 = next;
        } else {
            ++first1;
        }
    }
    if (first2 != last2) transfer(last1, first2, last2);
}

template<class T>
template<class UnaryPredicate>
void list<T>::remove_if(UnaryPredicate p) {
    iterator first = begin();
    iterator last = end();
    while (first != last) {
        iterator next = first;
        ++next;
        if (p(*first)) erase(first);
        first = next;
    }
}

template<class T>
template<class BinaryPredicate>
void list<T>::unique(BinaryPredicate p) {
    iterator first = begin();
    iterator last = end();
    if (first == last) return;
    iterator next = first;
    while (++next != last) {
        if (p(*first, *next))
            erase(next);
        else
            first = next;
        next = first;
    }
}

template<class T>
template<class Compare>
void list<T>::sort(Compare comp) {
    if (node->next == node || node->next->next == node)
        return;
    list<T> carry;
    list<T> counter[64];
    int fill = 0;
    while (!empty()) {
        carry.splice(carry.begin() *this, begin());
        int i = 0;
        while (i < fill && !counter[i].empty()) {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill) ++fill;
    }
    for (int i = 1; i < fill; ++i) {
        counter[i].merge(counter[i-1]);
    }
    swap(counter[fill-1]);
}

//template<class T>
//bool list<T>::operator==(list& other) {
//    iterator it1 = begin(), it2 = other.begin();
//    for (;it1 != end() && it2 != other.end(); ++it1, ++it2) {
//        if (*it1 != *it2) return false;
//    }
//    return it1 == end() && it2 == end();
//}
//
//template<class T>
//bool list<T>::operator!=(list& other) {
//    return !(operator==(other));
//}
//
//template<class T>
//bool list<T>::operator<(list& other) {
//    iterator it1 = begin(), it2 = other.begin();
//    for (; it1 != end() && it2 != other.end(); ++it1, ++it2) {
//        if (*it1 != *it2) return *it1 < *it2;
//    }
//
//}

#endif //STL2_LIST_IMPL_HPP
