#include "defines.h"
#include <fstream>
/*
double randDouble() {
    return double(std::rand()%16383)/16383;
}*/

void writeMatrixAsMatlab(const Eigen::MatrixXd mat,std::fstream& file) {
    file<<"[\n";
    for(int r=0;r<mat.rows();r++) {
        for(int c=0;c<mat.cols();c++) {
            file<<mat(r,c);
            if(c+1>=mat.cols()) {
                file<<";\n";
            } else {
                file<<",";
            }
        }
    }
    file<<"];\n";
}
