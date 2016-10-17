#ifndef IMPEDENCEVIEW_H
#define IMPEDENCEVIEW_H

#include <QDialog>

#include "globalconstants.h"
#include "qtincludes.h"
#include "qcustomplot.h"

class QTableWidget;
class QTableWidgetItem;

class ImpedanceView: public QDialog
{
    Q_OBJECT

public:
    explicit ImpedanceView(QWidget *parent,QString parameter);
    //@parameter contains the name of the csv file stored in a previous step
    // if @parameter is set to "DEMO" then the data are produced by ImpedanceView in demo mode

    const static QString _DEMO;
private:
     QDialogButtonBox *buttonBox;
     void plotImpedenceData(QString parameter);
     void plotBoxplot();
     //QPushButton* switchViewBtn;
     QCustomPlot *customPlot;
     //int status; // 1- for values 2- for Boxplot
     QVector<double> getDemoData();
     QVector<double> getCSVData(QString fname);

private slots:
     void switchView();
};

#endif // IMPEDENCEVIEW_H
