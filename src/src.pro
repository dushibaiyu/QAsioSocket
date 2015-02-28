QT       += core

TEMPLATE = lib
DEFINES += QASIOSOCKET_LIBRARY

DEFINES += _WIN32_WINNT=0x0501

DESTDIR   = $$PWD/../lib

TARGET  = $$qtLibraryTarget(QAsioSocket)

include($$PWD/../QAsioSocket.pri)

INCLUDEPATH += C:/local/boost_1_57_0
DEPENDPATH += C:/local/boost_1_57_0

