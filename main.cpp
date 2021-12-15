#include "Gene.h"
#include "GeneticRNN.h"

#include <ctime>
#include <cmath>
#include <windows.h>

#ifdef EIGEN_DONT_PARALLELIZE
#error PARALLELIZE is disabled
#endif

int main(int argc, char *argv[])
{
    SetConsoleTitleA("TE-GA-RNN");
    Eigen::initParallel();
    std::cout<<"Thread count = "<<Eigen::nbThreads()<<std::endl;
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
            bool RNN_res=result.run_sort(j);
                accuracyMat(!RNN_res,!j.second)++;
        }
    }

    for(int c=0;c<2;c++) {
        accuracyMat.col(c)/=accuracyMat.col(c).sum();
    }

    std::cout<<"Accuracy Matrix=\n"<<accuracyMat<<std::endl;

    system("pause");
    return 0;
}
