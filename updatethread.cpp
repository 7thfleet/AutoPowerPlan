#include "updatethread.h"
#include "globals.h"
#include "mainwindow.h"
#include "powerscheme.h"

#include <QSettings>
#include <QString>
#include <QDebug>

#include <mutex>
#include <thread>

#include "rpc.h"
#include "rpcdce.h"
#include "Windows.h"

//Needed for proper compilation in QT, else functions like IIDFromString make the compiler throw LNK errors
extern "C"{
    #include <powrprof.h>
    #include <objbase.h>
}

#pragma comment(lib, "powrprof.lib")



PowerMonitor::PowerMonitor(MainWindow *wp) :
    wp(wp)
{
    threadStarted= false;
    updateDesiredScheme();

    return;
}

PowerMonitor::~PowerMonitor(){}

//Starts the thread that checks for changes in power status & desired plan. This should be run only once.
void PowerMonitor::threadStart()
{
    if(!threadStarted){
        threadStarted = true;
        std::thread waitThread(&PowerMonitor::threadedWait, this);
        waitThread.detach();
        qDebug() << "waitThread launched";
        return;
    }

    qDebug() <<"[Error]waitThread already launched";
    return;
}

//Sets the active PowerScheme to desiredScheme
void PowerMonitor::updatePowerScheme()
{
    PowerSetActiveScheme(NULL, &desiredScheme->schemeGUID);
    qDebug() << "Setting active power scheme to " << desiredScheme->friendlyName;
    return;
}


//This function is meant to be launched, only once, and as a thread. It periodically checks for a change in power status, and changes plans if needed
void PowerMonitor::threadedWait()
{
    std::mutex mtx;
    GUID* currentSchemeGUID = new GUID(GUID_NULL);
    while(true){
        qDebug() << "threadedWait running";
        QSettings userSettings;
        DWORD sleepTime;

        //Check for invalid values
        if(! (userSettings.value(settingsUpdateFreq_Name).toInt() >= minUpdateFreq && userSettings.value(settingsUpdateFreq_Name).toInt() <= maxUpdateFreq) ){
            mtx.lock();
            userSettings.setValue(settingsUpdateFreq_Name, settingsDefUpdateFreq_Value);
            userSettings.sync();
            mtx.unlock();
            qDebug() << "Invalid updateFrequency, resetting value to default";
        }

        sleepTime = (DWORD)userSettings.value(settingsUpdateFreq_Name).toInt();

        sleepTime*= 1000; //Seconds to ms
        qDebug() << "Sleeping for " << sleepTime << " ms";
        Sleep(sleepTime);
        qDebug() << "Checking power status & current plan";
        //Check for changes
        setGUIDToActiveScheme(currentSchemeGUID);
        updateDesiredScheme();
        //Check if current and desired GUIDs match
        if(*currentSchemeGUID != desiredScheme->schemeGUID){
            qDebug() << "GUIDs of current and desired power schemes don't match, updating.";
            update();
        }

    }
}

//Switches the current power scheme and updates UI elements
void PowerMonitor::update()
{
    //Switch scheme
    updatePowerScheme();

    //Update UI elements
    wp->updateStatus();

    return;
}

//Updates desiredScheme from QSettings
void PowerMonitor::updateDesiredScheme()
{
    QSettings userSettings;
    if(isOnACPower()){
        desiredScheme = schemeFromFriendlyName(userSettings.value(settingsPwrPlnACFrndName_Name).toString());
        qDebug() << "Setting desired scheme to " << desiredScheme->friendlyName;

    }else{
        desiredScheme = schemeFromFriendlyName(userSettings.value(settingsPwrPlnBatFrndName_Name).toString());
        qDebug() << "Setting desired scheme to " << desiredScheme->friendlyName;

    }
    return;
}
