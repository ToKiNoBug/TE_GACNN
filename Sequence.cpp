#include "Sequence.h"
#include <iostream>
#include <fstream>

RawData Sequence::min,Sequence::max;

Sequence::Sequence()
{
val.clear();
}

bool Sequence::load(const std::string & fileName) {
    std::fstream file;
    file.open(fileName,std::ios::in|std::ios::binary);
    if(file.eof()) {
        std::cerr<<"Failed to load data from file "<<fileName<<std::endl;
        return false;
    }

    file.seekg(0,std::ios::end);
    const int fileSize=file.tellg();
    file.seekg(0,std::ios::beg);
    const int statueCount=std::floor(double(fileSize)/sizeof(RawData));
    val.clear();
    val.reserve(statueCount);
    int loadedState=0;
    while(loadedState<statueCount) {
        val.emplace_back(Input());
        file.read((char*)val.back().data(),sizeof(RawData));
        loadedState++;
    }
    return true;
}

void Sequence::beginMinMax() const {
    min=val.front().segment<InputCount>(0);
    max=min;
    updateMinMax();
}

void Sequence::updateMinMax() const {
    for(const auto & it : val) {
        auto curSeg=it.segment<InputCount>(0).array();
        min=min.array().min(curSeg);
        max=max.array().max(curSeg);
    }
}

void Sequence::mapMinMax() {
    RawData range_Mul=1.0/(max-min).array();
    for(auto & it : val) {
        it.segment<InputCount>(0)-=min;
        it.segment<InputCount>(0).array()*=range_Mul.array();
        it(InputCount)=1;
    }
}

void test_Sequence() {
    std::string path="D:/Git/TE_GARNN/DataSet/Normal.data";
    Sequence s;
    s.isNormal=true;
    if(!s.load(path)) {
        return;
    }
    std::cout<<"s.front=\n"
            <<s.val.front().transpose()<<"\ns.back=\n"<<s.val.back().transpose()<<std::endl;
    std::cout<<"s.size="<<s.val.size()<<std::endl;
    s.beginMinMax();
    std::cout<<"min and max=\n"
            <<s.min.transpose()<<"\n , max=\n"<<s.max.transpose()<<std::endl;

    s.mapMinMax();

    std::cout<<"s.front=\n"<<
               s.val.front().transpose()<<"\ns.back=\n"<<s.val.back().transpose()<<std::endl;
}
