QT       += core

TEMPLATE = lib
DEFINES += QASIOSOCKET_LIBRARY

#DEFINES += BOOST_ALL_NO_LIB BOOST_ERROR_CODE_HEADER_ONLY
#DEFINES += BOOST_SYSTEM_NO_DEPRECATED

DESTDIR   = $$PWD/../lib

TARGET  = $$qtLibraryTarget(QAsioSocket)

#INCLUDEPATH += D:/boost_1_57_0
#INCLUDEPATH += /home/dushibaiyu/Code/boost_1_57_0/

include($$PWD/../QAsioSocket.pri)

VERSION = 0.0.1

