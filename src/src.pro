include(../qhttpserver.pri)
QHTTPSERVER_BASE = ..
TEMPLATE = lib

TARGET = qhttpserver
VERSION = 0.1.0

QT += network
QT -= gui

CONFIG += dll debug

INCLUDEPATH += $$QHTTPSERVER_BASE/http-parser

PRIVATE_HEADERS += $$QHTTPSERVER_BASE/http-parser/http_parser.h qhttpconnection.h

PUBLIC_HEADERS += qhttpserver.h qhttprequest.h qhttpresponse.h

HEADERS = $$PRIVATE_HEADERS $$PUBLIC_HEADERS
SOURCES = *.cpp $$QHTTPSERVER_BASE/http-parser/http_parser.c

OBJECTS_DIR = $$QHTTPSERVER_BASE/build
MOC_DIR = $$QHTTPSERVER_BASE/build
DESTDIR = $$QHTTPSERVER_BASE/lib

target.path = $$LIBDIR
headers.path = $$INCLUDEDIR
headers.files = $$PUBLIC_HEADERS
INSTALLS += target headers

HEADERS += \
    qauthenticatorrealm.h

HEADERS += \
    qsslserver.h

HEADERS += \
    qhttpsconnection.h

HEADERS += \
    sslserver.h
