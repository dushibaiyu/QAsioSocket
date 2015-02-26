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

include(../../QAsioSocket.pri)

SOURCES += main.cpp \
    testserver.cpp

HEADERS += \
    testserver.h



#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocket0
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocketd0

#INCLUDEPATH += $$PWD/../../include
#DEPENDPATH += $$PWD/../../include
