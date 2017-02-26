#include <cstddef>
#include <algorithm>
#include <iostream>

template <class T>
class my_vector{
public:
    my_vector();
    my_vector(size_t n);
    my_vector(const my_vector<T>& other);
    my_vector& operator=(my_vector<T> other);
    ~my_vector();

    size_t size() const;
    size_t capacity() const;
    bool empty() const;

    void resize(size_t n);
    void reserve(size_t n);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    void push_back(const T& t);
    void pop_back();
    void clear();

private:
    size_t capacity_;
    size_t size_;
    T* array_;
};

template<class T>
std::ostream& operator<<(std::ostream& os, const my_vector<T>& v) {
    for (size_t i = 0; i != v.size(); i++) {
        os << v[i];
        if (i + 1 != v.size())
            os << " ";
    }
    return os;
}

template<class T>
my_vector<T>::my_vector() {
    capacity_ = 1;
    array_ = new T[1];
    size_ = 0;
}

template<class T>
my_vector<T>::my_vector(size_t n) {
    capacity_ = std::max((size_t)1, n);
    while (capacity_ & (capacity_ - 1))
        capacity_++;
    array_ = new T[capacity_];
    size_ = n;
}

template<class T>
my_vector<T>::my_vector(const my_vector<T>& other) {
    capacity_ = other.capacity_;
    size_ = other.size_;
    array_ = new T[capacity_];
    std::copy(other.array_, other.array_ + size_, array_);
}

template<class T>
my_vector<T>& my_vector<T>::operator=(my_vector<T> other) {
    std::swap(array_, other.array_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    return *this;
}

template<class T>
my_vector<T>::~my_vector() {
    delete[] array_;
}

template<class T>
size_t my_vector<T>::size() const {
    return size_;
}

template<class T>
size_t my_vector<T>::capacity() const {
    return capacity_;
}

template<class T>
bool my_vector<T>::empty() const {
    return size_ == 0;
}

template<class T>
void my_vector<T>::resize(size_t n) {
    if (n < size_)
        for (T* it = array_ + n; it != array_ + size_; it++)
            it->~T();
    reserve(n);
    size_ = n;
}

template<class T>
void my_vector<T>::reserve(size_t n) {
    n = std::max((size_t)1, n);
    if (n > capacity_) {
        while (n & (n - 1))
            n++;
        T* new_array = new T[n];
        std::copy(array_, array_ + size_, new_array);
        delete[] array_;
        array_ = new_array;
        capacity_ = n;
    }
}

template<class T>
T& my_vector<T>::operator[](size_t index) {
    return array_[index];
}

template<class T>
const T& my_vector<T>::operator[](size_t index) const {
    return array_[index];
}

template<class T>
void my_vector<T>::push_back(const T& t) {
    reserve(size_ + 1);
    array_[size_++] = t;
}

template<class T>
void my_vector<T>::pop_back() {
    resize(size_ - 1);
}

template<class T>
void my_vector<T>::clear() {
    resize(0);
}
