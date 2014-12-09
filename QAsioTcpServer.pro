#-------------------------------------------------
#
# Project created by QtCreator 2014-12-08T16:04:57
#
#-------------------------------------------------

QT       += core core_private

QT       -= gui

TARGET = QAsioTcpServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp \
    qasiotcpsocket.cpp \
    ioserverthread.cpp

HEADERS += \
    qasiotcpsocket.h \
    ioserverthread.h \
    qasioevent.h

INCLUDEPATH += $$PWD/include
