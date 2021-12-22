#include "DataSetGenerator.h"

#include <unordered_set>

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

const Input* stochaticSample(const std::list<Sequence>&,
                       std::unordered_set<const Input*> & selected);

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
        for(auto & i : normalDataSource) {
            i.updateMinMax();
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

    std::unordered_set<const Input*> normal_selected,abnormal_selected;
    normal_selected.clear();abnormal_selected.clear();
    normal_selected.reserve(DataSetSize*0.6);
    abnormal_selected.reserve(DataSetSize*0.6);

    for(int inserted=0;inserted<DataSetSize;) {
        const Input* result;
        if(randDouble()<normalRatio) {
            result=stochaticSample(normalDataSource,normal_selected);
        } else {
            result=stochaticSample(abnormalDataSource,abnormal_selected);
        }
        if(result==nullptr) {
            continue;
        }
        inserted++;
    }

    std::vector<Sample> FullSample;
    FullSample.clear();
    FullSample.reserve(normal_selected.size()+abnormal_selected.size());

    for(auto i : normal_selected) {
        FullSample.emplace_back(std::make_pair(i,true));
    }
    for(auto i : abnormal_selected) {
        FullSample.emplace_back(std::make_pair(i,false));
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
    for(auto & it : dest) {
        std::cout<<it.size()<<" , ";
        it.makeTrueCount();
    }
    std::cout<<"]"<<std::endl;

}

const Input* stochaticSample(const std::list<Sequence>& src,
                       std::unordered_set<const Input*> & selected) {
    std::list<Sequence>::const_iterator selectedSeq;
    int n=src.size();
    //choose seq randomly
    for(auto it=src.cbegin();it!=src.cend();it++) {
        if(std::rand()%n==0) {
            selectedSeq=it;
            break;
        }
        n--;
    }

    const Input* result;
    for(uint8_t failTimes=0;;failTimes++) {
        if(failTimes>=255) {
            std::cerr<<"Error! failed to make unique sample"<<std::endl;
            result=nullptr;
            return result;
        }
        int offset=std::rand()%(selectedSeq->val.size()-SampleLength+1);
        result=selectedSeq->val.data()+offset;
        if(selected.find(result)==selected.end()) {
            break;
        }
    }
    selected.insert(result);

    return result;
}
