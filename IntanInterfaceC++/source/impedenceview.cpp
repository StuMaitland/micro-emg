#include "impedenceview.h"

#include <QtGui>

#include "qtincludes.h"

#include "globalconstants.h"

#include "qcustomplot.h"
#include "experimentdetails.h"
#include <iostream>
#include "datasummary.h"
#include "stats.h"
#include <math.h>
#include "needle.h"

using namespace std;

const QString ImpedanceView::_DEMO = "DEMO";

ImpedanceView::ImpedanceView(QWidget *parent,QString parameter) :
    QDialog(parent)
{
    setWindowTitle(tr("Impedence Data"));
    setFixedWidth(1000);
    setFixedHeight(1000);
    //status=1;

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    //switchViewBtn = new QPushButton(tr("&BoxPlot"));
    //buttonBox->addButton(switchViewBtn,QDialogButtonBox::ActionRole);

    customPlot = new QCustomPlot;
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    //connect(switchViewBtn, SIGNAL(clicked()), this, SLOT(switchView()));
    this->plotImpedenceData(parameter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(customPlot);
    mainLayout->addWidget(buttonBox);


    setLayout(mainLayout);
}

void ImpedanceView::plotImpedenceData(QString parameter){

    //get the data
    QVector<double> data;
    if(ImpedanceView::_DEMO==parameter){
        data= getDemoData();//getCSVData("D:\\DataBackUP\\20141217_RVI\\impedence.csv");//
    }else{
        data= getCSVData(parameter);
    }

    DataSummary summary = Stats::summarizeVector(data);

    int chs=data.size();

    // create empty bar chart objects:
    QCPBars *electrodes = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(electrodes);
    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    electrodes->setName("Electrodes");
    pen.setColor(QColor(255, 131, 0));
    electrodes->setPen(pen);
    electrodes->setBrush(QColor(255, 131, 0, 50));

    // prepare x axis with electrode labels:
    QVector<double> ticks;
    QVector<QString> labels;
    for(int i=1;i<=chs;i++){
        ticks << i;
        labels<< QString::number(i);
    }
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 1);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, chs+2);

    // prepare y axis:
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("Impedance Magnitude at 1000 Hz (ohms)");
    customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot->yAxis->grid()->setSubGridPen(gridPen);
    customPlot->yAxis->setRangeUpper(summary.getMax());

    // Add data:

    electrodes->setData(ticks, data);
}

void ImpedanceView::switchView(){
    QString label ="Boxplot";
    customPlot->removeGraph(0);

    /*switch(status){
        case 1:
            status=2;
            label="Data";
            plotBoxplot();
            break;
        case 2:
            status =1;
            plotImpedenceData();
            break;
    }
    switchViewBtn->setText(label);*/

}

void ImpedanceView::plotBoxplot(){
    // create empty statistical box plottables:
    QCPStatisticalBox *sample1 = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);

    customPlot->addPlottable(sample1);
    QBrush boxBrush(QColor(60, 60, 255, 100));
    boxBrush.setStyle(Qt::Dense6Pattern); // make it look oldschool
    sample1->setBrush(boxBrush);

    QVector<double> data = getDemoData();
    DataSummary summary = Stats::summarizeVector(data);
    // set data:
    sample1->setKey(1);
    sample1->setMinimum(summary.getMin());
    sample1->setLowerQuartile(summary.getLowerQuartile());
    sample1->setMedian(summary.getMedian());
    sample1->setUpperQuartile(summary.getUpperQuartile());
    sample1->setMaximum(summary.getMax());

    // prepare manual x axis labels:
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 1);
    customPlot->xAxis->setTickLabelRotation(20);
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(QVector<double>() << 1);
    customPlot->xAxis->setTickVectorLabels(QVector<QString>() << "Impedence");

    // prepare axes:
    customPlot->yAxis->setLabel(QString::fromUtf8("Impedance Magnitude at 1000 Hz (ohms)"));
    customPlot->rescaleAxes();
    //customPlot->xAxis->scaleRange(1.7, customPlot->xAxis->range().center());
    //customPlot->yAxis->setRange(1E+5, 1E+8);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

QVector<double> ImpedanceView::getDemoData(){
    QVector<double> impData;
    /*impData  << 1.40E+06 << 1.39E+06 << 1.40E+06  << 1.41E+06 << 1.40E+06  << 1.38E+06  << 1.40E+06 << 1.41E+06 << 1.45E+06  << 1.39E+06
             << 1.41E+06 << 1.36E+06 << 1.36E+06 << 1.37E+06 << 1.35E+06 << 1.42E+06 << 1.37E+06
             << 1.35E+06 << 1.39E+06 << 1.41E+06 << 1.42E+06 << 1.42E+06 << 1.42E+06
             << 1.38E+06 << 1.37E+06 << 1.39E+06 << 1.40E+06 << 1.16E+06 << 1.40E+06
             << 1.07E+06 << 1.40E+06 << 1.03E+06 << 1.58E+06 << 1.49E+06 << 1.57E+06
             << 1.49E+06 << 1.54E+06 << 1.48E+06 << 1.57E+06 << 1.58E+06 << 1.55E+06
             << 1.44E+06 << 1.56E+06 << 1.48E+06 << 6.30E+06 << 1.56E+06 << 1.57E+06
             << 1.59E+06 << 1.56E+06 << 1.53E+06 << 1.60E+06 << 1.54E+06 << 1.46E+06
             << 1.58E+06 << 1.59E+06 << 1.54E+06 << 1.56E+06 << 1.58E+06 << 1.57E+06
             << 1.57E+06 << 1.57E+06 << 1.17E+06 << 1.59E+06 << 1.21E+06;*/
    impData  <<36200<<40400<<
    132000<<
    58800<<
    42600<<
    48500<<
    41700<<
    2.56e+007<<
    43700<<
    136000<<
    43500<<
    33900<<
    2.49e+007<<
    46600<<
    170000<<
    49300<<
    197000<<
    59500<<
    25300<<
    24200<<
    1.66e+007<<
    3.31e+007<<
    3.72e+007<<
    2e+007<<
    1.61e+007<<
    1.58e+007<<
    2.84e+007<<
    1.62e+007<<
    1.67e+007<<
    2.72e+007<<
    2.85e+007<<
    1.46e+007<<
    5.98e+006<<
    2.46e+007<<
    60900<<
    77400<<
    73200<<
    75500<<
    77400<<
    84500<<
    89400<<
    114000<<
    130000<<
    121000<<
    122000<<
    154000<<
    98600<<
    188000<<
    150000<<
    160000<<
    145000<<
    201000<<
    161000<<
    107000<<
    161000<<
    179000<<
    160000<<
    153000<<
    265000<<
    180000<<
    443000<<
    424000<<
    206000<<
    155000;
    return impData;
}

QVector<double> ImpedanceView::getCSVData(QString fname){
    QFile file(fname);
    QVector<double> result;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        int i=0;
        while (!file.atEnd()){
               QString line = file.readLine();
               if(i==0){
                   //skip the first line
                   i++;
                   continue;
               }
               QStringList list = line.split(",");
               result<<log10(list.at(4).toDouble());//the log10 impedance value
       }
       file.close();

       result = Needle::orderChannels(result);

       }else{
        cout<<"File did not open!"<<endl;
    }
    return result;
}
