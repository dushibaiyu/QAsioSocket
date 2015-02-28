CONFIG += C++11
DEFINES += ASIO_STANDALONE

SOURCES += \
    $$PWD/src/ioserverthread.cpp \
    $$PWD/src/qasiotcpsocket.cpp \
    $$PWD/src/qasiotcpserver.cpp \
    $$PWD/src/qasiotcpsocketparent.cpp \
    $$PWD/src/qasiotcpserverparent.cpp

HEADERS += \
    $$PWD/src/ioserverthread.h \
    $$PWD/src/qasiotcpsocketparentprivate.h \
    $$PWD/src/qasiotcpserverparentprivate.h \
    $$PWD/include/qasiosocket.h \
    $$PWD/include/qasiotcpsocketparent.h \
    $$PWD/include/qasiotcpserverparent.h \
    $$PWD/include/qasiotcpsocket.h \
    $$PWD/include/qasiotcpserver.h


INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/src/include_asio

win32:win32-g++:LIBS += -lMswsock
win32:LIBS += -lws2_32
