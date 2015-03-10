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

SOURCES += main.cpp \
    threadServer.cpp \
    threadhandle.cpp \
    mysocket.cpp

HEADERS += \
    threadServer.h \
    threadhandle.h \
    mysocket.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocket
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocketd
else:unix: LIBS += -L$$PWD/../../lib/ -lQAsioSocket

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
