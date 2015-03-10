#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T15:02:46
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = HandleInThread
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mysocket.cpp \
    myserver.cpp

HEADERS += \
    mysocket.h \
    myserver.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocket
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocketd
else:unix: LIBS += -L$$PWD/../../lib/ -lQAsioSocket

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
