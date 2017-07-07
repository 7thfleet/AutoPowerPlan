#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T18:20:29
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
