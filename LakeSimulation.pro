#-------------------------------------------------
#
# Project created by QtCreator 2017-07-08T12:03:12
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LakeSimulation
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    waterobject.cpp \
    fishes.cpp \
    waterplants.cpp \
    factories.cpp \
    lakesimulation.cpp \
    point2d.cpp \
    parametersdialog.cpp \
    deadzone.cpp \
    databaseinterface.cpp


HEADERS += \
        mainwindow.h \
    waterobject.h \
    fishes.h \
    waterplants.h \
    factories.h \
    lakesimulation.h \
    point2d.h \
    parametersdialog.h \
    deadzone.h \
    databaseinterface.h

FORMS += \
        mainwindow.ui \
    parametersdialog.ui

RESOURCES += \
    images.qrc
