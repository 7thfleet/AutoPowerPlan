/*
Copyright (C) {2017} {7thfleet}

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*/
#include "mainwindow.h"
#include "powerscheme.h"
#include "globals.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QStandardItemModel>
#include <QListView>
#include <QString>
#include <QSettings>
#include <QVariant>
#include <QDataStream>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

#include "Windows.h"

//Needed for proper compilation in QT, else functions like IIDFromString make the compiler throw LNK errors
extern "C"{
    #include <powrprof.h>
    #include <objbase.h>
}
#pragma comment(lib, "powrprof.lib")
#pragma comment(lib, "Ole32.lib")



MainWindow::MainWindow(QWidget *parent) :
    ui(new Ui::MainWindow)
{
    //QSettings/Configuration
    QSettings userSettings;

    if(! (userSettings.value(settingsHasRun_Name).toBool()) ){ //Will also evaluate to false if no settings are found
        //Set default values
        userSettings.setValue(settingsPwrPlnBatFrndName_Name, settingsDefPwrPlnBatFrndName_Value);
        userSettings.setValue(settingsPwrPlnACFrndName_Name, settingsDefPwrPlnACFrndName_Value);
        userSettings.setValue(settingsHasRun_Name, settingsDefHasRun_Value);
        userSettings.setValue(settingsUpdateFreq_Name, settingsDefUpdateFreq_Value);
        userSettings.setValue(settingsAutoStart_Name, settingsDefAutoStart_Value);

    }

    userSettings.sync();

    updateAutoStartSetting();   //Update auto start settings based on values in QSettings

    //Create UI
    ui->setupUi(this);

    //Set up status labels
    this->updateStatus();

    //Note on naming: UF=Update Frequency | BPP=Battery Power Plan | PIPP=Plugged In Power Plan


    //Create system tray
    QSystemTrayIcon* tray = new QSystemTrayIcon(parent);
    tray->setToolTip(applicationName);
    QIcon trayIcon;
    trayIcon.addPixmap(QPixmap(":/Resources/Images/PPS.png"), QIcon::Normal, QIcon::Off);
    tray->setIcon(trayIcon);
    tray->setVisible(true);
    QMenu *traymenu = new QMenu("Menu", parent);
    traymenu->addAction("Exit");
    tray->setContextMenu(traymenu);
    tray->show();

    this->setWindowIcon(trayIcon);


    //Set the update frequency value from file
    ui->dynamicSettingsUFSpinBox->setValue(userSettings.value(settingsUpdateFreq_Name).toInt());

    //Create the text for the ComboBoxes
        //List of options for power plans on battery and AC
    QString ACCommandsList[3] = {highPerformanceScheme->friendlyName, powerSaverScheme->friendlyName, automaticScheme->friendlyName};
    QString batteryCommandsList[3] = {ACCommandsList[0], ACCommandsList[1], ACCommandsList[2]};


        //Put the currently selected option first in the list

            //AC list
    bool notFound = true;
    for(int i = 0; i<3; i++){

        if(QString(userSettings.value(settingsPwrPlnACFrndName_Name).toString()) == ACCommandsList[i] && notFound){
            QString* tempstring = new QString(ACCommandsList[0]);
            ACCommandsList[0] = (userSettings.value(settingsPwrPlnACFrndName_Name)).toString();
            ACCommandsList[i] = *tempstring;
            delete tempstring;
            notFound = false;

        }
    }

            //Battery list
    notFound = true;
    for(int i = 0; i<3; i++){

        if(QString(userSettings.value(settingsPwrPlnBatFrndName_Name).toString()) == batteryCommandsList[i] && notFound){
            QString* tempstring = new QString(batteryCommandsList[0]);
            batteryCommandsList[0] = (userSettings.value(settingsPwrPlnBatFrndName_Name)).toString();
            batteryCommandsList[i] = *tempstring;
            delete tempstring;
            notFound = false;

        }
    }




        //Create ordered lists of the options
    QStringList onACCommands = {ACCommandsList[0], ACCommandsList[1],ACCommandsList[2]};
    QStringList onBatteryCommands = {batteryCommandsList[0], batteryCommandsList[1],batteryCommandsList[2]};


    //Create list of options for auto start
    QStringList autoStartCommands = {"No", "Yes"};
    if(userSettings.value(settingsAutoStart_Name).toBool()){
        //Application is set to auto start with windows
        autoStartCommands = QStringList{"Yes", "No"};

    }

        //Set the combo box items to the lists
    ui->dynamicSettingsBPPComboBox->addItems(onBatteryCommands);
    ui->dynamicSettingsPIPPComboBox->addItems(onACCommands);
    ui->dynamicSettingsASComboBox->addItems(autoStartCommands);

    //Set clamps for update value
    ui->dynamicSettingsUFSpinBox->setMaximum(maxUpdateFreq);
    ui->dynamicSettingsUFSpinBox->setMinimum(minUpdateFreq);





    //Connections
        //Plugged in Power Plan ComboBox option selected
    connect(ui->dynamicSettingsPIPPComboBox, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), this, &MainWindow::onACComboBoxSelected);
        //On Battery Power Plan ComboBox option selected
    connect(ui->dynamicSettingsBPPComboBox, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), this, &MainWindow::onBatteryComboBoxSelected);
        //Automatically start ComboBox option selected
    connect(ui->dynamicSettingsASComboBox, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), this, &MainWindow::onAutoStartComboBoxSelected);
        //Update Frequency value changed
    connect(ui->dynamicSettingsUFSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::onSpinBoxValueChange);
        //"Exit" option in system tray icon clicked
    connect(traymenu, &QMenu::triggered,this, &MainWindow::exitClicked);
        //system tray icon clicked
    connect(tray, &QSystemTrayIcon::activated,this, &MainWindow::onActivated);

    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onACComboBoxSelected(const QString &text){
    //write to QSettings
    qDebug() << "[dynamicSettingsBPPComboBox] setting PwrPlnACFrndName to " << text;
    writeToSettings(settingsPwrPlnACFrndName_Name, text);
    return;
}

void MainWindow::onBatteryComboBoxSelected(const QString &text){
    //write to QSettings
    qDebug() << "[dynamicSettingsBPPComboBox] setting PwrPlnBatFrndName to " << text;
    writeToSettings(settingsPwrPlnBatFrndName_Name, text);
    return;
}

void MainWindow::onAutoStartComboBoxSelected(const QString &text){
    //write to QSettings
    bool autoStartValue = true;
    if(text == "No"){
        autoStartValue = false;
    }
    qDebug() << "[dynamicSettingsASComboBox] setting AutoStart to " << (QString)autoStartValue;
    writeToSettings(settingsAutoStart_Name, autoStartValue);
    updateAutoStartSetting();
    return;
}

void MainWindow::onSpinBoxValueChange(int i){
    //Write to QSettings
    if(i>0){ //Just to be safe
        qDebug() << "[dynamicSettingsUFSpinBox] setting UpdateFreq to " << i;
        writeToSettings(settingsUpdateFreq_Name, i);
    }
    return;
}

//Update displayed values
void MainWindow::updateStatus(void){

    //Set power source status
    ui->dynamicStatusCPSLabel->setText("On Battery");
    if(isOnACPower()){
        ui->dynamicStatusCPSLabel->setText("Plugged In");
        qDebug() << "[dynamicStatusCPSLabel] setText as Plugged In";

    }else{
        qDebug() << "[dynamicStatusCPSLabel] setText as On Battery";

    }


    //Set text based on current power plan
    static GUID* activeSchemeGUID = new GUID(GUID_NULL);
    setGUIDToActiveScheme(activeSchemeGUID);
    if(*activeSchemeGUID == highPerformanceScheme->schemeGUID){
        ui->dynamicStatusCPPLabel->setText(highPerformanceScheme->friendlyName);
        qDebug() << "[dynamicStatusCPPLabel] setText as High Performance Plan";

    }
    if(*activeSchemeGUID == powerSaverScheme->schemeGUID){
        ui->dynamicStatusCPPLabel->setText(powerSaverScheme->friendlyName);
        qDebug() << "[dynamicStatusCPPLabel] setText as Power Saver Plan";

    }
    if(*activeSchemeGUID == automaticScheme->schemeGUID){
        ui->dynamicStatusCPPLabel->setText(automaticScheme->friendlyName);
        qDebug() << "[dynamicStatusCPPLabel] setText as Balanced Plan";

    }

    return;
}

//"Exit" clicked in the system tray icon menu
void MainWindow::exitClicked(QAction *action){
    QApplication::quit();
    return;
}

//System tray icon clicked
void MainWindow::onActivated(QSystemTrayIcon::ActivationReason reason){
    if(reason ==  QSystemTrayIcon::Trigger){
    this->show();
    //Bring to front
    this->activateWindow();
    }

}

//Intercept the clicking of the 'X'/close button
void MainWindow::closeEvent(QCloseEvent *event){

        this->hide();
        event->ignore();
        return;
}

//Write supplied setting & sync
void MainWindow::writeToSettings(QString settingName, int settingValue){
    QSettings settings;
    settings.setValue(settingName, settingValue);
    settings.sync();
    return;
}

//Write supplied setting & sync
void MainWindow::writeToSettings(QString settingName, QString settingValue){
    QSettings settings;
    settings.setValue(settingName, settingValue);
    settings.sync();
    return;
}

//Write supplied setting & sync
void MainWindow::writeToSettings(QString settingName, bool settingValue){
    QSettings settings;
    settings.setValue(settingName, settingValue);
    settings.sync();
    return;
}

//Change auto start value
void MainWindow::updateAutoStartSetting(void){
    QSettings usersettings;
    bool autoStart = usersettings.value(settingsAutoStart_Name).toBool();

    QSettings bootSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(autoStart){
        qDebug() << "Adding application to startup";
        bootSettings.setValue(applicationName, QCoreApplication::applicationFilePath().replace('/', '\\'));
    }else{
        qDebug() << "Removing application from startup";
        bootSettings.remove(applicationName);
    }

    bootSettings.sync();
    return;
}
