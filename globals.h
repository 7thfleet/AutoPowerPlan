#ifndef GLOBALS_H
#define GLOBALS_H
#include "powerscheme.h"

#include <QString>

#include "Windows.h"

//Clamps
extern const int minUpdateFreq;
extern const int maxUpdateFreq;

//Name of settings
extern const QString settingsPwrPlnBatFrndName_Name;
extern const QString settingsPwrPlnACFrndName_Name;
extern const QString settingsUpdateFreq_Name;
extern const QString settingsHasRun_Name;


//Default value of the settings
extern const QString settingsDefPwrPlnBatFrndName_Value;
extern const QString settingsDefPwrPlnACFrndName_Value;
extern const int settingsDefUpdateFreq_Value;
extern const bool settingsDefHasRun_Value;


//Default Power Schemes
extern PowerScheme* highPerformanceScheme;
extern PowerScheme* powerSaverScheme;
extern PowerScheme* automaticScheme;


//Misc Functions
void setGUIDToActiveScheme(GUID *&g);
bool isOnACPower(void);
PowerScheme* schemeFromFriendlyName(QString friendlyName);
#endif // GLOBALS_H
