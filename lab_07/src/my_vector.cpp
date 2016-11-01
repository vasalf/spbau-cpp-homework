#include "my_vector.h"
#include <algorithm>

MyVector::MyVector() : _sz(0), _cp(2) {
    _data = new int[_cp];
}

MyVector::MyVector(std::size_t init_capacity) : _sz(0), _cp(init_capacity) {
    _data = new int[_cp];
}

MyVector::~MyVector() {
    delete _data;
}

void MyVector::set(std::size_t index, int value) {
    _data[index] = value;
}

int MyVector::get(std::size_t index) {
    return _data[index];
}

std::size_t MyVector::size() {
    return _sz;
}

std::size_t MyVector::capacity() {
    return _cp;
}

void MyVector::reserve(std::size_t new_capacity) {
    if (_cp >= new_capacity)
        return;
    int *new_data = new int[new_capacity];
    std::copy(_data, _data + _sz, new_data);
    delete _data;
    _data = new_data;
    _cp = new_capacity;
}

void MyVector::resize(std::size_t new_size) {
    std::size_t new_capacity = _cp;
    while (new_capacity < new_size)
        new_capacity *= 2;
    reserve(new_capacity);
    if (new_size > _sz)
        std::fill(_data + _sz, _data + new_size, 0);
    _sz = new_size;
}

void MyVector::push_back(int value) {
    if (_sz == _cp) {
        reserve(2 * _cp);
    }
    _data[_sz++] = value;
}

void MyVector::insert(std::size_t index, int value) {
    if (_sz == _cp) {
        reserve(2 * _cp);
    }
    std::copy_backward(_data + index, _data + _sz, _data + _sz + 1);
    _data[index] = value;
    _sz++;
}

void MyVector::erase(std::size_t index) {
    std::copy(_data + index + 1, _data + _sz, _data + index);
    _sz--;
}
