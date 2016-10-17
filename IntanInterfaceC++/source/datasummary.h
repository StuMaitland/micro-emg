#ifndef DATASUMMARY_H
#define DATASUMMARY_H

class DataSummary
{
private:
    double min;
    double max;
    double median;
    double lowerQuartile;
    double upperQuartile;
    double mean;
    double stdev;

public:
    DataSummary();
    void setMin(double min);
    double getMin();

    void setMax(double max);
    double getMax();

    void setMean(double mean);
    double getMean();

    void setMedian(double median);
    double getMedian();

    void setLowerQuartile(double lowerQuartile);
    double getLowerQuartile();

    void setUpperQuartile(double upperQuartile);
    double getUpperQuartile();

    void setStdev(double stdev);
    double getStdev();
};

#endif // DATASUMMARY_H
