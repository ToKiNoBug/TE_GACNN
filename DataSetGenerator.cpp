#include "DataSetGenerator.h"

const std::string normalDatas[]={
    "./DataSet/Normal.data"
};

const std::string abnormalDatas[]={
    "./DataSet/Error01.data",
    "./DataSet/Error02.data",
    "./DataSet/Error03.data",
    "./DataSet/Error04.data",
    "./DataSet/Error05.data",
    "./DataSet/Error06.data",
    "./DataSet/Error07.data",
    "./DataSet/Error08.data",
    "./DataSet/Error09.data",
    "./DataSet/Error10.data",
    "./DataSet/Error11.data",
    "./DataSet/Error12.data",
    "./DataSet/Error13.data",
    "./DataSet/Error14.data",
    "./DataSet/Error15.data",
    "./DataSet/Error16.data",
    "./DataSet/Error17.data",
    "./DataSet/Error18.data",
    "./DataSet/Error19.data",
    "./DataSet/Error20.data",
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

    //std::cout<<abnormalDataSource.size()<<std::endl;

    normalDataSource.front().beginMinMax();
    {
        auto it=normalDataSource.begin();
        it++;
        while(it!=normalDataSource.end()) {
            it->updateMinMax();
        }
        for(auto & i : abnormalDataSource) {
            i.updateMinMax();
        }
    }

    {
        for(auto & i : normalDataSource) {
            i.mapMinMax();
        }
        for(auto & i : abnormalDataSource) {
            i.mapMinMax();
        }
    }

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
