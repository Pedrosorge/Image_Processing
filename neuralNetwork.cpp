// #ifndef NEURAL_NETWORK_HPP
// #define NEURAL_NETWORK_HPP

#include "matrix.hpp"
#include "staticmatrix.hpp"
#include <cmath>
#include <cstdlib>
#include <time.h>

float sigmoid(float x) { return 1.0f / (1.0f + std::exp(-x)); }

float sigmoid_derivative(float x) { return x * (1.0f - x); }

float random_weight() { return ( (float)rand() / RAND_MAX) * 2.0 - 1.0; }

int main(){

    srand(clock());

    std::vector<StaticMatrix<float, 1, 2>> inputs = {
        {{0.0f, 0.0f}},
        {{0.0f, 1.0f}},
        {{1.0f, 0.0f}},
        {{1.0f, 1.0f}}
    };

    std::vector<StaticMatrix<float, 1, 1>> targets = {
        {{0.0f}},
        {{1.0f}},
        {{1.0f}},
        {{0.0f}}
    };
 
    auto rand_fn = [](float) { return random_weight(); };

    // Camada oculta 1
    StaticMatrix<float,2,4> matrix_w1;
    StaticMatrix<float,1,4> matrix_b1;

    matrix_w1.apply(rand_fn);
    matrix_b1.apply(rand_fn);
    
    // Camada oculta 2
    StaticMatrix<float,4,1> matrix_w2; 
    StaticMatrix<float,1,1> matrix_b2;

    matrix_w2.apply(rand_fn);
    matrix_b2.apply(rand_fn);

    StaticMatrix<float,2,1> matrix_inp;
    StaticMatrix<float,1,1> matrix_out;
     
    int epochs = 1000000;
    float learning_rate = 0.5;

    for(int e=0;e<epochs;e++){

        for(size_t i=0;i<inputs.size();i++){


            const auto &X =  inputs[i];
            const auto &Y =  targets[i];
            
            // FORWARD PASS
            auto Z1 = (X * matrix_w1) + matrix_b1;
            auto A1 = Z1.map(sigmoid);

            auto Z2 = (A1 * matrix_w2) + matrix_b2;
            auto A2 = Z2.map(sigmoid);


            // BACKPROPAGATION
            auto error_out = Y - A2;
            auto dA2 = A2.map(sigmoid_derivative);
            auto delta2 = error_out.hadamart_product(dA2);

            auto error_hidden = delta2 * matrix_w2.transpose();
            auto dA1 = A1.map(sigmoid_derivative);
            auto delta1 = error_hidden.hadamart_product(dA1);

            matrix_w2 = matrix_w2 + (A1.transpose() * delta2) * learning_rate;
            matrix_b2 = matrix_b2 + delta2 *learning_rate;
            
            matrix_w1 = matrix_w1 + (X.transpose() * delta1) * learning_rate;
            matrix_b1 = matrix_b1 + delta1 *learning_rate;

        }
    }

    // Teste do Modelo Treinado
    std::cout << "--- Resultados XOR com StaticMatrix ---" << std::endl;
    for (size_t i = 0; i < inputs.size(); ++i) {
        auto Z1 = (inputs[i] * matrix_w1) + matrix_b1;
        auto A1 = Z1.map(sigmoid);

        auto Z2 = (A1 * matrix_w2) + matrix_b2;
        auto A2 = Z2.map(sigmoid);

        std::cout << inputs[i](0, 0) << " XOR " << inputs[i](0, 1)
                  << " = " << A2(0, 0)
                  << " (Esperado: " << targets[i](0, 0) << ")" << std::endl;
    } 


}




// #endif