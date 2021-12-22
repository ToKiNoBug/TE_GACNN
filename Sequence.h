#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "defines.h"

class Sequence
{
public:
    Sequence();
    ~Sequence();
    std::vector<Input> val;
    bool isNormal;
    bool load(const std::string &);
    //void logBy(const double);
    void mapMinMax();

    static Eigen::Vector<double,InputCount> min;
    static Eigen::Vector<double,InputCount> max;
    void beginMinMax() const;//reset whole
    void updateMinMax() const;//update
};

void test_Sequence();
#endif // SEQUENCE_H
