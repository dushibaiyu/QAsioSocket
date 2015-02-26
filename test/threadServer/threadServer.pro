#-------------------------------------------------
#
# Project created by QtCreator 2014-12-08T16:04:57
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ThreadServer
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += D:/boost_1_57_0
DEFINES += ASIO_HAS_BOOST_DATE_TIME BOOST_ALL_NO_LIB

SOURCES += main.cpp \
    threadServer.cpp \
    threadhandle.cpp \
    mysocket.cpp

HEADERS += \
    threadServer.h \
    threadhandle.h \
    mysocket.h

include(../../QAsioSocket.pri)
