#include "matrix.h"
#include <algorithm>

MatrixException::MatrixException(std::string owner, std::string message) {
    _owner = owner;
    _message = message;
}

std::string MatrixException::reason() const {
    return _owner + ": " + _message + ".";
}


Matrix::MatrixRow::MatrixRow(int *row, std::size_t m) {
    _row = row;
    _m = m;
}

const int& Matrix::MatrixRow::operator[](std::size_t i) const {
    if (i >= _m)
        throw MatrixException("ACCESS", "bad index");
    return _row[i];
}

int& Matrix::MatrixRow::operator[](std::size_t i) {
    if (i >= _m)
        throw MatrixException("ACCESS", "bad index");
    return _row[i];
}

Matrix::Matrix() {
    _n = _m = 0;
    _values = new int[_n * _m];
}

Matrix::Matrix(std::size_t n, std::size_t m) {
    _n = n;
    _m = m;
    _values = new int[n * m];
}

Matrix::Matrix(const Matrix& other) {
    _n = other._n;
    _m = other._m;
    _values = new int[_n * _m];
    std::copy(other._values, other._values + _n * _m, _values);
}

Matrix& Matrix::operator=(Matrix other) {
    std::swap(_n, other._n);
    std::swap(_m, other._m);
    std::swap(_values, other._values);
    return *this;
}

Matrix::~Matrix() {
    delete[] _values;
}

Matrix::MatrixRow Matrix::operator[](std::size_t i) const {
    if (i >= _n)
        throw MatrixException("ACCESS", "bad index");
    return MatrixRow(_values + i * _m, _m);
}

size_t Matrix::rows() const {
    return _n;
}

size_t Matrix::columns() const {
    return _m;
}

Matrix operator+(const Matrix& a, const Matrix& b) {
    if (a.rows() != b.rows() || a.columns() != b.columns())
        throw MatrixException("ADD", "dimensions does not match");
    Matrix ans(a.rows(), a.columns());
    for (size_t i = 0; i < a.rows(); i++)
        for (size_t j = 0; j < a.columns(); j++)
            ans[i][j] = a[i][j] + b[i][j];
    return ans;
}

Matrix& operator+=(Matrix& a, const Matrix& b) {
    a = a + b;
    return a;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    if (a.columns() != b.rows())
        throw MatrixException("MUL", "#arg1.columns != #arg2.rows");
    Matrix ans(a.rows(), b.columns());
    for (size_t i = 0; i != ans.rows(); i++)
        for (size_t j = 0; j < ans.columns(); j++)
            ans[i][j] = 0;
    for (size_t k = 0; k != a.columns(); k++)
        for (size_t i = 0; i != a.rows(); i++)
            for (size_t j = 0; j != b.columns(); j++)
                ans[i][j] += a[i][k] * b[k][j];
    return ans;
}

Matrix& operator*=(Matrix& a, const Matrix& b) {
    a = a * b;
    return a;
}
