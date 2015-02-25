SOURCES += \
    $$PWD/src/qasiotcpsocket.cpp \
    $$PWD/src/ioserverthread.cpp \
    $$PWD/src/qasiotcpserver.cpp \
    $$PWD/src/qasiotcpsocketparent.cpp \
    $$PWD/src/qasiotcpserverparent.cpp

HEADERS += \
    $$PWD/include/qasiotcpsocket.h \
    $$PWD/src/ioserverthread.h \
    $$PWD/include/qasiotcpserver.h \
    $$PWD/include/qasiotcpsocketparent.h \
    $$PWD/include/qasiotcpserverparent.h \
    $$PWD/include/qasiosocket.h

INCLUDEPATH += $$PWD/include

win32:LIBS += -lws2_32
win32:win32-g++:LIBS += -lMswsock
win32:DEFINES += _WIN32_WINNT=0x0601
win32:win32-g++:DEFINES += __USE_W32_SOCKETS
