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

win32:!win32-g++:DEFINES += NOMINMAX
include(../../src/AsioTcp.pri)

SOURCES += main.cpp \
    mysocket.cpp \
    myserver.cpp

HEADERS += \
    mysocket.h \
    myserver.h
