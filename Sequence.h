#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "TE_defines.h"
#include<vector>
class Sequence
{
public:
    Sequence();
    std::vector<Input> val;
    bool isNormal;
    bool load(const std::string &,uint32_t ignored=0);

    void beginMinMax() const;//reset whole
    void updateMinMax() const;//update
    void mapMinMax();

    static RawData min;
    static RawData max;
};

void test_Sequence();

#endif // SEQUENCE_H
