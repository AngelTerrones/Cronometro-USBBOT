#-------------------------------------------------
#
# Project created by QtCreator 2016-11-05T22:11:05
#
#-------------------------------------------------

QT += core gui widgets serialport

TARGET = Cronometro
DESTDIR = bin
TEMPLATE = app

SOURCES += \
    src/main.cpp\
    src/mainwindow.cpp

HEADERS  += \
    src/mainwindow.h

FORMS += \
    ui/mainwindow.ui

OBJECTS_DIR = build
MOC_DIR     = build
RCC_DIR     = build
UI_DIR      = build

win32:RC_FILE += ui/crono.rc
