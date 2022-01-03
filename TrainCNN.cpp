#include "TrainCNN.h"

#ifdef TE_CNN_PARALLELIZE
Algo_t::Algo_t() {}

Algo_t::~Algo_t() {}

void Algo_t::calculateAll() {
    std::queue<QFuture<void>> waitting;
    for(auto & i : _population) {
        QFuture<void> future=QtConcurrent::run(
                    [](fitnessFun _fFun,Algo_t::Gene* g,const Algo_t::ArgsType * args) {
                g->_Fitness=_fFun(&g->self,args);
    }
                    //_fitnessFun,&i.self,&_args
                    );
    }
}

void Algo_t::calculate(fitnessFun _fFun,
                       Algo_t::Gene* g,
                       const Algo_t::ArgsType * args) {
    g->_Fitness=_fFun(&g->self,args);
}
#endif

void trainCNN() {

}
