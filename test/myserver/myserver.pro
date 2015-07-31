#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T15:30:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = msserver
TEMPLATE = app
CONFIG   += console

CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp \
    testserver.cpp

HEADERS  += mainwindow.h \
    testserver.h

FORMS    += mainwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocket
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocketd
else:unix: LIBS += -L$$PWD/../../lib/ -lQAsioSocket

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
