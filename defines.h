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

double randDouble();

/*
double randD();
double randD() {
    //[-1,1]
    //return (double(std::rand()%0xFFF)/0xFFF-0.5)*2;
}
*/

#endif // DEFINES_H
