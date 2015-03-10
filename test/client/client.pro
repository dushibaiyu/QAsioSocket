#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T08:44:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpClient
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocket
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocketd
else:unix: LIBS += -L$$PWD/../../lib/ -lQAsioSocket

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
