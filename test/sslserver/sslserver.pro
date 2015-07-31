#-------------------------------------------------
#
# Project created by QtCreator 2014-12-08T16:04:57
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QAsioSSLServer
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += QASIO_SSL

include($$PWD/../../QAsioSocket.pri)

SOURCES += main.cpp \
    testserver.cpp

HEADERS += \
    testserver.h

win32{
    INCLUDEPATH += C:/OpenSSL-Win32/include/
    msvc {
        CONFIG(release, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -lssleay32M
        else:CONFIG(debug, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -lssleay32Md

        CONFIG(release, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -llibeay32M
        else:CONFIG(debug, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -llibeay32Md
    } else {
        LIBS += -LC:/OpenSSL-Win32/lib/MinGW/ -leay32
        LIBS += -LC:/OpenSSL-Win32/lib/MinGW/ -lssleay32
    }
}

