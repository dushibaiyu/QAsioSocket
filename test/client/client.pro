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

INCLUDEPATH += D:/boost_1_57_0

include(../../QAsioSocket.pri)

INCLUDEPATH += C:/local/boost_1_57_0
DEPENDPATH += C:/local/boost_1_57_0

win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_date_time-vc100-mt-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_date_time-vc100-mt-gd-1_57
win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_date_time-vc100-mt-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_date_time-vc100-mt-gd-1_57.lib

win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_system-vc100-mt-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_system-vc100-mt-gd-1_57
win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_system-vc100-mt-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_system-vc100-mt-gd-1_57.lib

win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_thread-vc100-mt-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_thread-vc100-mt-gd-1_57
win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_thread-vc100-mt-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_thread-vc100-mt-gd-1_57.lib



#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocket
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lQAsioSocketd
#else:unix: LIBS += -L$$PWD/../../lib/ -lQAsioSocket

#INCLUDEPATH += $$PWD/../../include
#DEPENDPATH += $$PWD/../../include
