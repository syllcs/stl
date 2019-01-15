//
// Created by Cangsu Lei on 2019-01-10.
//

#ifndef STL2_MEMORY_HPP
#define STL2_MEMORY_HPP

template<class T1, class T2>
inline void construct(T1 *ptr1, T2& value) {
    new(ptr1) T1(value);
}

template<class T>
inline void destroy(T* ptr) {
    ptr->~T();
}



#endif //STL2_MEMORY_HPP
