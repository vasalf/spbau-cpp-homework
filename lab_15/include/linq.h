// -*- mode:c++; -*-

#ifndef LINQ_H_
#define LINQ_H_

#include <utility>
#include <vector>
#include <iterator>
#include <type_traits>

template<typename T, typename Iter>
class range_enumerator;
template<typename T>
class drop_enumerator;
template<typename T>
class take_enumerator;
template<typename T, typename U, typename F>
class select_enumerator;
template<typename T, typename F>
class until_enumerator;
template<typename T, typename F>
class where_enumerator;

template<typename T>
class enumerator {
public:
    virtual const T& operator*() = 0; // Получает текущий элемент.
    virtual enumerator<T>& operator++() = 0;  // Переход к следующему элементу
    explicit virtual operator bool() = 0;  // Возвращает true, если есть текущий элемент

    auto drop(int count) {
        return drop_enumerator<T>(*this, count);
    }

    auto take(int count) {
        return take_enumerator<T>(*this, count);
    }

    template<typename U = T, typename F>
    auto select(F func) {
        return select_enumerator<U, T, F>(*this, func);
    }

    template<typename F>
    auto until(F func) {
        return until_enumerator<T, F>(*this, func);
    }
    
    auto until_eq(const T& v) {
        return this->until([v](const T& a) { return a == v; });
    }
    
    template<typename F>
    auto where(F func) {
        return where_enumerator<T, F>(*this, func);
    }

    auto where_neq(const T& v) {
        return this->where([v](const T& a) {return a != v;});
    }

    std::vector<T> to_vector() {
        std::vector<T> res;
        this->copy_to(std::back_inserter(res));
        return res;
    }

    template<typename Iter>
    void copy_to(Iter it) {
        while (*this) {
            *(it++) = *(*this);
            ++(*this);
        }
    }
};

template<typename T, typename Iter>
class range_enumerator : public enumerator<T> {
public:
    range_enumerator(Iter begin, Iter end) : begin_(begin), end_(end) {
    }

    virtual const T& operator*() {
        return *begin_;
    }

    virtual enumerator<T>& operator++() {
        ++begin_;
        return *this;
    }

    explicit virtual operator bool() {
        return begin_ != end_;
    }

private:
    Iter begin_, end_;
};

template<typename Iter>
auto from(Iter begin, Iter end) {
    typedef typename std::remove_reference<decltype(*begin)>::type maybe_const_T;
    typedef typename std::remove_const<maybe_const_T>::type T;
    return range_enumerator<T, Iter>(begin, end);
}

template<typename T>
class drop_enumerator : public enumerator<T> {
public:
    drop_enumerator(enumerator<T> &parent, int count) : parent_(parent), count_(count) {
    }

    virtual const T& operator*() {
        while (count_) {
            count_--;
            ++parent_;
        }
        return *parent_;
    }

    virtual enumerator<T>& operator++() {
        while (count_) {
            count_--;
            ++parent_;
        }
        return ++parent_;
    }

    explicit virtual operator bool() {
        while (count_) {
            count_--;
            ++parent_;
        }
        return (bool)parent_;
    }

private:
    enumerator<T> &parent_;
    int count_;
};

template<typename T>
class take_enumerator : public enumerator<T> {
public:
    take_enumerator(enumerator<T>& parent, int count) : parent_(parent), count_(count) {
    }

    virtual const T& operator*() {
        return *parent_;
    }

    virtual enumerator<T>& operator++() {
        if (count_) {
            count_--;
            ++parent_;
        }
        return *this;
    }

    explicit virtual operator bool() {
        return count_ && (bool)parent_;
    }

private:
    enumerator<T>& parent_;
    int count_;
};

template<typename T, typename U, typename F>
class select_enumerator : public enumerator<T> {
public:
    select_enumerator(enumerator<U> &parent, F& func) : parent_(parent), func_(func) {
    }

    virtual const T& operator*() {
        current_ = func_(*parent_);
        return current_;
    }

    virtual enumerator<T>& operator++() {
        ++parent_;
        return *this;
    }

    explicit virtual operator bool() {
        return (bool)parent_;
    }

private:
    T current_;
    enumerator<U>& parent_;
    F func_;
};

template<typename T, typename F>
class until_enumerator : public enumerator<T> {
public:
    until_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), func_(predicate) {
    }

    virtual const T& operator*() {
        return *parent_;
    }

    virtual enumerator<T>& operator++() {
        ++parent_;
        return *this;
    }
    
    explicit virtual operator bool() {
        return (bool)parent_ && !func_(*parent_);
    }
    
private:
    enumerator<T>& parent_;
    F func_;
};

template<typename T, typename F>
class where_enumerator : public enumerator<T> {
public:
    where_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), func_(predicate) {
        cur_ = (bool)parent_ && func_(*parent_);
    }

    virtual const T& operator*() {
        while (!cur_)
            cur_ = func_(*(++parent_));
        return *parent_;
    }

    virtual enumerator<T>& operator++() {
        while (!cur_)
            cur_ = func_(*(++parent_));
        cur_ = func_(*(++parent_));
        return *this;
    }

    explicit virtual operator bool() {
        while (parent_ && !cur_) {
            ++parent_;
            cur_ = (bool)parent_ && func_(*parent_);
        }
        return (bool)parent_;
    }
private:
    bool cur_;
    enumerator<T>& parent_;
    F func_;
};

#endif
