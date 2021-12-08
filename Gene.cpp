#include "Gene.h"

ParameterPack::ParameterPack() {

}

ParameterPack::~ParameterPack() {

}

Gene::Gene() {
    isCalculated=false;
}

Gene::~Gene() {

}

void Gene::initialize(double scale) {
    value.DNA.setRandom();
    value.DNA*=scale;
    isCalculated=false;
}

void Gene::crossover(Gene * a, Gene * b, int idx) {
    if(idx>=geneLength) {
        std::cerr<<"Wrong crossover index "<<idx<<std::endl;
        exit(1);
        return;
    }


    a->value.DNA.segment(idx,geneLength-idx).swap(
                b->value.DNA.segment(idx,geneLength-idx));
    a->isCalculated=false;
    b->isCalculated=false;
}

void Gene::mutate(int idx) {
    double & cur=value.DNA(idx);
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
    double error=0;
    for(auto it : batch) {
        error+=value.network.run(it);
    }
    error/=batch.size();
    fitness=1/(1e-8+error);
    isCalculated=true;
}

void test_network() {
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
    std::cout<<"network U=\n"<<g.value.network.U<<std::endl;
    std::cout<<"network W=\n"<<g.value.network.W<<std::endl;
    std::cout<<"network V=\n"<<g.value.network.V<<std::endl;

}

void Gene::setUncalculated() {
    isCalculated=false;
}

void test_Gene() {
    Gene g1,g2;
    g1.value.DNA.setZero();
    g2.value.DNA.setOnes();

    std::cout<<"g1=\n"<<g1.value.DNA.transpose()<<std::endl;
    std::cout<<"g2=\n"<<g2.value.DNA.transpose()<<std::endl;

    Gene::crossover(&g1,&g2,533);
    std::cout<<"Crossover\n";

    //std::cout<<"g1=\n"<<g1.value.DNA.transpose()<<std::endl;
    //std::cout<<"g2=\n"<<g2.value.DNA.transpose()<<std::endl;

    std::cout<<"g1.U=\n"<<g1.value.network.U<<std::endl;
    std::cout<<"g2.U=\n"<<g2.value.network.U<<std::endl;


    std::cout<<"g1.W=\n"<<g1.value.network.W<<std::endl;
    std::cout<<"g2.W=\n"<<g2.value.network.W<<std::endl;


    std::cout<<"g1.V=\n"<<g1.value.network.V<<std::endl;
    std::cout<<"g2.V=\n"<<g2.value.network.V<<std::endl;
}
