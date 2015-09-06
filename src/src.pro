QT       += core

TEMPLATE = lib
DEFINES += QASIOSOCKET_LIBRARY QASIO_SSL
DESTDIR   = $$PWD/../lib
DEFINES += _WIN32_WINNT=0x0501

TARGET  = $$qtLibraryTarget(QAsioSocket)

include($$PWD/../QAsioSocket.pri)

win32{
    INCLUDEPATH += $$(OPENSSL)/include/
    msvc {
        CONFIG(release, debug|release): LIBS += -L$$(OPENSSL)/lib/VC/ -lssleay32M
        else:CONFIG(debug, debug|release): LIBS += -L$$(OPENSSL)/lib/VC/ -lssleay32Md

        CONFIG(release, debug|release): LIBS += -L$$(OPENSSL)/lib/VC/ -llibeay32M
        else:CONFIG(debug, debug|release): LIBS += -L$$(OPENSSL)/lib/VC/ -llibeay32Md
    } else {
        LIBS += -L$$(OPENSSL)/lib/MinGW/ -leay32
        LIBS += -L$$(OPENSSL)/lib/MinGW/ -lssleay32
    }
}


