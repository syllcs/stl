//
// Created by Cangsu Lei on 2019-01-07.
//

#ifndef STL2_ALGORITHM_HPP
#define STL2_ALGORITHM_HPP
#include <type_traits>
#include <iterator>

template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last);
template<class RandomAccessIterator, class Distance, class T>
inline void _push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance, T);
template<class RandomAccessIterator, class Distance, class T>
void _push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value);
template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last);
template<class RandomAccessIterator, class T>
inline void _pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T);
template<class RandomAccessIterator, class T, class Distance>
inline void _pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance);
template<class RandomAccessIterator, class Distance, class T>
void _adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value);
template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last);
template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last);
template<class RandomAccessIterator, class T, class Distance>
void _make_heap(RandomAccessIterator first, RandomAccessIterator last, T, Distance);



template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    _push_heap_aux(first, last, typename std::iterator_traits<RandomAccessIterator>::difference_type(), typename std::iterator_traits<RandomAccessIterator>::value_type());
}

template<class RandomAccessIterator, class Distance, class T>
inline void _push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance, T) {
    _push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

template<class RandomAccessIterator, class Distance, class T>
void _push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + parent) < value) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    _pop_heap_aux(first, last, typename std::iterator_traits<RandomAccessIterator>::value_type());
}

template<class RandomAccessIterator, class T>
inline void _pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T) {
    _pop_heap(first, last - 1, last - 1, T(*(last-1)), typename std::iterator_traits<RandomAccessIterator>::difference_type());
}

template<class RandomAccessIterator, class T, class Distance>
inline void _pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance) {
    *result = *first;
    _adjust_heap(first, Distance(0), Distance(last - first), value);
}

template<class RandomAccessIterator, class Distance, class T>
void _adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (*(first + secondChild) < *(first + (secondChild - 1)))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    *(first + holeIndex) = value;
}

template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
    while (last - first > 1)
        pop_heap(first, last--);
}

template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
    _make_heap(first, last, typename std::iterator_traits<RandomAccessIterator>::value_type(), typename std::iterator_traits<RandomAccessIterator>::difference_type());
}

template<class RandomAccessIterator, class T, class Distance>
void _make_heap(RandomAccessIterator first, RandomAccessIterator last, T, Distance) {
    if (last - first < 2) return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;

    while (true) {
        _adjust_heap(first, parent, len, T(*(first + parent)));
        if (parent == 0) return;
        parent--;
    }
}

#endif //STL2_ALGORITHM_HPP
