#include "RNN.h"

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
    OutLayer out=V*HL;
    if(s.second) {
        return (out-normalOut).squaredNorm();
    } else {
        return (out-abnormalOut).squaredNorm();
    }
}

Eigen::Map<Eigen::ArrayXd> RNN::toMap() const {
    return Eigen::Map<Eigen::ArrayXd>((double*)this,geneLength);
}
