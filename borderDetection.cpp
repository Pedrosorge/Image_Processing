#include "matrix.hpp"
#include "staticmatrix.hpp"
#include "imageManipulation.hpp"
#include <cmath>

void detect_borders(const std::string &path){
    
    ImageObject img;
    img.carregar_png(path);
   
    Matrix<float> gray_img = img.grayscale_transform();


    StaticMatrix<float, 3, 3> sobel_x;
    sobel_x(0,0) = -1.0f; sobel_x(0,1) = 0.0f; sobel_x(0,2) = 1.0f;
    sobel_x(1,0) = -2.0f; sobel_x(1,1) = 0.0f; sobel_x(1,2) = 2.0f;
    sobel_x(2,0) = -1.0f; sobel_x(2,1) = 0.0f; sobel_x(2,2) = 1.0f;

    StaticMatrix<float, 3, 3> sobel_y;
    sobel_y(0,0) = -1.0f; sobel_y(0,1) = -2.0f; sobel_y(0,2) = -1.0f;
    sobel_y(1,0) = 0.0f; sobel_y(1,1) = 0.0f; sobel_y(1,2) = 0.0f;
    sobel_y(2,0) = 1.0f; sobel_y(2,1) = 2.0f; sobel_y(2,2) = 1.0f;
    
    Matrix<float> borders(gray_img.get_rows(),gray_img.get_cols());

    StaticMatrix<float, 3, 3> aux;
    for(int i=1;i<gray_img.get_rows()-1;i++){
        for(int j=1;j<gray_img.get_cols()-1;j++){
            
            // Copies the 3x3 submatrix arount the block (i,j) from gray image to aux
            for(int r=0;r<3;r++){
                for(int c=0;c<3;c++){
                    aux(r,c) = gray_img(i+r-1,j+c-1);
                }
            }
            
            float gx = aux.dot(sobel_x);
            float gy = aux.dot(sobel_y);

            borders(i,j) = std::sqrt(gx*gx + gy*gy);

        }
    }


    int pivot = path.find_last_of('/')+1;
    std::string new_path = path.substr(0,pivot) + "bordered_" + path.substr(pivot,path.size());
    ImageObject::salvar_png_grayscale(borders,new_path);

}

int main(int argc, char * argv[]){

    if(argc < 1){
        std::cout << "Nenhum caminho de documento passado!\n";
        return 0;
    }
    std::string arg = argv[1];

    detect_borders(arg);

    return 0;

}