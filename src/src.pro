QT       += core

TEMPLATE = lib
DEFINES += QASIOSOCKET_LIBRARY=Q_CORE_EXPORT

DESTDIR   = $${PWD}/../lib

TARGET  = $$qtLibraryTarget(QSsh)

include(AsioTcp.pri)