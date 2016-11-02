#ifndef MVCVIEW_H
#define MVCVIEW_H

#include <QDialog>
#include "globalconstants.h"
#include "qtincludes.h"
#include<QTimer>
#include<QFont>
#include "datastreamlistener.h"
#include "signalprocessor.h"

class MVCView :public QDialog,DataStreamListener
{
    Q_OBJECT
public:
    explicit MVCView(QWidget *parent, SignalProcessor *inSignalProcessor);
    static const int _TEST_TIME;//in seconds
    static double _BASE_LINE;
    static double _MAX_VOLT;//maximum voltage possible as an input from the load cell
    static const double _MIN_VOLT;
    static const double _MAX_KG;
    static const double _TOLERANCE; //tolerance in the transformation from volts to KG

    void updateData(int blockNumber);
    void setId(int id);
    int getId();
    double getMVC();
    static double toKg(double volt);
    void setAUX(int i);
protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent * event);
private slots:
    void countDownTimer();
    void endRecording();
private:
    QRect frame;
    QPixmap pixmap;
    QTimer *timer;
    int countDownCurrent;
    SignalProcessor *signalProcessor;
    int id;
    double currentMax=0;
    int aux=0;//0-7
    int maxDrawing = 495;
    int minDrawing = 20;
    int waitingTimes=0;
    bool waiting =0;
    void clearView();
    void countDown();
    void updateForceView(double force);
};

#endif // MVCVIEW_H
