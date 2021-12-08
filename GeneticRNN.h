#ifndef GENETICRNN_H
#define GENETICRNN_H

//#include <QObject>

#include "defines.h"
#include "DataSetGenerator.h"
#include "Gene.h"

class GeneticRNN //: public QObject
{
    //Q_OBJECT
public:
    explicit GeneticRNN();
    ~GeneticRNN();
    void run();

    enum AlgoStatue {
        none,
        initialized,
        running,
        finished
    };

    const Gene * elite() const;
    AlgoStatue currentStatue() const;
    void saveTrainingRecord(const std::string &) const;

    static const int reportRate=20;

private:
    std::vector<Gene> population;
    std::vector<Batch> dataSet;
    uint16_t eliteIdx;
    int generation;
    int failTimes;
    int currentBatchIdx;
    int epochNum;

    AlgoStatue statue;

    std::vector<double> record;

    void initialize();

    void calculateAll();
    void select();
    void crossover();
    void mutate();
};

#endif // GENETICRNN_H
