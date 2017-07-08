/*
Copyright (C) {2017} {7thfleet}

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*/
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
