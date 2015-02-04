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

win32:LIBS += -lws2_32
win32:win32-g++:LIBS += -lMswsock
win32:win32-g++:DEFINES += D_WIN32_WINNT=0x0501 DWINVER=0x0501
#win32:win32-g++:DEFINES += D_WIN32_WINNT=0X0601
