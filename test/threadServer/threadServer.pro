#-------------------------------------------------
#
# Project created by QtCreator 2014-12-08T16:04:57
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QAsioTcpServer
CONFIG   += console
CONFIG   -= app_bundle

include(../../src/AsioTcp.pri)

SOURCES += main.cpp \
    threadServer.cpp \
    threadhandle.cpp \
    mysocket.cpp

HEADERS += \
    threadServer.h \
    threadhandle.h \
    mysocket.h

win32:!win32-g++:DEFINES += NOMINMAX
