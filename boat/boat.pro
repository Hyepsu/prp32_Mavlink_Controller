#-------------------------------------------------
#
# Project created by QtCreator 2017-12-03T14:14:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

QT += network

INCLUDEPATH += D:\code\prp_32_program\mavlink\mavlink
INCLUDEPATH += D:\code\prp_32_program\mavlink\mavlink_CN200
