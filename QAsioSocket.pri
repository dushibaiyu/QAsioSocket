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

win32:!win32-g++:DEFINES += NOMINMAX

#win32:LIBS += -lws2_32
win32:win32-g++:LIBS += -lMswsock
#win32:DEFINES += _WIN32_WINNT=0x0601
#win32:win32-g++:DEFINES += __USE_W32_SOCKETS

INCLUDEPATH += C:/local/boost_1_57_0
DEPENDPATH += C:/local/boost_1_57_0

win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_system-vc100-s-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_system-vc100-sgd-1_57
win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_system-vc100-s-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_system-vc100-sgd-1_57.lib


win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_thread-vc100-mt-s-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_thread-vc100-mt-sgd-1_57
win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_thread-vc100-mt-s-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_thread-vc100-mt-sgd-1_57.lib


win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_date_time-vc100-mt-s-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_date_time-vc100-mt-sgd-1_57
win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_date_time-vc100-mt-s-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_date_time-vc100-mt-sgd-1_57.lib

#win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_regex-vc100-s-1_57
#else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_regex-vc100-sgd-1_57
#win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_regex-vc100-s-1_57.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_regex-vc100-sgd-1_57.lib

win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_serialization-vc100-mt-s-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -llibboost_serialization-vc100-mt-sgd-1_57
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_serialization-vc100-mt-s-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/libboost_serialization-vc100-mt-sgd-1_57.lib
