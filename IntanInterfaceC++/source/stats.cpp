#include "stats.h"
#include <cmath>
#include <QVectorIterator>

DataSummary Stats::summarizeVector(QVector<double> data){
    DataSummary result = *new DataSummary();

    qSort(data.begin(), data.end());

    int mIdx = floor(data.size()/2.0)+1;
    result.setMedian(data.at(mIdx));

    result.setMin(data.at(0));
    result.setMax(data.at(data.size()-1));

    mIdx = floor(data.size()/4.0)+1;

    result.setLowerQuartile(data.at(mIdx));

    mIdx = floor(3*data.size()/4.0)+1;
    result.setUpperQuartile(data.at(mIdx));

    double mean = calcMean(data);
    result.setMean(mean);
    result.setStdev(calcStdev(data,mean));
    return result;
}

double Stats::calcMean(QVector<double> data){
    double result=0;

    QVectorIterator<double> itr(data);

    while(itr.hasNext()){
        result+=itr.next();
    }

    result/=result/data.size();

    return result;
}

double Stats::calcStdev(QVector<double> data){
    return calcStdev(data,calcMean(data));
}

double Stats::calcStdev(QVector<double> data, double mean){
    double result=0;

    QVectorIterator<double> itr(data);

    while(itr.hasNext()){
        result+=pow(itr.next()-mean,2.0);
    }

    result/=data.size();
    return sqrt(result);
}
