CONFIG += C++11
DEFINES += ASIO_STANDALONE

SOURCES += \
    $$PWD/qasiotcpsocket.cpp \
    $$PWD/ioserverthread.cpp \
    $$PWD/qasiotcpserver.cpp

HEADERS += \
    $$PWD/qasiotcpsocket.h \
    $$PWD/ioserverthread.h \
    $$PWD/qasioevent.h \
    $$PWD/qasiotcpserver.h

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/include_asio

win32:LIBS += -lws2_32 -lMswsock
win32:DEFINES += _WIN32_WINNT=0X0601
