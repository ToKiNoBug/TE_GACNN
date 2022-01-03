#include "CNN.h"

CNN::CNN()
{

}

void CNN::run(const Batch * b,ConfusionMat & cm) const {
    cm.setZero();
    SFL10Out out;
    for(const auto & it : *b) {
        run(&it,out);
        int res=-1;
        out.maxCoeff(&res);
        cm(!res,!it.second)++;
    }
    for(uint16_t c=0;c<cm.cols();c++) {
        cm.col(c)/=b->size();
    }
}

void CNN::run(const Sample * s, SFL10Out &SFL10O) const {
    //expand a statue into a matrix(stored in an vector of double[54]
    //so it can be taken as double[54][10]
    sampleMat raw(s->first->data(),InputCount+2,SampleLength);
    ConvL1Out CL1O;
    CL1O.setConstant(std::nan(""));
    for(uint16_t r=0;r<CL1O.rows();r++) {
        for(uint16_t c=0;c<CL1O.cols();c++) {
            CL1O(r,c)=ReLu(
                        (raw.block<ConvL1CoreL,ConvL1CoreL>(r,c)*Core1).sum()
                        );
        }
    }

    ConvL1Out CL2O;
    CL2O.setConstant(std::nan(""));
    for(uint16_t r=0;r<CL2O.rows();r++) {
        for(uint16_t c=0;c<CL2O.cols();c++) {
            CL1O(r,c)=ReLu(
                        (CL1O.block<ConvL2CoreL,ConvL2CoreL>(r,c)*Core2).sum()
                        );
        }
    }

    PoolL3Out PL3O;
    PL3O.setConstant(std::nan(""));
    for(uint16_t r=0;r<PL3O.rows();r++) {
        for(uint16_t c=0;c<PL3O.cols();c++) {
            PL3O(r,c)=
                    CL2O.block<PoolL3StrideR,PoolL3StrideC>
                    (r*PoolL3StrideR,c*PoolL3StrideC).sum()
                    /(PoolL3StrideR*PoolL3StrideC);
        }
    }

    FCL9In FCL9I(PL3O.data());

    FCL9Out FCL9O;
    FCL9O.setConstant(std::nan(""));

    auto temp1=W39*FCL9I+B9;

    FCL9O.segment<FC9Count>(0)=eig_logsig(temp1);

    FCL9O(FC9Count)=1;

    //SFL10Out SFL10O;

    SFL10O.setConstant(std::nan(""));

    SFL10O=eig_logsig(W910*FCL9O);

}
