#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T15:02:46
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = HandleInThread
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../../QAsioSocket.pri)

SOURCES += main.cpp \
    mysocket.cpp \
    myserver.cpp

HEADERS += \
    mysocket.h \
    myserver.h
