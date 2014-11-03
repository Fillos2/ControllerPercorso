#-------------------------------------------------
#
# Project created by QtCreator 2014-09-17T16:13:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = ControllerPercorso
TEMPLATE = app


SOURCES += main.cpp\
        controllerpercorso.cpp \
    block.cpp \
    settingsdialog.cpp

HEADERS  += controllerpercorso.h \
    block.h \
    settingsdialog.h

FORMS    += controllerpercorso.ui \
    settingsdialog.ui

RESOURCES += \
    controllerPercorso.qrc
