#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T11:34:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PingTongTest
TEMPLATE = app

CONFIG -= console

SOURCES += main.cpp\
    mysocket.cpp \
    sockets.cpp \
    form.cpp

HEADERS  += \
    mysocket.h \
    sockets.h \
    form.h

FORMS += \
    form.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocket
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocketd
else:unix: LIBS += -L$$PWD/../../lib/ -lQAsioSocket

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
