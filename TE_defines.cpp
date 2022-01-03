#include "TE_defines.h"

double ReLu(const double x) {
    return std::max(x,0.0);
}

void writeMatrixAsMatlab(Eigen::MatrixXd _mat,std::ofstream& _file) {
    _file<<"[\n";
    for(int r=0;r<_mat.rows();r++) {
        for(int c=0;c<_mat.cols();c++) {
            _file<<_mat(r,c);
            if(c+1>=_mat.cols()) {
                _file<<";\n";
            } else {
                _file<<",";
            }
        }
    }
    _file<<"];\n";
}
