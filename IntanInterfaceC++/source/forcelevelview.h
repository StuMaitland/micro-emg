#ifndef FORCELEVELVIEW_H
#define FORCELEVELVIEW_H

#include <QDialog>
#include "globalconstants.h"
#include "qtincludes.h"
#include<QTimer>
#include<QFont>
#include "datastreamlistener.h"

class SignalProcessor;
class MainWindow;
class Rhd2000DataBlock;



class ForceLevelView : public QWidget, DataStreamListener
{
    Q_OBJECT
public:
    explicit ForceLevelView(SignalProcessor *inSignalProcessor);
    void updateData(int blockNumber);
    void setId(int id);
    int getId();
    void setForceLevel(double level);
    void setMVC(double MVC);
    void endRecording();
    void setAUX(int i);
protected:
    void paintEvent(QPaintEvent *event);
private slots:


private:
    static const int _DEV;//allowed deviation from the set force level
    QRect frame;
    QPixmap pixmap;
    QTimer *timer;
    SignalProcessor *signalProcessor;
    int id;
    double forceLevel;
    double minForce;
    double maxForce;
    double minVisibleForce;
    double maxVisibleForce;
    double MVC;
    void clearView();
    void updatePlot(int cond, double force);
    int aux=0;

};

#endif // FORCELEVELVIEW_H
