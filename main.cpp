#include <QCoreApplication>
//#include "Sequence.h"
#include "TE_defines.h"
#include "Sequence.h"
#include "CNN.h"
#include "DataSetMaker.h"
#include <Genetic>
#include <ctime>
void initializeSrand();

int main(int argc, char *argv[])
{
    initializeSrand();

    //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    initializeDataSet();

    //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    std::vector<Batch> trainning;
    uint32_t dataSetSize,batchSize;
    std::cout<<"data set size = ";
    std::cin>>dataSetSize;
    std::cout<<"batch size = ";
    std::cin>>batchSize;
    makeBatches(&trainning,dataSetSize,batchSize,true);

    test_CNN(trainning.data());

    //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    std::cout<<"Eigen's thread num : "<<Eigen::nbThreads()<<std::endl;


    //test_Sequence();
    //QCoreApplication a(argc, argv);
    system("pause");
    return 0;
}

void initializeSrand() {
    std::time_t t=std::time(nullptr);
    std::srand((t>>32)^(t&0xFFFFFFFF));
}
