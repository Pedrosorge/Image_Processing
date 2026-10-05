#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>
#include <stdexcept>

// Matrix definition
template<typename T>

struct Matrix{ 
    int rows,cols;
    std::vector<T> data;

    // Matrix constructors 
    Matrix() : rows(0), cols(0) {}    
    Matrix(int rows, int cols) : rows(rows), cols(cols), data(rows*cols, T{}) {}

    // Matrix element access
    T& operator()(int r, int c){
        return data[r*cols + c];
    }

    const T& operator()(int r, int c) const {
        return data[r*cols + c];
    }

    // Get matrix dimensions
    int get_rows() const { return rows; }
    int get_cols() const { return cols; }

    // Overload of '+' to sum matrices
    Matrix<T> operator+(const Matrix<T>& B) const {

        if(this->rows != B.rows || this->cols != B.cols){
            throw std::invalid_argument("These matrices can't be summed!!");
        }

        Matrix<T> C(this->rows,this->cols);
        for(int i=0;i<this->rows;i++){
            for(int j=0;j<this->cols;j++){
                C(i,j) = (*this)(i,j) + B(i,j);
            }
        }

        return C;
    
    }

    // Overload of '-' to subtract matrices
    Matrix<T> operator-(const Matrix<T>& B) const {

        if(this->rows != B.rows || this->cols != B.cols){
            throw std::invalid_argument("These matrices can't be subtracted!!");
        }

        Matrix<T> C(this->rows,this->cols);
        for(int i=0;i<this->rows;i++){
            for(int j=0;j<this->cols;j++){
                C(i,j) = (*this)(i,j) - B(i,j);
            }
        }

        return C;
    
    }

    // Overload of '*' operator to multiply matrices
    Matrix<T> operator*(const Matrix<T>& B) const {

        if(this->rows != B.cols){
            throw std::invalid_argument("These matrices can't be multiplyed!!");
        }

        Matrix<T> C(this->rows,B.cols);

        for(int i=0;i<this->rows;i++){
            for(int k=0;k<this->cols;k++){
                T factor = (*this)(i,k);
                for(int j=0;j<B.cols;j++){
                    C(i,j) += factor*B(k,j);
                }
            }
        }

        return C;

    }

    // Overload of '~' operator to transpose matrices
    Matrix<T> operator~() const {

        Matrix<T> C(this->cols, this->rows);

        for(int i=0;i<this->rows;i++){
            for(int j=0;j<this->cols;j++){
                C(j,i) = (*this)(i,j);
            }
        }

        return C;

    }

    // Function to print a matrix
    void print() const {

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                std::cout << (*this)(i,j) << " ";
            }
            std::cout << "\n";
        }

    }

};

#endif