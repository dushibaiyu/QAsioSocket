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

DEFINES += QASIOSOCKET_NOLIB QASIO_SSL

include($$PWD/../../QAsioSocket.pri)

SOURCES += main.cpp \
    testserver.cpp

HEADERS += \
    testserver.h

win32{
    INCLUDEPATH += $$(OPENSSL)/include/
    msvc {
        CONFIG(release, debug|release): LIBS += -L$$(OPENSSL)/lib/VC/ -lssleay32M
        else:CONFIG(debug, debug|release): LIBS += -L$$(OPENSSL)/lib/VC/ -lssleay32Md

        CONFIG(release, debug|release): LIBS += -L$$(OPENSSL)/lib/VC/ -llibeay32M
        else:CONFIG(debug, debug|release): LIBS += -L$$(OPENSSL)/lib/VC/ -llibeay32Md
    } else {
        LIBS += -L$$(OPENSSL)/lib/MinGW/ -leay32
        LIBS += -L$$(OPENSSL)/lib/MinGW/ -lssleay32
    }
}

