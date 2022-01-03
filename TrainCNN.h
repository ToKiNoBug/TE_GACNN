#ifndef TRAINCNN_H
#define TRAINCNN_H

#define

#include <Genetic>
#include "TE_defines.h"
#include "CNN.h"
#include "DataSetMaker.h"
#include "Batch.h"


typedef AT::GA<CNN, //network
false,//less fitness means better
const std::vector<Batch>*,//batches
uint32_t,//current batch idx
double,//min val
double,//max val
double//learning rate
> AlgoBase;

///used of index in tuple
const uint32_t dataIdx=0,batchIdxIdx=1,minIdx=2,maxIdx=3,LrIdx=4;

#ifdef TE_CNN_PARALLELIZE
class Algo_t : public AlgoBase
{
public:
    Algo_t();
    ~Algo_t();

protected:
    void calculateAll();
    static void calculate(fitnessFun,Algo_t::Gene*,const Algo_t::ArgsType *);
};
#else
typedef AlgoBase Algo_t;
#endif
///initialize function
void initializer(CNN*,const Algo_t::ArgsType*);
///fitness function
double crossEntropy(const CNN*,const Algo_t::ArgsType*);
///crossover function
void discreteSwap(CNN*,CNN*,const Algo_t::ArgsType*);
///mutate function
void mutate(CNN*,const Algo_t::ArgsType*);
///switch batch
void switchBatch(Algo_t::ArgsType*,
                 std::vector<Algo_t::Gene>*,
                 size_t generation,
                 size_t failTimes,
                 const AT::GAOption*);

#endif // TRAINCNN_H
