//
// Created by Cangsu Lei on 2019-01-06.
//

#ifndef STL2_DEQUE_IMPL_HPP
#define STL2_DEQUE_IMPL_HPP

template<class T, size_t BufSiz>
deque<T, BufSiz>::deque(int n, const value_type &value) : start(), finish(), map(0), map_size(0) {
    fill_initialize(n, value);
}


template<class T, size_t BufSiz>
void deque<T, BufSiz>::fill_initialize(deque::size_type n, const value_type &value) {
    create_map_and_nodes(n);
    map_pointer curr;
    for (curr = start.node; curr < finish.node; ++curr)
        std::uninitialized_fill(*curr, *curr + iterator::buffer_size(), value);
    std::uninitialized_fill(finish.first, finish.curr, value);
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::create_map_and_nodes(deque::size_type num_elements) {
    size_type num_nodes = num_elements / iterator::buffer_size() + 1;
    map_size = std::max(initial_map_size(), num_nodes + 2);
    map = map_allocator.allocate(map_size);
    map_pointer nstart = map + (map_size - num_nodes) / 2;
    map_pointer nfinish = nstart + num_nodes - 1;
    map_pointer curr;
    for(curr = nstart; curr <= nfinish; ++curr) {
        *curr = allocate_node();
    }
    start.set_node(nstart);
    finish.set_node(nfinish);
    start.curr = start.first;
    finish.curr = finish.first + num_elements % iterator::buffer_size();
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::push_back(const value_type &value) {
    if (finish.curr != finish.last - 1) {
        data_allocator.construct(finish.curr, value);
        ++finish.curr;
    } else {
        push_back_aux(value);
    }
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::push_back_aux(const value_type &value) {
    value_type _copy = value;
    reverse_map_at_back();
    *(finish.node + 1) = allocate_node();

    data_allocator.construct(finish.curr, _copy);
    finish.set_node(finish.node + 1);
    finish.curr = finish.first;
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::push_front(const value_type &value) {
    if (start.curr != start.first) {
        data_allocator.construct(start.curr - 1, value);
        --start.curr;
    } else {
        push_front_aux(value);
    }
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::push_front_aux(const value_type &value) {
    value_type _copy = value;
    reverse_map_at_front();
    *(start.node-1) = allocate_node();

    start.set_node(start.node-1);
    start.curr = start.last - 1;
    data_allocator.construct(start.curr, _copy);
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::reverse_map_at_back(deque::size_type nodes_to_add) {
    if (nodes_to_add + 1 > map_size - (finish.node - map))
        reallocate_map(nodes_to_add, false);
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::reverse_map_at_front(deque::size_type nodes_to_add) {
    if (nodes_to_add > start.node - map)
        reallocate_map(nodes_to_add, true);
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::reallocate_map(deque::size_type nodes_to_add, bool add_at_front) {
    size_type old_num_nodes = finish.node -start.node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;
    map_pointer new_nstart;
    if (map_size > 2 * new_num_nodes) {
        new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        if (new_nstart < start.node)
            std::copy(start.node, finish.node + 1, new_nstart);
        else
            std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
    } else {
        size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
        map_pointer new_map;
        new_map = map_allocator.allocate(new_map_size);
        new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        std::copy(start.node, finish.node + 1, new_nstart);
        map_allocator.deallocate(map, map_size);

        map = new_map;
        map_size = new_map_size;
    }
    start.set_node(new_nstart);
    finish.set_node(new_nstart + old_num_nodes - 1);
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::pop_back() {
    if (finish.curr != finish.first) {
        --finish.curr;
        data_allocator.destroy(finish.curr);
    } else
        pop_back_aux();
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::pop_back_aux() {
    deallocate_node(finish.first);
    finish.set_node(finish.node - 1);
    finish.curr = finish.last - 1;
    data_allocator.destroy(finish.curr);
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::pop_front() {
    if (start.curr != start.last - 1) {
        data_allocator.destroy(start.curr);
        ++start.curr;
    } else
        pop_front_aux();
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::pop_front_aux() {
    data_allocator.destroy(start.curr);
    deallocate_node(start.first);
    start.set_node(start.node+1);
    start.curr = start.first;
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::clear() {
    for (map_pointer node = start.node + 1; node < finish.node; ++node) {
        data_allocator.destroy(*node, *node + iterator::buffer_size());
        data_allocator.deallocate(*node, iterator::buffer_size());
    }
    if (start.node != finish.node) {
        data_allocator.destroy(start.curr, start.last);
        data_allocator.destroy(finish.first, finish.curr);
        data_allocator.deallocate(finish.first, iterator::buffer_size());
    } else
        data_allocator.destroy(start.curr, finish.curr);
    finish = start;
}

template<class T, size_t BufSiz>
typename deque<T, BufSiz>::iterator deque<T, BufSiz>::erase(deque::iterator pos) {
    iterator next = pos;
    ++next;
    difference_type idx = pos - start;
    if (idx < (size() >> 1)) {
        std::copy_backward(start, pos, next);
        pop_front();
    } else {
        std::copy(next, finish, pos);
        pop_back();
    }
    return start + idx;
}

template<class T, size_t BufSiz>
typename deque<T, BufSiz>::iterator deque<T, BufSiz>::erase(deque::iterator first, deque::iterator last) {
    if (first == start && last == finish) {
        clear();
        return finish;
    } else {
        difference_type n = last - first;
        difference_type elems_before = first - start;
        if (elems_before < (size() - n) / 2) {
            std::copy_backward(start, first, last);
            iterator new_start = start + n;
            data_allocator.destroy(start, new_start);
            for (map_pointer curr = start.node; curr < new_start.node; ++curr) {
                data_allocator.deallocate(*curr, iterator::buffer_size());
            }
            start = new_start;
        } else {
            std::copy(last, finish, first);
            iterator new_finish = finish - n;
            data_allocator.destroy(new_finish, finish);
            for (map_pointer curr = new_finish.node + 1; curr <= finish.node; ++curr)
                data_allocator.deallocate(*curr, iterator::buffer_size());
            finish = new_finish;
        }
        return start + elems_before;
    }
}

template<class T, size_t BufSiz>
typename deque<T, BufSiz>::iterator deque<T, BufSiz>::insert(deque::iterator pos, const value_type &value) {
    if (pos.curr == start.curr) {
        push_front(value);
        return start;
    } else if (pos.curr == finish.curr) {
        push_back(value);
        iterator tmp = finish;
        --tmp;
        return tmp;
    } else {
        return insert_aux(pos, value);
    }
}

template<class T, size_t BufSiz>
typename deque<T, BufSiz>::iterator deque<T, BufSiz>::insert_aux(deque::iterator pos, const value_type &value) {
    difference_type idx = pos - start;
    value_type _copy = value;
    if (idx < size() / 2) {
        push_front(front());
        iterator front1 = start;
        ++front1;
        iterator front2 = front1;
        ++front2;
        pos = start + idx;
        iterator pos1 = pos;
        ++pos1;
        std::copy(front2, pos1, front1);
    } else {
        push_back(back());
        iterator back1 = finish;
        --back1;
        iterator back2 = back1;
        --back2;
        pos = start + idx;
        std::copy_backward(pos, back2, back1);
    }
    *pos = _copy;
    return pos;
}

template<class T, size_t BufSiz>
typename deque<T, BufSiz>::pointer deque<T, BufSiz>::allocate_node() {
    pointer tmp;
    tmp = data_allocator.allocate(iterator::buffer_size());
    return tmp;
}

template<class T, size_t BufSiz>
void deque<T, BufSiz>::deallocate_node(deque<T, BufSiz>::pointer mp) {
    data_allocator.deallocate(mp, iterator::buffer_size());
}

template<class T, size_t BufSiz>
typename deque<T, BufSiz>::size_type deque<T, BufSiz>::initial_map_size() {
    return 8;
}


#endif //STL2_DEQUE_IMPL_HPP
