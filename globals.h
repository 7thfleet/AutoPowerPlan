/*
Copyright (C) {2017} {7thfleet}

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*/
#ifndef GLOBALS_H
#define GLOBALS_H
#include "powerscheme.h"

#include <QString>

#include "Windows.h"

//Misc
extern const DWORD autoStartDetectTime;
extern const QString applicationName;

//Clamps
extern const int minUpdateFreq;
extern const int maxUpdateFreq;

//Name of settings
extern const QString settingsPwrPlnBatFrndName_Name;
extern const QString settingsPwrPlnACFrndName_Name;
extern const QString settingsUpdateFreq_Name;
extern const QString settingsHasRun_Name;
extern const QString settingsAutoStart_Name;

//Default value of the settings
extern const QString settingsDefPwrPlnBatFrndName_Value;
extern const QString settingsDefPwrPlnACFrndName_Value;
extern const int settingsDefUpdateFreq_Value;
extern const bool settingsDefHasRun_Value;
extern const bool settingsDefAutoStart_Value;


//Default Power Schemes
extern PowerScheme* highPerformanceScheme;
extern PowerScheme* powerSaverScheme;
extern PowerScheme* automaticScheme;


//Misc Functions
void setGUIDToActiveScheme(GUID *&g);
bool isOnACPower(void);
PowerScheme* schemeFromFriendlyName(QString friendlyName);
#endif // GLOBALS_H
