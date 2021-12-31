#include "GeneticRNN.h"
#include <fstream>
#include <QtConcurrent>
#include <queue>

GeneticRNN::GeneticRNN(){
    statue=AlgoStatue::none;
}

GeneticRNN::~GeneticRNN() {
    //QObject::~QObject();
}

const Gene * GeneticRNN::elite() const {
    if(statue<AlgoStatue::finished) {
        return nullptr;
    }
    return &population[eliteIdx];
}

GeneticRNN::AlgoStatue GeneticRNN::currentStatue() const {
    return statue;
}

void GeneticRNN::initialize() {
    makeDataSet(dataSet);
    std::cerr<<"data set initialized\n";

    std::cout<<"Max=\n"<<Sequence::max.transpose()<<std::endl;
    std::cout<<"Min=\n"<<Sequence::min.transpose()<<std::endl;

    population.resize(populationSize);
    for(auto & i : population) {
        i.initialize(iniWeightScale);
    }
    generation=0;
    failTimes=0;
    eliteIdx=0;
    currentBatchIdx=0;
    epochNum=0;
    statue=AlgoStatue::initialized;
    record.clear();
    record.reserve(maxGeneration);

    std::cerr<<"population initialized\n";
    std::cerr<<"GA-RNN algorithm initialized"<<std::endl;
}

void GeneticRNN::calculateAll() {
    static int prevBatch=-1;
    bool isBatchSwitched=false;
    if(prevBatch!=currentBatchIdx) {
        prevBatch=currentBatchIdx;
        isBatchSwitched=true;
    }
    if(isBatchSwitched)
        for(auto & i : population) {
                i.setUncalculated();
            //i.calculateFitness(dataSet[currentBatchIdx]);
        }

    static const uint32_t threadCount=std::thread::hardware_concurrency();
    static const uint32_t basicTaskCount=population.size()/threadCount;

    std::queue<QFuture<void>> tasks;
    for(auto & i : population) {
        tasks.emplace(QtConcurrent::run(
                          [](Gene * g,const Batch & b){
                          g->calculateFitness(b);
                      },&i,dataSet[currentBatchIdx]));
    }
    /*
    uint32_t taken=0;
    for(uint32_t i=0;i<threadCount;i++) {
        uint32_t curCount=basicTaskCount
                +(i<(population.size()-basicTaskCount*threadCount));

        auto future=QtConcurrent::run(
                    [](Gene * g,const Batch * b,const uint32_t count){
                for(uint32_t idx=0;idx<count;idx++) {
                    (g+idx)->calculateFitness(*b);
        }},population.data()+taken,&dataSet[currentBatchIdx],curCount);
    //std::cout<<"taskSize="<<curCount<<std::endl;
        taken+=curCount;
        tasks.emplace(future);
    }*/

while(!tasks.empty()) {
    tasks.front().waitForFinished();
    tasks.pop();
}



}

void GeneticRNN::select() {
    //uint16_t prevElite=eliteIdx;

    uint16_t bestIdx=0,worstIdx=0;
    double bestFitness=population.front().fitness,worstFitness=bestFitness;

    for(uint16_t i=1;i<population.size();i++) {
        if(bestFitness<population[i].fitness) {
            bestIdx=i;
            bestFitness=population[i].fitness;
        }
        if(worstFitness>population[i].fitness) {
            worstIdx=i;
            worstFitness=population[i].fitness;
        }
    }

    population[worstIdx]=population[bestIdx];

    if(eliteIdx==bestIdx) {
        failTimes++;
    } else {
        failTimes=0;
        eliteIdx=bestIdx;
    }
    //std::cerr<<"selection finished"<<std::endl;
}

void GeneticRNN::crossover() {
    std::vector<uint16_t> crossoverQueue;
    crossoverQueue.clear();
    crossoverQueue.reserve(populationSize);

    for(uint16_t idx=0;idx<populationSize;idx++) {
        if(idx==eliteIdx) {
            continue;
        }
        if(randDouble()<=crossoverProb) {
            crossoverQueue.emplace_back(idx);
        }
    }

    std::random_shuffle(crossoverQueue.begin(),crossoverQueue.end());

    if(crossoverQueue.size()%2==1) {
        crossoverQueue.pop_back();
    }

    for(uint16_t idx=0;idx<crossoverQueue.size();) {
        int crossoverIdx=std::rand()%geneLength;
        uint16_t aIdx=crossoverQueue[idx++];
        uint16_t bIdx=crossoverQueue[idx++];
        Gene::crossover(&population[aIdx],
                &population[bIdx],crossoverIdx);
    }

    //std::cerr<<"crossover finished"<<std::endl;
}

void GeneticRNN::mutate() {
    for(uint16_t idx=0;idx<populationSize;idx++) {
        if(idx==eliteIdx) {
            continue;
        }
        if(randDouble()<=mutateProb) {
            int mutateIdx=std::rand()%geneLength;
            population[idx].mutate(mutateIdx);
        }
    }
    //std::cerr<<"mutation finished"<<std::endl;
}

void GeneticRNN::run() {
    initialize();
    statue=AlgoStatue::running;

    clock_t curTime=std::clock();
    while (true) {

        calculateAll();

        select();
        if(generation>maxGeneration) {
            std::cerr<<"finished by maxGeneration limit\n";
            break;
        }
        if(failTimes>maxFailTimes) {
            std::cerr<<"finished by maxFailTimes limit\n";
            break;
        }

        record.emplace_back(population[eliteIdx].fitness);
        std::cerr<<"generation "<<generation;
        std::cerr<<" , best fitness="<<population[eliteIdx].fitness<<std::endl;

        crossover();
        mutate();

        generation++;
        if(generation%generation_per_batch==0) {
            currentBatchIdx++;
            if((unsigned)currentBatchIdx>=dataSet.size()) {
                epochNum++;
                currentBatchIdx=currentBatchIdx%dataSet.size();
            }
            std::cerr<<"Batch "<<currentBatchIdx<<std::endl;
        }


    }


    int timeUsed=(std::clock()-curTime)/CLOCKS_PER_SEC;
    std::cout<<"Time spend : "<<timeUsed<<"s"<<std::endl;

    std::cerr<<"Trainning finished"<<std::endl;
    statue=AlgoStatue::finished;
}

void GeneticRNN::saveTrainingRecord(const std::string & fileName) const {
    std::fstream file;
    file.open(fileName,std::ios::out);

    file<<"Fitness=[\n";
    for(auto i : record) {
        file<<i<<";\n";
    }
    file<<"];\n";

    file.close();
}

const std::vector<Batch> & GeneticRNN::getDataSet() const {
    return dataSet;
}
