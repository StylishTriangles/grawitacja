#-------------------------------------------------
#
# Project created by QtCreator 2016-01-14T13:02:00
#
#-------------------------------------------------

QT       += core gui
CONFIG       += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = symulacja
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    celestialbody.cpp

HEADERS  += mainwindow.h \
    celestialbody.h

FORMS    += mainwindow.ui
