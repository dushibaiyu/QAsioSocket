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

TEMPLATE = app

CONFIG += C++11
DEFINES += ASIO_STANDALONE

SOURCES += main.cpp \
    qasiotcpsocket.cpp \
    ioserverthread.cpp

HEADERS += \
    qasiotcpsocket.h \
    ioserverthread.h \
    qasioevent.h

INCLUDEPATH += $$PWD/include

win32:LIBS += -lws2_32
