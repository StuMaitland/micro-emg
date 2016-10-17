#include "summaryview.h"
#include "experimentdetails.h"
#include "mainwindow.h"

SummaryView::SummaryView() :
    QWidget()
{
    setWindowTitle(tr("Experiment Summary"));
    //setFixedWidth(500);
    setFixedHeight(300);

    QVBoxLayout *summaryLayout = new QVBoxLayout;

    summaryLayout->addWidget(new QLabel("Data Folder: "+ExperimentDetails::getDataFolder()));
    summaryLayout->addWidget(new QLabel("Location: "+ExperimentDetails::getLocation()));
    summaryLayout->addWidget(new QLabel("Subject: "+ExperimentDetails::getSubjectName()));
    summaryLayout->addWidget(new QLabel("Muscle: "+ExperimentDetails::getMuscle()));
    summaryLayout->addWidget(new QLabel("Force Level: "+QString::number(ExperimentDetails::getForceLevel())));
    summaryLayout->addWidget(new QLabel("Needle ID: "+ExperimentDetails::getNeedleId()));
    summaryLayout->addWidget(new QLabel("Total Time (min): "+QString::number(ExperimentDetails::getTotalTime())));
    summaryLayout->addStretch(1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(run()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    QGroupBox *mainGroupBox1 = new QGroupBox();
    mainGroupBox1->setLayout(summaryLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mainGroupBox1);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

}

void SummaryView::run(){
    this->hide();
    MainWindow::getInstance().startRecordExperiment();
}
