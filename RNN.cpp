#include "RNN.h"
#include <fstream>
RNN::RNN() {
U.setZero();
W.setZero();
V.setZero();
}

RNN::~RNN() {

}


double RNN::run(Sample s) const {

    HiddenLayer HL;
    HL.setZero();HL(HiddenCount)=1;

    for(int idx=0;idx<SampleLength;idx++) {
        auto In=U*(*s.first);
        auto Se=W*HL.segment<HiddenCount>(0).eval();
        HL.segment<HiddenCount>(0).array()=
                1.0/(1.0+(-(In+Se).array()).exp());
        s.first++;
    }
    OutLayer out=1.0/(1.0+((V*HL).array()).exp());
    if(s.second) {
        return (out-normalOut).squaredNorm();
    } else {
        return (out-abnormalOut).squaredNorm();
    }
}

bool RNN::run_sort(Sample s) const {
    HiddenLayer HL;
    HL.setZero();HL(HiddenCount)=1;

    for(int idx=0;idx<SampleLength;idx++) {
        auto In=U*(*s.first);
        auto Se=W*HL.segment<HiddenCount>(0).eval();
        HL.segment<HiddenCount>(0).array()=
                1.0/(1.0+(-(In+Se).array()).exp());
        s.first++;
    }
    OutLayer out=1.0/(1.0+((V*HL).array()).exp());
    bool isNormal=out(0)>out(1);

    return isNormal==s.second;
}

Eigen::Map<Eigen::ArrayXd> RNN::toMap() const {
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
