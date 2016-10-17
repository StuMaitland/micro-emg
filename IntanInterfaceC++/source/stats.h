#ifndef STATS_H
#define STATS_H

#include "datasummary.h"
#include <QVector>

class Stats
{
public:
    static Stats& getInstance()
    {
        static Stats    instance; // Guaranteed to be destroyed.
                                          // Instantiated on first use.
        return instance;
    }

    static DataSummary summarizeVector(QVector<double> data);
    static double calcMean(QVector<double> data);
    static double calcStdev(QVector<double> data);
    static double calcStdev(QVector<double> data,double mean);

private:
       Stats() {};                   // Constructor? (the {} brackets) are needed here.


       // C++ 11
       // =======
       // We can use the better technique of deleting the methods
       // we don't want.
       Stats(Stats const&)               = delete;
       void operator=(Stats const&)  = delete;
};

#endif // STATS_H
