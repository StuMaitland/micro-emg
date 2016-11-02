#include "adaptiveforcelevelview.h"
#include "mainwindow.h"
#include "rhd2000datablock.h"
#include "signalprocessor.h"
#include "mvcview.h"

#include <iostream>
#include <QMessageBox>
#include <QDateTime>

using namespace std;

AdaptiveForceLevelView::AdaptiveForceLevelView(SignalProcessor *inSignalProcessor):
    QWidget()
{
    MainWindow::addDataStreamListener(this);
    setWindowTitle(tr("Force Level"));
    setFixedWidth(400);
    setFixedHeight(500);

    this->signalProcessor = inSignalProcessor;

    setForceLevel(10);

    this->startTime = QDateTime::currentMSecsSinceEpoch();
    // Pixel map  used for double buffering.
    pixmap = QPixmap(400, 500);
    pixmap.fill(Qt::white);

    frame = QRect(5, 5, 390, 490);
    clearView();
}

const int AdaptiveForceLevelView::_DEV=30;

void AdaptiveForceLevelView::clearView()
{
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);

    painter.initFrom(this);

    painter.setPen(Qt::black);

    // Draw box outline.
    painter.drawRect(frame);
    int edge = frame.y()+frame.height()/2;
    painter.drawLine(frame.x(),edge,frame.x()+frame.width(),edge);

    painter.setPen(Qt::red);

    edge = frame.y()+frame.height()/4;
    painter.drawLine(frame.x(),edge,frame.x()+frame.width(),edge);

    edge = frame.y()+3*frame.height()/4;
    painter.drawLine(frame.x(),edge,frame.x()+frame.width(),edge);
    painter.end();
}


void AdaptiveForceLevelView::paintEvent(QPaintEvent * /* event */)
{
    QStylePainter stylePainter(this);
    stylePainter.drawPixmap(0, 0, pixmap);
}

void AdaptiveForceLevelView::endRecording(){
    MainWindow::getInstance().stopInterfaceBoard();
    MainWindow::removeDataStreamListener(this);
    this->close();
}

void AdaptiveForceLevelView::setAUX(int i){
    this->aux=i;
}

void AdaptiveForceLevelView::updateData(int blockNumber){

    if(this->isVisible()){
        long current=QDateTime::currentMSecsSinceEpoch();
        if (current-this->startTime > 60000){
            startTime=current;
            this->state++;
            switch(state){
            case 1: this->setForceLevel(20);
                    break;
            case 2: this->setForceLevel(40);
                break;
            case 3: this->setForceLevel(20);
                break;
            case 4: this->setForceLevel(10);
                break;
            }
        }
        int length = Rhd2000DataBlock::getSamplesPerDataBlock() * blockNumber;
        double value=0;
        for(int i=0;i<length;i++){
            value+=signalProcessor->boardAdc.at(aux).at(i);
        }
        value/=length;
        double force=100*MVCView::toKg(value)/MVC;
        if (force<=maxForce & force>=minForce){
            updatePlot(1,force);
        }else{
            if(force>maxForce)
                updatePlot(2,force);
            else
                updatePlot(3,force);
        }
    }
}

void AdaptiveForceLevelView::updatePlot(int cond,double force){
    clearView();
    QPainter painter(&pixmap);

    painter.initFrom(this);
    switch(cond){
    case 1:
        painter.fillRect(frame.x(),frame.y()+frame.height()/4,frame.width(),frame.height()/2,Qt::green);
        break;
    case 2:
        painter.fillRect(frame.x(),frame.y(),frame.width(),frame.height()/4,Qt::red);
        break;
    case 3:
        painter.fillRect(frame.x(),frame.y()+3*frame.height()/4,frame.width(),frame.height()/4,Qt::red);
        break;
    }

    if(!(force>maxVisibleForce || force<minVisibleForce)){
        double norm = (force-minVisibleForce)/(maxVisibleForce-minVisibleForce);
        int y = frame.height()-norm*frame.height();
        painter.fillRect(frame.x(),y,frame.width(),10,Qt::black);
    }

    painter.end();
    update();

}

void AdaptiveForceLevelView::setForceLevel(double level){
    forceLevel=level;
    minForce = forceLevel-forceLevel*_DEV/100;
    maxForce = forceLevel+forceLevel*_DEV/100;
    minVisibleForce = forceLevel-forceLevel*2*_DEV/100;
    maxVisibleForce = forceLevel+forceLevel*2*_DEV/100;
}

void AdaptiveForceLevelView::setMVC(double MVC){
    this->MVC=MVC;
}

void AdaptiveForceLevelView::setId(int id){
    this->id=id;
}

int AdaptiveForceLevelView::getId(){
    return id;
}
