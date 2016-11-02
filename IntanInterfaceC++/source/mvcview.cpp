#include "mvcview.h"

#include <iostream>
#include <cmath>
#include <QCloseEvent>

using namespace std;

MVCView::MVCView(QWidget *parent,SignalProcessor *inSignalProcessor):
    QDialog(parent)
{
    MainWindow::addDataStreamListener(this);
    setWindowTitle(tr("Maximum Voluntery Contraction"));
    setFixedWidth(300);
    setFixedHeight(500);

    this->signalProcessor = inSignalProcessor;

    // Pixel map  used for double buffering.
    pixmap = QPixmap(300, 500);
    pixmap.fill(Qt::white);

    frame = QRect(18, 15, 295-18, 480);

    clearView();
    countDownCurrent=10;
    countDown();
}

void MVCView::closeEvent(QCloseEvent *event)
{
    if (countDownCurrent>0){
        event->ignore();
        return;
    }
    endRecording();
    event->accept();
}

void MVCView::setAUX(int i){
    this->aux = i;
}

void MVCView::clearView()
{
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);

    painter.initFrom(this);

    painter.setPen(Qt::black);

    // Draw box outline.
    painter.drawRect(frame);

    int step = (maxDrawing-minDrawing)/12;
    int pos=maxDrawing;
    for(int i=0;i<=60;i+=5){
        QString text;
        if (i<10){
            text ="0"+QString::number(i)+"-";
        }else{
            text  = QString::number(i)+"-";
        }

        painter.drawText(1,pos,text);
        pos = pos-step;
    }

    painter.end();
}


void MVCView::paintEvent(QPaintEvent * /* event */)
{
    QStylePainter stylePainter(this);
    stylePainter.drawPixmap(0, 0, pixmap);
}

void MVCView::countDown(){
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(countDownTimer()));
    MainWindow::getInstance().runInterfaceBoard();
    timer->start(1000);
}

void MVCView::countDownTimer(){
    clearView();
    QPainter painter(&pixmap);

    painter.initFrom(this);
    painter.setPen(Qt::red);

    QFont font = painter.font() ;

    /* twice the size than the current font size */
    font.setPointSize(font.pointSize()* 10);

    /* set the modified font to the painter */
    painter.setFont(font);
    painter.drawText(110,260,QString::number(countDownCurrent));
    painter.end();
    countDownCurrent--;
    update();
    if(countDownCurrent<0){
        waiting=0;
        MVCView::_BASE_LINE = MVCView::_BASE_LINE/waitingTimes;
        timer->stop();
    }

}

const int MVCView::_TEST_TIME=30;
double MVCView::_BASE_LINE=0;
double MVCView::_MAX_VOLT=3.3;
const double MVCView::_MIN_VOLT=0;
const double MVCView::_TOLERANCE=0.0155;
const double MVCView::_MAX_KG=53.23;


void MVCView::endRecording(){
    MainWindow::getInstance().stopInterfaceBoard();
    MainWindow::removeDataStreamListener(this);
    this->close();
}

void MVCView::updateData(int blockNumber){

    int length = Rhd2000DataBlock::getSamplesPerDataBlock() * blockNumber;

    if(waiting){
        waitingTimes++;
        double meanBaseline=0;
        for(int i=0;i<length;i++){
          meanBaseline+=signalProcessor->boardAdc.at(this->aux).at(i);
        }
        meanBaseline= meanBaseline/length;
        MVCView::_BASE_LINE+=meanBaseline;
        return;
    }
    double meanKg=0;
    for(int i=0;i<length;i++){

        meanKg +=toKg(signalProcessor->boardAdc.at(this->aux).at(i));
    }
    meanKg/=length;
    if(currentMax<meanKg){
        currentMax=meanKg;
    }
    updateForceView(meanKg);
}

void MVCView::updateForceView(double force){
    clearView();
    QPainter painter(&pixmap);

    painter.initFrom(this);
    painter.setPen(Qt::black);
    int y = maxDrawing - (force/MVCView::_MAX_KG)*(maxDrawing-minDrawing);
    painter.fillRect(frame.x()+frame.width()/2-20,y,40,maxDrawing-y,Qt::red);

    //mark the current max
    y = maxDrawing - (currentMax/MVCView::_MAX_KG)*(maxDrawing-minDrawing);
    painter.fillRect(frame.x()+frame.width()/2-20,y,40,10,Qt::black);
    painter.end();
    update();
}

double MVCView::toKg(double volt){
    double kg = MVCView::_BASE_LINE;
    if (volt>(MVCView::_BASE_LINE-MVCView::_TOLERANCE)){
        kg=MVCView::_MAX_KG*(volt-MVCView::_BASE_LINE)/(MVCView::_MAX_VOLT-MVCView::_BASE_LINE);
    }else{
        kg=MVCView::_MAX_KG*(1-(volt-MVCView::_MIN_VOLT)/(MVCView::_BASE_LINE-MVCView::_MIN_VOLT));
    }
    return kg;
}

void MVCView::setId(int id){
    this->id=id;
}

int MVCView::getId(){
    return id;
}

double MVCView::getMVC(){
    return currentMax;
}
