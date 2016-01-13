#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T07:04:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SigmaCars
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    carblock.cpp

HEADERS  += mainwindow.h \
    carblock.h

FORMS    += mainwindow.ui \
    carblock.ui

RESOURCES += \
    resources.qrc
