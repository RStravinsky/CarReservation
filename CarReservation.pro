#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T07:04:31
#
#-------------------------------------------------

QT       += core gui sql printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarReservation
TEMPLATE = app
CONFIG += c++11
CONFIG += debug
RC_ICONS = \images\icon.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    carblock.cpp \
    bookingdialog.cpp \
    bookingblock.cpp \
    namedialog.cpp \
    notesdialog.cpp \
    noteblock.cpp \
    database.cpp \
    helpdialog.cpp \
    serviceblock.cpp \
    reportdialog.cpp \
    oildialog.cpp \
    dbconfigdialog.cpp

HEADERS  += mainwindow.h \
    carblock.h \
    bookingdialog.h \
    bookingblock.h \
    namedialog.h \
    notesdialog.h \
    noteblock.h \
    database.h \
    helpdialog.h \
    serviceblock.h \
    reportdialog.h \
    oildialog.h \
    dbconfigdialog.h

FORMS    += mainwindow.ui \
    carblock.ui \
    bookingdialog.ui \
    bookingblock.ui \
    namedialog.ui \
    notesdialog.ui \
    noteblock.ui \
    helpdialog.ui \
    serviceblock.ui \
    reportdialog.ui \
    oildialog.ui \
    dbconfigdialog.ui

RESOURCES += \
    resources.qrc
