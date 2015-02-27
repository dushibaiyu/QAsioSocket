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
