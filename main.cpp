/*Auto Power Plan
Automatically switches power plans depending on if the computer is plugged in/on AC or on battery.
Last Updated: 2017-7-6
Created with QT 5
Supported platforms: Windows
*/

#include "powerscheme.h"
#include "globals.h"
#include "mainwindow.h"
#include "updatethread.h"

#include <QApplication>
#include <QString>
#include <QSettings>
#include "rpc.h"
#include "rpcdce.h"
#include "Windows.h"

//Needed for proper compilation in QT, else functions like IIDFromString make the compiler throw LNK errors
extern "C"{
    #include <powrprof.h>
    #include <objbase.h>
}

#pragma comment(lib, "powrprof.lib")
#pragma comment(lib, "Ole32.lib")

//Extern variables
PowerScheme* highPerformanceScheme;
PowerScheme* powerSaverScheme;
PowerScheme* automaticScheme;

int main(int argc, char *argv[])
{
    //Create default power schemes(extern)
    //High Performance
    LPCOLESTR highPerfGUIDStr = OLESTR("{8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c}");
    GUID highPerfGUID;
    IIDFromString(highPerfGUIDStr, &highPerfGUID);
    highPerformanceScheme = new PowerScheme("High Performance", highPerfGUID);

    //Power Saver
    LPCOLESTR powerSaverGUIDStr = OLESTR("{a1841308-3541-4fab-bc81-f71556f20b4a}");
    GUID powerSaverGUID;
    IIDFromString(powerSaverGUIDStr, &powerSaverGUID);
    powerSaverScheme = new PowerScheme("Power Saver", powerSaverGUID);

    //Automatic a.k.a. balanced
    LPCOLESTR automaticGUIDStr = OLESTR("{381b4222-f694-41f0-9685-ff5bb260df2e}");
    GUID automaticGUID;
    IIDFromString(automaticGUIDStr, &automaticGUID);
    automaticScheme = new PowerScheme("Balanced", automaticGUID);





    QApplication a(argc, argv);

    //Set values used for QSettings
    QCoreApplication::setOrganizationName("Auto Power Plan");
    QCoreApplication::setApplicationName("Auto Power Plan");

    bool firstTimeRun = false;
    //Set application to auto start with Windows
    QSettings settings;
    if(!(settings.value(settingsHasRun_Name).toBool())){
        //First run
        QSettings bootSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        bootSettings.setValue("Auto Power Plan", QCoreApplication::applicationFilePath().replace('/', '\\'));
        bootSettings.sync();
        firstTimeRun = true;
    }

    MainWindow w;

    //Prevents application from constantly showing each time Windows
    //starts
    if(firstTimeRun){
        w.show();
    }

    MainWindow* wp = &w;

    //Monitor power status changes
    PowerMonitor powerMonitor(wp);
    powerMonitor.threadStart();

    return a.exec();
}