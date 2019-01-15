//
// Created by Cangsu Lei on 2019-01-02.
//

#ifndef STL2_VECTOR_IMPL_HPP
#define STL2_VECTOR_IMPL_HPP

template<class T, class Alloc>
vector<T, Alloc>::vector(const size_type n) {
    allocateAndFillN(n, value_type());
}

template<class T, class Alloc>
vector<T, Alloc>::vector(const size_type n, const value_type& value) {
    allocateAndFillN(n, value);
}

template<class T, class Alloc>
template<class InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
    allocateAndCopy(first, last);
}

template<class T, class Alloc>
vector<T, Alloc>::vector(const vector& v) {
    allocateAndCopy(v.start_, v.finish_);
}

// TODO: noexcept
template<class T, class Alloc>
vector<T, Alloc>::vector(vector &&v) {
    start_ = v.start_;
    finish_ = v.finish_;
    endOfStorage_ = v.endOfStorage_;
    v.start_ = v.finish_ = v.endOfStorage_ = 0;
}

template<class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator = (const vector& v) {
    if (this != &v) {
        allocateAndCopy(v.start_, v.finish_);
    }
    return *this;
}

// TODO: noexcept
template<class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator = (vector&& v) {
    if (this != &v) {
        start_ = v.start_;
        finish_ = v.finish_;
        endOfStorage_ = v.endOfStorage_;
        v.start_ = v.finish_ = v.endOfStorage_ = 0;
    }
    return *this;
}

template<class T, class Alloc>
vector<T, Alloc>::~vector() {
    destroyAndDeallocateAll();
}

template<class T, class Alloc>
bool vector<T, Alloc>::operator == (const vector& v) const {
    if (size() != v.size()) return false;
    for (auto p1 = start_, p2 = v.start_; p1 != finish_; ++p1, ++p2) {
        if (*p1 != *p2) return false;
    }
    return true;
}

template<class T, class Alloc>
bool vector<T, Alloc>::operator != (const vector& v) const {
    return !(*this == v);
}

template<class T, class Alloc>
void vector<T, Alloc>::resize (size_type n, value_type val) {
    if (n < size()) {
        for (iterator it = start_ + n; it != finish_; it++) {
            dataAllocator.destroy(it);
        }
        finish_ = start_ + n;
    } else if (n > size() && n <= capacity()) {
        size_type insertSize = n - size();
        finish_ = std::uninitialized_fill_n(finish_, insertSize, val);
    } else if (n > capacity()) {
        size_type newSize = getNewCapacity(n);
        iterator newStart_ = dataAllocator.allocate(newSize);
        iterator newFinish_ = std::uninitialized_copy(start_, finish_, newStart_);
        newFinish_ = std::uninitialized_fill_n(newFinish_, n - size(), val);
        destroyAndDeallocateAll();
        start_ = newStart_;
        finish_ = newFinish_;
        endOfStorage_ = start_ + newSize;
    }
}

template<class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {
    if (n > capacity()) {
        iterator newStart_ = dataAllocator.allocate(n);
        iterator newFinish_ = std::uninitialized_copy(start_, finish_, newStart_);
        destroyAndDeallocateAll();
        start_ = newStart_;
        finish_ = newFinish_;
        endOfStorage_ = start_ + n;
    }
}

template<class T, class Alloc>
void vector<T, Alloc>::shrink_to_fit() {
    if (size() < capacity()) {
        iterator newStart_ = dataAllocator.allocate(size());
        iterator newFinish_ = std::uninitialized_copy(start_, finish_, newStart_);
        destroyAndDeallocateAll();
        start_ = newStart_;
        finish_ = newFinish_;
        endOfStorage_ = finish_;
    }
}

template<class T, class Alloc>
void vector<T, Alloc>::clear() {
    for (iterator it = start_; it != finish_; ++it) {
        dataAllocator.destroy(it);
    }
    finish_ = start_;
}

template<class T, class Alloc>
void vector<T, Alloc>::swap(vector &v) {
    if (this != &v) {
        std::swap(start_, v.start_);
        std::swap(finish_, v.finish_);
        std::swap(endOfStorage_, v.endOfStorage_);
    }
}

template<class T, class Alloc>
void vector<T, Alloc>::push_back(const value_type &value) {
    if (finish_ == endOfStorage_) {
        size_type newSize = getNewCapacity(size() + 1);
        reserve(newSize);
    }
    dataAllocator.construct(finish_, value);
    ++finish_;
}

template<class T, class Alloc>
void vector<T, Alloc>::pop_back() {
    --finish_;
    dataAllocator.destroy(finish_);

}

template<class T, class Alloc>
template<class InputIterator>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
    size_type inputSize = last - first;
    _moveBehind(position, inputSize);
    std::copy(first, last, position);
}

// TODO: fix bug
template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const size_type n, const value_type &val) {
    _moveBehind(position, n);
    std::uninitialized_fill_n(position, n, val);
}

template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type &val) {
    return insert(position, 1, val);
}

template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
    size_type eraseSize = last - first;
    _moveFront(last, eraseSize);
    for (iterator it = finish_ - eraseSize; it != finish_; ++it) {
        dataAllocator.destroy(it);
    }
    finish_ -= eraseSize;
}

template<class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
    erase(position, position+1);
}

template<class T, class Alloc>
void vector<T, Alloc>::destroyAndDeallocateAll() {
    if (capacity() != 0) {
        for (iterator it = start_; it != finish_; ++it) {
            dataAllocator.destroy(it);
        }
        dataAllocator.deallocate(start_, capacity());
    }
}

template<class T, class Alloc>
void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type &val) {
    assert(n >= 0);
    start_ = dataAllocator.allocate(n);
    finish_ = std::uninitialized_fill_n(start_, n, val);
    endOfStorage_ = finish_;
}

template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::allocateAndCopy(InputIterator first, InputIterator last) {
    size_type insertSize = last - first;
    start_ = dataAllocator.allocate(insertSize);
    finish_ = std::uninitialized_copy(first, last, start_);
    endOfStorage_ = finish_;
}

template<class T, class Alloc>
void vector<T, Alloc>::_moveBehind(iterator position, const size_type n) {
    if (endOfStorage_ < finish_ + n) {
        size_type newSize = getNewCapacity(size() + n);
        reserve(newSize);
    }
    std::copy_backward(position, finish_, finish_ + n);
    finish_ = finish_ + n;
}

template<class T, class Alloc>
void vector<T, Alloc>::_moveFront(iterator position, const size_type n) {
    std::copy(position, finish_, position - n);
    finish_ -= n;
    for (iterator it = finish_; it != finish_ + n; ++it) {
        dataAllocator.destroy(it);
    }
}

// TODO: const
template<class T, class Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(const size_type len) {
    size_type cap = capacity() == 0 ? 1 : capacity();
    while (cap < len) {
        cap <<= 1;
    }
    return cap;
}

#endif //STL2_VECTOR_IMPL_HPP
