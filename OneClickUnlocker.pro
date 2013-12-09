#-------------------------------------------------
#
# Project created by QtCreator 2013-11-11T16:11:54
#
#-------------------------------------------------

QT       += core gui declarative network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(debug, debug|release) {
TARGET = Unlockerd
} else {
TARGET = Unlocker
}

INCLUDEPATH += $$PWD/src $$PWD/src/utils

TEMPLATE = app
DESTDIR = ../bin

win32 {
    LIBS += User32.lib \
            gdi32.lib \
            Ole32.lib Shell32.lib

    QMAKE_CXXFLAGS += /wd\"4819\" /wd\"4100\" /wd\"4189\" /wd\"4102\" /wd\"4290\"
#    QMAKE_LFLAGS += /MAP /MAPINFO:EXPORTS \
#                    /MANIFESTUAC:\"level=\'requireAdministrator\'\"
}

macx {
    ICON = icon.icns
}


RC_FILE = app.rc
TRANSLATIONS = language/zh_CN.ts

include(src/unlocker.pri)
