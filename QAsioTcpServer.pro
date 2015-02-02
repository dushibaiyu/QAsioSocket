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

SOURCES += main.cpp \
    testserver.cpp


include(AsioTcp/AsioTcp.pri)

HEADERS += \
    testserver.h
