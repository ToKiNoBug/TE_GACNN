#include "CNN.h"

CNN::CNN()
{

}

Eigen::Map<Eigen::ArrayXd> CNN::toMap() {
    return Eigen::Map<Eigen::ArrayXd>((double *)this,GeneL_str);
}

Eigen::Map<const Eigen::ArrayXd> CNN::toConstMap() const {
    return Eigen::Map<const Eigen::ArrayXd>((const double *)this,GeneL_str);
}

void CNN::dispOffset() {
    std::cout<<"offsetof Core1 = "<<offsetof(CNN,Core1)
            <<" , sizeof Core1 = "<<sizeof(Core1)<<std::endl;
    std::cout<<"offsetof Core2 = "<<offsetof(CNN,Core2)
            <<" , sizeof Core2 = "<<sizeof(Core2)<<std::endl;
    std::cout<<"offsetof W39= "<<offsetof(CNN,W39)
            <<" , sizeof W39 = "<<sizeof(W39)<<std::endl;
    std::cout<<"offsetof B9 = "<<offsetof(CNN,B9)
            <<" , sizeof B9 = "<<sizeof(B9)<<std::endl;
    std::cout<<"offsetof W910 = "<<offsetof(CNN,W910)
            <<" , sizeof W910= "<<sizeof(W910)<<std::endl;
}

void CNN::run(const Batch * b,ConfusionMat & cm,bool applyNorm) const {
    cm.setZero();
    SFL10Out out;
    for(const auto & it : *b) {
        run(&it,out);
        int res=-1;
        out.maxCoeff(&res);
        cm(!res,!it.second)++;
    }
    /*
    for(uint16_t c=0;c<cm.cols();c++) {
        cm.col(c)/=b->size();
    }
    */
    if(applyNorm) {
        for(uint16_t c=0;c<cm.cols();c++) {
            cm.col(c)/=(cm.col(c).sum()+1e-10);
        }
    }

}

void CNN::run(const Sample * s, SFL10Out &SFL10O) const {
    //expand a statue into a matrix(stored in an vector of double[54]
    //so it can be taken as double[54][10]
    //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    sampleMat raw(s->first->data(),InputCount+2,SampleLength);
    ConvL1Out CL1O;
#ifdef TE_CNN_FILL_NAN
    CL1O.setConstant(std::nan(""));
#endif
    for(uint16_t r=0;r<CL1O.rows();r++) {
        for(uint16_t c=0;c<CL1O.cols();c++) {
            CL1O(r,c)=ReLu(
                        (raw.block<ConvL1CoreL,ConvL1CoreL>(r,c)*Core1).sum()
                        );
        }
    }

    //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    ConvL2Out CL2O;
#ifdef TE_CNN_FILL_NAN
    CL2O.setConstant(std::nan(""));
#endif
    for(uint16_t r=0;r<CL2O.rows();r++) {
        for(uint16_t c=0;c<CL2O.cols();c++) {
            CL2O(r,c)=ReLu(
                        (CL1O.block<ConvL2CoreL,ConvL2CoreL>(r,c)*Core2).sum()
                        );
        }
    }

    //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    PoolL3Out PL3O;
#ifdef TE_CNN_FILL_NAN
    PL3O.setConstant(std::nan(""));
#endif
    for(uint16_t r=0;r<PL3O.rows();r++) {
        for(uint16_t c=0;c<PL3O.cols();c++) {
            PL3O(r,c)=
                    CL2O.block<PoolL3StrideR,PoolL3StrideC>
                    (r*PoolL3StrideR,c*PoolL3StrideC).sum()
                    /(PoolL3StrideR*PoolL3StrideC);
        }
    }

    //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    FCL9In FCL9I(PL3O.data());

    FCL9Out FCL9O;
#ifdef TE_CNN_FILL_NAN
    FCL9O.setConstant(std::nan(""));
#endif

    auto temp1=W39*FCL9I+B9;

    FCL9O.segment<FC9Count>(0)=eig_logsig(temp1);

    FCL9O(FC9Count)=1;

    //SFL10Out SFL10O;
#ifdef TE_CNN_FILL_NAN
    SFL10O.setConstant(std::nan(""));
#endif

    SFL10O=eig_logsig(W910*FCL9O);

}

void test_CNN(const Batch *b) {

    Eigen::Array22d mat22d;
    mat22d.setConstant(std::nan(""));
    std::cout<<"nan(2,2)=\n"<<mat22d<<std::endl;

    CNN::dispOffset();

    CNN network;
    network.toMap().setRandom();
    std::cout<<"Core1=\n"<<network.Core1<<std::endl;
    std::cout<<"Core2=\n"<<network.Core2<<std::endl;
    std::cout<<"W39=\n"<<network.W39<<std::endl;

    std::cout<<"B9^T=\n"<<network.B9.transpose()<<std::endl;

    std::cout<<"W910=\n"<<network.W910<<std::endl;

    SFL10Out r;
    network.run(&b->front(),r);
    std::cout<<"r=\n"<<r<<std::endl;

    ConfusionMat cm;

    network.run(b,cm);
    std::cout<<"ConfusionMat=\n"<<cm<<std::endl;

}
