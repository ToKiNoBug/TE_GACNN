#include "RNN.h"
#include <fstream>
RNN::RNN() {
U.setZero();
W.setZero();
V.setZero();
}

RNN::~RNN() {

}

void RNN::run_output(Sample s, OutLayer & out) const {
    HiddenLayer HL;
    HL.setZero();HL(HiddenCount)=1;

    for(int idx=0;idx<SampleLength;idx++) {
        auto In=U*(*s.first);
        auto Se=W*HL.segment<HiddenCount>(0).eval();
        HL.segment<HiddenCount>(0).array()=
                1.0/(1.0+(-(In+Se).array()).exp());
        s.first++;
    }
    out=1.0/(1.0+((V*HL).array()).exp());
}

double RNN::run(const Sample & s) const {
    OutLayer out;
    run_output(s,out);
    if(s.second) {
        return (out-normalOut).squaredNorm();
    } else {
        return (out-abnormalOut).squaredNorm();
    }
}

bool RNN::run_sort(const Sample & s) const {
    OutLayer out;
    run_output(s,out);
    bool isNormal=out(0)>out(1);

    return isNormal;
}

Eigen::Map<Eigen::ArrayXd> RNN::toMap() const {
    //static const int u=sizeof(Eigen::Map<Eigen::ArrayXd>);
    return Eigen::Map<Eigen::ArrayXd>((double*)this,geneLength);
}

void RNN::writeToFile(const std::string & fileName) const {
    std::fstream file;
    file.open(fileName,std::ios::out);

    file<<"U=";
    writeMatrixAsMatlab(U,file);
    file<<"\n\nW=";
    writeMatrixAsMatlab(W,file);
    file<<"\n\nV=";
    writeMatrixAsMatlab(V,file);

    file<<"\n\n";
    file.close();
}
