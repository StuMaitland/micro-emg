#include "experimentdialog.h"


#include <QtGui>

#include "qtincludes.h"

#include "globalconstants.h"
#include "ExperimentDialog.h"
#include "impedenceview.h"
#include <iostream>
#include "mvcview.h"
#include "experimentdetails.h"

using namespace std;

// Save file format selection dialog.
// Allows users to select a new save file format, along with various options.

ExperimentDialog::ExperimentDialog(QWidget *parent, SignalProcessor *inSignalProcessor):
    ExperimentDialog(parent, inSignalProcessor, -1){

}

ExperimentDialog::ExperimentDialog(QWidget *parent, SignalProcessor *signalProcessor, double MaximumVC):
    QDialog(parent)
{
    setWindowTitle(tr("Run Multi-Electrode EMG Experiment"));
    setFixedWidth(360);

    this->signalProcessor = signalProcessor;

    runImpedenceCheckBox = new QCheckBox(tr("Run Impedence Test"));
    runImpedenceCheckBox->setChecked(true);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton* runMVCButton = new QPushButton(tr("&Run MVC"));
    QPushButton* getPathButton = new QPushButton(tr("&Change"));


    muscleCBox = new QComboBox(this);
    muscleCBox->addItem("Left TA");
    muscleCBox->addItem("Right TA");
    muscleCBox->addItem("Left Bicep");
    muscleCBox->addItem("Right Bicep");

    timeEdit = new QLineEdit();
    timeEdit->setText("5");

    MVCEdit = new QLineEdit(" ");
    this->MVC = MaximumVC;

    if(MaximumVC>-1){
        MVCEdit->setText(QString::number(MaximumVC));
        forceLevelEdit = new QLineEdit(QString::number(ExperimentDetails::getForceLevel()));
        needleIdEdit = new QLineEdit(ExperimentDetails::getNeedleId());
        adcBoardEdit = new QLineEdit(QString::number(ExperimentDetails::getadcBoard()));
        subjectEdit  = new QLineEdit(ExperimentDetails::getSubjectName());
        locationEdit = new QLineEdit(ExperimentDetails::getLocation());
        pathEdit     = new QLineEdit(ExperimentDetails::getPath());
        recordingIdEdit = new QLineEdit(ExperimentDetails::getExperimentID());

    }else{
        forceLevelEdit = new QLineEdit("10");
        needleIdEdit = new QLineEdit("64CH-00");
        adcBoardEdit = new QLineEdit("1");
        subjectEdit  = new QLineEdit("");
        locationEdit = new QLineEdit("RVI");
        pathEdit     = new QLineEdit(getDataFolder());
        recordingIdEdit = new QLineEdit(QDate::currentDate().toString("dd_MM_yyyy"));
    }
    //connect signals
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(run()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(runMVCButton,SIGNAL(clicked()),this,SLOT(runMVC()));

    QHBoxLayout *newRecordingLayout = new QHBoxLayout;
    newRecordingLayout->addWidget(new QLabel(tr("Start new Recording")));
    newRecordingLayout->addStretch(1);

    QHBoxLayout *adcBoardLayout = new QHBoxLayout;
    adcBoardLayout->addWidget(new QLabel(tr("ADC Board ID:  ")));
    adcBoardLayout->addWidget(adcBoardEdit);
    adcBoardLayout->addStretch(1);

    QHBoxLayout *subjectLayout = new QHBoxLayout;
    subjectLayout->addWidget(new QLabel(tr("Subject:           ")));
    subjectLayout->addWidget(subjectEdit);
    subjectLayout->addStretch(1);

    QHBoxLayout *locationLayout = new QHBoxLayout;
    locationLayout->addWidget(new QLabel(tr("Location:          ")));
    locationLayout->addWidget(locationEdit);
    locationLayout->addStretch(1);

    QHBoxLayout *pathLayout = new QHBoxLayout;
    pathLayout->addWidget(new QLabel(tr("Path:     ")));
    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(getPathButton);

    QHBoxLayout *recordingIDLayout = new QHBoxLayout;
    recordingIDLayout->addWidget(new QLabel(tr("Recording ID:   ")));
    recordingIDLayout->addWidget(recordingIdEdit);
    recordingIDLayout->addStretch(1);


    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(new QLabel(tr("Time(Minutes): ")));
    timeLayout->addWidget(timeEdit);
    timeLayout->addStretch(1);

    QHBoxLayout *MVCLayout = new QHBoxLayout;
    MVCLayout->addWidget(new QLabel(tr("MVC(Kg):         ")));
    MVCLayout->addWidget(MVCEdit);
    MVCLayout->addWidget(runMVCButton);

    QHBoxLayout *needleLayout = new QHBoxLayout;
    needleLayout->addWidget(new QLabel(tr("Needle ID:        ")));
    needleLayout->addWidget(needleIdEdit);
    needleLayout->addStretch(1);

    QHBoxLayout *forceLayout = new QHBoxLayout;
    forceLayout->addWidget(new QLabel(tr("Force Level(%):")));
    forceLayout->addWidget(forceLevelEdit);
    forceLayout->addStretch(1);

    QHBoxLayout *muscleLayout = new QHBoxLayout;
    muscleLayout->addWidget(new QLabel(tr("Muscle:            ")));
    muscleLayout->addWidget(muscleCBox);
    muscleLayout->addStretch(1);

    QVBoxLayout *boxLayout1 = new QVBoxLayout;
    boxLayout1->addLayout(newRecordingLayout);
    boxLayout1->addLayout(adcBoardLayout);
    boxLayout1->addWidget(runImpedenceCheckBox);

    boxLayout1->addLayout(subjectLayout);
    boxLayout1->addLayout(locationLayout);
    boxLayout1->addLayout(pathLayout);
    boxLayout1->addLayout(recordingIDLayout);
    boxLayout1->addLayout(timeLayout);
    boxLayout1->addLayout(MVCLayout);
    boxLayout1->addLayout(needleLayout);
    boxLayout1->addLayout(forceLayout);
    boxLayout1->addLayout(muscleLayout);

    QGroupBox *mainGroupBox1 = new QGroupBox();
    mainGroupBox1->setLayout(boxLayout1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mainGroupBox1);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
    cancelled=false;
}

void ExperimentDialog::run(){

    cancelled=false;

    if(subjectEdit->text().isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Please Enter Subject Name!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QDir dir(pathEdit->text());
    if(pathEdit->text().isEmpty() || !dir.exists()){
        QMessageBox msgBox;
        msgBox.setText("Please Enter a Valid File Path!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    if(recordingIdEdit->text()==QDate::currentDate().toString("dd_MM_yyyy")){
        QMessageBox msgBox;
        msgBox.setText("Please Enter a Valid Recroding ID!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    bool checkLevel = checkDouble(forceLevelEdit->text());
    if(!checkLevel){
        QMessageBox msgBox;
        msgBox.setText("Please Enter a Valid Number for Force Level");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    bool checkTotalTime = checkDouble(timeEdit->text());
    if(!checkTotalTime){
        QMessageBox msgBox;
        msgBox.setText("Please Enter a Valid Number for Time");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    bool checkMVC = checkDouble(MVCEdit->text());

    if(checkMVC){
        MVC=MVCEdit->text().toDouble();
    }
    if(MVC==-1){
        QMessageBox msgBox;
        msgBox.setText("Please Check MVC value");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QDir dir1(getExperimentFolder());
    if(dir1.exists()){
        QMessageBox msgBox;
        msgBox.setText("Experiment Folder Already Exists Please Change Recording ID");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    dir1.mkpath(".");

    QDialog::accept();
}

void ExperimentDialog::reject(){
    cancelled=true;
    QDialog::reject();

}

bool ExperimentDialog::getImpedenceState(){
    return this->runImpedenceCheckBox->isChecked();
}

bool ExperimentDialog::isCancelled(){
    return cancelled;
}

void ExperimentDialog::runMVC(){
    //RUN MVC interface and save the MVC recorded in the current run of the system
    MVCView mvc(this,signalProcessor);
    mvc.setAUX(adcBoardEdit->text().toInt());
    mvc.exec();
    this->MVC = mvc.getMVC();
    this->MVCEdit->setText(QString::number(MVC));
    update();
}

QString ExperimentDialog::getNeedleId(){
    return needleIdEdit->text();
}

int ExperimentDialog::getAUX(){
    return adcBoardEdit->text().toInt();
}

double ExperimentDialog::getMVC(){
    return MVC;
}

double ExperimentDialog::getForceLevel(){
    return forceLevelEdit->text().toDouble();
}

int ExperimentDialog::getTotalTime(){
    return timeEdit->text().toInt();
}

bool ExperimentDialog::checkDouble(QString value){
    bool isNumeric = false;
    value.toDouble(&isNumeric);

    return isNumeric;

}

bool ExperimentDialog::checkInt(QString value){
    bool isNumeric = false;
    value.toInt(&isNumeric);

    return isNumeric;

}

QString ExperimentDialog::getSubjectName(){
    return subjectEdit->text();
}

QString ExperimentDialog::getLocation(){
    return locationEdit->text();
}

QString ExperimentDialog::getPath(){
    return pathEdit->text();
}

QString ExperimentDialog::getDataFolder(){
    QString result = QDir::currentPath();

    QStringList list = result.split("/");
    result="";
    for(int i=0;i<list.size()-1;i++){
        result+=list.at(i)+"/";
    }
    result +=QString("Data");
    return result;
}

QString ExperimentDialog::getExperimentFolder(){
    QString result= getDataFolder()+"/"+subjectEdit->text()+"_"+locationEdit->text()+"_"+needleIdEdit->text()+"_"+muscleCBox->currentText()+"_"+recordingIdEdit->text();
    return result;
}

QString ExperimentDialog::getExperimentRHDFile(){
    QString experiment = subjectEdit->text()+"_"+locationEdit->text()+"_"+needleIdEdit->text()+"_"+muscleCBox->currentText()+"_"+recordingIdEdit->text();
    QString result= getDataFolder()+"/"+experiment+"/"+experiment;
    return result;
}

QString ExperimentDialog::getMuscle(){
    return muscleCBox->currentText();
}

QString ExperimentDialog::getRecordingID(){
    return recordingIdEdit->text();
}
