/*
Copyright (C) {2017} {7thfleet}

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*/
#include "globals.h"
#include "powerscheme.h"

#include <QString>
#include <QDebug>

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


//Clamps
const int minUpdateFreq = 5;
const int maxUpdateFreq = 60;

//Setting Names
const QString settingsPwrPlnBatFrndName_Name = "Plans/Battery";
const QString settingsPwrPlnACFrndName_Name = "Plans/AC";
const QString settingsUpdateFreq_Name = "UpdateFrequency";
const QString settingsHasRun_Name = "HasRun";

//Settings default values
//!!!!!NOTE!!!!!
    //friendlyName is the value saved to file for the power plans
const QString settingsDefPwrPlnBatFrndName_Value = "Balanced";
const QString settingsDefPwrPlnACFrndName_Value = "High Performance";
const int settingsDefUpdateFreq_Value = 20;
const bool settingsDefHasRun_Value = true;


//Misc Functions
    //Sets supplied GUID to the active power scheme GUID
void setGUIDToActiveScheme(GUID* &g){
    PowerGetActiveScheme(NULL, &g);
    return;
}

    //Returns bool indicating if computer is plugged in/on ac power
bool isOnACPower(void){
    _SYSTEM_POWER_STATUS* powerStatus = new _SYSTEM_POWER_STATUS;
    GetSystemPowerStatus(powerStatus);
    bool onAC = (bool)powerStatus->ACLineStatus;
    delete powerStatus;
    return onAC;
}

    //Returns a powerscheme given the friendlyName of one
PowerScheme* schemeFromFriendlyName(QString friendlyName){
    if(friendlyName == highPerformanceScheme->friendlyName){
        return highPerformanceScheme;
    }

    if(friendlyName == powerSaverScheme->friendlyName){
        return powerSaverScheme;
    }

    if(friendlyName == automaticScheme->friendlyName){
        return automaticScheme;
    }

    qDebug() << "[Error]Invalid QString friendlyName supplied "
             <<"to function schemeFromFriendlyName. Returning automatic scheme. "
            << "Invalid string value: " << friendlyName;

    return automaticScheme;

}



