#ifndef TE_DEFINES_H
#define TE_DEFINES_H

//#include <Genetic>

#define EIGEN_DONT_PARALLELIZEs
#include <Eigen/Dense>

#include <cmath>
#include <iostream>
#include <fstream>
//using namespace Eigen;
//using namespace std;
double ReLu(const double);
void writeMatrixToFile(Eigen::MatrixXd _mat,std::fstream& _file);

#define eig_logsig(x) (1.0/(1.0+(-(x)).array().exp()))
///tansig(x) = 2/(1+exp(-2*net))-1
#define eig_tansig(x) (2.0/(1.0+(-2*(x)).array().exp())-1.0)
#define eig_ReLu(x) ((x).array().max(0))

constexpr const int InputCount=52;
typedef Eigen::Vector<double,InputCount> RawData;
typedef Eigen::Vector<double,InputCount+2> Input;// single statue

constexpr const double PaddingVal=0;

constexpr const int SampleLength=10;
typedef std::pair<const Input*,bool> Sample;

//network strcuture : conv1->conv2->pool3(30)->fully connect9(15+1)->softmax10(2)

const int ConvL1CoreL=3;
//const int PoolL1Stride=2;
const int ConvL2CoreL=3;
const int PoolL3StrideR=1;
const int PoolL3StrideC=3;

constexpr const int PoolL3Out_rows=(InputCount-2)/PoolL3StrideR;
constexpr const int PoolL3Out_cols=(SampleLength-4)/PoolL3StrideC;

constexpr const int FC9In=PoolL3Out_rows*PoolL3Out_cols;
constexpr const int FC9Count=15;

constexpr const int SF10Count=2;

constexpr const int SuggestedBatchSize=50;
constexpr const int DataSetSize=2000;
//constexpr const double normalRatio=0.3;

typedef Eigen::Map<const Eigen::Array<double,InputCount+2,SampleLength>> sampleMat;
typedef Eigen::Array<double,ConvL1CoreL,ConvL1CoreL> ConvL1Core;//
typedef Eigen::Array<double,ConvL2CoreL,ConvL2CoreL> ConvL2Core;//
typedef Eigen::Array<double,InputCount,SampleLength-2> ConvL1Out;
typedef Eigen::Array<double,InputCount-2,SampleLength-4> ConvL2Out;
typedef Eigen::Array<double,PoolL3Out_rows,PoolL3Out_cols> PoolL3Out;
typedef Eigen::Map<Eigen::Vector<double,FC9In>> FCL9In;
typedef Eigen::Matrix<double,FC9Count,FC9In> Weight39;//
typedef Eigen::Vector<double,FC9Count> Bias9;//
typedef Eigen::Vector<double,FC9Count+1> FCL9Out;
typedef Eigen::Matrix<double,SF10Count,FC9Count+1> Weight910;//
typedef Eigen::Vector<double,SF10Count> SFL10Out;
typedef Eigen::Array<double,SF10Count,SF10Count> ConfusionMat;

constexpr const bool POOLING_NO_SIZE_ERROR=((InputCount-2)%PoolL3StrideR==0)
        &&((SampleLength-4)%PoolL3StrideC==0);

#if POOLING_NO_SIZE_ERROR==true
#error POOLING_SIZE_ERROR
#endif

//#define TE_CNN_FILL_NAN
//#define TE_CNN_PARALLELIZE

#endif // TE_DEFINES_H
