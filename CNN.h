#ifndef CNN_H
#define CNN_H
#include "TE_defines.h"
#include "Batch.h"
class CNN
{
public:
    CNN();
    ConvL1Core Core1;
    ConvL2Core Core2;
    Weight39 W39;
    Bias9 B9;
    Weight910 W910;
    void run(const Sample *, SFL10Out &) const;
    void run(const Batch *,ConfusionMat &,bool=true) const;
    Eigen::Map<Eigen::ArrayXd> toMap();
    Eigen::Map<const Eigen::ArrayXd> toConstMap() const;
    static const int GeneL=
            (sizeof(Core1)+sizeof(Core2)+sizeof(W39)+sizeof(B9)+sizeof(W910))/sizeof(double);
    static void dispOffset();
};

const double GeneL_d_str=sizeof(CNN)/double(sizeof(double));
const uint32_t GeneL_str=GeneL_d_str;

void test_CNN(const Batch *);

#endif // CNN_H
