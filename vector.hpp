//
// Created by Cangsu Lei on 2019-01-02.
//

#ifndef STL2_VECTOR_HPP
#define STL2_VECTOR_HPP
#include <iterator>
#include <memory>
#include <algorithm>
#include <assert.h>

template<class T, class Alloc = std::allocator<T>>
class vector{
private:
    T *start_;
    T *finish_;
    T *endOfStorage_;
    Alloc dataAllocator;
public:
    typedef T						            value_type;
    typedef T*							        iterator;
    typedef const T*							const_iterator;
    typedef std::reverse_iterator<T*>			reverse_iterator;
    typedef std::reverse_iterator<const T*>		const_reverse_iterator;
    typedef iterator							pointer;
    typedef T&									reference;
    typedef const T&							const_reference;
    typedef size_t								size_type;
    typedef ptrdiff_t	                        difference_type;
public:
    vector()
            :start_(0), finish_(0), endOfStorage_(0){}
    explicit vector(const size_type n);
    vector(const size_type n, const value_type& value);
    template<class InputIterator>
    vector(InputIterator first, InputIterator last);
    vector(const vector& v);
    vector(vector&& v);
    vector& operator = (const vector& v);
    vector& operator = (vector&& v);
    ~vector();


    bool operator == (const vector& v)const;
    bool operator != (const vector& v)const;


    iterator begin(){ return (start_); }
    const_iterator begin()const{ return (start_); }
    const_iterator cbegin()const{ return (start_); }
    iterator end(){ return (finish_); }
    const_iterator end()const{ return (finish_); }
    const_iterator cend()const{ return (finish_); }
    reverse_iterator rbegin(){ return reverse_iterator(finish_); }
    const_reverse_iterator crbegin()const{ return const_reverse_iterator(finish_); }
    reverse_iterator rend(){ return reverse_iterator(start_); }
    const_reverse_iterator crend()const{ return const_reverse_iterator(start_); }


    difference_type size()const{ return finish_ - start_; }
    difference_type capacity()const{ return endOfStorage_ - start_; }
    bool empty()const{ return start_ == finish_; }
    void resize(size_type n, value_type val);
    void reserve(size_type n);
    void shrink_to_fit();


    reference operator[](const difference_type i){ return *(begin() + i); }
    const_reference operator[](const difference_type i)const{ return *(cbegin() + i); }
    reference front(){ return *(begin()); }
    reference back(){ return *(end() - 1); }
    pointer data(){ return start_; }


    void clear();
    void swap(vector& v);
    void push_back(const value_type& value);
    void pop_back();
    iterator insert(iterator position, const value_type& val);
    iterator insert(iterator position, const size_type n, const value_type& val);
    template <class InputIterator>
    iterator insert(iterator position, InputIterator first, InputIterator last);
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);



private:
    void destroyAndDeallocateAll();
    void allocateAndFillN(const size_type n, const value_type& val);
    template<class InputIterator>
    void allocateAndCopy(InputIterator first, InputIterator last);

    void _moveBehind(iterator position, const size_type n);
    void _moveFront(iterator position, const size_type n);
    size_type getNewCapacity(const size_type len);
};// end of class vector


#include "Detail/vector.impl.hpp"
#endif //STL2_VECTOR_HPP
