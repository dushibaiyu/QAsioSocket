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

DEFINES += QASIOSOCKET_NOLIB

include($$PWD/../../QAsioSocket.pri)

SOURCES += main.cpp \
    testserver.cpp

HEADERS += \
    testserver.h



#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocket
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocketd
#else:unix: LIBS += -L$$PWD/../../lib/ -lQAsioSocket


