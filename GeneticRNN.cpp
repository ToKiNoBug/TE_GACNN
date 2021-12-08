#include "GeneticRNN.h"

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

    std::cerr<<"population initialized\n";
    std::cerr<<"GA-RNN algorithm initialized"<<std::endl;
}

void GeneticRNN::calculateAll() {
    for(auto & i : population) {
        i.setUncalculated();
        i.calculateFitness(dataSet[currentBatchIdx]);
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

    while (true) {
        if(generation>maxGeneration) {
            std::cerr<<"finished by maxGeneration limit\n";
            break;
        }
        if(failTimes>maxFailTimes) {
            std::cerr<<"finished by maxFailTimes limit\n";
        }

        calculateAll();
        select();
        crossover();
        mutate();

        generation++;
        currentBatchIdx++;
        if((unsigned)currentBatchIdx>=dataSet.size()) {
            epochNum++;
            currentBatchIdx=currentBatchIdx%dataSet.size();
        }

        std::cerr<<"generation "<<generation<<"\n";
        std::cerr<<"best fitness="<<population[eliteIdx].fitness<<std::endl;

    }
}