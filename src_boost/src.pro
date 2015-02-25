QT       += core

TEMPLATE = lib
DEFINES += QASIOSOCKETH_LIBRARY

DEFINES += BOOST_ALL_NO_LIB

DESTDIR   = $${PWD}/../lib

TARGET  = $$qtLibraryTarget(QAsioSocket)

INCLUDEPATH += D:/boost_1_57_0

include(AsioTcp.pri)

