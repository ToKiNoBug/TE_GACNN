#ifndef CNN_H
#define CNN_H

#include "defines.h"
#include "Batch.h"
class CNN
{
public:
    CNN();
    ~CNN();
    ConvL1Core Core1;
    ConvL2Core Core2;
    Weight39 W39;
    Bias9 B9;
    Weight910 W910;
    void run(const Sample *, SFL10Out &) const;
    void run(const Batch *,ConfusionMat &) const;
    Eigen::Map<Eigen::ArrayXd> toMap();
};

const uint32_t GeneL=sizeof(CNN)/sizeof(double);

#endif // CNN_H