#include "Gene.h"
/*
ParameterPack::ParameterPack() {
DNA.setZero();
}

ParameterPack::~ParameterPack() {
;
}
*/


Gene::Gene() {
    isCalculated=false;
    fitness=0;
}

Gene::~Gene() {
;
}


void Gene::initialize(double scale) {
    /*
    network.U.setRandom();
    network.U*=scale;
    network.V.setRandom();
    network.V*=scale;
    network.W.setRandom();
    network.W*=scale;
    */
    Eigen::Map<Eigen::ArrayXd> map=network.toMap();
    map.setRandom();
    map*=scale;
    isCalculated=false;
}

void Gene::crossover(Gene * a, Gene * b, int idx) {
    if(idx>=geneLength) {
        std::cerr<<"Wrong crossover index "<<idx<<std::endl;
        exit(1);
        return;
    }

    /*
    RNN & netA=a->network,netB=b->network;

    if(idx<netA.U.size()) {
        netA
    }
    */
    Eigen::Map<Eigen::ArrayXd>
            mapA=a->network.toMap(),
            mapB=b->network.toMap();


    /*
    a->value.DNA.segment(idx,geneLength-idx).swap(
                b->value.DNA.segment(idx,geneLength-idx));
    */
    mapA.segment(idx,geneLength-idx).swap(mapB.segment(idx,geneLength-idx));

    a->isCalculated=false;
    b->isCalculated=false;
}

void Gene::mutate(int idx) {
    double & cur=network.toMap()(idx);
    cur+=LearningRate*(2*randDouble()-1);
    if(cur>1) {
        cur=1;
    }
    if(cur<-1) {
        cur=-1;
    }
    isCalculated=false;
}

void Gene::calculateFitness(const Batch & batch) {
    if(batch.size()<=0) {
        return;
    }
    if(isCalculated) {
        return;
    }
#ifdef Fitness_By_Accuracy
    int accuracyTimes=0;
    for(auto it : batch) {
        accuracyTimes+=network.run_sort(it);
    }
    fitness=double(100*accuracyTimes)/batch.size();
#else
    Eigen::Array22d confusion;
    confusion.setConstant(1e-8);
    for(const auto & it : batch) {
        confusion(!network.run_sort(it),!it.second)++;
    }
    confusion.col(0)/=confusion.col(0).sum();
    confusion.col(1)/=confusion.col(1).sum();

    double && negLog2_conf_00=-std::log2(confusion(0,0));
    double && negLog2_conf_11=-std::log2(confusion(1,1));

    double crossEntropy=
            batch.trueCount()*negLog2_conf_00
            +(batch.size()-batch.trueCount())*negLog2_conf_11;

    crossEntropy/=batch.size();
    //error/=batch.size();
    fitness=-crossEntropy;
#endif
    isCalculated=true;
}

void test_Network() {
    std::string path1="D:/Git/TE_GARNN/DataSet/Normal.data",
            path2="D:/Git/TE_GARNN/DataSet/Error01.data";
    Sequence s1,s2;
    if(!s1.load(path1)) {
        return;
    }

    if(!s2.load(path2)) {
        return;
    }

    s1.isNormal=true;
    s2.isNormal=false;
    s1.beginMinMax();
    s2.updateMinMax();
    s1.mapMinMax();
    s2.mapMinMax();
    Batch b;
    b.clear();
    b.reserve(50);
    while(b.size()<50) {
        int offset=std::rand()%914;
        if(offset%2) {
            b.emplace_back(std::make_pair(s1.val.data()+offset,s1.isNormal));
        }
        else {
            b.emplace_back(std::make_pair(s2.val.data()+offset,s2.isNormal));
        }
    }

    Gene g;
    g.initialize(0.5);
    g.calculateFitness(b);
    std::cout<<"g: fitness="<<g.fitness<<std::endl;
    std::cout<<"network U=\n"<<g.network.U<<std::endl;
    std::cout<<"network W=\n"<<g.network.W<<std::endl;
    std::cout<<"network V=\n"<<g.network.V<<std::endl;

}

void Gene::setUncalculated() {
    isCalculated=false;
}

const Gene & Gene::operator=(const Gene & src) {
    this->isCalculated=src.isCalculated;
    this->network.toMap()=src.network.toMap();
    this->fitness=src.fitness;
    return *this;
}

void test_Gene() {
    Gene g1,g2;
    g1.network.toMap().setZero();
    g2.network.toMap().setOnes();

    std::cout<<"g1=\n"<<g1.network.toMap().transpose()<<std::endl;
    std::cout<<"g2=\n"<<g2.network.toMap().transpose()<<std::endl;

    Gene::crossover(&g1,&g2,std::rand()%geneLength);
    std::cout<<"Crossover\n";

    //std::cout<<"g1=\n"<<g1.value.DNA.transpose()<<std::endl;
    //std::cout<<"g2=\n"<<g2.value.DNA.transpose()<<std::endl;

    std::cout<<"g1.U=\n"<<g1.network.U<<std::endl;
    std::cout<<"g2.U=\n"<<g2.network.U<<std::endl;


    std::cout<<"g1.W=\n"<<g1.network.W<<std::endl;
    std::cout<<"g2.W=\n"<<g2.network.W<<std::endl;


    std::cout<<"g1.V=\n"<<g1.network.V<<std::endl;
    std::cout<<"g2.V=\n"<<g2.network.V<<std::endl;

    g1.mutate(2);
    std::cout<<"g1.U=\n"<<g1.network.U<<std::endl;
}

void test_Network2() {
    Gene g;
    g.initialize(0.5);
    Sequence s;
    s.isNormal=true;
    if(!s.load("D:/Git/TE_GARNN/DataSet/Normal.data")) {
        std::cerr<<"Failed to load sequence data"<<std::endl;
        return;
    }

    s.beginMinMax();
    s.mapMinMax();

    Batch b;
    b.clear();
    b.reserve(3);

    b.emplace_back(std::make_pair(s.val.data(),s.isNormal));
    b.emplace_back(std::make_pair(s.val.data()+10,s.isNormal));
    b.emplace_back(std::make_pair(s.val.data()+20,s.isNormal));

    std::vector<OutLayer> outputs;
    outputs.clear();
    outputs.reserve(b.capacity());

    for(const auto & sam : b) {
        OutLayer o;
        g.network.run_output(sam,o);
        outputs.emplace_back(o);
    }

    for(const auto & o : outputs) {
        std::cout<<"Output 1 : =\n"<<o<<std::endl;
    }

}
