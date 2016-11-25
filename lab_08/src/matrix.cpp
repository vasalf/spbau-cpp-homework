#include "matrix.h"
#include <algorithm>
#include <cstdio>

void Matrix::_init_by_size(std::size_t r, std::size_t c) {
    _rows = r;
    _cols = c;
    _data = new int*[r];
    for (std::size_t i = 0; i < r; i++) {
        _data[i] = new int[c];
        std::fill(_data[i], _data[i] + c, 0);
    }
}

Matrix::Matrix(std::size_t r, std::size_t c) {
    _init_by_size(r, c);
}

Matrix::Matrix(const Matrix &object) {
    _data = NULL;
    *this = object;
}

Matrix::~Matrix() {
    for (std::size_t i = 0; i < _rows; i++)
        delete[] _data[i];
    delete[] _data;
}

Matrix& Matrix::operator=(const Matrix &object) {
    int **old_data = _data;
    std::size_t old_rows = _rows;
    _init_by_size(object.get_rows(), object.get_cols());
    for (std::size_t i = 0; i < object.get_rows(); i++) {
        for (std::size_t j = 0; j < object.get_cols(); j++) {
            set(i, j, object.get(i, j));
        }
    }
    if (old_data != NULL && this != &object) {
        for (std::size_t i = 0; i < old_rows; i++)
            delete[] old_data[i];
        delete[] old_data;
    }
    return *this;
}

std::size_t Matrix::get_rows() const {
    return _rows;
}

std::size_t Matrix::get_cols() const {
    return _cols;
}

void Matrix::set(std::size_t i, std::size_t j, int val) {
    _data[i][j] = val;
}

int Matrix::get(std::size_t i, std::size_t j) const {
    return _data[i][j];
}

void Matrix::print(FILE *f) const {
    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
            std::fprintf(f, "%d", _data[i][j]);
            if (j < _cols - 1)
                std::fprintf(f, " ");
        }
        std::fprintf(f, "\n");
    }
}

Matrix Matrix::operator+(const Matrix& m) const {
    Matrix result(_rows, _cols);
    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
            result.set(i, j, this->get(i, j) + m.get(i, j));
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& m) const {
    Matrix result(_rows, _cols);
    for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
            result.set(i, j, this->get(i, j) - m.get(i, j));
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& m) const {
    Matrix result(_rows, m.get_cols());
    for (std::size_t i = 0; i < _rows; i++)
        for (std::size_t k = 0; k < _cols; k++)
            for (std::size_t j = 0; j < m.get_cols(); j++)
                result.set(i, j, result.get(i, j) + this->get(i, k) * m.get(k, j));
    return result;
}

Matrix& Matrix::operator+=(const Matrix& m) {
    return *this = *this + m;
}

Matrix& Matrix::operator-=(const Matrix& m) {
    return *this = *this - m;
}

Matrix& Matrix::operator*=(const Matrix& m) {
    return *this = *this * m;
}

bool Matrix::operator==(const Matrix& m) const {
    if (_rows != m.get_rows() || _cols != m.get_cols())
        return false;
    for (std::size_t i = 0; i < _rows; i++)
        for (std::size_t j = 0; j < _cols; j++)
            if (this->get(i, j) != m.get(i, j))
                return false;
    return true;
}

bool Matrix::operator!=(const Matrix& m) const {
    return !(*this == m);
}
