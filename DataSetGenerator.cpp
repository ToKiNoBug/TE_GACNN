#include "DataSetGenerator.h"

const std::string normalDatas[]={
    "D:/Git/TE_GARNN/DataSet/Normal.data"
};

const std::string abnormalDatas[]={
    "D:/Git/TE_GARNN/DataSet/Error01.data",
    "D:/Git/TE_GARNN/DataSet/Error02.data",
    "D:/Git/TE_GARNN/DataSet/Error03.data",
    "D:/Git/TE_GARNN/DataSet/Error04.data",
    "D:/Git/TE_GARNN/DataSet/Error05.data",
    "D:/Git/TE_GARNN/DataSet/Error06.data",
    "D:/Git/TE_GARNN/DataSet/Error07.data",
    "D:/Git/TE_GARNN/DataSet/Error08.data",
    "D:/Git/TE_GARNN/DataSet/Error09.data",
    "D:/Git/TE_GARNN/DataSet/Error10.data",
    "D:/Git/TE_GARNN/DataSet/Error11.data",
    "D:/Git/TE_GARNN/DataSet/Error12.data",
    "D:/Git/TE_GARNN/DataSet/Error13.data",
    "D:/Git/TE_GARNN/DataSet/Error14.data",
    "D:/Git/TE_GARNN/DataSet/Error15.data",
    "D:/Git/TE_GARNN/DataSet/Error16.data",
    "D:/Git/TE_GARNN/DataSet/Error17.data",
    "D:/Git/TE_GARNN/DataSet/Error18.data",
    "D:/Git/TE_GARNN/DataSet/Error19.data",
    "D:/Git/TE_GARNN/DataSet/Error20.data",
};

std::list<Sequence> normalDataSource,abnormalDataSource;

Sample stochaticSample(const std::list<Sequence>&,bool isNormal);

void makeDataSet(std::vector<Batch> & dest) {
    dest.clear();
    dest.resize(DataSetSize/SuggestedBatchSize);

    normalDataSource.clear();
    abnormalDataSource.clear();
    Sequence s;
    for(uint32_t idx=0;idx<(sizeof(normalDatas)/sizeof(std::string));idx++) {
        if(!s.load(normalDatas[idx])) {
            continue;
        }
        s.isNormal=true;
        normalDataSource.emplace_back(s);
    }

    for(uint32_t idx=0;idx<(sizeof(abnormalDatas)/sizeof(std::string));idx++) {
        if(!s.load(abnormalDatas[idx])) {
            continue;
        }
        s.isNormal=false;
        abnormalDataSource.emplace_back(s);
    }

    std::cout<<abnormalDataSource.size()<<std::endl;

    std::vector<Sample> FullSample;
    FullSample.clear();
    FullSample.reserve(DataSetSize);

    while(FullSample.size()<DataSetSize) {
        if(randDouble()<=normalRatio) {
            FullSample.emplace_back(stochaticSample(normalDataSource,true));
        } else {
            FullSample.emplace_back(stochaticSample(abnormalDataSource,false));
        }
    }

    std::random_shuffle(FullSample.begin(),FullSample.end());

    for(auto & it : dest) {
        it.clear();
        it.reserve(SuggestedBatchSize+1);
    }

    for(uint32_t sampleIdx=0;sampleIdx<FullSample.size();sampleIdx++) {
        dest[sampleIdx%dest.size()].emplace_back(FullSample[sampleIdx]);
    }

    std::cout<<"Batch count : "<<dest.size()<<std::endl;

    std::cout<<"Batch sizes : [";
    for(const auto & it : dest) {
        std::cout<<it.size()<<" , ";
    }
    std::cout<<"]"<<std::endl;

}

Sample stochaticSample(const std::list<Sequence>& src,bool isNormal) {
    std::list<Sequence>::const_iterator selectedSeq;
    int n=src.size();
    for(auto it=src.cbegin();it!=src.cend();it++) {
        if(std::rand()%n==0) {
            selectedSeq=it;
            break;
        }
        n--;
    }

    Sample result;
    int offset=std::rand()%(selectedSeq->val.size()-SampleLength+1);
    result.first=selectedSeq->val.data()+offset;
    result.second=isNormal;
    return result;
}
