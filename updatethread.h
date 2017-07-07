#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H
#include "powerscheme.h"
#include "mainwindow.h"

#include <QApplication>

class PowerMonitor
{
public:
    PowerMonitor(MainWindow* wp);
    ~PowerMonitor();
    void threadStart();
private:
    void updatePowerScheme();
    bool threadStarted;
    void threadedWait();
    void update();
    PowerScheme* desiredScheme;
    MainWindow* wp;
    void updateDesiredScheme();
};
#endif // UPDATETHREAD_H
