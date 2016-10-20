#ifndef EXPERIMENTDIALOG_H
#define EXPERIMENTDIALOG_H



#include <QDialog>

#include "globalconstants.h"
#include "qtincludes.h"
#include "signalprocessor.h"

class QDialogButtonBox;
class QCheckBox;
class QButtonGroup;

class ExperimentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExperimentDialog(QWidget *parent, SignalProcessor *inSignalProcessor);

     explicit ExperimentDialog(QWidget *parent, SignalProcessor *inSignalProcessor,double MVC);

    QCheckBox *runImpedenceCheckBox;
    QDialogButtonBox *buttonBox;
    QButtonGroup *buttonGroup;
    QLineEdit *timeEdit;
    QLineEdit *MVCEdit;
    QLineEdit *needleIdEdit;
    QLineEdit *forceLevelEdit;
    QLineEdit *adcBoardEdit;
    QLineEdit *subjectEdit;
    QLineEdit *locationEdit;
    QLineEdit *pathEdit;
    QLineEdit *recordingIdEdit;
    QLineEdit *numberChannelsEdit;
    QComboBox *muscleCBox;

    bool getImpedenceState();
    bool isCancelled();

    QString getNeedleId();
    double getMVC();
    double getForceLevel();
    int getTotalTime();
    int getAUX();
    int getNumChannels();
    QString getSubjectName();
    QString getLocation();
    QString getPath();
    QString getMuscle();
    QString getExperimentRHDFile();
    QString getExperimentFolder();
    QString getRecordingID();

signals:

public slots:

private slots:

    void run();
    void reject();
    void runMVC();

private:
    bool cancelled=false;
    double MVC=-1;
    SignalProcessor *signalProcessor;
    bool checkDouble(QString value);
    bool checkInt(QString value);
    QString getDataFolder();

};

#endif // EXPERIMENTDIALOG_H
