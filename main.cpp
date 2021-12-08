#include "Gene.h"
#include "GeneticRNN.h"

#include <ctime>
#include <cmath>

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));

    GeneticRNN algorithm;
    algorithm.run();

    algorithm.elite()->network.writeToFile("./result.m");
    algorithm.saveTrainingRecord("./TrainingRecord.m");

    system("pause");
    return 0;
}
