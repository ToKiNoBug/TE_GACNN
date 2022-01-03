#include "DataSetMaker.h"

void pri_makeAnyDataSet(std::vector<Sequence>*,std::vector<Sequence>*,
                        const std::string*,const std::string*,bool=true);

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

const std::string normalDatas_T[]={};

const std::string abnormalDatas_T[]={};

///for trainning
std::vector<Sequence> normalDataSource,abnormalDataSource;
///for testing
std::vector<Sequence> normalDataSource_T,abnormalDataSource_T;

void initializeDataSet(bool isTrainning) {
    if(isTrainning) {
        pri_makeAnyDataSet(&normalDataSource,&abnormalDataSource,
                           normalDatas,abnormalDatas,true);
    }
    else {
        pri_makeAnyDataSet(&normalDataSource_T,&abnormalDataSource_T,
                           normalDatas_T,abnormalDatas_T,false);
    }
}

void pri_makeAnyDataSet(std::vector<Sequence>* n_dst,std::vector<Sequence>* a_dst,
                        const std::string* n_src,const std::string* a_src,bool isTrainning) {
    static const int n_src_size=1,a_src_size=20;
    n_dst->resize(n_src_size);
    a_dst->resize(a_src_size);

    for(int idx=0;idx<n_src_size;idx++) {
        n_dst->at(idx).load(n_src[idx]);
        n_dst->at(idx).isNormal=true;
    }
    for(int idx=0;idx<a_src_size;idx++) {
        a_dst->at(idx).load(a_src[idx]);
        a_dst->at(idx).isNormal=false;
    }
    if(isTrainning) {
        n_dst->front().beginMinMax();
        for(int idx=0;idx<n_src_size;idx++) {
            n_dst->at(idx).updateMinMax();
        }
        for(int idx=0;idx<a_src_size;idx++) {
            a_dst->at(idx).updateMinMax();
        }

    }

    for(int idx=0;idx<n_src_size;idx++) {
        n_dst->at(idx).mapMinMax();
    }
    for(int idx=0;idx<a_src_size;idx++) {
        a_dst->at(idx).mapMinMax();
    }

}

void makeBatches(std::vector<Batch> * dest,
                 const uint32_t dataSetSize,
                 const uint32_t batchSize,
                 const bool isTrainning) {
    std::array<std::vector<Sequence>*,2> src;
    if(isTrainning) {
        src[0]=&normalDataSource;
        src[1]=&abnormalDataSource;
    } else {
        src[0]=&normalDataSource_T;
        src[1]=&abnormalDataSource_T;
    }
    std::vector<Sample> fullSamples;
    fullSamples.clear();
    //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    fullSamples.reserve(dataSetSize);
    //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    const uint32_t batchNum=dataSetSize/batchSize;
    dest->resize(batchNum);
    for(auto & i : *dest) {
        i.clear();
        //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
        i.reserve(batchSize+1);
        //std::cerr<<__FILE__<<" , "<<__LINE__<<std::endl;
    }

    auto fillSamples=[](const std::vector<Sequence>* s,std::vector<Sample>* d) {
        for(const auto & seq : *s) {
            for(uint32_t offset=0;offset+SampleLength<=seq.val.size();offset++) {
                if(d->size()>=d->capacity()) {
                    return;
                }
                d->emplace_back(std::make_pair(seq.val.data()+offset,seq.isNormal));
            }
        }
    };

    fillSamples(src[0],&fullSamples);
    fillSamples(src[1],&fullSamples);

    std::random_shuffle(fullSamples.begin(),fullSamples.end());
    std::random_shuffle(fullSamples.begin(),fullSamples.end());

    for(uint32_t sIdx=0;sIdx<fullSamples.size();sIdx++) {
        uint32_t batchIdx=sIdx%batchNum;
        dest->at(batchIdx).emplace_back(fullSamples[sIdx]);
    }

    for(auto & b : *dest) {
        b.makeTrueCount();
    }

    std::cout<<"Made "
            <<fullSamples.size()
           <<(isTrainning?" trainning":" testing")
          <<" samples in "<<batchNum<<" batches "<<std::endl;


}
