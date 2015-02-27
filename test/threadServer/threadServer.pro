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
