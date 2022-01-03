#include <QCoreApplication>
//#include "Sequence.h"
#include "TE_defines.h"
#include "Sequence.h"
#include "CNN.h"
#include <Genetic>
int main(int argc, char *argv[])
{
    std::cout<<"Eigen's thread num : "<<Eigen::nbThreads()<<std::endl;

    test_Sequence();
    //QCoreApplication a(argc, argv);
    system("pause");
    return 0;
}
