#ifndef RNN_H
#define RNN_H

#include "defines.h"
#include "Sequence.h"

class RNN
{
public:
    RNN();
    ~RNN();
    In2Hid U;
    Hid2Hid W;
    Hid2Out V;

    double run(const Sample &) const;
    bool run_sort(const Sample &) const;

    void run_output(Sample,OutLayer&) const;

    Eigen::Map<Eigen::ArrayXd> toMap() const;

    void writeToFile(const std::string &) const;

    //static const int geneLength;
};

const int geneLength=sizeof(RNN)/sizeof(double);

#endif // RNN_H
