#ifndef SUMMARYVIEW_H
#define SUMMARYVIEW_H

#include <QWidget>

class MainWindow;

class SummaryView : public QWidget
{
    Q_OBJECT
public:
    explicit SummaryView();

signals:

private slots:
    void run();
};

#endif // SUMMARYVIEW_H
