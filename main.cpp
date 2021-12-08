#include "MainWindow.h"

#include <QApplication>

#include "Sequence.h"
#include "Gene.h"

#include "DataSetGenerator.h"

#include <ctime>
#include <cmath>

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    std::vector<Batch> dest;
    makeDataSet(dest);

    Gene g;
    g.initialize(0.5);

    std::vector<double> fitnesses;
    fitnesses.clear();
    fitnesses.reserve(dest.size());

    for(const Batch & i : dest) {
        g.setUncalculated();
        g.calculateFitness(i);
        fitnesses.emplace_back(g.fitness);
    }
    double mean=0;
    std::cout<<"Fitnesses on all batchs = [";
    for(auto i : fitnesses) {
        mean+=i;
        std::cout<<i<<", ";
    }
    std::cout<<"]"<<std::endl;
    std::cout<<"mean Fitness="<<mean/fitnesses.size()<<std::endl;

    return a.exec();
}
