#ifndef MY_ARRAY_H_
#define MY_ARRAY_H_

#include <cstddef>
//#include <cstdint>
#include <cassert>
#include <algorithm>

// We are forced to use c++03 in this task, so I need to declare this type:
typedef unsigned char uint8_t;

template<typename T, std::size_t N>
class my_array {
    T _arr[N];
public:
    my_array() {}
    T& at(std::size_t index);
    const T& at(std::size_t index) const;
    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    bool empty() const;
    std::size_t size() const;

    void fill(T val);
};

template<typename T, std::size_t N>
T& my_array<T, N>::at(std::size_t index) {
    assert(index >= 0 && index < N);
    return _arr[index];
}

template<typename T, std::size_t N>
const T& my_array<T, N>::at(std::size_t index) const {
    assert(index >= 0 && index < N);
    return _arr[index];
}

template<typename T, std::size_t N>
T& my_array<T, N>::operator[](std::size_t index) {
    return _arr[index];
}

template<typename T, std::size_t N>
const T& my_array<T, N>::operator[](std::size_t index) const {
    return _arr[index];
}

template<typename T, std::size_t N>
bool my_array<T, N>::empty() const {
    return N == 0;
}

template<typename T, std::size_t N>
std::size_t my_array<T, N>::size() const {
    return N;
}

template<typename T, std::size_t N>
void my_array<T, N>::fill(T val) {
    std::fill(_arr, _arr + N, val);
}

template<std::size_t N>
class my_array<bool, N> {
    uint8_t _arr[(N + 7) / 8];
    class bitreference {
        uint8_t *_where;
        size_t _which_bit;
    public:
        bitreference(uint8_t *where, size_t which_bit);
        bitreference& operator=(bool val);
        operator bool();
    };
public:
    my_array() {}
    bitreference at(std::size_t index);
    bool at(std::size_t index) const;
    bitreference operator[](std::size_t index);
    bool operator[](std::size_t index) const;

    bool empty() const;
    std::size_t size() const;

    void fill(bool val);
};

template<std::size_t N>
my_array<bool, N>::bitreference::bitreference(uint8_t *where, size_t which_bit) {
    _where = where;
    _which_bit = which_bit;
}

template<std::size_t N>
typename my_array<bool, N>::bitreference& my_array<bool, N>::bitreference::operator=(bool val) {
    *_where &= ~(1 << _which_bit);
    *_where |= ((int)val << _which_bit);
    return *this;
}

template<std::size_t N>
my_array<bool, N>::bitreference::operator bool() {
    return ((*_where) >> _which_bit) & 1;
}

template<std::size_t N>
typename my_array<bool, N>::bitreference my_array<bool, N>::at(std::size_t index) {
    assert(index >= 0 && index < N);
    return my_array<bool, N>::bitreference(_arr + (index / 8), index % 8);
}

template<std::size_t N>
bool my_array<bool, N>::at(std::size_t index) const {
    assert(index >= 0 && index < N);
    return ((*(_arr + (index / 8))) >> (index % 8)) & 1;    
}

template<std::size_t N>
typename my_array<bool, N>::bitreference my_array<bool, N>::operator[](std::size_t index) {
    return my_array<bool, N>::bitreference(_arr + (index / 8), index % 8);
}

template<std::size_t N>
bool my_array<bool, N>::operator[](std::size_t index) const {
    return ((*(_arr + (index / 8))) >> (index % 8)) & 1;    
}

template<std::size_t N>
bool my_array<bool, N>::empty() const {
    return N == 0;
}

template<std::size_t N>
std::size_t my_array<bool, N>::size() const {
    return N;
}

template<std::size_t N>
void my_array<bool, N>::fill(bool val) {
    std::fill(_arr, _arr + (N + 7) / 8, ((int)val << 8) - val);
}

#endif //MY_ARRAY_H_
