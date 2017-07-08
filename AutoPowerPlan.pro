#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T18:20:29
Copyright (C) {2017} {7thfleet}

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoPowerPlan
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    powerscheme.cpp \
    globals.cpp \
    updatethread.cpp

HEADERS  += mainwindow.h \
    powerscheme.h \
    globals.h \
    updatethread.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

win32:RC_ICONS += Images/PPS.ico
