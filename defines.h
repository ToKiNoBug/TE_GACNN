#ifndef DEFINES_H
#define DEFINES_H

//#define EIGEN_NO_DEBUG
#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>

//#define logsig(x) 1.0/(1.0+(-(x)).array.exp())

const int InputCount=52;
typedef Eigen::Vector<double,InputCount> RawData;
typedef Eigen::Vector<double,InputCount+1> Input;// single statue

const int HiddenCount=14;
typedef Eigen::Matrix<double,HiddenCount,InputCount+1> In2Hid;//U
typedef Eigen::Vector<double,HiddenCount+1> HiddenLayer;
typedef Eigen::Matrix<double,HiddenCount,HiddenCount> Hid2Hid;//W

const int OutputCount=2;
typedef Eigen::Vector<double,OutputCount> OutLayer;
typedef Eigen::Matrix<double,2,HiddenCount+1> Hid2Out;//V

const int SampleLength=48;
typedef std::pair<const Input*,bool> Sample;

const OutLayer normalOut={1,0},abnormalOut={0,1};

const double LearningRate=0.05;

typedef std::vector<Sample> Batch;
const int SuggestedBatchSize=50;
const int DataSetSize=2000;
const double normalRatio=0.4;


const int populationSize=50;
//种群规模
const double crossoverProb=0.8;
//交叉概率
const double mutateProb=0.05;
//变异概率
const int maxGeneration=3000;
//最大允许代数
const int maxFailTimes=50;
//连续50代没有更优的个体产生，则结束

const double iniWeightScale=0.5;
//权值初始化范围-0.5~0.5

double randDouble();

void writeMatrixAsMatlab(const Eigen::MatrixXd,std::fstream&);

#endif // DEFINES_H
