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

#include(../../QAsioSocket.pri)

SOURCES += main.cpp \
    mysocket.cpp \
    myserver.cpp

HEADERS += \
    mysocket.h \
    myserver.h

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
