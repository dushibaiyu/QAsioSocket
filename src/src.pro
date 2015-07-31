QT       += core

TEMPLATE = lib
DEFINES += QASIOSOCKET_LIBRARY QASIO_SSL
DESTDIR   = $$PWD/../lib
DEFINES += _WIN32_WINNT=0x0501

TARGET  = $$qtLibraryTarget(QAsioSocket)

include($$PWD/../QAsioSocket.pri)

win32{
    INCLUDEPATH += C:/OpenSSL-Win32/include/
    msvc {
        CONFIG(release, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -lssleay32M
        else:CONFIG(debug, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -lssleay32Md

        CONFIG(release, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -llibeay32M
        else:CONFIG(debug, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -llibeay32Md
    } else {
        LIBS += -LC:/OpenSSL-Win32/lib/MinGW/ -leay32
        LIBS += -LC:/OpenSSL-Win32/lib/MinGW/ -lssleay32
    }
}


