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

    Eigen::Array22d accuracyMat;
    accuracyMat.setZero();
    const RNN & result=algorithm.elite()->network;

    std::cout<<"Traversing the whole data set..."<<std::endl;

    for(const Batch & i : algorithm.getDataSet()) {
        for(const Sample & j : i) {
            bool isRight=result.run_sort(j);
            if(j.second) {
                if(isRight) {
                    accuracyMat(0,0)++;
                } else {
                    accuracyMat(1,0)++;
                }
            } else {
                if(isRight) {
                    accuracyMat(1,1)++;
                }
                else {
                    accuracyMat(0,1)++;
                }
            }
        }
    }
    accuracyMat/=accuracyMat.sum();
    std::cout<<"Accuracy Matrix=\n"<<accuracyMat<<std::endl;

    system("pause");
    return 0;
}
