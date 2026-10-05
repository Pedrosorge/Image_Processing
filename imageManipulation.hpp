#ifndef IMAGEMANIPULATION_HPP
#define IMAGEMANIPULATION_HPP

#include "matrix.hpp"
#include <iostream>
#include <fstream>
#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct ImageObject{

    struct Pixel{
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    
    std::string type;
    int width, height, max_val;
    Matrix<Pixel> matrix;

    // Construtor padrão
    ImageObject() = default;

    // Construtor para carregar imagem .ppm
    ImageObject(std::string Path){

        std::ifstream file(Path);

        if(!file.is_open()){
            std::cerr << "The image file could not be open!!";
            return;
        }

        file >> type >> width >> height >> max_val;

        matrix = Matrix<Pixel>(height, width);

        int r, g, b;
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                file >> r >> g >> b;
                matrix(i, j) = { 
                    static_cast<uint8_t>(r), 
                    static_cast<uint8_t>(g), 
                    static_cast<uint8_t>(b)
                };
            }
        }

        file.close();

    }

    // Função para carregar PNG, JPG, BMP, etc.
    bool carregar_png(const std::string& path) {
        int canais_originais = 0;

        // stbi_load descompacta o PNG na RAM.
        // O parâmetro '3' força a saída em 3 canais (RGB), descartando o canal Alfa (transparência) se houver.
        unsigned char* img_bytes = stbi_load(path.c_str(), &width, &height, &canais_originais, 3);

        if (!img_bytes) {
            std::cerr << "Erro ao carregar o PNG: " << stbi_failure_reason() << "\n";
            return false;
        }

        // Re-inicializa a matriz dinâmica com o tamanho correto da imagem
        matrix = Matrix<Pixel>(height, width);

        // Copia os pixels da memória descompactada para a sua Matrix<Pixel>
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                // Cálculo da posição linear do pixel no buffer
                int index = (i * width + j) * 3;

                matrix(i, j) = {
                    img_bytes[index],     // R
                    img_bytes[index + 1], // G
                    img_bytes[index + 2]  // B
                };
            }
        }

        // Libera a memória alocada internamente pela stb_image
        stbi_image_free(img_bytes);
        return true;
    }

    // Salva uma Matrix<Pixel> (RGB) diretamente em arquivo .PNG
    static bool salvar_png_rgb(const Matrix<Pixel>& img, const std::string& filename) {
        int width = img.get_cols();
        int height = img.get_rows();

        // Como a struct Pixel contém uint8_t r, g, b consecutivos em memória (3 bytes por pixel),
        // podemos passar o ponteiro de dados direto da matriz!
        int sucesso = stbi_write_png(
            filename.c_str(), 
            width, 
            height, 
            3,                       // 3 canais (RGB)
            img.data.data(),         // Ponteiro para o início dos pixels
            width * 3                // Stride (passo da linha em bytes)
        );

        return sucesso != 0;
    }

    // Salva a matriz de grayscale(float) como png
    static bool salvar_png_grayscale(const Matrix<float>& img, const std::string& filename) {
        int width = img.get_cols();
        int height = img.get_rows();

        // Buffer temporário de 1 byte por pixel
        std::vector<uint8_t> buffer(width * height);

        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                float val = img(r, c);

                // Clamping para evitar overflow/underflow
                if (val < 0.0f) val = 0.0f;
                if (val > 1.0f) val = 1.0f;

                buffer[r * width + c] = static_cast<uint8_t>(val * 255.0f);
            }
        }

        // Argumentos do stbi_write_png:
        // (nome_arquivo, largura, altura, num_canais=1, ponteiro_dados, stride_bytes=largura)
        return stbi_write_png(filename.c_str(), width, height, 1, buffer.data(), width) != 0;
    }

    // Salva a imagem como arquivo .ppm
    static void salvar_ppm(const Matrix<float>& img, const std::string& filename) {

        std::ofstream file(filename);
        if (!file.is_open()) return;

        file << "P2\n";
        file << img.get_cols() << " " << img.get_rows() << "\n";
        file << "255\n"; 

        for (int r = 0; r < img.get_rows(); ++r) {
            for (int c = 0; c < img.get_cols(); ++c) {
                float val = img(r, c);
                
                if (val < 0.0f) val = 0.0f;
                if (val > 1.0f) val = 1.0f;

                int pixel_uint8 = static_cast<int>(val * 255.0f);
                file << pixel_uint8 << " ";
            }
            file << "\n";
        }
        file.close();
    }
    
    // Transforma imagem rgb para matrix em grayscale
    Matrix<float> grayscale_transform() const{

        Matrix<float> gray_matrix(height, width);
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                float r = matrix(i, j).r / 255.0f;
                float g = matrix(i, j).g / 255.0f;
                float b = matrix(i, j).b / 255.0f; 
                gray_matrix(i,j) = 0.299f*r + 0.587f*g + 0.114f*b;
            }
        }
        return gray_matrix;
    }


};

#endif