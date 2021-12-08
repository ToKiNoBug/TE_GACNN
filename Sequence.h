#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "defines.h"

class Sequence
{
public:
    Sequence();
    std::vector<Input> val;
    bool isNormal;
    bool load(const std::string &);
    void mapMinMax();


    static RawData min;
    static RawData max;
    void beginMinMax() const;//reset whole
    void updateMinMax() const;//update
};

void test_Sequence();
#endif // SEQUENCE_H