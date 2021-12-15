#ifndef BATCH_H
#define BATCH_H
#include "defines.h"

class Batch : public std::vector<Sample>
{
public:
    Batch();

    void makeTrueCount();
    uint32_t trueCount() const;
private:
    uint32_t TrueCount;
};

#endif // BATCH_H
