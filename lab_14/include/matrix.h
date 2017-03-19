#ifndef MATRIX_H_
#define MATRIX_H_

#include <string>
#include <cstddef>

class MatrixException {
    std::string _owner;
    std::string _message;
 public:
    MatrixException(std::string owner, std::string message);
    std::string reason() const;
};

class Matrix {
    int *_values;
    std::size_t _n, _m;
    struct MatrixRow {
        std::size_t _m;
        int *_row; // It is a reference, so it mustn't be destroyed.
        MatrixRow(int *row, std::size_t m);
        const int& operator[](std::size_t i) const;
        int& operator[](std::size_t i);
    };
public:
    Matrix();
    Matrix(std::size_t n, std::size_t m);
    Matrix(const Matrix& other);
    Matrix& operator=(Matrix other);
    ~Matrix();
    MatrixRow operator[](std::size_t i) const;
    size_t rows() const;
    size_t columns() const;
};

Matrix operator+(const Matrix& a, const Matrix& b);
Matrix& operator+=(Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& a, const Matrix& b);
Matrix& operator*=(Matrix& a, const Matrix& b);

#endif //MATRIX_H_
