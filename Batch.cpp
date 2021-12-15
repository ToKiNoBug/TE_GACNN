#include "Batch.h"

Batch::Batch() {
TrueCount=0;
}

void Batch::makeTrueCount() {
    TrueCount=0;
    for(const auto & i : (*this)) {
        if(i.second) {
            TrueCount++;
        }
    }
}

uint32_t Batch::trueCount() const {
    return TrueCount;
}
