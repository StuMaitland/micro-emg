#ifndef EXPERIMENTDETAILS_H
#define EXPERIMENTDETAILS_H

#include "qtincludes.h"

class ExperimentDetails
{
public:
    static ExperimentDetails& getInstance()
    {
        static ExperimentDetails    instance; // Guaranteed to be destroyed.
                                          // Instantiated on first use.
        return instance;
    }

    static void newExperiment(QString needleId,QString dataFolder,double MVC,
                              double forceLevel,int totalTime,QString location,
                              QString subjectName, QString muscle,QString experimentID,
                              QString path,int adc,int numChannels);
    static int saveExperiment(QString fname);

    static QString getNeedleId();
    static QString getDataFolder();
    static double getMVC();
    static double getForceLevel();
    static int getTotalTime();
    static QString getLocation();
    static QString getSubjectName();
    static QString getMuscle();
    static QString getExperimentID();
    static QString getPath();
    static int getadcBoard();
    static int getNumChannels();
    static int getForceBaseline();
private:
    ExperimentDetails();

    static QString needleId;
    static QString dataFolder;
    static double MVC;
    static double forceLevel;
    static int totalTime;
    static QString location;
    static QString subjectName;
    static QString muscle;
    static QString experimentID;
    static QString path;
    static int adcBoard;
    static int numChannels;
    ExperimentDetails(ExperimentDetails const&) = delete;
    void operator=(ExperimentDetails const&) = delete;
};

#endif // EXPERIMENTDETAILS_H
