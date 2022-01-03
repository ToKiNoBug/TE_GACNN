#ifndef DATASETMAKER_H
#define DATASETMAKER_H
#include "Sequence.h"
#include "Batch.h"
///for trainning
extern const std::string normalDatas[];
extern const std::string abnormalDatas[];

///for testing
extern const std::string normalDatas_T[];
extern const std::string abnormalDatas_T[];

//extern std::list<Sequence> normalDataSource;
//extern std::list<Sequence> abnormalDataSource;
///batches are based on data set (use pointers)
void initializeDataSet(bool =true);

///make batches for trainning or testing
void makeBatches(std::vector<Batch> * dest,
                 const uint32_t dataSetSize,
                 const uint32_t batchSize,
                 const bool =true);

#endif // DATASETMAKER_H
