#ifndef SMATRIX_HPP
#define SMATRIX_HPP

#include <array>
#include <iostream>
#include <stdexcept>
#include <initializer_list> 

template<typename T, int ROWS, int COLS>
struct StaticMatrix {
    std::array<T, ROWS * COLS> data;

    // Constructor
    StaticMatrix() {
        data.fill(T(0));
    }

    StaticMatrix(std::initializer_list<std::initializer_list<T>> list) {
        data.fill(T(0));
        
        int r = 0;
        for (const auto& row_list : list) {
            if (r >= ROWS) break; 
            
            int c = 0;
            for (const auto& val : row_list) {
                if (c >= COLS) break; 
                
                data[r * COLS + c] = val;
                c++;
            }
            r++;
        }
    }

    // Element access
    inline T& operator()(int r, int c) {
        return data[r * COLS + c];
    }
    
    inline const T& operator()(int r, int c) const {
        return data[r * COLS + c];
    }

    // Overload of '+' to sum matrices
    inline StaticMatrix<T, ROWS, COLS> operator+(const StaticMatrix<T, ROWS, COLS>& B) const {
        StaticMatrix<T, ROWS, COLS> C;
        for (int i = 0; i < ROWS * COLS; ++i) {
            C.data[i] = data[i] + B.data[i];
        }
        return C;
    }

    // Overload of '-' to subtract matrices
    inline StaticMatrix<T, ROWS, COLS> operator-(const StaticMatrix<T, ROWS, COLS>& B) const {
        StaticMatrix<T, ROWS, COLS> C;
        for (int i = 0; i < ROWS * COLS; ++i) {
            C.data[i] = data[i] - B.data[i];
        }
        return C;
    }

    // Overload of '*' to scallar multiplication
    inline StaticMatrix<T, ROWS, COLS> operator*(T scalar) const {
        StaticMatrix<T, ROWS, COLS> C;
        for (int i = 0; i < ROWS * COLS; ++i) {
            C.data[i] = data[i] * scalar;
        }
        return C;
    }

    // Overload of '*' to matrices multiplication 
    template<int OTHER_COLS>
    inline StaticMatrix<T, ROWS, OTHER_COLS> operator*(const StaticMatrix<T, COLS, OTHER_COLS>& B) const {
        StaticMatrix<T, ROWS, OTHER_COLS> C; 
        
        for (int i = 0; i < ROWS; ++i) {
            for (int k = 0; k < COLS; ++k) {
                T factor = (*this)(i, k);
                for (int j = 0; j < OTHER_COLS; ++j) {
                    C(i, j) += factor * B(k, j);
                }
            }
        }
        return C;
    }

    // Matrix transposition
    inline StaticMatrix<T, COLS, ROWS> transpose() const {
        StaticMatrix<T, COLS, ROWS> C;
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                C(c, r) = (*this)(r, c);
            }
        }
        return C;
    }

    // Overload of '~' to matrix transposition
    inline StaticMatrix<T, COLS, ROWS> operator~() const {
        return transpose();
    }

    // Determinant calculation
    inline T det() const {
        static_assert(ROWS == COLS, "The matrix should be square.");
        
        if constexpr (ROWS == 2) {
            return data[0] * data[3] - data[1] * data[2];
        } 
        else if constexpr (ROWS == 3) {
            return data[0] * (data[4] * data[8] - data[5] * data[7]) -
                   data[1] * (data[3] * data[8] - data[5] * data[6]) +
                   data[2] * (data[3] * data[7] - data[4] * data[6]);
        } 
        else {
            static_assert(ROWS <= 3, "Implemente Decomposição LU para matrizes maiores que 3x3.");
            return T(0);
        }
    }

    // Matrix inversion
    inline StaticMatrix<T, ROWS, COLS> inverse() const {
        static_assert(ROWS == COLS, "The matrix should be square!");
        T d = det();
        if (d == T(0)) {
            throw std::runtime_error("Matriz singular não possui inversa (determinante é zero)!");
        }

        StaticMatrix<T, ROWS, COLS> inv;
        T inv_d = T(1) / d;

        if constexpr (ROWS == 2) {
            inv(0, 0) =  data[3] * inv_d;
            inv(0, 1) = -data[1] * inv_d;
            inv(1, 0) = -data[2] * inv_d;
            inv(1, 1) =  data[0] * inv_d;
        } 
        else if constexpr (ROWS == 3) {
            inv(0, 0) = (data[4] * data[8] - data[5] * data[7]) * inv_d;
            inv(0, 1) = (data[2] * data[7] - data[1] * data[8]) * inv_d;
            inv(0, 2) = (data[1] * data[5] - data[2] * data[4]) * inv_d;
            
            inv(1, 0) = (data[5] * data[6] - data[3] * data[8]) * inv_d;
            inv(1, 1) = (data[0] * data[8] - data[2] * data[6]) * inv_d;
            inv(1, 2) = (data[2] * data[3] - data[0] * data[5]) * inv_d;
            
            inv(2, 0) = (data[3] * data[7] - data[4] * data[6]) * inv_d;
            inv(2, 1) = (data[1] * data[6] - data[0] * data[7]) * inv_d;
            inv(2, 2) = (data[0] * data[4] - data[1] * data[3]) * inv_d;
        }

        return inv;
    }

    // Intern Product of two static matrices
    inline T dot(const StaticMatrix<T, ROWS, COLS> &B) const {
        T sum =  T(0);
        for(int i=0;i<ROWS*COLS;i++){
            sum += data[i] * B.data[i];
        }
        return sum;
    }

    // Direct multiplications of matrix elements
    inline StaticMatrix<T, ROWS, COLS> hadamart_product(const StaticMatrix<T,ROWS,COLS>& B) {
        StaticMatrix<T,ROWS, COLS> C;
        for(int i=0;i<ROWS*COLS;i++){
            C.data[i] = data[i] * B.data[i];
        }
        return C;
    }

    // Returns the number of rows of the matrix
    inline int get_rows(){
        return ROWS;
    }
    
    // Returns the number of columns of the matrix
    inline int get_cols(){
        return COLS;
    }

    // Aplies a function to all elemets of the matrix one by one
    template<typename Func>
    void apply(Func func){
        for(int i=0;i<ROWS*COLS;i++){
            data[i] = func(data[i]);
        }
    }  
    
    // Aplies a function to all elements of the matrix without changing the original one
    template<typename Func>
    StaticMatrix<T, ROWS, COLS> map(Func func) const {
        StaticMatrix<T, ROWS, COLS> result;
        for (int i = 0; i < ROWS * COLS; i++) {
            result.data[i] = func(data[i]);
        }
        return result;
    }

    // Matrix printing
    void print() const {
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                std::cout << (*this)(r, c) << " ";
            }
            std::cout << "\n";
        }
    }
};

#endif