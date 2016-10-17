#include "datasummary.h"
#include <float.h>

DataSummary::DataSummary()
{
    this->min= -DBL_MAX;
    this->max= -DBL_MAX;
    this->mean= -DBL_MAX;
    this->stdev= -DBL_MAX;;
    this->upperQuartile= -DBL_MAX;
    this->lowerQuartile= -DBL_MAX;
    this->median= -DBL_MAX;
}

void DataSummary::setMin(double min)
{
    this->min=min;
}

double DataSummary::getMin(){
    return min;
}

void DataSummary::setMax(double max){
    this->max=max;
}

double DataSummary::getMax(){
    return max;
}

void DataSummary::setMean(double mean){
    this->mean=mean;
}

double DataSummary::getMean(){
    return mean;
}

void DataSummary::setMedian(double median){
    this->median=median;
}

double DataSummary::getMedian(){
    return median;
}

void DataSummary::setLowerQuartile(double lowerQuartile){
    this->lowerQuartile=lowerQuartile;
}

double DataSummary::getLowerQuartile(){
    return lowerQuartile;
}

void DataSummary::setUpperQuartile(double upperQuartile){
    this->upperQuartile=upperQuartile;
}

double DataSummary::getUpperQuartile(){
    return upperQuartile;
}

void DataSummary::setStdev(double stdev){
    this->stdev =stdev;
}

double DataSummary::getStdev(){
    return stdev;
}

