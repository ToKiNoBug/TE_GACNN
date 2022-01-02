#ifndef DEFINES_H
#define DEFINES_H

#include <Genetic>

//#define EIGEN_NO_DEBUG
#define EIGEN_DONT_PARALLELIZE
#include <Eigen/Dense>


#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>

//#define Fitness_By_Accuracy

//2/(1+exp(-2*net))-1
#define eig_logsig(x) (1.0/(1.0+(-(x)).array().exp()))
#define eig_tansig(x) (2.0/(1.0+(-2*(x)).array().exp())-1.0)
#define eig_ReLu(x) ((x).array().max(0))

const int InputCount=52;
typedef Eigen::Vector<double,InputCount> RawData;
typedef Eigen::Vector<double,InputCount+2> Input;// single statue

const double PaddingVal=0;

const int SampleLength=10;
typedef std::pair<const Input*,bool> Sample;
typedef Eigen::Map<const Eigen::Array<double,InputCount+2,SampleLength>> sampleMat;

//network strcuture : conv1->conv2->pool3(30)->fully connect9(15+1)->softmax10(2)

const int ConvL1CoreL=3;
//const int PoolL1Stride=2;
const int ConvL2CoreL=3;
const int PoolL3StrideR=5;
const int PoolL3StrideC=2;

typedef Eigen::Array<double,ConvL1CoreL,ConvL1CoreL> ConvL1Core;//
typedef Eigen::Array<double,ConvL2CoreL,ConvL2CoreL> ConvL2Core;//

typedef Eigen::Array<double,InputCount,SampleLength-2> ConvL1Out;
typedef Eigen::Array<double,InputCount-2,SampleLength-4> ConvL2Out;
typedef Eigen::Array<
double,(InputCount-2)/PoolL3StrideR,(SampleLength-4)/PoolL3StrideC> PoolL3Out;

const int FC9In=((InputCount-2)/PoolL3StrideR)*((SampleLength-4)/PoolL3StrideC);
const int FC9Count=15;
typedef Eigen::Map<Eigen::Vector<double,FC9In>> FCL9In;
typedef Eigen::Matrix<double,FC9Count,FC9In> Weight39;//
typedef Eigen::Vector<double,FC9Count> Bias9;//
typedef Eigen::Vector<double,FC9Count+1> FCL9Out;
const int SF10Count=2;
typedef Eigen::Matrix<double,SF10Count,FC9Count+1> Weight910;//
typedef Eigen::Vector<double,SF10Count> SFL10Out;
//const double LearningRate=0.01;

typedef Eigen::Array<double,SF10Count,SF10Count> ConfusionMat;


const int SuggestedBatchSize=50;
const int DataSetSize=2000;
const double normalRatio=0.3;


//const int populationSize=50;
//种群规模
//const double crossoverProb=0.8;
//交叉概率
#define CROSSOVER_BY_DISCRETE_SWAP

//const double mutateProb=0.05;
//变异概率
//const int maxGeneration=2000-1;
//最大允许代数
//const int maxFailTimes=50;
//连续50代没有更优的个体产生，则结束

//const double iniWeightScale=0.9;
//权值初始化范围-0.5~0.5

//double randDouble();

const bool POOLING_NO_SIZE_ERROR=((InputCount-2)%PoolL3StrideR==0)
        &&((SampleLength-4)%PoolL3StrideC==0);

#if POOLING_NO_SIZE_ERROR==true
#error POOLING_SIZE_ERROR
#endif

double ReLu(const double x) {
    return std::max(x,0.0);
}

void writeMatrixAsMatlab(const Eigen::MatrixXd,std::fstream&);

#endif // DEFINES_H
