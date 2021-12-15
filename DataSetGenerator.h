#ifndef DATASETGENERATOR_H
#define DATASETGENERATOR_H
#include "defines.h"
#include "Batch.h"
#include "Sequence.h"

extern const std::string normalDatas[];
extern const std::string abnormalDatas[];

//extern std::list<Sequence> normalDataSource;
//extern std::list<Sequence> abnormalDataSource;

void makeDataSet(std::vector<Batch> & dest);

#endif // DATASETGENERATOR_H
