/*
Copyright (C) {2017} {7thfleet}

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSettings>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateStatus(void);

private slots:
    void onACComboBoxSelected(const QString& text);
    void onBatteryComboBoxSelected(const QString& text);
    void onAutoStartComboBoxSelected(const QString& text);
    void onSpinBoxValueChange(int i);
    void exitClicked(QAction *action);
    void onActivated(QSystemTrayIcon::ActivationReason reason);
    void closeEvent(QCloseEvent *event);


private:
    void writeToSettings(QString settingName, QString settingValue);
    void writeToSettings(QString settingName, int settingValue);
    void writeToSettings(QString settingName, bool settingValue);
    void updateAutoStartSetting();

    Ui::MainWindow *ui;
};





#endif // MAINWINDOW_H
