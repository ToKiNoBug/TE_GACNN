#ifndef GENE_H
#define GENE_H
#include "defines.h"
#include "RNN.h"

union ParameterPack {
public:
    ParameterPack();
    ~ParameterPack();
    RNN network;
    Eigen::Array<double,geneLength,1> DNA;
};

class Gene {
public:
    Gene();
    ~Gene();
    ParameterPack value;
    bool isCalculated;
    double fitness;

    void initialize(double scale);
    static void crossover(Gene*,Gene*,int);
    void mutate(int);

    void calculateFitness(const Batch &);
    void setUncalculated();
};

void test_Network();

void test_Gene();
#endif // GENE_H
