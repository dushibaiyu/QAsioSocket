#-------------------------------------------------
#
# Project created by QtCreator 2015-02-06T11:30:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PingTongTest
TEMPLATE = app

win32:!win32-g++:DEFINES += NOMINMAX
include(../../src/AsioTcp.pri)

SOURCES += main.cpp\
    mysocket.cpp \
    sockets.cpp \
    form.cpp

HEADERS  += \
    mysocket.h \
    sockets.h \
    form.h

RESOURCES += \
    data.qrc

FORMS += \
    form.ui
