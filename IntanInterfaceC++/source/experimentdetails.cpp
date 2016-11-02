#include "experimentdetails.h"
#include "mvcview.h"
#include <QTextStream>

QString ExperimentDetails::needleId="";
QString ExperimentDetails::dataFolder ="";
double ExperimentDetails::MVC =0;
double ExperimentDetails::forceLevel =0;
int ExperimentDetails::totalTime =0;
QString ExperimentDetails::location="";
QString ExperimentDetails::subjectName="";
QString ExperimentDetails::muscle="";
QString ExperimentDetails::experimentID="";
QString ExperimentDetails::path="";
int ExperimentDetails::adcBoard=0;
int ExperimentDetails::numChannels=64;

QString ExperimentDetails::getDataFolder(){
    return ExperimentDetails::dataFolder;
}

QString ExperimentDetails::getNeedleId(){
    return needleId;
}

double ExperimentDetails::getForceLevel(){
    return forceLevel;
}

double ExperimentDetails::getMVC(){
    return MVC;
}

int ExperimentDetails::getTotalTime(){
    return totalTime;
}

QString ExperimentDetails::getLocation(){
    return location;
}

QString ExperimentDetails::getSubjectName(){
    return subjectName;
}

QString ExperimentDetails::getMuscle(){
    return muscle;
}

QString ExperimentDetails::getExperimentID(){
    return experimentID;
}

QString ExperimentDetails::getPath(){
    return path;
}
int ExperimentDetails::getadcBoard(){
    return adcBoard;
}

int ExperimentDetails::getNumChannels(){
    return numChannels;
}

int ExperimentDetails::getForceBaseline(){
    return MVCView::_BASE_LINE;
}

void ExperimentDetails::newExperiment(QString needleId,QString dataFolder,double MVC,
                                      double forceLevel,int totalTime,QString location,
                                      QString subjectName, QString muscle,QString experimentID,
                                      QString path,int adc,int numChannels)
{
    ExperimentDetails::needleId=needleId;
    ExperimentDetails::dataFolder=dataFolder;
    ExperimentDetails::MVC=MVC;
    ExperimentDetails::forceLevel=forceLevel;
    ExperimentDetails::totalTime=totalTime;
    ExperimentDetails::location=location;
    ExperimentDetails::subjectName=subjectName;
    ExperimentDetails::muscle=muscle;
    ExperimentDetails::experimentID=experimentID;
    ExperimentDetails::path=path;
    ExperimentDetails::adcBoard=adc;
    ExperimentDetails::numChannels= numChannels;
}

int ExperimentDetails::saveExperiment(QString fname){
    QFile file(fname);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen()){
       return -1;
    }

     QTextStream outStream(&file);
     outStream << "Needle Id:"<<getNeedleId()<<"\r\n";
     outStream << "Data Folder:"<<getDataFolder()<<"\r\n";
     outStream << "MVC:"<<getMVC()<<"\r\n";
     outStream << "Force Level:"<<getForceLevel()<<"\r\n";
     outStream << "Total Time:"<<getTotalTime()<<"\r\n";
     outStream << "Location:"<<getLocation()<<"\r\n";
     outStream << "Subject Name:"<<getSubjectName()<<"\r\n";
     outStream << "Muscle:"<<getMuscle()<<"\r\n";
     outStream << "Recording ID:"<<getExperimentID()<<"\r\n";
     outStream << "Number of Channels:"<<getNumChannels()<<"\r\n";
     outStream << "Force Baseline:"<<getForceBaseline()<<"\r\n";
     file.close();
     return 1;
}
