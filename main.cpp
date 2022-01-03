//#include <QCoreApplication>

#include "TE_defines.h"
#include "Sequence.h"
#include "CNN.h"
#include "DataSetMaker.h"
#include "TrainCNN.h"
//#include "TE_defines.cpp"
#include <ctime>
void initializeSrand();


int main(int argc, char *argv[])
{

    std::cout<<"Eigen's thread num : "<<Eigen::nbThreads()<<std::endl;

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

    Algo_t algo;
    AT::GAOption opt;
    std::cout<<"population Size = ";
    std::cin>>opt.populationSize;
    std::cout<<"max generations = ";
    std::cin>>opt.maxGenerations;
    std::cout<<"max fail times = ";
    std::cin>>opt.maxFailTimes;
    std::cout<<"crossover prob = ";
    std::cin>>opt.crossoverProb;
    std::cout<<"mutate prob = ";
    std::cin>>opt.mutateProb;


    Algo_t::ArgsType args;
    //sizeof(args);
    std::get<dataIdx>(args)=&trainning;
    std::get<batchIdxIdx>(args)=0;
    std::cout<<"weight min = ";
    std::cin>>std::get<minIdx>(args);
    std::cout<<"weight max = ";
    std::cin>>std::get<maxIdx>(args);
    std::cout<<"Learning rate = ";
    std::cin>>std::get<LrIdx>(args);

    algo.initialize(initializer,
                    crossEntropy,
                    discreteSwap,
                    mutate,switchBatch,
                    opt,args);
    std::cout<<"GA initialized , press to start trainning"<<std::endl;

    system("pause");
    algo.run();
    std::cout<<"Finished with "<<algo.generation()<<" generations"<<std::endl;
    const CNN & result=algo.result();
    ConfusionMat cm;
    cm.setZero();
    for(const auto & i : trainning) {
        ConfusionMat _cm;
        result.run(&i,_cm,false);
        cm+=_cm;
    }

    for(uint32_t c=0;c<cm.cols();c++) {
        cm.col(c)/=(cm.col(c).sum()+1e-10);
    }

    const auto & record=algo.recording();


    {
        Eigen::Map<const Eigen::ArrayXd> eig_re(record.data(),record.size());
        std::fstream file;
        file.open("./result.m",std::ios::out|std::ios::trunc);
        file<<"%% trainning result\n";
        file<<"\nCore1=";
        writeMatrixToFile(result.Core1,file);
        file<<"\nCore2=";
        writeMatrixToFile(result.Core2,file);
        file<<"\nW39=";
        writeMatrixToFile(result.W39,file);
        file<<"\nB9=";
        writeMatrixToFile(result.B9,file);
        file<<"\nW910=";
        writeMatrixToFile(result.W910,file);
        file<<"\n\n\nConfusionMatrix=";
        writeMatrixToFile(cm,file);
        file<<"\n\nFitnesses=";
        writeMatrixToFile(eig_re,file);
        file.close();
    }

    std::cout<<"Confusion matrix = \n"<<cm<<std::endl;
    //QCoreApplication a(argc, argv);
    system("pause");
    return 0;
}

void initializeSrand() {
    std::time_t t=std::time(nullptr);
    std::srand((t>>32)^(t&0xFFFFFFFF));
}

void initializer(CNN* network,const Algo_t::ArgsType* args) {
auto g=network->toMap();
for(auto & x : g) {
    x=AT::randD(std::get<minIdx>(*args),std::get<maxIdx>(*args));
}
}

double crossEntropy(const CNN* network,const Algo_t::ArgsType* args) {
    /*
    static size_t prevBatchIdx=-1;
    bool isBatchSwitched=false;
    if(prevBatchIdx!=std::get<batchIdxIdx>(*args)) {
        prevBatchIdx=std::get<batchIdxIdx>(*args);
        isBatchSwitched=true;
    }
    */
    const Batch * curBatch=std::get<dataIdx>(*args)->data()+std::get<batchIdxIdx>(*args);
    ConfusionMat cm;
    network->run(curBatch,cm);
    static const ConfusionMat E={{1,0},{0,1}};
    return (cm-E).abs().sum();
    /*
    double trueProb=double(curBatch->trueCount())/curBatch->size();
    double falseProb=1-trueProb;
    double entropy=-(trueProb*std::log(1e-10+cm(0,0))+falseProb*std::log(1e-10+cm(1,1)));
    return entropy;*/
}

void discreteSwap(const CNN * par1,const CNN * par2,
        CNN * ch1,CNN * ch2,
                  const Algo_t::ArgsType*) {
    const auto & parent1=par1->toConstMap(),parent2=par2->toConstMap();
    auto child1=ch1->toMap(),child2=ch2->toMap();

    for(uint32_t i=0;i<parent1.size();i++) {
        child1[i]=(std::rand()%2)?parent1[i]:parent2[i];
        child2[i]=(std::rand()%2)?parent1[i]:parent2[i];
    }

}

void mutate(CNN* network,const Algo_t::ArgsType* args) {
    auto x=network->toMap();
    uint32_t mutateIdx=std::rand()%(x.size());
    double & mutateVar=x[mutateIdx];
    mutateVar+=AT::randD(-1,1)*std::get<LrIdx>(*args);
    if(AT::randD()<0.3) {
        mutateVar*=-1;
    }

    mutateVar=std::min(mutateVar,std::get<maxIdx>(*args));
    mutateVar=std::max(mutateVar,std::get<minIdx>(*args));
}

void switchBatch(Algo_t::ArgsType* args,
                 std::list<Algo_t::Gene>* pop,
                 size_t generation,
                 size_t,
                 const AT::GAOption*) {
    if(generation%10==0) {
        std::get<batchIdxIdx>(*args)=(std::get<batchIdxIdx>(*args)+1)
                %(std::get<dataIdx>(*args)->size());
        for(auto & i : *pop) {
            i.setUncalculated();
        }
    }
}


