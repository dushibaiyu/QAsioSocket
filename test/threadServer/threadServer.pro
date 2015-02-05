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
    threadServer.cpp \
    qasiotcpsocketparent.cpp

HEADERS += \
    threadServer.h \
    qasiotcpsocketparent.h

win32:!win32-g++:DEFINES += NOMINMAX
include(../../src/AsioTcp.pri)
